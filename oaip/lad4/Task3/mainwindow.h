#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QPushButton;
class QTextEdit;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void computeMedians();

private:
    QVector<int> parseInput(const QString &text) const;
    double medianOfThree(int a, int b, int c) const;
    double averageOfTwo(int a, int b) const;

    QLineEdit *inputEdit;
    QPushButton *computeBtn;
    QTextEdit *outputEdit;
};

#endif // MAINWINDOW_H
