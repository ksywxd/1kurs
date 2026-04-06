#ifndef TEST_H
#define TEST_H

#include <QObject>

class TestVectorApp : public QObject
{
    Q_OBJECT

private slots:
    void pushBack();
    void popBack();
    void insert();
    void erase();
    void clear();
    void resize();
    void reserve();
    void assign();
    void atAndFrontBack();
    void swap();
};

#endif // TEST_H
