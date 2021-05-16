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
    QVector<float> solve(float threshold, const QVector<AbstractMetaball*>& metaballs);


private:
    // void calculateCellValue(Cell * const cell, float threshold) const;
    void calculateCellValue(Cell * const cell, float threshold,
                            QVector<float>& resulting_vertices,
                            const QVector<AbstractMetaball*>& metaballs) const;
    float calculateVertexValue(const QVector2D &vertex, const QVector<AbstractMetaball*>& metaballs) const;

    Grid *m_grid;
};

inline void MarchingSquaresSolver::setGrid(Grid *grid)
{
    m_grid = grid;
}

#endif // MARCHINGSQUARESSOLVER_H
