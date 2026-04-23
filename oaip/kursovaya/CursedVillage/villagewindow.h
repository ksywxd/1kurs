#ifndef VILLAGEWINDOW_H
#define VILLAGEWINDOW_H

#include "gamewindow.h"

class VillageWindow : public GameWindow
{
    Q_OBJECT

public:
    explicit VillageWindow(QWidget *parent = nullptr);

    void resetWindow()  override;
    void onEnterScene() override;
    void enter()        override;

private:
    void showActionButtons();
    void hideActionButtons();

    //кнопки
    QPushButton *m_blacksmithBtn;
    QPushButton *m_witchBtn;
    QPushButton *m_forestBtn;
    QPushButton *m_gatesBtn;

    bool m_dialogActive;

protected:

};

#endif // VILLAGEWINDOW_H
