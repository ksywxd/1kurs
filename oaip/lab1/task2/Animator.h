#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <QObject>
#include <QTimer>
#include <memory>
#include "Shape.h"
#include <QVariant>
#include <QPointF>

class Animator : public QObject
{
    Q_OBJECT

public:
    enum TransformType { Translate, Rotate, Scale };

    Animator(std::shared_ptr<Shape> shape, TransformType type,
             const QVariant& target, int durationMs, QObject* parent = nullptr);

signals:
    void step();
    void finished();

public slots:
    void start();
    void stop();

private slots:
    void onTimerTick();

private:
    std::shared_ptr<Shape> m_shape;
    TransformType m_type;
    QVariant m_target;
    int m_duration;
    QTimer* m_timer;
    int m_elapsed;
    QPointF m_pivot;
    QPointF m_initialCenter;
    double m_initialAngle;
    double m_initialScale;
};

#endif // ANIMATOR_H
