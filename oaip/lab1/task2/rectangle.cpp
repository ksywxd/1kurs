#include "Rectangle.h"
#include <cmath>
#include <stdexcept>

std::vector<QPointF> Rectangle::computeVertices(const QPointF& center, double width, double height, double angleDeg)
{
    double w2 = width / 2.0;
    double h2 = height / 2.0;
    std::vector<QPointF> local = {
        QPointF(-w2, -h2),
        QPointF( w2, -h2),
        QPointF( w2,  h2),
        QPointF(-w2,  h2)
    };
    double rad = qDegreesToRadians(angleDeg);
    double cosA = std::cos(rad);
    double sinA = std::sin(rad);
    for (auto& p : local) {
        double x = p.x();
        double y = p.y();
        p.setX(x * cosA - y * sinA + center.x());
        p.setY(x * sinA + y * cosA + center.y());
    }
    return local;
}

Rectangle::Rectangle(const QPointF& center, double width, double height, double angleDeg)
    : Polygon(computeVertices(center, width, height, angleDeg)),
    m_center(center), m_width(width), m_height(height), m_angle(angleDeg)
{
    if (width <= 0 || height <= 0)
        throw std::invalid_argument("Width and height must be positive");
}

std::shared_ptr<Shape> Rectangle::clone() const
{
    return std::make_shared<Rectangle>(*this);
}

void Rectangle::updateVertices()
{
    m_vertices = computeVertices(m_center, m_width, m_height, m_angle);
}

void Rectangle::setWidth(double w)
{
    if (w <= 0) throw std::invalid_argument("Width must be positive");
    m_width = w;
    updateVertices();
}

void Rectangle::setHeight(double h)
{
    if (h <= 0) throw std::invalid_argument("Height must be positive");
    m_height = h;
    updateVertices();
}

void Rectangle::setAngle(double angleDeg)
{
    m_angle = angleDeg;
    updateVertices();
}

QMap<QString, double> Rectangle::parameters() const
{
    QMap<QString, double> params;
    params["center_x"] = m_center.x();
    params["center_y"] = m_center.y();
    params["width"] = m_width;
    params["height"] = m_height;
    params["angle"] = m_angle;
    return params;
}

void Rectangle::setParameter(const QString& name, double value)
{
    if (name == "center_x") {
        m_center.setX(value);
        updateVertices();
    } else if (name == "center_y") {
        m_center.setY(value);
        updateVertices();
    } else if (name == "width") {
        setWidth(value);
    } else if (name == "height") {
        setHeight(value);
    } else if (name == "angle") {
        setAngle(value);
    } else {
        throw std::invalid_argument("Unknown parameter");
    }
}

QJsonObject Rectangle::toJson() const
{
    QJsonObject obj;
    obj["type"] = "Rectangle";
    obj["center_x"] = m_center.x();
    obj["center_y"] = m_center.y();
    obj["width"] = m_width;
    obj["height"] = m_height;
    obj["angle"] = m_angle;
    return obj;
}

void Rectangle::fromJson(const QJsonObject& json)
{
    m_center.setX(json["center_x"].toDouble());
    m_center.setY(json["center_y"].toDouble());
    m_width = json["width"].toDouble();
    m_height = json["height"].toDouble();
    m_angle = json["angle"].toDouble();
    updateVertices();
}
