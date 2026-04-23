#ifndef INTROWINDOW_H
#define INTROWINDOW_H

#include "gamewindow.h"

class IntroWindow : public GameWindow
{
    Q_OBJECT

public:
    explicit IntroWindow(QWidget *parent = nullptr);
    void onEnterScene() override;

protected:
    void enter()        override;
    void onActionBtn()  override;

private:
    void finishPrologue();

    QPushButton *m_actionButton;
    QVBoxLayout *m_mainLayout;
    QStringList  m_prologueParts;

    int  m_prologueIndex;
    bool m_inPrologue;
};

#endif // INTROWINDOW_H
