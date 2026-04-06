#ifndef TEST_H
#define TEST_H

#include <QObject>

class TestVectorPairApp : public QObject
{
    Q_OBJECT

private slots:
    void addRow();
    void deleteRow();
    void clearAll();
    void showInfo();
    void addNumberToFirst();
    void addPairToSecond();
    void editNumber();
    void editPair();
    void updateTables();
    void infoLabelUpdate();
};

#endif // TEST_H
