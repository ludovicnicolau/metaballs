#ifndef GRID_H
#define GRID_H

#include <QObject>
#include <QVector>
#include "cell.h"

#include <memory>
#include <vector>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

class Grid : public QObject
{
    Q_OBJECT
public:
    Grid(float width, float height, unsigned int nCols, unsigned int nRows);
    ~Grid();

    unsigned int numberOfCells() const noexcept;
    inline unsigned int numCols() const noexcept;
    inline unsigned int numRows() const noexcept;
    Cell* cellAt(unsigned int index) const;

    void draw();
    void initializeGL();

public slots:
    void resize(float width, float height, unsigned int nCols, unsigned int nRows);


private:
    void initializeGLBuffers();

    unsigned int m_n_cols;
    unsigned int m_n_rows;

    std::vector<std::unique_ptr<Cell>> m_cells;

    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;

    unsigned int m_n_vertices;
};

inline unsigned int Grid::numCols() const noexcept
{
    return m_n_cols;
}

inline unsigned int Grid::numRows() const noexcept
{
    return m_n_rows;
}

#endif // GRID_H
