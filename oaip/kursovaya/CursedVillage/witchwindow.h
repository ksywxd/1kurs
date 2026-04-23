#ifndef WITCHWINDOW_H
#define WITCHWINDOW_H

#include "gamewindow.h"

class WitchWindow : public GameWindow
{
    Q_OBJECT

public:
    explicit WitchWindow(QWidget *parent = nullptr);

    void resetWindow()  override;
    void onEnterScene() override;
    void enter()        override;

private:
    void showActionButtons();
    void hideActionButtons();

    //кнопки
    QPushButton *m_glowingMossBtn;
    QPushButton *m_giveMossBtn;
    QPushButton *m_aboutBaronBtn;
    QPushButton *m_villageBtn;

    bool m_dialogActive;

private slots:
    void onGlowingMossClicked();
    void onGiveMossClicked();
    void onAboutBaronClicked();

protected:

};

#endif // WITCHWINDOW_H
