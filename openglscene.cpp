#include "openglscene.h"

#include "metaballslistmodel.h"

#include <QMouseEvent>
#include <QDebug>

#include <QOpenGLShader>
#include <QOpenGLShaderProgram>


#include <QMatrix4x4>

OpenGLScene::OpenGLScene(QWidget *parent)
    : QOpenGLWidget(parent), m_grid(width(), height(), 40, 40),  m_is_grid_visible(false),
      m_cursor_vao(this), m_ms_solver(&m_grid), m_ms_threshold(0.5f),
      m_n_vertices_on_surface(0)
{
    setMouseTracking(true);
}

OpenGLScene::~OpenGLScene()
{
    makeCurrent();

    m_cursor_vbo.destroy();
    m_cursor_vao.destroy();

    m_surface_vbo.destroy();
    m_surface_vao.destroy();
}

void OpenGLScene::setMetaballsListModel(MetaballsListModel *model)
{
    m_metaballs_list_model = model;


    m_current_metaball = new MurakamiMetaball(QVector2D(0.f, 0.f), 100.0f);

    connect(m_metaballs_list_model, SIGNAL(rowsRemoved(const QModelIndex &, int, int)),
            this, SLOT(onRowsRemoved(const QModelIndex &, int, int )));
}

void OpenGLScene::setGridVisible(bool isVisible)
{
    m_is_grid_visible = isVisible;
    update();
}

void OpenGLScene::setCurrentMetaballRadius(float radius)
{
    m_current_metaball->setRadius(radius);
    evaluateMarchingSquares();
    update();
}

void OpenGLScene::setThreshold(float threshold)
{
    m_ms_threshold = threshold;
    evaluateMarchingSquares();
    update();
}

void OpenGLScene::setOperation(int operation)
{
    m_current_metaball->setOperation(static_cast<AbstractMetaballOperation>(operation));
}

void OpenGLScene::setSelectedItemInListView(const QModelIndex &current, const QModelIndex &)
{
    m_current_selected_metaball = current;
    update();
}

void OpenGLScene::resizeGrid(unsigned int nCols, unsigned int nRows)
{
    makeCurrent();
    m_grid.resize(width(), height(), nCols, nRows);
    m_grid.initializeGL();
    evaluateMarchingSquares();
    update();
}

void OpenGLScene::onRowsRemoved(const QModelIndex &, int, int)
{
    evaluateMarchingSquares();
    update();
}

void OpenGLScene::mouseMoveEvent(QMouseEvent *event)
{
    m_current_metaball->setPosition(QVector2D(event->pos()));

    evaluateMarchingSquares();
    update();
}

void OpenGLScene::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MouseButton::LeftButton)
    {
        AbstractMetaball *previous = m_current_metaball;
        m_current_metaball = new MurakamiMetaball(QVector2D(event->pos()), m_current_metaball->radius());
        m_current_metaball->setOperation(previous->operation());

        m_metaballs_list_model->addMetaball(previous);
    }
}

void OpenGLScene::leaveEvent(QEvent *)
{
    evaluateMarchingSquares();
    update();
}

