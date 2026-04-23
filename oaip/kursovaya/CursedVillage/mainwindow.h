#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "mainmenu.h"
#include "introwindow.h"
#include "villagewindow.h"
#include "blacksmithwindow.h"
#include "witchwindow.h"
#include "forestwindow.h"
#include "gateswindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void loadGame();
    void changeScene(const QString& nextSceneId);

    Ui::MainWindow *ui;

    QStackedWidget   *m_stack;      //для переключения страниц
    MainMenu         *m_menu;       //окно меню
    IntroWindow      *m_intro;      //окно предисловия
    VillageWindow    *m_village;    //окно деревни
    BlacksmithWindow *m_blacksmith; //окно кузницы
    WitchWindow      *m_witch;      //окно знахарни
    ForestWindow     *m_forest;     //окно леса
    GatesWindow      *m_gates;      //окно ворот

protected:
    void keyPressEvent(QKeyEvent *event)  override;
    void resizeEvent(QResizeEvent *event) override;
};
#endif // MAINWINDOW_H
