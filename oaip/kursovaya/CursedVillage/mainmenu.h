#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include <QKeyEvent>
#include <QPixmap>          //для работы с картинками
#include <QResizeEvent>     //для обработки изменения размера
#include <QMediaPlayer>
#include <QAudioOutput>

class QPushButton;
class QVBoxLayout;
class QHBoxLayout;

class MainMenu : public QWidget
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);

signals:
    void newGameClicked();
    void continueClicked();
    void exitClicked();

private slots:
    void onNewGame();
    void onContinue();
    void onExit();
    void onVolume();

public slots:
    void stopMusic();
    void playMusic();

private:
    void updateBackground();

    bool m_muted = false;

    QPushButton *m_newGameBtn;
    QPushButton *m_continueBtn;
    QPushButton *m_exitBtn;
    QPushButton *m_muteButton;

    QVBoxLayout *m_Vlayout;
    QHBoxLayout *m_Hlayout;
    QHBoxLayout *m_mainlayout;
    QHBoxLayout *m_soundlayout;

    QMediaPlayer *m_mediaPlayer;

    QPixmap m_backgroundMenu;

protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // MAINMENU_H
