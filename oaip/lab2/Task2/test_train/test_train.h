#ifndef TEST_TRAIN_H
#define TEST_TRAIN_H

#include <QObject>
#include "train.h"

class TestTrain : public QObject
{
    Q_OBJECT

private slots:
    void testConstructorWithParams();
    void testDefaultConstructor();
    void testComputedFields();
    void testSetters();
    void testSerialization();
    void testFromStringInvalid();
    void testLessThanOperator();
    void testLeapYearDate();
    void testTime24Hour();
    void testNegativeSeats();
};

#endif // TEST_TRAIN_H
