#ifndef TEST_H
#define TEST_H

#include <QObject>

class TestInterpolation : public QObject
{
    Q_OBJECT

private slots:
    void interpolationSort_known();
    void interpolationSort_empty();
    void interpolationSort_alreadySorted();
    void binpow_basic();
    void binpow_mod1();
    void binpow_zeroExponent();
    void binarySearchAndPow_valid();
    void binarySearchAndPow_notFound();
    void binarySearchAndPow_invalidInput();
    void generateRandom();
};

#endif // TEST_H
