#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QPixmap>
#include <QSoundEffect>
#include "pausemenu.h"
#include "gamestate.h"

class GameWindow : public QWidget
{
    Q_OBJECT
public:
    explicit GameWindow(QWidget *parent = nullptr);
    virtual ~GameWindow();

    void startNewGame();               //все очищается и заново
    void saveGame();
    void loadGame(const GameState& state);
    void goToScene(const QString& id); // вызывает onEnterScene
    void updateEnterPosition();
    void stopTyping();

    //доступ к состоянию для действий
    GameState* getState() const { return m_state; }

signals:
    void backToMenuClicked(); //для mainwindow
    void sceneFinished(const QString& nextSceneId);

protected:
    // виртуальные методы для переопределения в наследниках
    virtual void onEnterScene() = 0;// настройка сцены
    virtual void enter();           // обработка enter

    void togglePause();

    void typeText(const QString& text, QLabel *targetLabel, int speed = 50);

    void updateBackground();
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event)  override;

    // общие виджеты и данные
    QLabel    *m_enter;
    GameState *m_state;
    QPixmap    m_backgroundPixmap;
    PauseMenu *m_pauseMenu = nullptr;
    bool       m_isPaused = false;

    //для печатной машинки
    QSoundEffect *m_typingSound;
    QTimer       *m_typeTimer;
    QLabel       *m_activeLabel;
    QString       m_fullText;      // фулл текст
    QString       m_currentText;   // уже отрисован
    int           m_charIndex;     // текущая буква индекс

protected slots:
    virtual void onActionBtn();

    void onResumeGame();
    void onSave();
    void onBackToMenu();

private slots:
    void onTypeTick(); // для прорисовки буквы

private:
    QLabel *m_saveNotification;
};

#endif // GAMEWINDOW_H
