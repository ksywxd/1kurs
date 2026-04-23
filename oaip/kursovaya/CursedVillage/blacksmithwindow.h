#ifndef BLACKSMITHWINDOW_H
#define BLACKSMITHWINDOW_H

#include "gamewindow.h"

class BlacksmithWindow : public GameWindow
{
    Q_OBJECT

public:
    explicit BlacksmithWindow(QWidget *parent = nullptr);

    void resetWindow()  override;
    void onEnterScene() override;
    void enter()        override;

private:
    void showActionButtons();
    void hideActionButtons();

    //кнопки
    QPushButton *m_silverDustBtn;
    QPushButton *m_baronsSwordBtn;
    QPushButton *m_enchantBtn;
    QPushButton *m_villageBtn;

    bool m_dialogActive;

private slots:
    void onSilverDustClicked();
    void onBaronsSwordClicked();
    void onEnchantClicked();

protected:

};

#endif // BLACKSMITHWINDOW_H
