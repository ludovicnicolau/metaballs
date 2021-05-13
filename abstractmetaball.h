#ifndef ABSTRACTMETABALL_H
#define ABSTRACTMETABALL_H

#include <QVector2D>

class AbstractMetaball
{
public:
    AbstractMetaball(const QVector2D& position, float radius) noexcept;
    AbstractMetaball(const AbstractMetaball& other) noexcept = default;
    AbstractMetaball(AbstractMetaball&& other) noexcept = default;

    AbstractMetaball& operator=(const AbstractMetaball& other) noexcept = default;
    AbstractMetaball& operator=(AbstractMetaball&& other) noexcept = default;

    virtual ~AbstractMetaball() = default;

    inline QVector2D position() const noexcept;
    inline float radius() const noexcept;

    inline void setPosition(const QVector2D& position) noexcept;
    inline void setRadius(float radius) noexcept;

    virtual float evaluate(const QVector2D& point) const = 0;

protected:
    QVector2D m_position;
    float m_radius;
};

inline QVector2D AbstractMetaball::position() const noexcept
{
    return m_position;
}

inline float AbstractMetaball::radius() const noexcept
{
    return m_radius;
}

inline void AbstractMetaball::setPosition(const QVector2D &position) noexcept
{
    m_position = position;
}

inline void AbstractMetaball::setRadius(float radius) noexcept
{
    m_radius = radius;
}

#endif // ABSTRACTMETABALL_H
