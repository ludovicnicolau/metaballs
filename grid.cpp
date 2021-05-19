#include "grid.h"

#include <QDebug>
#include <QOpenGLFunctions>

Grid::Grid(float width, float height, unsigned int nCols, unsigned int nRows)
    : m_n_cols(nCols), m_n_rows(nRows)
{
    resize(width, height, nCols, nRows);
}

Grid::~Grid()
{
    m_vbo.destroy();
    m_vao.destroy();
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
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    m_vao.bind();
    f->glDrawArrays(GL_LINES, 0, m_n_vertices);
    m_vao.release();
}


void Grid::initializeGL()
{
    QVector<float> data(2 * (m_n_cols - 1) * 2 + 2 * (m_n_rows - 1) * 2);

    int index = 0;
    for (unsigned int i = 0;i < m_n_cols-1;++i)
    {
        QVector2D first = m_cells[i]->vertexTopRight();
        data[index++] = first.x();
        data[index++] = first.y();
        QVector2D second = m_cells[i + (m_n_rows - 1) * m_n_cols]->vertexBottomRight();
        data[index++] = second.x();
        data[index++] = second.y();
    }
    for (unsigned int i = 0;i < m_n_rows-1;++i)
    {
        QVector2D first = m_cells[i * m_n_cols]->vertexBottomLeft();
        data[index++] = first.x();
        data[index++] = first.y();

        QVector2D second = m_cells[i * m_n_cols + m_n_cols - 1]->vertexBottomRight();
        data[index++] = second.x();
        data[index++] = second.y();
    }

    m_n_vertices = data.size() / 2;

    if (m_vao.isCreated())
        m_vao.destroy();

    if (m_vbo.isCreated())
        m_vbo.destroy();

    m_vao.create();
    m_vao.bind();

    m_vbo.create();
    m_vbo.bind();

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    f->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), data.data(), GL_STATIC_DRAW);
    f->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    f->glEnableVertexAttribArray(0);

    m_vao.release();
    m_vbo.release();
}

void Grid::resize(float width, float height, unsigned int nCols, unsigned int nRows)
{
    m_n_cols = nCols;
    m_n_rows = nRows;

    m_cells.clear();

    float colsStep = width / m_n_cols;
    float rowsStep = height / m_n_rows;

    float current_y = 0.f;
    for (unsigned int i = 0;i < m_n_rows;++i)
    {
        float current_x = 0.f;
        for (unsigned int j = 0;j < m_n_cols;++j)
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
