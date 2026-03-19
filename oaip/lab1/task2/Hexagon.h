#ifndef HEXAGON_H
#define HEXAGON_H

#include "RegularPolygon.h"

class Hexagon : public RegularPolygon
{
public:
    Hexagon(const QPointF& center, double radius, double startAngleDeg = 0);
    ~Hexagon() override = default;

    QString name() const override { return "Hexagon"; }
    std::shared_ptr<Shape> clone() const override;
    QJsonObject toJson() const override;
    void fromJson(const QJsonObject& json) override;
};

#endif // HEXAGON_H
