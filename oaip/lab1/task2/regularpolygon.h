#ifndef REGULARPOLYGON_H
#define REGULARPOLYGON_H

#include "Polygon.h"
#include <QtMath>

class RegularPolygon : public Polygon
{
public:
    RegularPolygon(const QPointF& center, double radius, int sides, double startAngleDeg = 0);
    ~RegularPolygon() override = default;

    QString name() const override { return "RegularPolygon"; }
    std::shared_ptr<Shape> clone() const override;
    double radius() const { return m_radius; }
    int sides() const { return m_sides; }
    double startAngle() const { return m_startAngle; }
    void setRadius(double r);
    void setStartAngle(double angleDeg);
    QMap<QString, double> parameters() const override;
    void setParameter(const QString& name, double value) override;
    QJsonObject toJson() const override;
    void fromJson(const QJsonObject& json) override;

protected:
    void updateVertices();
    static std::vector<QPointF> computeVertices(const QPointF& center, double radius, int sides, double startAngleDeg);

    QPointF m_center;
    double m_radius;
    int m_sides;
    double m_startAngle;
};

#endif // REGULARPOLYGON_H
