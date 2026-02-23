#include "moveablerect.h"

MoveableRect::MoveableRect(int startX, int startY, QObject *parent)
    : QObject(parent), m_x(startX), m_y(startY)
{
}

int MoveableRect::getX() const { return m_x; }
int MoveableRect::getY() const { return m_y; }
int MoveableRect::getWidth() const { return m_width; }
int MoveableRect::getHeight() const { return m_height; }

void MoveableRect::move(int dx, int dy)
{
    m_x += dx;
    m_y += dy;
    emit positionChanged();
}
