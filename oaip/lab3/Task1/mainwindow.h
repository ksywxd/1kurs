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
    void convert();
private:
    QString integerToBinary(int n) const;
    QString fractionalToBinary(double frac, int depth = 10) const;
    QLineEdit *input;
    QLabel *result;
};

#endif // MAINWINDOW_H
