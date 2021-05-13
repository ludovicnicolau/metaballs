#include "openglscene.h"

#include <QMouseEvent>
#include <QDebug>

#include <QOpenGLShader>
#include <QOpenGLShaderProgram>


#include <QMatrix4x4>

#include "murakamimetaball.h"

OpenGLScene::OpenGLScene(QWidget *parent)
    : QOpenGLWidget(parent), m_grid(width(), height(), 16, 16), m_cursor_vao(this)
{
    setMouseTracking(true);

    m_current_metaball = new MurakamiMetaball(QVector2D(0.f, 0.f), 10.0f);
}

OpenGLScene::~OpenGLScene()
{
    delete m_current_metaball;
}

void OpenGLScene::mouseMoveEvent(QMouseEvent *event)
{
    m_current_metaball->setPosition(QVector2D(event->pos()));

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

    glBufferData(GL_ARRAY_BUFFER, sizeof(cursor), cursor, GL_STREAM_DRAW);

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

    glEnable(GL_PROGRAM_POINT_SIZE);

}

void OpenGLScene::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    m_shader_program->bind();

    // Draw grid
    QMatrix4x4 grid_model;
    grid_model.setToIdentity();
    m_shader_program->setUniformValue(m_shader_program->uniformLocation("M"), grid_model);
    m_grid.draw();

    // Draw cursor
    m_cursor_model.setToIdentity();
    QVector2D position = m_current_metaball->position();
    m_cursor_model.translate(position.x(), position.y());
    m_shader_program->setUniformValue(m_shader_program->uniformLocation("M"), m_cursor_model);

    m_cursor_vao.bind();
    glDrawArrays(GL_POINTS, 0, 1);
    m_cursor_vao.release();

}

void OpenGLScene::resizeGL(int w, int h)
{
    m_ortho_projection.setToIdentity();
    m_ortho_projection.ortho(0.0, w, h, 0.0, -.1f, .1f);
    m_shader_program->setUniformValue(m_shader_program->uniformLocation("P"), m_ortho_projection);

    m_grid.resize(w, h);
    m_grid.initializeGL();
    qDebug() << "Width: " << QString::number(w) << "Height:" << QString::number(h);
}
