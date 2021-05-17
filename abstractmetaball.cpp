#include "abstractmetaball.h"

AbstractMetaball::AbstractMetaball(const QVector2D &position, float radius) noexcept
    : m_position(position), m_radius(radius), m_operation_sign(1)
{

}
