#ifndef OPENGLSCENE_H
#define OPENGLSCENE_H

#include <QOpenGLWidget>

#include "circle.h"
#include "grid.h"
#include "marchingsquaressolver.h"
#include "murakamimetaball.h"

#include <QMatrix4x4>
#include <QModelIndex>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>

class MetaballsListModel;

class OpenGLScene : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    OpenGLScene(QWidget *parent = nullptr);

    ~OpenGLScene();

    void setMetaballsListModel(MetaballsListModel *model);

public slots:
    void setGridVisible(bool isVisible);
    void setCurrentMetaballRadius(float radius);
    void setThreshold(float threshold);
    void setOperation(int operation);
    void setSelectedItemInListView(const QModelIndex &current, const QModelIndex &previous);

    void resizeGrid(unsigned int nCols, unsigned int nRows);

private slots:
    void onRowsRemoved(const QModelIndex &, int, int);

protected:
    void mouseMoveEvent(class QMouseEvent *event) override;
    void mousePressEvent(class QMouseEvent *event) override;
    void leaveEvent(class QEvent *event) override;
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private:
    void initializeGLSurfaceBuffers();
    void updateSurfaceBuffers(const QVector<float> &vertices);
    void paintGLSurface();
    void evaluateMarchingSquares();

    Grid m_grid;
    bool m_is_grid_visible;

    QOpenGLVertexArrayObject m_cursor_vao;
    QOpenGLBuffer m_cursor_vbo;
    QMatrix4x4 m_cursor_model;
    QMatrix4x4 m_ortho_projection;

    MarchingSquaresSolver m_ms_solver;
    float m_ms_threshold;

    QOpenGLVertexArrayObject m_surface_vao;
    QOpenGLBuffer m_surface_vbo;
    unsigned int m_n_vertices_on_surface;

    class QOpenGLShaderProgram *m_shader_program;

    class AbstractMetaball *m_current_metaball;

    Circle m_circle;

    MetaballsListModel *m_metaballs_list_model;

    QModelIndex m_current_selected_metaball;
};

#endif // OPENGLSCENE_H
