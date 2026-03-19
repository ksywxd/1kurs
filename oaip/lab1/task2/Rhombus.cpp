#include "Rhombus.h"
#include <cmath>
#include <stdexcept>

std::vector<QPointF> Rhombus::computeVertices(const QPointF& center, double diag1, double diag2, double angleDeg)
{
    double d1h = diag1 / 2.0;
    double d2h = diag2 / 2.0;
    std::vector<QPointF> local = {
        QPointF( d1h, 0),
        QPointF( 0, d2h),
        QPointF(-d1h, 0),
        QPointF( 0, -d2h)
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

Rhombus::Rhombus(const QPointF& center, double diag1, double diag2, double angleDeg)
    : Polygon(computeVertices(center, diag1, diag2, angleDeg)),
    m_center(center), m_diag1(diag1), m_diag2(diag2), m_angle(angleDeg)
{
    if (diag1 <= 0 || diag2 <= 0)
        throw std::invalid_argument("Diagonals must be positive");
}

std::shared_ptr<Shape> Rhombus::clone() const
{
    return std::make_shared<Rhombus>(*this);
}

void Rhombus::updateVertices()
{
    m_vertices = computeVertices(m_center, m_diag1, m_diag2, m_angle);
}

void Rhombus::setDiag1(double d)
{
    if (d <= 0) throw std::invalid_argument("Diagonal must be positive");
    m_diag1 = d;
    updateVertices();
}

void Rhombus::setDiag2(double d)
{
    if (d <= 0) throw std::invalid_argument("Diagonal must be positive");
    m_diag2 = d;
    updateVertices();
}

void Rhombus::setAngle(double angleDeg)
{
    m_angle = angleDeg;
    updateVertices();
}

QMap<QString, double> Rhombus::parameters() const
{
    QMap<QString, double> params;
    params["center_x"] = m_center.x();
    params["center_y"] = m_center.y();
    params["diag1"] = m_diag1;
    params["diag2"] = m_diag2;
    params["angle"] = m_angle;
    return params;
}

void Rhombus::setParameter(const QString& name, double value)
{
    if (name == "center_x") {
        m_center.setX(value);
        updateVertices();
    } else if (name == "center_y") {
        m_center.setY(value);
        updateVertices();
    } else if (name == "diag1") {
        setDiag1(value);
    } else if (name == "diag2") {
        setDiag2(value);
    } else if (name == "angle") {
        setAngle(value);
    } else {
        throw std::invalid_argument("Unknown parameter");
    }
}

QJsonObject Rhombus::toJson() const
{
    QJsonObject obj;
    obj["type"] = "Rhombus";
    obj["center_x"] = m_center.x();
    obj["center_y"] = m_center.y();
    obj["diag1"] = m_diag1;
    obj["diag2"] = m_diag2;
    obj["angle"] = m_angle;
    return obj;
}

void Rhombus::fromJson(const QJsonObject& json)
{
    m_center.setX(json["center_x"].toDouble());
    m_center.setY(json["center_y"].toDouble());
    m_diag1 = json["diag1"].toDouble();
    m_diag2 = json["diag2"].toDouble();
    m_angle = json["angle"].toDouble();
    updateVertices();
}
