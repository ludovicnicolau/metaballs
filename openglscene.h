#ifndef OPENGLSCENE_H
#define OPENGLSCENE_H

#include <QOpenGLWidget>
#include "grid.h"
#include "marchingsquaressolver.h"

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QMatrix4x4>

class OpenGLScene : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    OpenGLScene(QWidget *parent = nullptr);

    ~OpenGLScene();

protected:
    void mouseMoveEvent(class QMouseEvent *event) override;
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private:
    Grid m_grid;

    QOpenGLVertexArrayObject m_cursor_vao;
    QOpenGLBuffer m_cursor_vbo;
    QMatrix4x4 m_cursor_model;
    QMatrix4x4 m_ortho_projection;

    MarchingSquaresSolver m_ms_solver;
    float m_ms_threshold;

    class QOpenGLShaderProgram *m_shader_program;

    class AbstractMetaball *m_current_metaball;
};

#endif // OPENGLSCENE_H
