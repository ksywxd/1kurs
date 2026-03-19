#include "Polygon.h"
#include <QPolygonF>
#include <QPainterPath>
#include <cmath>
#include <stdexcept>

Polygon::Polygon(const std::vector<QPointF>& vertices) : m_vertices(vertices)
{
    if (vertices.size() < 3)
        throw std::invalid_argument("Polygon must have at least 3 vertices");
}

double Polygon::area() const
{
    double sum = 0.0;
    int n = m_vertices.size();
    for (int i = 0; i < n; ++i) {
        const QPointF& p1 = m_vertices[i];
        const QPointF& p2 = m_vertices[(i + 1) % n];
        sum += p1.x() * p2.y() - p2.x() * p1.y();
    }
    return std::abs(sum) / 2.0;
}

double Polygon::perimeter() const
{
    double perim = 0.0;
    int n = m_vertices.size();
    for (int i = 0; i < n; ++i) {
        const QPointF& p1 = m_vertices[i];
        const QPointF& p2 = m_vertices[(i + 1) % n];
        perim += std::hypot(p2.x() - p1.x(), p2.y() - p1.y());
    }
    return perim;
}

QPointF Polygon::centerOfMass() const
{
    double cx = 0.0, cy = 0.0;
    double A = area();
    if (A == 0) return m_vertices[0];
    int n = m_vertices.size();
    for (int i = 0; i < n; ++i) {
        const QPointF& p1 = m_vertices[i];
        const QPointF& p2 = m_vertices[(i + 1) % n];
        double cross = p1.x() * p2.y() - p2.x() * p1.y();
        cx += (p1.x() + p2.x()) * cross;
        cy += (p1.y() + p2.y()) * cross;
    }
    double factor = 1.0 / (6.0 * A);
    return QPointF(cx * factor, cy * factor);
}

void Polygon::setCenterOfMass(const QPointF& newCenter)
{
    QPointF old = centerOfMass();
    QPointF delta = newCenter - old;
    translate(delta);
}

void Polygon::translate(const QPointF& delta)
{
    for (auto& v : m_vertices)
        v += delta;
}

void Polygon::rotate(double angleDeg, const QPointF& pivot)
{
    double angleRad = qDegreesToRadians(angleDeg);
    double cosA = std::cos(angleRad);
    double sinA = std::sin(angleRad);
    for (auto& v : m_vertices) {
        QPointF rel = v - pivot;
        v.setX(pivot.x() + rel.x() * cosA - rel.y() * sinA);
        v.setY(pivot.y() + rel.x() * sinA + rel.y() * cosA);
    }
}

void Polygon::scale(double factor, const QPointF& pivot)
{
    if (factor <= 0)
        throw std::invalid_argument("Scale factor must be positive");
    for (auto& v : m_vertices) {
        QPointF rel = v - pivot;
        v = pivot + rel * factor;
    }
}

void Polygon::draw(QPainter& painter) const
{
    QPolygonF poly;
    for (const auto& p : m_vertices)
        poly << p;
    painter.drawPolygon(poly);
}

void Polygon::setVertices(const std::vector<QPointF>& vertices)
{
    if (vertices.size() < 3)
        throw std::invalid_argument("Polygon must have at least 3 vertices");
    m_vertices = vertices;
}
