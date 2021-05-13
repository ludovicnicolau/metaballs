#include "marchingsquaressolver.h"

#include "abstractmetaball.h"
#include "grid.h"

MarchingSquaresSolver::MarchingSquaresSolver(Grid *grid)
    : m_grid(grid)
{

}

void MarchingSquaresSolver::addMetaball(AbstractMetaball *metaball)
{
    m_metaballs.push_back(metaball);
}

void MarchingSquaresSolver::solve(float threshold)
{
    QVector<float> vector;

    unsigned int nCells = m_grid->numberOfCells();

    QVector<float> resulting_vertices; // Vertices that will be created with marching squares
    for (unsigned int i = 0;i < nCells;++i)
    {
        Cell *cell = m_grid->cellAt(i);
        calculateCellValue(cell, threshold, resulting_vertices);



    }
}

void MarchingSquaresSolver::calculateCellValue(Cell * const cell, float threshold, QVector<float> &resulting_vertices) const
{
    short isIn = 0x0;

    std::array<QVector2D, 4> vertices = cell->vertices();
    for (int i = 0;i < 4;++i)
    {
        float f = calculateVertexValue(vertices[i]);
        if (f >= threshold)
        {
            isIn |= 0x1 << i;
        }
    }

    for (int i = 0;i < 4;++i)
    {
        bool inOut = ((isIn & 0x1 << i) && !(isIn & 0x1 << (i+1%4))) || (!(isIn & 0x1 << i) && (isIn & 0x1 << (i+1%4)));
        if (inOut)
        {
            QVector2D vertex = (vertices[i] + vertices[(i+1)%4]) / 2.0f;
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

float MarchingSquaresSolver::calculateVertexValue(const QVector2D &vertex) const
{
    unsigned int nMetaballs = m_metaballs.size();
    float value = 0.f;
    for (unsigned int j = 0; j< nMetaballs;++j)
    {
        AbstractMetaball *metaball = m_metaballs[j];
        value += metaball->evaluate(vertex);
    }
    return value;
}
