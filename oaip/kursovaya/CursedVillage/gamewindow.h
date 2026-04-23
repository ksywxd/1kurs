#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QPixmap>
#include <QSoundEffect>
#include <QPainter>
#include "pausemenu.h"
#include "gamestate.h"

class GameWindow : public QWidget
{
    Q_OBJECT
public:
    explicit GameWindow(QWidget *parent = nullptr);
    virtual ~GameWindow();

    virtual void resetWindow() {};
    virtual void onEnterScene() = 0;// настройка сцены
    virtual void enter();           // обработка enter

    GameState* getState() const { return m_state; }

    void startNewGame();               //все очищается и заново
    void saveGame();
    void loadGame(const GameState& state);
    void goToScene(const QString& id); // вызывает onEnterScene
    void updateEnterPosition();
    void stopTyping();

signals:
    void backToMenuClicked(); //для mainwindow
    void sceneFinished(const QString& nextSceneId);

protected:
    //ui
    void setupBaseUI();
    void togglePause();
    void updateBackground();

    //events
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event)  override;
    void paintEvent(QPaintEvent *event) override;

    void typeText(const QString& text, QLabel *targetLabel, int speed = 50);

    //виджеты
    QLabel      *m_dialogLabel;      // текст диалога
    QLabel      *m_characterLabel;   // картинка персонажа
    QWidget     *m_buttonsContainer; // контейнер для кнопок действий
    QVBoxLayout *m_buttonsCol;       // столбец кнопок
    QLabel      *m_enter;

    //данные
    GameState *m_state;
    PauseMenu *m_pauseMenu = nullptr;
    QPixmap    m_backgroundPixmap;
    bool       m_isPaused = false;

    //состояние диалога
    QStringList  m_dialogParts;
    int          m_dialogIndex = 0;
    bool         m_isDialogFinished = false;

    //печать
    QTimer       *m_typeTimer;
    QSoundEffect *m_typingSound;
    QLabel       *m_activeLabel;
    QString       m_fullText;
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
