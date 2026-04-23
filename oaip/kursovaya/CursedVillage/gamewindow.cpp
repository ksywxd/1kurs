#include "gamewindow.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <QStandardPaths>
#include <QDir>
#include <QTimer>

void GameWindow::setupBaseUI() {
    //перс
    m_characterLabel = new QLabel(this);
    m_characterLabel->setScaledContents(true);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 40, 60, 40);

    //справа
    m_buttonsContainer = new QWidget(this);
    m_buttonsContainer->setVisible(false);
    QHBoxLayout *hLayout = new QHBoxLayout(m_buttonsContainer);

    m_buttonsCol = new QVBoxLayout(); //столбец
    m_buttonsCol->setSpacing(15);

    hLayout->addStretch(5); //к правому краю
    hLayout->addLayout(m_buttonsCol);
    hLayout->addStretch(1); //чуть чуть левее

    mainLayout->addStretch(1);   // сверху
    mainLayout->addWidget(m_buttonsContainer);
    mainLayout->addStretch(1);   // снизу

    m_buttonsContainer->setStyleSheet(
        "QPushButton {"
        "    background-color: rgba(45, 25, 15, 230);" // коричневый
        "    color: #e6c9a8;"                         // тип золото
        "    border: 2px solid #8b5a2b;"              // рамка бронзовая
        "    border-radius: 10px;"                    // скругление
        "    padding: 15px;"
        "    font-size: 24px;"
        "    font-weight: bold;"
        "    min-width: 200px;"
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(80, 45, 25, 255);" // подсветка при наведении
        "    border-color: #d4a373;"                  // свечение рамки
        "}"
        );

    //окно диалога
    m_dialogLabel = new QLabel(this);
    m_dialogLabel->setWordWrap(true);
    m_dialogLabel->setMinimumHeight(180);
    m_dialogLabel->setStyleSheet("border: 4px solid Chocolate;"
                                 "background: rgba(20,20,20,220);"
                                 "color: white;"
                                 "padding: 20px;"
                                 "border-radius: 12px;"
                                 "font-size: 22px;");

    mainLayout->addWidget(m_dialogLabel);

    // перс за диалогом
    m_characterLabel->lower();
    m_dialogLabel->raise();
}

GameWindow::GameWindow(QWidget *parent) : QWidget(parent)
{
    m_characterLabel   = nullptr;
    m_dialogLabel      = nullptr;
    m_buttonsContainer = nullptr;
    m_buttonsCol       = nullptr;

    //enter
    m_enter = new QLabel("Enter", this);
    m_enter -> setStyleSheet("color: rgba(255,255,255,100); font-size: 18px;");
    m_enter -> adjustSize(); // вычислить размер
    m_enter -> hide();

    //пауза
    m_pauseMenu = new PauseMenu(this);
    m_pauseMenu -> hide(); //скрыто

    //уведомление сохранения
    m_saveNotification = new QLabel("Сохранено!", this);
    m_saveNotification->setStyleSheet(
        "background-color: rgba(50, 50, 50, 200);"
        "color: white;"
        "font: bold 16px;"
        "padding: 10px 20px;"
        "border-radius: 15px;"
        );
    m_saveNotification->hide();

    m_typeTimer = new QTimer(this);

    m_typingSound = new QSoundEffect(this);
    m_typingSound ->setSource(QUrl("qrc:/audio/audio/type_click1.wav"));
    m_typingSound ->setVolume(0.1);

    //коннекты
    connect(m_pauseMenu, &PauseMenu::resumeClicked,     this, &GameWindow::onResumeGame);
    connect(m_pauseMenu, &PauseMenu::saveClicked,       this, &GameWindow::onSave);
    connect(m_pauseMenu, &PauseMenu::backToMenuClicked, this, &GameWindow::onBackToMenu);

    connect(m_typeTimer, &QTimer::timeout, this, &GameWindow::onTypeTick);

    //стата
    m_state = new GameState();
}

GameWindow::~GameWindow()
{
    delete m_state;
}

void GameWindow::startNewGame()
{
    m_state -> clear();
    m_state -> setCurrentScene("intro");
}

void GameWindow::saveGame()
{
    QString saveDir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    if (!QDir().mkpath(saveDir)) {
        qDebug() << "Не удалось создать папку для сохранений";
        return;
    }
    QString filePath = saveDir + "/save.dat";

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Не удалось открыть файл для записи";
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_0);

    out << m_state -> getCurrentScene();
    out << m_state -> getInventory();
    out << m_state -> getFlags();

    file.close();
    qDebug() << "Игра сохранена";
}

void GameWindow::loadGame(const GameState& state)
{
    *m_state = state;
    onEnterScene();
}

