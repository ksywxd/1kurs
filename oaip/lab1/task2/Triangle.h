#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Polygon.h"
#include <QtMath>

class Triangle : public Polygon
{
public:
    Triangle(const QPointF& p1, const QPointF& p2, const QPointF& p3);
    ~Triangle() override = default;

    QString name() const override { return "Triangle"; }
    std::shared_ptr<Shape> clone() const override;
    QMap<QString, double> parameters() const override;
    void setParameter(const QString& name, double value) override;
    QJsonObject toJson() const override;
    void fromJson(const QJsonObject& json) override;
};

#endif // TRIANGLE_H
