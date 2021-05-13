#ifndef CELL_H
#define CELL_H

#include <QVector2D>
#include <array>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>


class Cell
{
public:
    Cell(const QVector2D& vertex_top_left,
         const QVector2D& vertex_top_right,
         const QVector2D& vertex_bottom_right,
         const QVector2D& vertex_bottom_left);

    ~Cell();

    std::array<QVector2D, 4> vertices() const noexcept;
    inline QVector2D vertexTopLeft() const noexcept;
    inline QVector2D vertexTopRight() const noexcept;
    inline QVector2D vertexBottomLeft() const noexcept;
    inline QVector2D vertexBottomRight() const noexcept;

    void setVertices(const std::array<QVector2D, 4>& vertices) noexcept;
    inline void setVertexTopLeft(const QVector2D& vertex) noexcept;
    inline void setVertexTopRight(const QVector2D& vertex) noexcept;
    inline void setVertexBottomRight(const QVector2D& vertex) noexcept;
    inline void setVertexBottomLeft(const QVector2D& vertex) noexcept;

    void draw();
    void initializeGL();


private:
    std::array<QVector2D, 4> m_vertices;

    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
};

inline QVector2D Cell::vertexTopLeft() const noexcept
{
    return m_vertices[0];
}

inline QVector2D Cell::vertexTopRight() const noexcept
{
    return m_vertices[1];
}

inline QVector2D Cell::vertexBottomRight() const noexcept
{
    return m_vertices[2];
}

inline QVector2D Cell::vertexBottomLeft() const noexcept
{
    return m_vertices[3];
}

inline void Cell::setVertexTopLeft(const QVector2D& vertex) noexcept
{
    m_vertices[0] = vertex;
}

inline void Cell::setVertexTopRight(const QVector2D& vertex) noexcept
{
    m_vertices[1] = vertex;
}

inline void Cell::setVertexBottomRight(const QVector2D& vertex) noexcept
{
    m_vertices[2] = vertex;
}

inline void Cell::setVertexBottomLeft(const QVector2D& vertex) noexcept
{
    m_vertices[3] = vertex;
}

#endif // CELL_H
