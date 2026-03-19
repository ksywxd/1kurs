#ifndef SHAPE_H
#define SHAPE_H

#include <QPointF>
#include <QPainter>
#include <QString>
#include <QMap>
#include <QJsonObject>
#include <memory>

class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual QPointF centerOfMass() const = 0;
    virtual void setCenterOfMass(const QPointF& newCenter) = 0;
    virtual void translate(const QPointF& delta) = 0;
    virtual void rotate(double angleDeg, const QPointF& pivot) = 0;
    virtual void scale(double factor, const QPointF& pivot) = 0;
    virtual void draw(QPainter& painter) const = 0;
    virtual QString name() const = 0;
    virtual std::shared_ptr<Shape> clone() const = 0;
    virtual QMap<QString, double> parameters() const = 0;
    virtual void setParameter(const QString& name, double value) = 0;
    virtual QJsonObject toJson() const = 0;
    virtual void fromJson(const QJsonObject& json) = 0;
};

#endif // SHAPE_H
