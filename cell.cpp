#include "cell.h"

#include <QOpenGLFunctions>

Cell::Cell(const QVector2D &vertex_top_left, const QVector2D &vertex_top_right, const QVector2D &vertex_bottom_right, const QVector2D &vertex_bottom_left)
{
    m_vertices[0] = vertex_top_left;
    m_vertices[1] = vertex_top_right;
    m_vertices[2] = vertex_bottom_right;
    m_vertices[3] = vertex_bottom_left;
}

void Cell::setVertices(const std::array<QVector2D, 4> &vertices) noexcept
{
    m_vertices = vertices;
}

std::array<QVector2D, 4> Cell::vertices() const noexcept
{
    return m_vertices;
}
