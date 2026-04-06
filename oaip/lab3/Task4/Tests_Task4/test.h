#ifndef TEST_H
#define TEST_H

#include <QObject>

class TestHanoi : public QObject
{
    Q_OBJECT

private slots:
    void hanoi_n1();
    void hanoi_n2();
    void hanoi_n3_full();
    void hanoi_n4_total();
    void hanoi_n5_total();
    void hanoi_n20_total();
    void hanoi_clear();
    void hanoi_format();
    void hanoi_spinbox_range();
    void hanoi_solveHanoi_calls();
};

#endif // TEST_H
