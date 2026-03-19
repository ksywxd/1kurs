#ifndef VILLAGEWINDOW_H
#define VILLAGEWINDOW_H

#include "gamewindow.h"

class VillageWindow : public GameWindow
{
    Q_OBJECT

public:
    explicit VillageWindow(QWidget *parent = nullptr);

private:
    QLabel *m_dialogLabel;
    QLabel *m_imageLabel;

    QStringList m_dialogParts;
    int m_dialogIndex;

protected:
    void onEnterScene() override;
    void enter()        override;
};

#endif // VILLAGEWINDOW_H
