#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Polygon.h"
#include <QtMath>

class Rectangle : public Polygon
{
public:
    Rectangle(const QPointF& center, double width, double height, double angleDeg = 0);
    ~Rectangle() override = default;

    QString name() const override { return "Rectangle"; }
    std::shared_ptr<Shape> clone() const override;
    double width() const { return m_width; }
    double height() const { return m_height; }
    double angle() const { return m_angle; }
    void setWidth(double w);
    void setHeight(double h);
    void setAngle(double angleDeg);
    QMap<QString, double> parameters() const override;
    void setParameter(const QString& name, double value) override;
    QJsonObject toJson() const override;
    void fromJson(const QJsonObject& json) override;

private:
    void updateVertices();
    static std::vector<QPointF> computeVertices(const QPointF& center, double width, double height, double angleDeg);

    QPointF m_center;
    double m_width;
    double m_height;
    double m_angle;
};

#endif // RECTANGLE_H
