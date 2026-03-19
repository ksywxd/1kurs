#include "Ellipse.h"
#include <QPainter>
#include <cmath>
#include <stdexcept>

Ellipse::Ellipse(const QPointF& center, double rx, double ry, double angleDeg)
    : m_center(center), m_rx(rx), m_ry(ry), m_angle(angleDeg)
{
    if (rx <= 0 || ry <= 0) throw std::invalid_argument("Radius must be positive");
}

double Ellipse::area() const
{
    return M_PI * m_rx * m_ry;
}

double Ellipse::perimeter() const
{
    double h = std::pow((m_rx - m_ry) / (m_rx + m_ry), 2);
    return M_PI * (m_rx + m_ry) * (1 + 3 * h / (10 + std::sqrt(4 - 3 * h)));
}

QPointF Ellipse::centerOfMass() const
{
    return m_center;
}

void Ellipse::setCenterOfMass(const QPointF& newCenter)
{
    m_center = newCenter;
}

void Ellipse::translate(const QPointF& delta)
{
    m_center += delta;
}

void Ellipse::rotate(double angleDeg, const QPointF& pivot)
{
    double rad = qDegreesToRadians(angleDeg);
    double cosA = std::cos(rad);
    double sinA = std::sin(rad);
    QPointF rel = m_center - pivot;
    m_center.setX(pivot.x() + rel.x() * cosA - rel.y() * sinA);
    m_center.setY(pivot.y() + rel.x() * sinA + rel.y() * cosA);
    m_angle += angleDeg;
}

void Ellipse::scale(double factor, const QPointF& pivot)
{
    if (factor <= 0) throw std::invalid_argument("Scale factor must be positive");
    QPointF rel = m_center - pivot;
    m_center = pivot + rel * factor;
    m_rx *= factor;
    m_ry *= factor;
}

void Ellipse::draw(QPainter& painter) const
{
    painter.save();
    painter.translate(m_center);
    painter.rotate(m_angle);
    painter.drawEllipse(QPointF(0, 0), m_rx, m_ry);
    painter.restore();
}

std::shared_ptr<Shape> Ellipse::clone() const
{
    return std::make_shared<Ellipse>(*this);
}

void Ellipse::setRx(double r)
{
    if (r <= 0) throw std::invalid_argument("Rx must be positive");
    m_rx = r;
}

void Ellipse::setRy(double r)
{
    if (r <= 0) throw std::invalid_argument("Ry must be positive");
    m_ry = r;
}

void Ellipse::setAngle(double angleDeg)
{
    m_angle = angleDeg;
}

QMap<QString, double> Ellipse::parameters() const
{
    QMap<QString, double> params;
    params["center_x"] = m_center.x();
    params["center_y"] = m_center.y();
    params["rx"] = m_rx;
    params["ry"] = m_ry;
    params["angle"] = m_angle;
    return params;
}

void Ellipse::setParameter(const QString& name, double value)
{
    if (name == "center_x") {
        m_center.setX(value);
    } else if (name == "center_y") {
        m_center.setY(value);
    } else if (name == "rx") {
        setRx(value);
    } else if (name == "ry") {
        setRy(value);
    } else if (name == "angle") {
        setAngle(value);
    } else {
        throw std::invalid_argument("Unknown parameter");
    }
}

QJsonObject Ellipse::toJson() const
{
    QJsonObject obj;
    obj["type"] = "Ellipse";
    obj["center_x"] = m_center.x();
    obj["center_y"] = m_center.y();
    obj["rx"] = m_rx;
    obj["ry"] = m_ry;
    obj["angle"] = m_angle;
    return obj;
}

void Ellipse::fromJson(const QJsonObject& json)
{
    m_center.setX(json["center_x"].toDouble());
    m_center.setY(json["center_y"].toDouble());
    m_rx = json["rx"].toDouble();
    m_ry = json["ry"].toDouble();
    m_angle = json["angle"].toDouble();
}
