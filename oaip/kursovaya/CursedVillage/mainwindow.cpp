#include <QMessageBox>
#include <QStandardPaths>
#include <QTimer>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_stack = new QStackedWidget(this); //стек
    setCentralWidget(m_stack);

    m_stack -> setContentsMargins(0, 0, 0, 0);
    setContentsMargins(0, 0, 0, 0);

    //страницы
    m_menu    = new MainMenu(this);
    m_intro   = new IntroWindow(this);
    m_village = new VillageWindow(this);

    //в стек
    m_stack -> addWidget(m_menu);    //0
    m_stack -> addWidget(m_intro);   //1
    m_stack -> addWidget(m_village); //2

    menuBar()   -> hide();
    statusBar() -> hide();

    // показать меню
    m_stack -> setCurrentIndex(0);

    //коннекты

    //в новую игру
    connect(m_menu, &MainMenu::newGameClicked, this, [this]() {
        m_menu  -> stopMusic();
        m_intro -> startNewGame();
        m_stack -> setCurrentWidget(m_intro);
    });

    // продолжить
    connect(m_menu,    &MainMenu::continueClicked,    this, &MainWindow::loadGame);

    //переход в некст сцену
    connect(m_intro,   &IntroWindow::sceneFinished,   this, &MainWindow::changeScene);
    connect(m_village, &VillageWindow::sceneFinished, this, &MainWindow::changeScene);

    // возврат в меню из любого окна
    auto backHandler = [this](GameWindow* window) {
        connect(window, &GameWindow::backToMenuClicked, this, [this, window]() {
            window  -> stopTyping();
            m_stack -> setCurrentWidget(m_menu);
            m_menu  -> playMusic();
        });
    };
    backHandler(m_intro);
    backHandler(m_village);

    //выход из игры
    connect(m_menu, &MainMenu::exitClicked, this, &MainWindow::close);

    qDebug() << "MainWindow client area size:" << centralWidget()->size();
    qDebug() << "MainWindow frame size:" << size();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    if (event -> key() == Qt::Key_F11) {
        if (isFullScreen())
            showMaximized();      // выйти из полноэкранного режима
        else
            showFullScreen();     // перейти в полноэкранный режим
    } else {
        QWidget::keyPressEvent(event);
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    m_stack->setGeometry(0, 0, width(), height());
    // Если есть текущий виджет в стеке, растягиваем его на весь стек
    if (m_stack->currentWidget()) {
        m_stack->currentWidget()->setGeometry(0, 0, width(), height());
    }
}

void MainWindow::loadGame() {
    QString savePath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/save.dat";
    QFile file(savePath);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, "Упс!", "Файл сохранения не найден.");
        return;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_0);

    // 1. Читаем данные из файла в том же порядке, в каком записывали
    QString sceneId;
    QSet<QString> inventory;
    QMap<QString, bool> flags;
    in >> sceneId >> inventory >> flags;
    file.close();

    // 2. Создаем временный объект состояния
    GameState loadedState;
    loadedState.setCurrentScene(sceneId);
    loadedState.setInventory(inventory);
    loadedState.setFlags(flags);

    // 3. Логика выбора окна (твоя прежняя, но компактнее)
    GameWindow *target = nullptr;
    if      (sceneId == "intro")   target = m_intro;
    else if (sceneId == "village") target = m_village;
    // Сюда легко добавить новые сцены: else if (sceneId == "castle") target = m_castle;

    if (target) {
        m_menu ->stopMusic();
        target ->loadGame(loadedState);   // Передаем данные в окно
        m_stack->setCurrentWidget(target);
        target ->setFocus();
    }
}

void MainWindow::changeScene(const QString& nextSceneId) {
    GameWindow* current = qobject_cast<GameWindow*>(m_stack->currentWidget());
    GameWindow* target  = nullptr;

    // 1. Определяем, куда идем
    if (nextSceneId == "village") {
        target = m_village;
    } else if (nextSceneId == "intro") {
        target = m_intro;
    }
    // Сюда будешь добавлять новые локации: else if (nextSceneId == "forest") target = m_forest;

    if (target && current) {
        //передаем состояние (флаги, предметы)
        target ->loadGame(*current->getState());

        // переключение сцены
        m_stack->setCurrentWidget(target);
        target ->setFocus();

        qDebug() << "Сцена успешно изменена на:" << nextSceneId;
    }
}
