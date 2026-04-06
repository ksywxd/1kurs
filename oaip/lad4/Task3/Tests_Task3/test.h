#ifndef TEST_H
#define TEST_H

#include <QObject>

class TestMedians : public QObject
{
    Q_OBJECT

private slots:
    void medianOfThree();
    void averageOfTwo();
    void parseInput_valid();
    void parseInput_empty();
    void parseInput_invalid();
    void computeMedians_full();
    void computeMedians_partial();
    void computeMedians_single();
    void computeMedians_emptyInput();
    void computeMedians_invalidInput();
};

#endif // TEST_H
