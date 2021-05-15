#ifndef CIRCLE_H
#define CIRCLE_H

#include <QVector2D>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

class Circle
{
public:
    Circle(const QVector2D &center = QVector2D(0.0f, 0.0f), float radius = 1.0f, unsigned int nVertices = 64);
    ~Circle();

    inline QVector2D center() const noexcept;
    inline float radius() const noexcept;

    inline void setCenter(const QVector2D& center) noexcept;
    inline void setRadius(float radius) noexcept;

    void draw();

    void initializeGL();

private:
    QVector2D m_center;
    float m_radius;
    unsigned int m_n_vertices;

    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;

};

inline QVector2D Circle::center() const noexcept
{
    return m_center;
}

inline float Circle::radius() const noexcept
{
    return m_radius;
}

inline void Circle::setCenter(const QVector2D &center) noexcept
{
    m_center = center;
}

inline void Circle::setRadius(float radius) noexcept
{
    m_radius = radius;
}

#endif // CIRCLE_H
