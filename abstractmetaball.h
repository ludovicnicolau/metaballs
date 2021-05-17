#ifndef ABSTRACTMETABALL_H
#define ABSTRACTMETABALL_H

#include <QVector2D>


enum class AbstractMetaballOperation
{
    Blend,
    Difference
};

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
    inline AbstractMetaballOperation operation() const noexcept;

    inline void setPosition(const QVector2D& position) noexcept;
    inline void setRadius(float radius) noexcept;
    inline void setOperation(AbstractMetaballOperation operation) noexcept;

    virtual float evaluate(const QVector2D& point) const = 0;

protected:
    QVector2D m_position;
    float m_radius;
    short m_operation_sign; // 1 if blend, 0 if difference
};

inline QVector2D AbstractMetaball::position() const noexcept
{
    return m_position;
}

inline float AbstractMetaball::radius() const noexcept
{
    return m_radius;
}

inline AbstractMetaballOperation AbstractMetaball::operation() const noexcept
{
    return m_operation_sign == 1 ? AbstractMetaballOperation::Blend : AbstractMetaballOperation::Difference;
}

inline void AbstractMetaball::setPosition(const QVector2D &position) noexcept
{
    m_position = position;
}

inline void AbstractMetaball::setRadius(float radius) noexcept
{
    m_radius = radius;
}

inline void AbstractMetaball::setOperation(AbstractMetaballOperation operation) noexcept
{
    switch(operation)
    {
    case AbstractMetaballOperation::Blend:
        m_operation_sign = 1;
        break;
    case AbstractMetaballOperation::Difference:
        m_operation_sign = -1;
        break;
    }
}

#endif // ABSTRACTMETABALL_H
