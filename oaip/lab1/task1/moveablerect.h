#ifndef MOVEABLERECT_H
#define MOVEABLERECT_H

#include <QObject>

class MoveableRect : public QObject
{
    Q_OBJECT
private:
    int m_x;
    int m_y;
    const int m_width = 100;
    const int m_height = 50;
public:
    explicit MoveableRect(int startX = 100, int startY = 100, QObject *parent = nullptr);
    int getX() const;
    int getY() const;
    int getWidth() const;
    int getHeight() const;
    void move(int dx, int dy);
signals:
    void positionChanged();
};

#endif // MOVEABLERECT_H
