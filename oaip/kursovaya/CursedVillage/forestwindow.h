#ifndef FORESTWINDOW_H
#define FORESTWINDOW_H

#include "gamewindow.h"

class ForestWindow : public GameWindow
{
    Q_OBJECT

public:
    explicit ForestWindow(QWidget *parent = nullptr);

    void resetWindow()  override;
    void onEnterScene() override;
    void enter()        override;

private:
    void showActionButtons();
    void hideActionButtons();

    //кнопки
    QPushButton *m_stumpBtn;
    QPushButton *m_bushesBtn;
    QPushButton *m_villageBtn;

    bool m_dialogActive;

private slots:
    void onStumpClicked();
    void onBushesClicked();
};

#endif // FORESTWINDOW_H
