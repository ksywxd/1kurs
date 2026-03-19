#include "Triangle.h"
#include <stdexcept>

Triangle::Triangle(const QPointF& p1, const QPointF& p2, const QPointF& p3)
    : Polygon({p1, p2, p3})
{
}

std::shared_ptr<Shape> Triangle::clone() const
{
    return std::make_shared<Triangle>(*this);
}

QMap<QString, double> Triangle::parameters() const
{
    QMap<QString, double> params;
    auto verts = getVertices();
    if (verts.size() >= 3) {
        params["x1"] = verts[0].x();
        params["y1"] = verts[0].y();
        params["x2"] = verts[1].x();
        params["y2"] = verts[1].y();
        params["x3"] = verts[2].x();
        params["y3"] = verts[2].y();
    }
    return params;
}

void Triangle::setParameter(const QString& name, double value)
{
    auto verts = getVertices();
    if (verts.size() < 3) verts.resize(3);

    if (name == "x1")
        verts[0].setX(value);
    else if (name == "y1")
        verts[0].setY(value);
    else if (name == "x2")
        verts[1].setX(value);
    else if (name == "y2")
        verts[1].setY(value);
    else if (name == "x3")
        verts[2].setX(value);
    else if (name == "y3")
        verts[2].setY(value);
    else
        throw std::invalid_argument("Unknown parameter");

    setVertices(verts);
}

QJsonObject Triangle::toJson() const
{
    QJsonObject obj;
    obj["type"] = "Triangle";
    auto verts = getVertices();
    if (verts.size() >= 3) {
        obj["x1"] = verts[0].x();
        obj["y1"] = verts[0].y();
        obj["x2"] = verts[1].x();
        obj["y2"] = verts[1].y();
        obj["x3"] = verts[2].x();
        obj["y3"] = verts[2].y();
    }
    return obj;
}

void Triangle::fromJson(const QJsonObject& json)
{
    std::vector<QPointF> verts(3);
    verts[0] = QPointF(json["x1"].toDouble(), json["y1"].toDouble());
    verts[1] = QPointF(json["x2"].toDouble(), json["y2"].toDouble());
    verts[2] = QPointF(json["x3"].toDouble(), json["y3"].toDouble());
    setVertices(verts);
}
