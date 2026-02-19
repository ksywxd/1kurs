#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dumptruck.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void writeSettings();
    void readSettings();

private slots:
    void on_leftButton_clicked();
    void on_rightButton_clicked();

    void on_raiseButton_clicked();
    void on_lowerButton_clicked();

    void onTruckMoved();

protected:
    void paintEvent(QPaintEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;
    DumpTruck *m_truck;
};

#endif // MAINWINDOW_H
