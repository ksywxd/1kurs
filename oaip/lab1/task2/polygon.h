#ifndef POLYGON_H
#define POLYGON_H

#include "Shape.h"
#include <vector>
#include <QtMath>

class Polygon : public Shape
{
public:
    Polygon(const std::vector<QPointF>& vertices);
    ~Polygon() override = default;

    double area() const override;
    double perimeter() const override;
    QPointF centerOfMass() const override;
    void setCenterOfMass(const QPointF& newCenter) override;
    void translate(const QPointF& delta) override;
    void rotate(double angleDeg, const QPointF& pivot) override;
    void scale(double factor, const QPointF& pivot) override;
    void draw(QPainter& painter) const override;

    const std::vector<QPointF>& getVertices() const { return m_vertices; }
    void setVertices(const std::vector<QPointF>& vertices);

protected:
    std::vector<QPointF> m_vertices;
};

#endif // POLYGON_H
