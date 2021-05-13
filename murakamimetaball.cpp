#include "murakamimetaball.h"

#include <QtMath>

MurakamiMetaball::MurakamiMetaball(const QVector2D& position, float radius)
    : AbstractMetaball(position, radius)
{

}

bool MurakamiMetaball::isIn(const QVector2D &point) const
{
    float r = m_position.distanceToPoint(point);
    if (r < 0.f || r > m_radius || m_radius == 0.f)
        return 0.f;
    return pow(1.f - pow(r / m_radius, 2.f), 2.f);
}
