#ifndef MURAKAMIMETABALL_H
#define MURAKAMIMETABALL_H

#include "abstractmetaball.h"

class MurakamiMetaball : public AbstractMetaball
{
public:
    MurakamiMetaball(const QVector2D& position, float radius);

    ~MurakamiMetaball() override = default;

    bool isIn(const QVector2D& point) const override;
};

#endif // MURAKAMIMETABALL_H
