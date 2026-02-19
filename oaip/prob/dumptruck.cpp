#include "dumptruck.h"
#include <QDebug>

DumpTruck::DumpTruck(int startX, int startY, QObject *parent)
    : MoveableRect(startX, startY, parent), m_bodyAngle(0)
{
}

void DumpTruck::raiseBody()
{
    qDebug() << "raiseBody: было" << m_bodyAngle;
    if (m_bodyAngle < 45) {
        m_bodyAngle += 15;
        qDebug() << "  стало" << m_bodyAngle;
        emit positionChanged();
    } else {
        qDebug() << "  уже максимум";
    }
}

void DumpTruck::lowerBody()
{
    qDebug() << "lowerBody: было" << m_bodyAngle;
    if (m_bodyAngle > 0) {
        m_bodyAngle -= 15;
        qDebug() << "  стало" << m_bodyAngle;
        emit positionChanged();
    } else {
        qDebug() << "  уже минимум";
    }
}

void DumpTruck::setBodyAngle(int angle)
{
    if (angle >= 0 && angle <= 45) {
        m_bodyAngle = angle;
        emit positionChanged();
    }
}

int DumpTruck::getBodyAngle() const
{
    return m_bodyAngle;
}
