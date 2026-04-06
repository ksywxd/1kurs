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
class QElapsedTimer;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void generateRandom();
    void sortHeap();
    void sortQuick();
    void sortMerge();
    void binarySearch();

private:
    void displayArray(const QVector<int>& arr, const QString& title);
    void heapSort(int* arr, int n);
    void quickSort(int* arr, int low, int high);
    int partition(int* arr, int low, int high);
    void mergeSort(int* arr, int l, int r);
    void merge(int* arr, int l, int m, int r);
    int binarySearch(const int* arr, int size, int target);

    QSpinBox *sizeSpin;
    QLineEdit *arrayEdit;
    QPushButton *genBtn;
    QPushButton *heapBtn;
    QPushButton *quickBtn;
    QPushButton *mergeBtn;
    QTextEdit *resultEdit;
    QLineEdit *searchEdit;
    QPushButton *searchBtn;
    QLabel *searchResultLabel;

    QVector<int> originalArray;
};

#endif // MAINWINDOW_H
