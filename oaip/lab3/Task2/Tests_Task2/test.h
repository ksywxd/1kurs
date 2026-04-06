#ifndef TEST_H
#define TEST_H

#include <QObject>

class TestAckermann : public QObject
{
    Q_OBJECT

private slots:
    void ackermann_0_0();
    void ackermann_0_5();
    void ackermann_1_0();
    void ackermann_1_3();
    void ackermann_2_2();
    void ackermann_2_5();
    void ackermann_3_1();
    void ackermann_3_2();
    void ackermann_4_0();
    void invalid_input();
};

#endif // TEST_H
