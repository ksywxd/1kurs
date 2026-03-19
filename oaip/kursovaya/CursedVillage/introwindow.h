#ifndef INTROWINDOW_H
#define INTROWINDOW_H

#include "gamewindow.h"

class IntroWindow : public GameWindow
{
    Q_OBJECT

public:
    explicit IntroWindow(QWidget *parent = nullptr);

protected:
    void onEnterScene() override;
    void enter()        override;
    void onActionBtn()  override;

private:
    void finishPrologue();

    QVBoxLayout *m_mainLayout;
    QLabel      *m_textLabel;
    QPushButton *m_actionButton;
    QStringList  m_prologueParts;

    int  m_prologueIndex;
    bool m_inPrologue;
};

#endif // INTROWINDOW_H
