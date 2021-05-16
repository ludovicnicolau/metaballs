#include "marchingsquaressolver.h"

#include "abstractmetaball.h"
#include "grid.h"

#include <QDebug>

MarchingSquaresSolver::MarchingSquaresSolver(Grid *grid)
    : m_grid(grid)
{

}

QVector<float> MarchingSquaresSolver::solve(float threshold, const QVector<AbstractMetaball*>& metaballs)
{
    QVector<float> vector;

    unsigned int nCells = m_grid->numberOfCells();

    QVector<float> resulting_vertices; // Vertices that will be created with marching squares
    for (unsigned int i = 0;i < nCells;++i)
    {
        Cell *cell = m_grid->cellAt(i);
        calculateCellValue(cell, threshold, resulting_vertices, metaballs);
    }
    return resulting_vertices;
}

void MarchingSquaresSolver::calculateCellValue(Cell * const cell, float threshold,
                                               QVector<float> &resulting_vertices,
                                               const QVector<AbstractMetaball*>& metaballs) const
{
    short isIn = 0x0;
    std::array<float, 4> values;

    std::array<QVector2D, 4> vertices = cell->vertices();
    for (int i = 0;i < 4;++i)
    {
        float f = calculateVertexValue(vertices[i], metaballs);
        values[i] = f;
        if (f >= threshold)
        {
            isIn |= 0x1 << i;
        }
    }

    for (int i = 0;i < 4;++i)
    {
        bool inOut = ((isIn & 0x1 << i) && !(isIn & 0x1 << ((i+1)%4))) || (!(isIn & 0x1 << i) && (isIn & 0x1 << ((i+1)%4)));
        if (inOut)
        {
            QVector2D p1 = vertices[i];
            QVector2D p2 = vertices[(i+1)%4];
            float d = (threshold - values[i]) / (values[(i+1)%4] - values[i]);
            QVector2D vertex = (1.f - d) * p1 + d * p2;
            resulting_vertices.push_back(vertex.x());
            resulting_vertices.push_back(vertex.y());
        }
    }
}

/*void MarchingSquaresSolver::calculateCellValue(Cell * const cell, float threshold) const
{
    int value = 0;
    float f = 0.f;

    f = calculateVertexValue(cell->vertexBottomLeft());
    if (f >= threshold)
        value |= 0x1;
    f = calculateVertexValue(cell->vertexBottomRight());
    if (f >= threshold)
        value |= 0x1 << 1;
    f = calculateVertexValue(cell->vertexTopRight());
    if (f >= threshold)
        value |= 0x1 << 2;
    f = calculateVertexValue(cell->vertexTopLeft());
    if (f >= threshold)
        value |= 0x1 << 3;

    cell->setValue(value);
}*/

float MarchingSquaresSolver::calculateVertexValue(const QVector2D &vertex, const QVector<AbstractMetaball*>& metaballs) const
{
    unsigned int nMetaballs = metaballs.size();
    float value = 0.f;
    for (unsigned int j = 0; j< nMetaballs;++j)
    {
        AbstractMetaball *metaball = metaballs[j];
        value += metaball->evaluate(vertex);
    }
    return value;
}
