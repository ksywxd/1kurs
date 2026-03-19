#include "Star.h"
#include <cmath>
#include <stdexcept>

std::vector<QPointF> Star::computeVertices(const QPointF& center, int rays, double outerRadius, double innerRadius, double startAngleDeg)
{
    std::vector<QPointF> verts;
    double step = M_PI / rays;
    double startRad = qDegreesToRadians(startAngleDeg);
    for (int i = 0; i < 2 * rays; ++i) {
        double angle = startRad + i * step;
        double r = (i % 2 == 0) ? outerRadius : innerRadius;
        verts.emplace_back(center.x() + r * std::cos(angle),
                           center.y() + r * std::sin(angle));
    }
    return verts;
}

Star::Star(const QPointF& center, int rays, double outerRadius, double innerRadius, double startAngleDeg)
    : Polygon(computeVertices(center, rays, outerRadius, innerRadius, startAngleDeg)),
    m_center(center), m_rays(rays), m_outerRadius(outerRadius), m_innerRadius(innerRadius), m_startAngle(startAngleDeg)
{
    if (rays < 3) throw std::invalid_argument("Star must have at least 3 rays");
    if (outerRadius <= 0 || innerRadius <= 0) throw std::invalid_argument("Radii must be positive");
}

std::shared_ptr<Shape> Star::clone() const
{
    return std::make_shared<Star>(*this);
}

void Star::updateVertices()
{
    m_vertices = computeVertices(m_center, m_rays, m_outerRadius, m_innerRadius, m_startAngle);
}

void Star::setRays(int r)
{
    if (r < 3) throw std::invalid_argument("Rays must be at least 3");
    m_rays = r;
    updateVertices();
}

void Star::setOuterRadius(double r)
{
    if (r <= 0) throw std::invalid_argument("Outer radius must be positive");
    m_outerRadius = r;
    updateVertices();
}

void Star::setInnerRadius(double r)
{
    if (r <= 0) throw std::invalid_argument("Inner radius must be positive");
    m_innerRadius = r;
    updateVertices();
}

void Star::setStartAngle(double angleDeg)
{
    m_startAngle = angleDeg;
    updateVertices();
}

QMap<QString, double> Star::parameters() const
{
    QMap<QString, double> params;
    params["center_x"] = m_center.x();
    params["center_y"] = m_center.y();
    params["rays"] = m_rays;
    params["outer_radius"] = m_outerRadius;
    params["inner_radius"] = m_innerRadius;
    params["start_angle"] = m_startAngle;
    return params;
}

void Star::setParameter(const QString& name, double value)
{
    if (name == "center_x") {
        m_center.setX(value);
        updateVertices();
    } else if (name == "center_y") {
        m_center.setY(value);
        updateVertices();
    } else if (name == "rays") {
        setRays(static_cast<int>(value));
    } else if (name == "outer_radius") {
        setOuterRadius(value);
    } else if (name == "inner_radius") {
        setInnerRadius(value);
    } else if (name == "start_angle") {
        setStartAngle(value);
    } else {
        throw std::invalid_argument("Unknown parameter");
    }
}

QJsonObject Star::toJson() const
{
    QJsonObject obj;
    obj["type"] = "Star";
    obj["center_x"] = m_center.x();
    obj["center_y"] = m_center.y();
    obj["rays"] = m_rays;
    obj["outer_radius"] = m_outerRadius;
    obj["inner_radius"] = m_innerRadius;
    obj["start_angle"] = m_startAngle;
    return obj;
}

void Star::fromJson(const QJsonObject& json)
{
    m_center.setX(json["center_x"].toDouble());
    m_center.setY(json["center_y"].toDouble());
    m_rays = json["rays"].toInt();
    m_outerRadius = json["outer_radius"].toDouble();
    m_innerRadius = json["inner_radius"].toDouble();
    m_startAngle = json["start_angle"].toDouble();
    updateVertices();
}
