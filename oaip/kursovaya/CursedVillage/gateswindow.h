#ifndef GATESWINDOW_H
#define GATESWINDOW_H

#include "gamewindow.h"

class GatesWindow : public GameWindow
{
    Q_OBJECT

public:
    explicit GatesWindow(QWidget *parent = nullptr);

    void resetWindow()  override;
    void onEnterScene() override;
    void enter()        override;

private:
    void showActionButtons();
    void hideActionButtons();

    //кнопки
    QPushButton *m_comeInBtn;
    QPushButton *m_chestBtn;
    QPushButton *m_villageBtn;

    bool m_dialogActive;

private slots:
    void onComeInClicked();
    void onChestClicked();
};

#endif // GATESWINDOW_H
