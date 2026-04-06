#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "vector.h"
#include "pair.h"

QT_BEGIN_NAMESPACE
class QTableWidget;
class QPushButton;
class QSpinBox;
class QLineEdit;
class QLabel;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void addRow();                 // добавить новую строку (пустые векторы)
    void deleteRow();              // удалить выбранную строку
    void clearAll();               // очистить всё
    void showInfo();               // показать информацию о векторе

    void addNumberToFirst();       // добавить число в первую матрицу (выбранная строка)
    void addPairToSecond();        // добавить пару во вторую матрицу
    void editNumber();             // изменить число в первой матрице
    void editPair();               // изменить пару во второй матрице

private:
    void updateTables();
    void updateInfoLabel();

    using DataVector = Vector<int>;
    using DataPair = Pair<int, double>;
    using DataPairVector = Vector<DataPair>;
    using MainPair = Pair<DataVector, DataPairVector>;
    using MainVector = Vector<MainPair>;

    MainVector m_data;

    QTableWidget *m_tableFirst;
    QTableWidget *m_tableSecond;
    QLabel *m_infoLabel;

    // Поля ввода
    QSpinBox *m_rowSpin;      // номер строки
    QSpinBox *m_posSpin;      // позиция в строке
    QLineEdit *m_valueEdit;   // значение (число или пара)
};

#endif // MAINWINDOW_H
