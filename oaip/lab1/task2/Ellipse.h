#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "Shape.h"
#include <QtMath>

class Ellipse : public Shape
{
public:
    Ellipse(const QPointF& center, double rx, double ry, double angleDeg = 0);
    ~Ellipse() override = default;

    double area() const override;
    double perimeter() const override;
    QPointF centerOfMass() const override;
    void setCenterOfMass(const QPointF& newCenter) override;
    void translate(const QPointF& delta) override;
    void rotate(double angleDeg, const QPointF& pivot) override;
    void scale(double factor, const QPointF& pivot) override;
    void draw(QPainter& painter) const override;
    QString name() const override { return "Ellipse"; }
    std::shared_ptr<Shape> clone() const override;
    double rx() const { return m_rx; }
    double ry() const { return m_ry; }
    double angle() const { return m_angle; }
    void setRx(double r);
    void setRy(double r);
    void setAngle(double angleDeg);
    QMap<QString, double> parameters() const override;
    void setParameter(const QString& name, double value) override;
    QJsonObject toJson() const override;
    void fromJson(const QJsonObject& json) override;

private:
    QPointF m_center;
    double m_rx;
    double m_ry;
    double m_angle;
};

#endif // ELLIPSE_H