void OpenGLScene::initializeGL()
{
    initializeOpenGLFunctions();

    m_cursor_vao.create();
    if (m_cursor_vao.isCreated())
        m_cursor_vao.bind();

    m_cursor_vbo.create();
    if (m_cursor_vbo.isCreated())
        m_cursor_vbo.bind();

    float cursor[] = {0, 0};

    glBufferData(GL_ARRAY_BUFFER, sizeof(cursor), cursor, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    m_cursor_vao.release();
    m_cursor_vbo.release();

    m_shader_program = new QOpenGLShaderProgram(this);
    m_shader_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/draw.vert");
    m_shader_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/draw.frag");
    m_shader_program->link();

    m_shader_program->bind();
    m_ortho_projection.ortho(0.0, width(), height(), 0.0, -.1f, 1.0f);
    m_shader_program->setUniformValue(m_shader_program->uniformLocation("P"), m_ortho_projection);

    m_grid.initializeGL();
    m_circle.initializeGL();

    glEnable(GL_PROGRAM_POINT_SIZE);

    initializeGLSurfaceBuffers();

    glClearColor(.23f, .23f, .23f, 1.f);
}

void OpenGLScene::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    m_shader_program->bind();

    // Draw grid
    QMatrix4x4 grid_model;
    grid_model.setToIdentity();
    m_shader_program->setUniformValue(m_shader_program->uniformLocation("M"), grid_model);
    m_shader_program->setUniformValue(m_shader_program->uniformLocation("color"), QVector4D(0.5f, 0.5f, 0.5f, 1.f));
    if (m_is_grid_visible)
        m_grid.draw();


    m_shader_program->setUniformValue(m_shader_program->uniformLocation("color"), QVector4D(0.f, 1.f, 0.f, 1.f));
    paintGLSurface();

    // Draw cursor
    if (underMouse())
    {
        m_cursor_model.setToIdentity();
        QVector2D position = m_current_metaball->position();
        m_cursor_model.translate(position.x(), position.y());
        m_cursor_model.scale(m_current_metaball->radius(), m_current_metaball->radius());
        m_shader_program->setUniformValue(m_shader_program->uniformLocation("M"), m_cursor_model);

        m_shader_program->setUniformValue(m_shader_program->uniformLocation("color"), QVector4D(1.f, 0.f, 0.f, 1.f));
        m_cursor_vao.bind();
        glDrawArrays(GL_POINTS, 0, 1);

        m_cursor_vao.release();

        m_shader_program->setUniformValue(m_shader_program->uniformLocation("color"), QVector4D(0.f, 0.f, 1.f, 1.f));
        m_circle.draw();
    }

    if (m_current_selected_metaball.isValid())
    {
        AbstractMetaball *m = m_metaballs_list_model->metaballAt(m_current_selected_metaball.row());
        if (m)
        {
            m_cursor_model.setToIdentity();
            QVector2D position = m->position();
            m_cursor_model.translate(position.x(), position.y());
            m_cursor_model.scale(m->radius(), m->radius());
            m_shader_program->setUniformValue(m_shader_program->uniformLocation("M"), m_cursor_model);
            m_shader_program->setUniformValue(m_shader_program->uniformLocation("color"), QVector4D(1.f, .6f, .0f, 1.f));
            m_circle.draw();
            m_cursor_vao.bind();
            glDrawArrays(GL_POINTS, 0, 1);
            m_cursor_vao.release();
        }
    }
}

void OpenGLScene::resizeGL(int w, int h)
{
    m_ortho_projection.setToIdentity();
    m_ortho_projection.ortho(0.0, w, h, 0.0, -.1f, .1f);
    m_shader_program->setUniformValue(m_shader_program->uniformLocation("P"), m_ortho_projection);

    m_grid.resize(w, h, m_grid.numCols(), m_grid.numRows());
    m_grid.initializeGL();
}

void OpenGLScene::initializeGLSurfaceBuffers()
{
    m_surface_vao.create();
    m_surface_vao.bind();
    m_surface_vbo.create();
    m_surface_vbo.bind();

    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * m_grid.numberOfCells() * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    m_surface_vao.release();
    m_surface_vbo.release();
}

void OpenGLScene::updateSurfaceBuffers(const QVector<float> &vertices)
{
    m_surface_vao.bind();
    m_surface_vbo.bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vertices.size(), vertices.data());
    m_surface_vao.release();
    m_surface_vbo.release();
}

void OpenGLScene::paintGLSurface()
{
    m_surface_vao.bind();
    glDrawArrays(GL_LINES, 0, m_n_vertices_on_surface * 0.5);
    m_surface_vao.release();
}

void OpenGLScene::evaluateMarchingSquares()
{
    QVector<AbstractMetaball*> metaballs = m_metaballs_list_model->metaballs();
    if (underMouse())
        metaballs.push_back(m_current_metaball);
    QVector<float> vertices = m_ms_solver.solve(m_ms_threshold, metaballs);
    m_n_vertices_on_surface = vertices.size();
    updateSurfaceBuffers(vertices);
}
