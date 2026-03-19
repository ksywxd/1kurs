#include "mainmenu.h"
#include <QPushButton>
#include <QVBoxLayout> //вертикальный
#include <QHBoxLayout> //горизонтальный
#include <QDebug> //для вывода в консоль
#include <QPixmap>
#include <QPalette> //для установки фона
#include <QResizeEvent>
#include <QMediaPlaylist>
#include <QLabel>

void MainMenu::updateBackground()
{
    if (m_backgroundMenu.isNull()) return;
    QPixmap scaled = m_backgroundMenu.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::FastTransformation);
    QPalette palette;
    palette.setBrush(backgroundRole(), QBrush(scaled));
    setPalette(palette);
}

void MainMenu::resizeEvent(QResizeEvent *event){
    QWidget::resizeEvent(event);
    updateBackground();
}

MainMenu::MainMenu(QWidget *parent) : QWidget(parent)
{
    m_newGameBtn  = new QPushButton("Новая игра", this);
    m_continueBtn = new QPushButton("Продолжить", this);
    m_exitBtn     = new QPushButton("Выход",      this);

    QString buttonStyle =
        "QPushButton {"
        "   background-color: Chocolate;"
        "   color: white;"
        "   font-weight: bold;"
        "   font-size: 20px;"
        "   padding: 15px;"
        "   min-width: 250px;"
        "   border-radius: 10px; }"
        "   min-width: 250px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgb(139, 69, 19);" // подсветка при наведении
        "}";

    m_newGameBtn  -> setStyleSheet(buttonStyle);
    m_continueBtn -> setStyleSheet(buttonStyle);
    m_exitBtn     -> setStyleSheet(buttonStyle);

    m_newGameBtn  -> setCursor(Qt::PointingHandCursor);
    m_continueBtn -> setCursor(Qt::PointingHandCursor);
    m_exitBtn     -> setCursor(Qt::PointingHandCursor);

    m_newGameBtn  -> setFixedWidth(350);
    m_continueBtn -> setFixedWidth(350);
    m_exitBtn     -> setFixedWidth(350);

    //вертикальный
    m_Vlayout = new QVBoxLayout; //без this тк потом добавляем его к родителю к гл layout

    m_Vlayout -> addStretch();             //пустое пространство сверху
    m_Vlayout -> addWidget(m_newGameBtn);
    m_Vlayout -> addWidget(m_continueBtn);
    m_Vlayout -> addWidget(m_exitBtn);
    m_Vlayout -> addStretch();             //пустое пространство снизу
    m_Vlayout -> setSpacing(15);           //пространство между кнопками

    //горизонтальный
    m_Hlayout = new QHBoxLayout;

    m_Hlayout -> addStretch();        //пространство слева
    m_Hlayout -> addLayout(m_Vlayout);
    m_Hlayout -> addStretch();        //пространство справа

    //саунд справа
    m_soundlayout = new QHBoxLayout;
    m_soundlayout->addStretch();

    //кнопка мута
    m_muteButton = new QPushButton(this);

    m_muteButton->setIcon(QIcon(":/images/images/volume_on.png")); // начальная иконка
    m_muteButton->setIconSize(QSize(50, 50));
    m_muteButton->setFlat(true);                     // убираем рамку
    m_muteButton->setCursor(Qt::PointingHandCursor); // курсор-рука

    m_soundlayout->addWidget(m_muteButton);

    m_muteButton->setStyleSheet(
        "QPushButton {"
        "   background-color: transparent;"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   background-color: transparent;"
        "}"
        "QPushButton:pressed {"
        "   background-color: transparent;"
        "}"
        );

    //главный
    QVBoxLayout *mainVertical = new QVBoxLayout(this);

    // заголовок
    QLabel *titleLabel = new QLabel(this);
    QPixmap titlePixmap(":/images/images/title.png"); // путь к картинке в ресурсах
    if (!titlePixmap.isNull()) {
        titleLabel -> setPixmap(titlePixmap);
        titleLabel -> setAlignment(Qt::AlignCenter);
        titleLabel -> setScaledContents(true);
        titleLabel -> setMaximumSize(1000, 200); // пример ограничения, подберите под свою картинку
    }

    titleLabel   -> setAlignment(Qt::AlignCenter);

    mainVertical -> addSpacing(70);
    mainVertical -> addWidget(titleLabel, 0, Qt::AlignHCenter);

    // растяжение между заголовком и кнопками
    mainVertical -> addStretch();

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout -> addLayout(m_Hlayout, 1);   // кнопки (уже с центрированием)
    bottomLayout -> addLayout(m_soundlayout); // звук справа

    mainVertical -> addLayout(bottomLayout);
    mainVertical -> addStretch();


    //задний фон
    m_backgroundMenu.load(":/images/images/backgroundMenu1.jpg");   // путь к картинке в ресурсах
    if (!m_backgroundMenu.isNull()) {
        updateBackground();            // установить масштабированный фон
        setAutoFillBackground(true);
        qDebug() << "Фон загружен";
    } else {
        qDebug() << "ОШИБКА: не удалось загрузить фоновую картинку!";
        setStyleSheet("background-color: #2c3e50;"); // запасной цвет фона
    }

    //музыка
    m_mediaPlayer = new QMediaPlayer(this);

    QMediaPlaylist *playlist = new QMediaPlaylist(this);
    playlist -> addMedia(QUrl("qrc:/audio/audio/undertaleMenu.wav"));
    playlist -> setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);

    m_mediaPlayer -> setPlaylist(playlist);
    m_mediaPlayer -> setVolume(10);
    m_mediaPlayer -> play();

    connect(m_newGameBtn,  &QPushButton::clicked, this, &MainMenu::onNewGame);
    connect(m_continueBtn, &QPushButton::clicked, this, &MainMenu::onContinue);
    connect(m_exitBtn,     &QPushButton::clicked, this, &MainMenu::onExit);
    connect(m_muteButton,  &QPushButton::clicked, this, &MainMenu::onVolume);
}

void MainMenu::onNewGame()
{
    qDebug() << "Новая игра";
    emit newGameClicked();
}

void MainMenu::onContinue()
{
    qDebug() << "Продолжить";
    emit continueClicked();
}

void MainMenu::onExit()
{
    qDebug() << "Выход";
    emit exitClicked();
}

void MainMenu::onVolume()
{
    m_muted = !m_muted; // переключаем состояние

    if (m_muted) {
        m_mediaPlayer -> setVolume(0);
        m_muteButton  -> setIcon(QIcon(":/images/images/volume_off.png"));
    } else {
        m_mediaPlayer -> setVolume(10);
        m_muteButton  -> setIcon(QIcon(":/images/images/volume_on.png"));
    }
}

void MainMenu::stopMusic()
{
    if(m_mediaPlayer)
        m_mediaPlayer->pause();
}

void MainMenu::playMusic()
{
    if (m_mediaPlayer)
        m_mediaPlayer->play();
}
