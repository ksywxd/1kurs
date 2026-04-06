#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLineEdit;
class QPushButton;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
private slots:
    void reverseNumber();
private:
    bool hasZero(int n) const;
    int reverseRecursive(int n, int rev = 0) const;
    QLineEdit *input;
    QLabel *result;
};

#endif // MAINWINDOW_H
