#include "EquilateralTriangle.h"
#include <cmath>
#include <stdexcept>

EquilateralTriangle::EquilateralTriangle(const QPointF& center, double side, double startAngleDeg)
    : RegularPolygon(center, side / std::sqrt(3), 3, startAngleDeg),
    m_side(side)
{
    if (side <= 0) throw std::invalid_argument("Side must be positive");
}

std::shared_ptr<Shape> EquilateralTriangle::clone() const
{
    return std::make_shared<EquilateralTriangle>(*this);
}

void EquilateralTriangle::setSide(double s)
{
    if (s <= 0) throw std::invalid_argument("Side must be positive");
    m_side = s;
    setRadius(s / std::sqrt(3));
}

QMap<QString, double> EquilateralTriangle::parameters() const
{
    QMap<QString, double> params = RegularPolygon::parameters();
    params["side"] = m_side;
    return params;
}

void EquilateralTriangle::setParameter(const QString& name, double value)
{
    if (name == "side")
        setSide(value);
    else
        RegularPolygon::setParameter(name, value);
}

QJsonObject EquilateralTriangle::toJson() const
{
    QJsonObject obj;
    obj["type"] = "EquilateralTriangle";
    obj["center_x"] = m_center.x();
    obj["center_y"] = m_center.y();
    obj["side"] = m_side;
    obj["start_angle"] = startAngle();
    return obj;
}

void EquilateralTriangle::fromJson(const QJsonObject& json)
{
    m_center.setX(json["center_x"].toDouble());
    m_center.setY(json["center_y"].toDouble());
    m_side = json["side"].toDouble();
    setRadius(m_side / std::sqrt(3));
    setStartAngle(json["start_angle"].toDouble());
}
