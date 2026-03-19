#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <memory>
#include <vector>
#include "Shape.h"

class Canvas : public QWidget
{
    Q_OBJECT

public:
    explicit Canvas(QWidget* parent = nullptr);

    void addShape(std::shared_ptr<Shape> shape);
    void removeShape(std::shared_ptr<Shape> shape);
    void clearShapes();
    const std::vector<std::shared_ptr<Shape>>& shapes() const { return m_shapes; }

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    std::vector<std::shared_ptr<Shape>> m_shapes;
};

#endif // CANVAS_H
