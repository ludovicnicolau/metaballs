#include "openglscene.h"

#include <QMouseEvent>
#include <QDebug>

#include <QOpenGLShader>
#include <QOpenGLShaderProgram>


#include <QMatrix4x4>

#include "murakamimetaball.h"

OpenGLScene::OpenGLScene(QWidget *parent)
    : QOpenGLWidget(parent), m_grid(width(), height(), 32, 32),  m_is_grid_visible(false),
      m_cursor_vao(this), m_ms_solver(&m_grid), m_ms_threshold(0.5f),
      m_n_vertices_on_surface(0)
{
    setMouseTracking(true);

    m_current_metaball = new MurakamiMetaball(QVector2D(0.f, 0.f), 100.0f);
    m_ms_solver.addMetaball(m_current_metaball);
}

OpenGLScene::~OpenGLScene()
{
    delete m_current_metaball;

    makeCurrent();

    m_cursor_vbo.destroy();
    m_cursor_vao.destroy();

    m_surface_vbo.destroy();
    m_surface_vao.destroy();
}

void OpenGLScene::setGridVisible(bool isVisible)
{
    m_is_grid_visible = isVisible;
    update();
}

void OpenGLScene::setThreshold(float threshold)
{
    m_ms_threshold = threshold;
    evaluateMarchingSquares();
    qDebug() << m_ms_threshold;
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

    }
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
    m_shader_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "../draw.vert");
    m_shader_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "../draw.frag");
    m_shader_program->link();

    m_shader_program->bind();
    m_ortho_projection.ortho(0.0, width(), height(), 0.0, -.1f, 1.0f);
    m_shader_program->setUniformValue(m_shader_program->uniformLocation("P"), m_ortho_projection);

    m_cursor_model.translate(15.0, 15.0);
    m_shader_program->setUniformValue(m_shader_program->uniformLocation("M"), m_cursor_model);


    m_grid.initializeGL();
    m_circle.initializeGL();

    glEnable(GL_PROGRAM_POINT_SIZE);

    initializeGLSurfaceBuffers();

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

void OpenGLScene::resizeGL(int w, int h)
{
    m_ortho_projection.setToIdentity();
    m_ortho_projection.ortho(0.0, w, h, 0.0, -.1f, .1f);
    m_shader_program->setUniformValue(m_shader_program->uniformLocation("P"), m_ortho_projection);

    m_grid.resize(w, h);
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
    glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * 2 * sizeof(float) * vertices.size(), vertices.data());
    m_surface_vao.release();
    m_surface_vbo.release();
}

void OpenGLScene::paintGLSurface()
{
    m_surface_vao.bind();
    glDrawArrays(GL_LINES, 0, m_n_vertices_on_surface);
    m_surface_vao.release();
}

void OpenGLScene::evaluateMarchingSquares()
{
    QVector<float> vertices = m_ms_solver.solve(m_ms_threshold);
    m_n_vertices_on_surface = vertices.size();
    updateSurfaceBuffers(vertices);
}
