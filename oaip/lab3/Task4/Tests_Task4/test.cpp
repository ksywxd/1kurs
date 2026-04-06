#include "test.h"
#include <QtTest>
#include <QTextEdit>
#include <QSpinBox>

#define private public
#include "mainwindow.h"
#undef private

// Вспомогательная функция для получения строк вывода
static QStringList getLines(MainWindow& w) {
    return w.m_output->toPlainText().split('\n', Qt::SkipEmptyParts);
}

void TestHanoi::hanoi_n1()
{
    MainWindow w;
    w.m_spinBox->setValue(1);
    w.solveHanoi();
    QStringList lines = getLines(w);
    QVERIFY(lines.size() >= 6); // заголовки + 1 перемещение + разделители + итог
    QCOMPARE(lines[3], QString("Переместить кольцо 1 с A на C"));
    QCOMPARE(lines.last(), QString("Всего перемещений: 1"));
}

void TestHanoi::hanoi_n2()
{
    MainWindow w;
    w.m_spinBox->setValue(2);
    w.solveHanoi();
    QStringList lines = getLines(w);
    QCOMPARE(lines[3], QString("Переместить кольцо 1 с A на B"));
    QCOMPARE(lines[4], QString("Переместить кольцо 2 с A на C"));
    QCOMPARE(lines[5], QString("Переместить кольцо 1 с B на C"));
    QCOMPARE(lines.last(), QString("Всего перемещений: 3"));
}

void TestHanoi::hanoi_n3_full()
{
    MainWindow w;
    w.m_spinBox->setValue(3);
    w.solveHanoi();
    QStringList lines = getLines(w);
    // Ожидаемая последовательность
    QStringList expected = {
        "Начальное состояние: 3 колец на стержне A",
        "Цель: переместить все кольца на стержень C",
        "----------------------------------------",
        "Переместить кольцо 1 с A на C",
        "Переместить кольцо 2 с A на B",
        "Переместить кольцо 1 с C на B",
        "Переместить кольцо 3 с A на C",
        "Переместить кольцо 1 с B на A",
        "Переместить кольцо 2 с B на C",
        "Переместить кольцо 1 с A на C",
        "----------------------------------------",
        "Всего перемещений: 7"
    };
    QCOMPARE(lines, expected);
}

void TestHanoi::hanoi_n4_total()
{
    MainWindow w;
    w.m_spinBox->setValue(4);
    w.solveHanoi();
    QStringList lines = getLines(w);
    QCOMPARE(lines.last(), QString("Всего перемещений: 15"));
    // Проверяем первые два хода
    QCOMPARE(lines[3], QString("Переместить кольцо 1 с A на B"));
    QCOMPARE(lines[4], QString("Переместить кольцо 2 с A на C"));
}

void TestHanoi::hanoi_n5_total()
{
    MainWindow w;
    w.m_spinBox->setValue(5);
    w.solveHanoi();
    QStringList lines = getLines(w);
    QCOMPARE(lines.last(), QString("Всего перемещений: 31"));
}

void TestHanoi::hanoi_n20_total()
{
    MainWindow w;
    w.m_spinBox->setValue(20);
    w.solveHanoi();
    QStringList lines = getLines(w);
    // 2^20 - 1 = 1048575
    QCOMPARE(lines.last(), QString("Всего перемещений: 1048575"));
}

void TestHanoi::hanoi_clear()
{
    MainWindow w;
    w.m_spinBox->setValue(3);
    w.solveHanoi();
    QString firstOutput = w.m_output->toPlainText();
    QVERIFY(!firstOutput.isEmpty());

    w.solveHanoi();
    QString secondOutput = w.m_output->toPlainText();

    // Проверяем, что вывод не накапливается: количество строк такое же
    int firstLines = firstOutput.split('\n', Qt::SkipEmptyParts).size();
    int secondLines = secondOutput.split('\n', Qt::SkipEmptyParts).size();
    QCOMPARE(secondLines, firstLines);

    // И что второй вывод начинается с заголовка
    QVERIFY(secondOutput.startsWith("Начальное состояние: 3 колец на стержне A"));
    // И что заголовок встречается только один раз (нет дублирования)
    QCOMPARE(secondOutput.count("Начальное состояние"), 1);
}

void TestHanoi::hanoi_format()
{
    MainWindow w;
    w.m_spinBox->setValue(1);
    w.solveHanoi();
    QStringList lines = getLines(w);
    QCOMPARE(lines[0], QString("Начальное состояние: 1 колец на стержне A"));
    QCOMPARE(lines[1], QString("Цель: переместить все кольца на стержень C"));
    QCOMPARE(lines[2], QString("----------------------------------------"));
    QCOMPARE(lines[lines.size()-2], QString("----------------------------------------"));
}

void TestHanoi::hanoi_spinbox_range()
{
    MainWindow w;
    QCOMPARE(w.m_spinBox->minimum(), 1);
    QCOMPARE(w.m_spinBox->maximum(), 20);
}

void TestHanoi::hanoi_solveHanoi_calls()
{
    MainWindow w;
    w.m_spinBox->setValue(2);
    w.solveHanoi();
    QStringList lines = getLines(w);
    // Проверяем, что вызывается hanoi и выводит правильные перемещения
    QCOMPARE(lines[3], QString("Переместить кольцо 1 с A на B"));
    QCOMPARE(lines[4], QString("Переместить кольцо 2 с A на C"));
    QCOMPARE(lines[5], QString("Переместить кольцо 1 с B на C"));
}

QTEST_MAIN(TestHanoi)
