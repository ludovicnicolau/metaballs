#include "circle.h"

#include <QOpenGLFunctions>
#include <QtMath>
#include <QVector>

#include <QDebug>

Circle::Circle(const QVector2D &center, float radius, unsigned int nVertices)
    : m_center(center), m_radius(radius), m_n_vertices(nVertices)
{
}

Circle::~Circle()
{
    m_vbo.destroy();
    m_vao.destroy();
}

void Circle::draw()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    m_vao.bind();
    f->glDrawArrays(GL_LINE_STRIP, 0, m_n_vertices + 1);
    m_vao.release();
}

void Circle::initializeGL()
{
    QVector<float> vertices(m_n_vertices * 2 + 2);

    float angle_step = 2.f * M_PI / m_n_vertices;
    float current_angle = 0.0f;
    unsigned int i;
    for (i = 0;i < m_n_vertices;++i)
    {
        vertices[i*2] = m_radius * cosf(current_angle);
        vertices[i*2 + 1] = m_radius * sinf(current_angle);
        current_angle += angle_step;
    }
    vertices[i*2] = vertices[0];
    vertices[i*2+1] = vertices[1];

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    m_vao.create();
    m_vbo.create();

    m_vao.bind();
    m_vbo.bind();

    f->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    f->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    f->glEnableVertexAttribArray(0);

    m_vao.release();
    m_vbo.release();
}
