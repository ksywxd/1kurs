#include "test.h"
#include <QtTest>
#include <QLineEdit>
#include <QSpinBox>
#include <QTextEdit>

#define private public
#include "mainwindow.h"
#undef private

void TestVectorApp::pushBack()
{
    MainWindow w;
    w.m_valueEdit->setText("10");
    w.onPushBack();
    QCOMPARE(w.m_vec.size(), 1u);
    QCOMPARE(w.m_vec[0], 10);
    w.m_valueEdit->setText("20");
    w.onPushBack();
    QCOMPARE(w.m_vec.size(), 2u);
    QCOMPARE(w.m_vec[1], 20);
}

void TestVectorApp::popBack()
{
    MainWindow w;
    w.m_vec.push_back(1);
    w.m_vec.push_back(2);
    w.onPopBack();
    QCOMPARE(w.m_vec.size(), 1u);
    QCOMPARE(w.m_vec[0], 1);
    w.onPopBack();
    QCOMPARE(w.m_vec.empty(), true);
    // Попытка pop_back на пустом векторе – должно показать предупреждение, но мы не проверяем GUI
    // Просто убедимся, что не падает
    w.onPopBack(); // должно показать QMessageBox
    QCOMPARE(w.m_vec.empty(), true);
}

void TestVectorApp::insert()
{
    MainWindow w;
    w.m_vec.push_back(10);
    w.m_vec.push_back(30);
    w.m_valueEdit->setText("20");
    w.m_indexSpin->setValue(1);
    w.onInsert();
    QCOMPARE(w.m_vec.size(), 3u);
    QCOMPARE(w.m_vec[0], 10);
    QCOMPARE(w.m_vec[1], 20);
    QCOMPARE(w.m_vec[2], 30);
    // Вставка в начало
    w.m_valueEdit->setText("5");
    w.m_indexSpin->setValue(0);
    w.onInsert();
    QCOMPARE(w.m_vec.size(), 4u);
    QCOMPARE(w.m_vec[0], 5);
    QCOMPARE(w.m_vec[1], 10);
    // Вставка в конец
    w.m_valueEdit->setText("40");
    w.m_indexSpin->setValue(4);
    w.onInsert();
    QCOMPARE(w.m_vec.size(), 5u);
    QCOMPARE(w.m_vec[4], 40);
}

void TestVectorApp::erase()
{
    MainWindow w;
    w.m_vec.push_back(10);
    w.m_vec.push_back(20);
    w.m_vec.push_back(30);
    w.m_indexSpin->setValue(1);
    w.onErase();
    QCOMPARE(w.m_vec.size(), 2u);
    QCOMPARE(w.m_vec[0], 10);
    QCOMPARE(w.m_vec[1], 30);
    // Удаление последнего
    w.m_indexSpin->setValue(1);
    w.onErase();
    QCOMPARE(w.m_vec.size(), 1u);
    QCOMPARE(w.m_vec[0], 10);
    // Удаление первого
    w.m_indexSpin->setValue(0);
    w.onErase();
    QCOMPARE(w.m_vec.empty(), true);
}

void TestVectorApp::clear()
{
    MainWindow w;
    w.m_vec.push_back(1);
    w.m_vec.push_back(2);
    w.onClear();
    QCOMPARE(w.m_vec.empty(), true);
    QCOMPARE(w.m_vec.size(), 0u);
}

void TestVectorApp::resize()
{
    MainWindow w;
    w.m_vec.push_back(1);
    w.m_vec.push_back(2);
    w.m_valueEdit->setText("5");
    w.onResize();
    QCOMPARE(w.m_vec.size(), 5u);
    QCOMPARE(w.m_vec[0], 1);
    QCOMPARE(w.m_vec[1], 2);
    QCOMPARE(w.m_vec[2], 0);
    QCOMPARE(w.m_vec[3], 0);
    QCOMPARE(w.m_vec[4], 0);
    w.m_valueEdit->setText("2");
    w.onResize();
    QCOMPARE(w.m_vec.size(), 2u);
    QCOMPARE(w.m_vec[0], 1);
    QCOMPARE(w.m_vec[1], 2);
}

void TestVectorApp::reserve()
{
    MainWindow w;
    w.m_valueEdit->setText("100");
    w.onReserve();
    QVERIFY(w.m_vec.capacity() >= 100);
    QCOMPARE(w.m_vec.size(), 0u);
    // Резервирование меньшей ёмкости не должно уменьшать
    w.m_valueEdit->setText("50");
    w.onReserve();
    QVERIFY(w.m_vec.capacity() >= 100);
}

void TestVectorApp::assign()
{
    MainWindow w;
    w.m_vec.assign(static_cast<size_t>(3), 5);
    QCOMPARE(w.m_vec.size(), 3u);
    for (size_t i = 0; i < w.m_vec.size(); ++i) {
        QCOMPARE(w.m_vec[i], 5);
    }
}

void TestVectorApp::atAndFrontBack()
{
    MainWindow w;
    w.m_vec.push_back(10);
    w.m_vec.push_back(20);
    w.m_vec.push_back(30);
    w.m_indexSpin->setValue(1);
    w.onAt(); // показывает диалог, но мы не проверяем GUI, только проверяем что метод не падает
    // Проверим front/back через прямой вызов
    QCOMPARE(w.m_vec.front(), 10);
    QCOMPARE(w.m_vec.back(), 30);
    // at с корректным индексом
    QCOMPARE(w.m_vec.at(1), 20);
    // at с некорректным индексом должен выбросить исключение
    bool thrown = false;
    try {
        w.m_vec.at(3);
    } catch (const std::out_of_range&) {
        thrown = true;
    }
    QVERIFY(thrown);
}

void TestVectorApp::swap()
{
    MainWindow w;
    w.m_vec.push_back(1);
    w.m_vec.push_back(2);
    w.onSwap();
    QCOMPARE(w.m_vec.size(), 2u);
    QCOMPARE(w.m_vec[0], 999);
    QCOMPARE(w.m_vec[1], 888);
    // Повторный swap вернёт обратно? Нет, потому что внутренний temp уничтожается после вызова.
    // Но мы просто проверим, что swap сработал.
    w.onSwap();
    QCOMPARE(w.m_vec.size(), 2u);
    QCOMPARE(w.m_vec[0], 999);
    QCOMPARE(w.m_vec[1], 888);
}

QTEST_MAIN(TestVectorApp)
