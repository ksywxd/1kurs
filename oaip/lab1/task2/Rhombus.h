#ifndef RHOMBUS_H
#define RHOMBUS_H

#include "Polygon.h"
#include <QtMath>

class Rhombus : public Polygon
{
public:
    Rhombus(const QPointF& center, double diag1, double diag2, double angleDeg = 0);
    ~Rhombus() override = default;

    QString name() const override { return "Rhombus"; }
    std::shared_ptr<Shape> clone() const override;
    double diag1() const { return m_diag1; }
    double diag2() const { return m_diag2; }
    double angle() const { return m_angle; }
    void setDiag1(double d);
    void setDiag2(double d);
    void setAngle(double angleDeg);
    QMap<QString, double> parameters() const override;
    void setParameter(const QString& name, double value) override;
    QJsonObject toJson() const override;
    void fromJson(const QJsonObject& json) override;

private:
    void updateVertices();
    static std::vector<QPointF> computeVertices(const QPointF& center, double diag1, double diag2, double angleDeg);

    QPointF m_center;
    double m_diag1;
    double m_diag2;
    double m_angle;
};

#endif // RHOMBUS_H
