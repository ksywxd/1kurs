#ifndef TEST_H
#define TEST_H

#include <QObject>

class TestReverseNumber : public QObject
{
    Q_OBJECT

private slots:
    void reverse_123();
    void reverse_972();
    void reverse_5();
    void reverse_1001_hasZero();
    void reverse_120_hasZero();
    void reverse_negative();
    void reverse_nonNumeric();
    void reverse_zero();
    void reverse_large();
    void invalid_input_empty();
};

#endif // TEST_H
