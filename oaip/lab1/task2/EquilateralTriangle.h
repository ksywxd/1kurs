#ifndef EQUILATERALTRIANGLE_H
#define EQUILATERALTRIANGLE_H

#include "RegularPolygon.h"

class EquilateralTriangle : public RegularPolygon
{
public:
    EquilateralTriangle(const QPointF& center, double side, double startAngleDeg = 0);
    ~EquilateralTriangle() override = default;

    QString name() const override { return "Equilateral Triangle"; }
    std::shared_ptr<Shape> clone() const override;
    double side() const { return m_side; }
    void setSide(double s);
    QMap<QString, double> parameters() const override;
    void setParameter(const QString& name, double value) override;
    QJsonObject toJson() const override;
    void fromJson(const QJsonObject& json) override;

private:
    double m_side;
};

#endif // EQUILATERALTRIANGLE_H
