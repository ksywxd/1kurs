#include "test.h"
#include <QtTest>
#include <QTableWidget>
#include <QLabel>
#include <QSpinBox>      // добавлено
#include <QLineEdit>     // добавлено
#include <QMessageBox>

#define private public
#include "mainwindow.h"
#undef private

void TestVectorPairApp::addRow()
{
    MainWindow w;
    size_t oldSize = w.m_data.size();
    w.addRow();
    QCOMPARE(w.m_data.size(), oldSize + 1);
    QCOMPARE(w.m_tableFirst->rowCount(), static_cast<int>(w.m_data.size()));
    QCOMPARE(w.m_tableSecond->rowCount(), static_cast<int>(w.m_data.size()));
}

void TestVectorPairApp::deleteRow()
{
    MainWindow w;
    w.addRow();
    w.addRow();
    int rowToDelete = 1;
    w.m_rowSpin->setValue(rowToDelete);
    size_t oldSize = w.m_data.size();
    w.deleteRow();
    QCOMPARE(w.m_data.size(), oldSize - 1);
    QCOMPARE(w.m_tableFirst->rowCount(), static_cast<int>(w.m_data.size()));
    QCOMPARE(w.m_tableSecond->rowCount(), static_cast<int>(w.m_data.size()));
}

void TestVectorPairApp::clearAll()
{
    MainWindow w;
    w.addRow();
    w.addRow();
    w.clearAll();
    QCOMPARE(w.m_data.size(), 0u);
    QCOMPARE(w.m_tableFirst->rowCount(), 0);
    QCOMPARE(w.m_tableSecond->rowCount(), 0);
}

void TestVectorPairApp::showInfo()
{
    MainWindow w;
    QString oldText = w.m_infoLabel->text();
    w.showInfo();
    QCOMPARE(w.m_infoLabel->text(), oldText);
}

void TestVectorPairApp::addNumberToFirst()
{
    MainWindow w;
    w.addRow();
    w.m_rowSpin->setValue(0);
    w.m_valueEdit->setText("42");
    size_t oldSize = w.m_data[0].first.size();
    w.addNumberToFirst();
    QCOMPARE(w.m_data[0].first.size(), oldSize + 1);
    QCOMPARE(w.m_data[0].first.back(), 42);
    QTableWidgetItem* item = w.m_tableFirst->item(0, 0);
    QVERIFY(item != nullptr);
    QVERIFY(item->text().contains("42"));
}

void TestVectorPairApp::addPairToSecond()
{
    MainWindow w;
    w.addRow();
    w.m_rowSpin->setValue(0);
    w.m_valueEdit->setText("7:3.14");
    size_t oldSize = w.m_data[0].second.size();
    w.addPairToSecond();
    QCOMPARE(w.m_data[0].second.size(), oldSize + 1);
    QCOMPARE(w.m_data[0].second.back().first, 7);
    QCOMPARE(w.m_data[0].second.back().second, 3.14);
    QTableWidgetItem* item = w.m_tableSecond->item(0, 0);
    QVERIFY(item != nullptr);
    QVERIFY(item->text().contains("7:3.14"));
}

void TestVectorPairApp::editNumber()
{
    MainWindow w;
    w.addRow();
    w.m_rowSpin->setValue(0);
    w.m_valueEdit->setText("10");
    w.addNumberToFirst();
    w.m_posSpin->setValue(0);
    w.m_valueEdit->setText("99");
    w.editNumber();
    QCOMPARE(w.m_data[0].first[0], 99);
    QTableWidgetItem* item = w.m_tableFirst->item(0, 0);
    QVERIFY(item->text().contains("99"));
}

void TestVectorPairApp::editPair()
{
    MainWindow w;
    w.addRow();
    w.m_rowSpin->setValue(0);
    w.m_valueEdit->setText("1:1.0");
    w.addPairToSecond();
    w.m_posSpin->setValue(0);
    w.m_valueEdit->setText("5:2.5");
    w.editPair();
    QCOMPARE(w.m_data[0].second[0].first, 5);
    QCOMPARE(w.m_data[0].second[0].second, 2.5);
    QTableWidgetItem* item = w.m_tableSecond->item(0, 0);
    QVERIFY(item->text().contains("5:2.5"));
}

void TestVectorPairApp::updateTables()
{
    MainWindow w;
    // Используем типы напрямую
    Vector<int> v1;
    v1.push_back(1);
    v1.push_back(2);
    Vector<Pair<int, double>> v2;
    v2.push_back(Pair<int, double>(3, 4.5));
    w.m_data.push_back(Pair<Vector<int>, Vector<Pair<int, double>>>(v1, v2));
    w.updateTables();
    QCOMPARE(w.m_tableFirst->rowCount(), 1);
    QCOMPARE(w.m_tableSecond->rowCount(), 1);
    QTableWidgetItem* itemFirst = w.m_tableFirst->item(0, 0);
    QVERIFY(itemFirst->text() == "1, 2");
    QTableWidgetItem* itemSecond = w.m_tableSecond->item(0, 0);
    QVERIFY(itemSecond->text() == "3:4.5");
}

void TestVectorPairApp::infoLabelUpdate()
{
    MainWindow w;
    w.addRow();
    w.addRow();
    QString labelText = w.m_infoLabel->text();
    QVERIFY(labelText.contains("Всего строк: 2"));
    w.m_rowSpin->setValue(0);
    w.m_valueEdit->setText("10");
    w.addNumberToFirst();
    QCOMPARE(w.m_infoLabel->text(), labelText);
    w.clearAll();
    QVERIFY(w.m_infoLabel->text().contains("Всего строк: 0"));
}

QTEST_MAIN(TestVectorPairApp)
