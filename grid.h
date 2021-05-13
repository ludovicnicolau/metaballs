#ifndef GRID_H
#define GRID_H

#include <QObject>
#include <QVector>
#include "cell.h"

#include <memory.h>
#include <vector>

class Grid : public QObject
{
    Q_OBJECT
public:
    Grid(float width, float height, unsigned int nCols, unsigned int nRows);
    ~Grid();

    unsigned int numberOfCells() const noexcept;
    // Cell cellAt(unsigned int col, unsigned int row) const;

    void draw();
    void initializeGL();

public slots:
    void resize(float width, float height);


private:
    void initializeGLBuffers();

    unsigned int m_n_cols;
    unsigned int m_n_rows;

    std::vector<std::unique_ptr<Cell>> m_cells;

    /*QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ebo;*/
};

#endif // GRID_H
