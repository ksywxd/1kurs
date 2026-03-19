#include "Circle.h"
#include <QPainter>
#include <cmath>
#include <stdexcept>

Circle::Circle(const QPointF& center, double radius)
    : m_center(center), m_radius(radius)
{
    if (radius <= 0)
        throw std::invalid_argument("Radius must be positive");
}

double Circle::area() const
{
    return M_PI * m_radius * m_radius;
}

double Circle::perimeter() const
{
    return 2 * M_PI * m_radius;
}

QPointF Circle::centerOfMass() const
{
    return m_center;
}

void Circle::setCenterOfMass(const QPointF& newCenter)
{
    m_center = newCenter;
}

void Circle::translate(const QPointF& delta)
{
    m_center += delta;
}

void Circle::rotate(double, const QPointF&)
{
    // Circle is rotation-invariant
}

void Circle::scale(double factor, const QPointF& pivot)
{
    if (factor <= 0)
        throw std::invalid_argument("Scale factor must be positive");
    QPointF rel = m_center - pivot;
    m_center = pivot + rel * factor;
    m_radius *= factor;
}

void Circle::draw(QPainter& painter) const
{
    painter.drawEllipse(m_center, m_radius, m_radius);
}

std::shared_ptr<Shape> Circle::clone() const
{
    return std::make_shared<Circle>(*this);
}

void Circle::setRadius(double r)
{
    if (r <= 0)
        throw std::invalid_argument("Radius must be positive");
    m_radius = r;
}

QMap<QString, double> Circle::parameters() const
{
    QMap<QString, double> params;
    params["center_x"] = m_center.x();
    params["center_y"] = m_center.y();
    params["radius"] = m_radius;
    return params;
}

void Circle::setParameter(const QString& name, double value)
{
    if (name == "center_x")
        m_center.setX(value);
    else if (name == "center_y")
        m_center.setY(value);
    else if (name == "radius")
        setRadius(value);
    else
        throw std::invalid_argument("Unknown parameter");
}

QJsonObject Circle::toJson() const
{
    QJsonObject obj;
    obj["type"] = "Circle";
    obj["center_x"] = m_center.x();
    obj["center_y"] = m_center.y();
    obj["radius"] = m_radius;
    return obj;
}

void Circle::fromJson(const QJsonObject& json)
{
    m_center.setX(json["center_x"].toDouble());
    m_center.setY(json["center_y"].toDouble());
    m_radius = json["radius"].toDouble();
}
