#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

QT_BEGIN_NAMESPACE
class QSpinBox;
class QLineEdit;
class QPushButton;
class QLabel;
class QTextEdit;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void generateRandom();
    void interpolationSort();
    void binarySearchAndPow();

private:
    void displayArray(const QVector<int>& arr, const QString& title);
    int binpow(int digit, int powder, int mod) const;

    QSpinBox *sizeSpin;
    QLineEdit *arrayEdit;
    QPushButton *genBtn;
    QPushButton *sortBtn;
    QTextEdit *outputEdit;
    QLineEdit *searchEdit;
    QLineEdit *modEdit;
    QPushButton *searchBtn;
    QLabel *resultLabel;

    QVector<int> originalArray;   // исходный (неотсортированный) массив
    QVector<int> sortedArray;      // отсортированный массив (после сортировки)
};

#endif // MAINWINDOW_H
