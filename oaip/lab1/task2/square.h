#ifndef SQUARE_H
#define SQUARE_H

#include "RegularPolygon.h"
#include <QtMath>

class Square : public RegularPolygon
{
public:
    Square(const QPointF& center, double side, double angleDeg = 0);
    ~Square() override = default;

    QString name() const override { return "Square"; }
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

#endif // SQUARE_H
