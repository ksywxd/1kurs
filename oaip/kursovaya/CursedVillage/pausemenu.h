#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class PauseMenu : public QWidget
{
    Q_OBJECT
public:
    explicit PauseMenu(QWidget *parent = nullptr);

signals:
    void resumeClicked();
    void saveClicked();
    void backToMenuClicked();

protected:
    // перехватываем все нажатия клавиш, пока открыта пауза
    void keyPressEvent(QKeyEvent *event) override;
};

#endif
