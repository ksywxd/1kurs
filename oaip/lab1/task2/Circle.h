#ifndef CIRCLE_H
#define CIRCLE_H

#include "Shape.h"

class Circle : public Shape
{
public:
    Circle(const QPointF& center, double radius);
    ~Circle() override = default;

    double area() const override;
    double perimeter() const override;
    QPointF centerOfMass() const override;
    void setCenterOfMass(const QPointF& newCenter) override;
    void translate(const QPointF& delta) override;
    void rotate(double angleDeg, const QPointF& pivot) override;
    void scale(double factor, const QPointF& pivot) override;
    void draw(QPainter& painter) const override;
    QString name() const override { return "Circle"; }
    std::shared_ptr<Shape> clone() const override;
    double radius() const { return m_radius; }
    void setRadius(double r);
    QMap<QString, double> parameters() const override;
    void setParameter(const QString& name, double value) override;
    QJsonObject toJson() const override;
    void fromJson(const QJsonObject& json) override;

private:
    QPointF m_center;
    double m_radius;
};

#endif // CIRCLE_H
