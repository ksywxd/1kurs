#include "Canvas.h"
#include <QPainter>

Canvas::Canvas(QWidget* parent) : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

void Canvas::addShape(std::shared_ptr<Shape> shape)
{
    m_shapes.push_back(shape);
    update();
}

void Canvas::removeShape(std::shared_ptr<Shape> shape)
{
    auto it = std::find(m_shapes.begin(), m_shapes.end(), shape);
    if (it != m_shapes.end()) {
        m_shapes.erase(it);
        update();
    }
}

void Canvas::clearShapes()
{
    m_shapes.clear();
    update();
}

void Canvas::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::lightGray);

    for (const auto& shape : m_shapes) {
        shape->draw(painter);
    }
}