void GameWindow::goToScene(const QString& id)
{
    qDebug() << "Переход на сцену:" << id;
    m_state -> setCurrentScene(id);
    emit sceneFinished(id);
    setFocus();
}

void GameWindow::onActionBtn()
{

}

void GameWindow::enter()
{

}

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    //esc пауза
    if (event->key() == Qt::Key_Escape) {
        togglePause();
        event->accept();
        return;
    }

    //игнор во время ивента чтоб до мейнвинд дошло
    if (event->key() == Qt::Key_F11) {
        event->ignore();
        return;
    }

    //игнор всего в паузе
    if (m_pauseMenu->isVisible()) {
        event->accept();
        return;
    }

    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        enter();
    }
}

void GameWindow::updateEnterPosition()
{
    if (m_enter && m_enter->isVisible()) {
        m_enter->move(width()  - m_enter->width()  - 10,
                      height() - m_enter->height() - 10);
    }
}

void GameWindow::onSave()
{
    saveGame();

    m_saveNotification->adjustSize();

    int x = (width() - m_saveNotification->width()) / 2;// посередине сверху экрана
    int y = 50;                                         // отступ от верхнего края
    m_saveNotification->move(x, y);

    m_saveNotification->show();
    m_saveNotification->raise();

    QTimer::singleShot(2000, m_saveNotification, &QLabel::hide);
}

//возврат в игру
void GameWindow::onResumeGame()
{
    m_pauseMenu -> hide();
    setFocus();
}

void GameWindow::onBackToMenu()
{
    m_pauseMenu->hide();
    qDebug() << "Сигнал: Выход в меню";
    emit backToMenuClicked(); //в mainwindow
}

void GameWindow::updateBackground() {
    if (m_backgroundPixmap.isNull()) return;

    QPixmap  scaled = m_backgroundPixmap.scaled(this->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QPalette palette;
    palette.setBrush(backgroundRole(), QBrush(scaled));
    setPalette(palette);
}

void GameWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    if (m_pauseMenu) {
        m_pauseMenu->setGeometry(0, 0, width(), height());
    }

    //для перса
    if (m_characterLabel && m_characterLabel->pixmap()) {
        int h = height() * 0.8; // 80% от высоты окна
        int w = (double)h / m_characterLabel->pixmap()->height() * m_characterLabel->pixmap()->width();
        m_characterLabel->setFixedSize(w, h);

        // чуть левее центра, низ уходит за диалоговое окно
        int x = (width() / 2) - w - 80;
        int y = height() - h + 130; // 130 это глубина погружения за диалог
        m_characterLabel->move(x, y);
    }
    else if (m_characterLabel) {
        m_characterLabel->hide();
    }

    updateBackground();
    updateEnterPosition();
}

void GameWindow::togglePause() {
    if (!m_pauseMenu->isVisible()) {
        setAttribute(Qt::WA_TranslucentBackground, false);
        m_pauseMenu->raise();
        if (m_typingSound) m_typingSound->stop();
        m_pauseMenu->show();
        m_pauseMenu->setFocus();

        qDebug() << "Пауза открыта";
    } else {
        onResumeGame();
    }
}

void GameWindow::typeText(const QString& text, QLabel* targetLabel, int speed) {
    if (!targetLabel) return;

    m_typeTimer->stop();          // сброс если чота печаталось
    m_fullText    = text;
    m_activeLabel = targetLabel;
    m_currentText = "";
    m_charIndex   = 0;

    m_activeLabel->setText("");   // очистка
    m_typeTimer  ->start(speed);  // го
}

//звук печати
void GameWindow::onTypeTick() {
    //если пауза
    if (m_pauseMenu && m_pauseMenu->isVisible()) {
        return;
    }

    for (int i = 0; i < 2; ++i) {
        if (m_charIndex < m_fullText.length()) {
            QChar nextChar = m_fullText[m_charIndex];
            m_currentText += nextChar;
            m_activeLabel->setText(m_currentText);

            if (!nextChar.isSpace() && m_typingSound) {
                m_typingSound->play();
            }

            m_charIndex++;
        } else {
            m_typeTimer->stop();
            break;
        }
    }
}

void GameWindow::stopTyping() {
    m_typeTimer->stop();
    if (m_typingSound) m_typingSound->stop();
    m_currentText = "";
    m_charIndex = 0;
    if (m_activeLabel) m_activeLabel->setText("");
}

void GameWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    if (!m_backgroundPixmap.isNull()) {
        // Отрисовка фона строго по размеру текущего окна
        painter.drawPixmap(0, 0, width(), height(), m_backgroundPixmap);
    }
}
