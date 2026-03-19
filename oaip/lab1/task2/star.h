#ifndef STAR_H
#define STAR_H

#include "Polygon.h"
#include <QtMath>

class Star : public Polygon
{
public:
    Star(const QPointF& center, int rays, double outerRadius, double innerRadius, double startAngleDeg = 0);
    ~Star() override = default;

    QString name() const override { return "Star"; }
    std::shared_ptr<Shape> clone() const override;
    int rays() const { return m_rays; }
    double outerRadius() const { return m_outerRadius; }
    double innerRadius() const { return m_innerRadius; }
    double startAngle() const { return m_startAngle; }
    void setRays(int r);
    void setOuterRadius(double r);
    void setInnerRadius(double r);
    void setStartAngle(double angleDeg);
    QMap<QString, double> parameters() const override;
    void setParameter(const QString& name, double value) override;
    QJsonObject toJson() const override;
    void fromJson(const QJsonObject& json) override;

private:
    void updateVertices();
    static std::vector<QPointF> computeVertices(const QPointF& center, int rays, double outerRadius, double innerRadius, double startAngleDeg);

    QPointF m_center;
    int m_rays;
    double m_outerRadius;
    double m_innerRadius;
    double m_startAngle;
};

#endif // STAR_H
