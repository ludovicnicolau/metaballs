#include "grid.h"

#include <QDebug>

Grid::Grid(float width, float height, unsigned int nCols, unsigned int nRows)
    : m_n_cols(nCols), m_n_rows(nRows)
{
    resize(width, height);
}

Grid::~Grid()
{
}

unsigned int Grid::numberOfCells() const noexcept
{
    return m_cells.size();
}

Cell *Grid::cellAt(unsigned int index) const
{
    if (index < 0 || index >= m_cells.size())
        return nullptr;
    return m_cells[index].get();
}

void Grid::draw()
{
    for (auto& cell : m_cells)
    {
        cell->draw();
    }
}

/*Cell Grid::cellAt(unsigned int col, unsigned int row) const
{
    return m_cells[row * m_n_cols + col];
}*/

void Grid::initializeGL()
{
    for (auto& cell : m_cells)
    {
        cell->initializeGL();
    }
}

void Grid::resize(float width, float height)
{
    m_cells.clear();

    float colsStep = width / m_n_cols;
    float rowsStep = height / m_n_rows;

    float current_y = 0.f;
    for (unsigned int i = 0;i < m_n_cols;++i)
    {
        float current_x = 0.f;
        for (unsigned int j = 0;j < m_n_rows;++j)
        {
            QVector2D top_left(current_x, current_y);
            QVector2D top_right(current_x + colsStep, current_y);
            QVector2D bottom_right(current_x + colsStep, current_y + rowsStep);
            QVector2D bottom_left(current_x, current_y + rowsStep);

            std::unique_ptr<Cell> cell = std::make_unique<Cell>(top_left, top_right, bottom_right, bottom_left);
            m_cells.push_back(std::move(cell));

            current_x += colsStep;
        }
        current_y += rowsStep;
    }
}

/*void Grid::initializeGLBuffers()
{
    m_vao.create();
    if (m_vao.isCreated())
        m_vao.bind();
    m_vbo.create();
    if (m_vbo.isCreated())
        m_vbo.bind();
    m_ebo.create();
    if (m_ebo.isCreated())
        m_ebo.bind();

    unsigned int nCells = m_cells.size();
    QVector<float> vertices(2 * (m_n_cols + 1) * (m_n_rows + 1));
    int index = 0;
    for (unsigned int i = 0;i < nCells;++i)
    {
        QVector2D top_left = m_cells[i].vertexTopLeft();
        vertices[index++] = top_left.x();
        vertices[index++] = top_left.y();
        if ((i + 1) % m_n_cols == 0)
        {
            QVector2D top_right = m_cells[i].vertexTopRight();
            vertices[index++] = top_right.x();
            vertices[index++] = top_right.y();
        }
    }
    for (unsigned int i = nCells - m_n_cols;i < nCells;++i)
    {
        QVector2D bottom_left = m_cells[i].vertexBottomLeft();
        vertices[index++] = bottom_left.x();
        vertices[index++] = bottom_left.y();
        if ((i + 1) % m_n_cols == 0)
        {
            QVector2D bottom_right = m_cells[i].vertexBottomRight();
            vertices[index++] = bottom_right.x();
            vertices[index++] = bottom_right.y();
        }
    }

    QVector<unsigned int> indices();

}
*/
