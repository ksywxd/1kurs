#ifndef DUMPTRUCK_H
#define DUMPTRUCK_H

#include "moveablerect.h"

class DumpTruck : public MoveableRect
{
    Q_OBJECT
private:
    int m_bodyAngle;
public:
    explicit DumpTruck(int startX = 100, int startY = 100, QObject *parent = nullptr);
    void raiseBody();
    void lowerBody();
    void setBodyAngle(int angle);
    int getBodyAngle() const;
};

#endif // DUMPTRUCK_H
