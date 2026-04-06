#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QSpinBox;
class QPushButton;
class QTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
private slots:
    void solveHanoi();
private:
    void hanoi(int n, char from, char to, char aux, QTextEdit *output);
    QSpinBox *m_spinBox;
    QPushButton *m_button;
    QTextEdit *m_output;
};

#endif // MAINWINDOW_H
