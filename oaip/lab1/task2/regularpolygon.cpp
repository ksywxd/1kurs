#include "RegularPolygon.h"
#include <cmath>
#include <stdexcept>

std::vector<QPointF> RegularPolygon::computeVertices(const QPointF& center, double radius, int sides, double startAngleDeg)
{
    std::vector<QPointF> verts;
    double step = 2 * M_PI / sides;
    double startRad = qDegreesToRadians(startAngleDeg);
    for (int i = 0; i < sides; ++i) {
        double angle = startRad + i * step;
        verts.emplace_back(center.x() + radius * std::cos(angle),
                           center.y() + radius * std::sin(angle));
    }
    return verts;
}

RegularPolygon::RegularPolygon(const QPointF& center, double radius, int sides, double startAngleDeg)
    : Polygon(computeVertices(center, radius, sides, startAngleDeg)),
    m_center(center), m_radius(radius), m_sides(sides), m_startAngle(startAngleDeg)
{
    if (sides < 3) throw std::invalid_argument("Regular polygon must have at least 3 sides");
    if (radius <= 0) throw std::invalid_argument("Radius must be positive");
}

std::shared_ptr<Shape> RegularPolygon::clone() const
{
    return std::make_shared<RegularPolygon>(*this);
}

void RegularPolygon::setRadius(double r)
{
    if (r <= 0) throw std::invalid_argument("Radius must be positive");
    m_radius = r;
    updateVertices();
}

void RegularPolygon::setStartAngle(double angleDeg)
{
    m_startAngle = angleDeg;
    updateVertices();
}

void RegularPolygon::updateVertices()
{
    m_vertices = computeVertices(m_center, m_radius, m_sides, m_startAngle);
}

QMap<QString, double> RegularPolygon::parameters() const
{
    QMap<QString, double> params;
    params["center_x"] = m_center.x();
    params["center_y"] = m_center.y();
    params["radius"] = m_radius;
    params["sides"] = m_sides;
    params["start_angle"] = m_startAngle;
    return params;
}

void RegularPolygon::setParameter(const QString& name, double value)
{
    if (name == "center_x") {
        m_center.setX(value);
        updateVertices();
    } else if (name == "center_y") {
        m_center.setY(value);
        updateVertices();
    } else if (name == "radius") {
        setRadius(value);
    } else if (name == "start_angle") {
        setStartAngle(value);
    } else if (name == "sides") {
        throw std::invalid_argument("Cannot change number of sides");
    } else {
        throw std::invalid_argument("Unknown parameter");
    }
}

QJsonObject RegularPolygon::toJson() const
{
    QJsonObject obj;
    obj["type"] = "RegularPolygon";
    obj["center_x"] = m_center.x();
    obj["center_y"] = m_center.y();
    obj["radius"] = m_radius;
    obj["sides"] = m_sides;
    obj["start_angle"] = m_startAngle;
    return obj;
}

void RegularPolygon::fromJson(const QJsonObject& json)
{
    m_center.setX(json["center_x"].toDouble());
    m_center.setY(json["center_y"].toDouble());
    m_radius = json["radius"].toDouble();
    m_sides = json["sides"].toInt();
    m_startAngle = json["start_angle"].toDouble();
    updateVertices();
}
