#include "Hexagon.h"
#include <cmath>

Hexagon::Hexagon(const QPointF& center, double radius, double startAngleDeg)
    : RegularPolygon(center, radius, 6, startAngleDeg)
{
}

std::shared_ptr<Shape> Hexagon::clone() const
{
    return std::make_shared<Hexagon>(*this);
}

QJsonObject Hexagon::toJson() const
{
    QJsonObject obj;
    obj["type"] = "Hexagon";
    obj["center_x"] = m_center.x();
    obj["center_y"] = m_center.y();
    obj["radius"] = radius();
    obj["start_angle"] = startAngle();
    return obj;
}

void Hexagon::fromJson(const QJsonObject& json)
{
    m_center.setX(json["center_x"].toDouble());
    m_center.setY(json["center_y"].toDouble());
    setRadius(json["radius"].toDouble());
    setStartAngle(json["start_angle"].toDouble());
}
