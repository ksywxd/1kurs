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
    void compute();
private:
    unsigned long long ackermann(int m, int n) const;
    QLineEdit *mEdit;
    QLineEdit *nEdit;
    QLabel *resultLabel;
};
#endif
