#ifndef MARCHINGSQUARESSOLVER_H
#define MARCHINGSQUARESSOLVER_H

#include <QVector>
#include <QVector2D>

class Grid;
class AbstractMetaball;

class Cell;

class MarchingSquaresSolver
{
public:
    MarchingSquaresSolver(Grid *grid);

    inline void setGrid(Grid *grid);

    void addMetaball(AbstractMetaball *metaball);
    void solve(float threshold);


private:
    // void calculateCellValue(Cell * const cell, float threshold) const;
    void calculateCellValue(Cell * const cell, float threshold, QVector<float>& resulting_vertices) const;
    float calculateVertexValue(const QVector2D &vertex) const;

    Grid *m_grid;
    QVector<AbstractMetaball*> m_metaballs;

};

inline void MarchingSquaresSolver::setGrid(Grid *grid)
{
    m_grid = grid;
}

#endif // MARCHINGSQUARESSOLVER_H
