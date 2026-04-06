#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "vector.h"

QT_BEGIN_NAMESPACE
class QLineEdit;
class QPushButton;
class QTextEdit;
class QLabel;
class QSpinBox;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void onPushBack();
    void onPopBack();
    void onInsert();
    void onErase();
    void onClear();
    void onResize();
    void onReserve();
    void onAssign();
    void onAt();
    void onFrontBack();
    void onSwap();

private:
    void updateDisplay();

    Vector<int> m_vec;
    QTextEdit *m_display;
    QLineEdit *m_valueEdit;
    QSpinBox *m_indexSpin;
    QLabel *m_infoLabel;
};

#endif // MAINWINDOW_H
