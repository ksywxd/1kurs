#include "Square.h"
#include <cmath>
#include <stdexcept>

Square::Square(const QPointF& center, double side, double angleDeg)
    : RegularPolygon(center, side * std::sqrt(2)/2, 4, angleDeg),
    m_side(side)
{
    if (side <= 0)
        throw std::invalid_argument("Side must be positive");
}

std::shared_ptr<Shape> Square::clone() const
{
    return std::make_shared<Square>(*this);
}

void Square::setSide(double s)
{
    if (s <= 0) throw std::invalid_argument("Side must be positive");
    m_side = s;
    setRadius(s * std::sqrt(2) / 2);
}

QMap<QString, double> Square::parameters() const
{
    QMap<QString, double> params = RegularPolygon::parameters();
    params["side"] = m_side;
    return params;
}

void Square::setParameter(const QString& name, double value)
{
    if (name == "side")
        setSide(value);
    else
        RegularPolygon::setParameter(name, value);
}

QJsonObject Square::toJson() const
{
    QJsonObject obj;
    obj["type"] = "Square";
    obj["center_x"] = m_center.x();
    obj["center_y"] = m_center.y();
    obj["side"] = m_side;
    obj["start_angle"] = startAngle();
    return obj;
}

void Square::fromJson(const QJsonObject& json)
{
    m_center.setX(json["center_x"].toDouble());
    m_center.setY(json["center_y"].toDouble());
    m_side = json["side"].toDouble();
    setRadius(m_side * std::sqrt(2) / 2);
    setStartAngle(json["start_angle"].toDouble());
}
