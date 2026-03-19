#include "Animator.h"
#include <QDateTime>
#include <cmath>

Animator::Animator(std::shared_ptr<Shape> shape, TransformType type,
                   const QVariant& target, int durationMs, QObject* parent)
    : QObject(parent), m_shape(shape), m_type(type), m_target(target), m_duration(durationMs),
    m_timer(new QTimer(this)), m_elapsed(0)
{
    m_timer->setInterval(50);
    connect(m_timer, &QTimer::timeout, this, &Animator::onTimerTick);
    m_pivot = shape->centerOfMass();
}

void Animator::start()
{
    m_elapsed = 0;
    m_timer->start();
}

void Animator::stop()
{
    m_timer->stop();
    emit finished();
}

void Animator::onTimerTick()
{
    m_elapsed += m_timer->interval();
    if (m_elapsed >= m_duration) {
        if (m_type == Translate) {
            QPointF delta = m_target.value<QPointF>();
            m_shape->translate(delta);
        } else if (m_type == Rotate) {
            double angle = m_target.toDouble();
            m_shape->rotate(angle, m_pivot);
        } else if (m_type == Scale) {
            double factor = m_target.toDouble();
            m_shape->scale(factor, m_pivot);
        }
        stop();
        emit step();
        return;
    }

    double progress = static_cast<double>(m_elapsed) / m_duration;

    if (m_type == Translate) {
        QPointF totalDelta = m_target.value<QPointF>();
        QPointF stepDelta = totalDelta * (m_timer->interval() / static_cast<double>(m_duration));
        m_shape->translate(stepDelta);
    } else if (m_type == Rotate) {
        double totalAngle = m_target.toDouble();
        double stepAngle = totalAngle * (m_timer->interval() / static_cast<double>(m_duration));
        m_shape->rotate(stepAngle, m_pivot);
    } else if (m_type == Scale) {
        double totalFactor = m_target.toDouble();
        static double prevDesired = 1.0;
        if (m_elapsed == m_timer->interval()) {
            prevDesired = 1.0;
        }
        double currentDesired = 1.0 + progress * (totalFactor - 1.0);
        double stepFactor = currentDesired / prevDesired;
        m_shape->scale(stepFactor, m_pivot);
        prevDesired = currentDesired;
    }

    emit step();
}
