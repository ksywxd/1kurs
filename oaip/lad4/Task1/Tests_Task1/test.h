#ifndef TEST_H
#define TEST_H

#include <QObject>

class TestSorting : public QObject
{
    Q_OBJECT

private slots:
    void heapSort();
    void quickSort();
    void mergeSort();
    void binarySearchFound();
    void binarySearchNotFound();
    void sortEmptyArray();
    void binarySearchEmptyArray();
    void generateRandom();
    void binarySearchManualInput();
    void sortManualInput();
};

#endif // TEST_H
