#include "cell.h"

#include <QOpenGLFunctions>

#include <QDebug>

Cell::Cell(const QVector2D &vertex_top_left, const QVector2D &vertex_top_right, const QVector2D &vertex_bottom_right, const QVector2D &vertex_bottom_left)
{
    m_vertices[0] = vertex_top_left;
    m_vertices[1] = vertex_top_right;
    m_vertices[2] = vertex_bottom_right;
    m_vertices[3] = vertex_bottom_left;
}

Cell::~Cell()
{
    m_vbo.destroy();
    m_vao.destroy();
}

void Cell::draw()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    m_vao.bind();
    f->glDrawArrays(GL_LINE_STRIP, 0, 4);
    m_vao.release();
}

void Cell::initializeGL()
{
    float vertices[8];
    for (unsigned int i = 0;i < 4;++i)
    {
        QVector2D vertex = m_vertices[i];
        vertices[i*2] = vertex.x();
        vertices[i*2 + 1] = vertex.y();
    }

    m_vao.create();
    m_vao.bind();
    m_vbo.create();
    m_vbo.bind();

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    f->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    f->glEnableVertexAttribArray(0);

    m_vao.release();
    m_vbo.release();
}

void Cell::setVertices(const std::array<QVector2D, 4> &vertices) noexcept
{
    m_vertices = vertices;
}

std::array<QVector2D, 4> Cell::vertices() const noexcept
{
    return m_vertices;
}
