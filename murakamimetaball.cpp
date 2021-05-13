#include "murakamimetaball.h"

#include <QtMath>

MurakamiMetaball::MurakamiMetaball(const QVector2D& position, float radius)
    : AbstractMetaball(position, radius)
{

}

float MurakamiMetaball::evaluate(const QVector2D &point) const
{
    float r_2 = (m_position - point).lengthSquared();
    float radius_2 = m_radius * m_radius;
    if (r_2 > radius_2 || m_radius == 0.f)
        return 0.f;
    return pow(1.f - r_2 / radius_2, 2.f);
}
