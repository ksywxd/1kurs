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
    m_menu       = new MainMenu(this);
    m_intro      = new IntroWindow(this);
    m_village    = new VillageWindow(this);
    m_blacksmith = new BlacksmithWindow(this);
    m_witch      = new WitchWindow(this);
    m_forest     = new ForestWindow(this);
    m_gates      = new GatesWindow(this);

    //в стек
    m_stack -> addWidget(m_menu);    //0
    m_stack -> addWidget(m_intro);   //1
    m_stack -> addWidget(m_village); //2
    m_stack -> addWidget(m_blacksmith);
    m_stack -> addWidget(m_witch);
    m_stack -> addWidget(m_forest);
    m_stack -> addWidget(m_gates);

    menuBar()   -> hide();
    statusBar() -> hide();

    // показать меню
    m_stack -> setCurrentIndex(0);

    //КОННЕКТЫ

    //в новую игру
    connect(m_menu, &MainMenu::newGameClicked, this, [this]() {
        m_menu->stopMusic();

        // Сбрасываем логику всех окон
        m_intro->resetWindow();
        m_village->resetWindow();
        m_blacksmith->resetWindow();
        m_witch->resetWindow();
        m_forest->resetWindow();
        m_gates->resetWindow();
        // ... сбрось тут остальные окна, если они есть

        m_intro->startNewGame(); // Это очистит GameState
        m_stack->setCurrentWidget(m_intro);

        m_intro->onEnterScene();
        m_intro->setFocus();
    });

    // продолжить
    connect(m_menu,    &MainMenu::continueClicked,    this, &MainWindow::loadGame);

    //переход в некст сцену
    connect(m_intro,      &IntroWindow::sceneFinished,      this, &MainWindow::changeScene);
    connect(m_village,    &VillageWindow::sceneFinished,    this, &MainWindow::changeScene);
    connect(m_blacksmith, &BlacksmithWindow::sceneFinished, this, &MainWindow::changeScene);
    connect(m_witch,      &WitchWindow::sceneFinished,      this, &MainWindow::changeScene);
    connect(m_forest,     &ForestWindow::sceneFinished,     this, &MainWindow::changeScene);
    connect(m_gates,      &GatesWindow::sceneFinished,      this, &MainWindow::changeScene);

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
    backHandler(m_blacksmith);
    backHandler(m_witch);
    backHandler(m_forest);
    backHandler(m_gates);

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
    if      (sceneId == "intro")      target = m_intro;
    else if (sceneId == "village")    target = m_village;
    else if (sceneId == "blacksmith") target = m_blacksmith;
    else if (sceneId == "witch")      target = m_witch;
    else if (sceneId == "forest")     target = m_forest;
    else if (sceneId == "gates")      target = m_gates;
    // Сюда легко добавить новые сцены: else if (sceneId == "castle") target = m_castle;

    if (target) {
        m_menu->stopMusic();

        // 1. Создаем объект состояния с загруженными данными
        GameState loadedState;
        loadedState.setCurrentScene(sceneId);
        loadedState.setInventory(inventory);
        loadedState.setFlags(flags);

        // 2. Передаем это состояние в окно (метод loadGame внутри GameWindow скопирует данные)
        target->loadGame(loadedState);

        // 3. Переключаем экран
        m_stack->setCurrentWidget(target);

        // 4. ОЧЕНЬ ВАЖНО: Вызываем логику входа.
        // Именно она проверит флаг "was_in_village" и покажет кнопки.
        target->onEnterScene();

        target->setFocus();
        qDebug() << "Игра успешно загружена. Текущая сцена:" << sceneId;
    }
}

void MainWindow::changeScene(const QString& nextSceneId) {
    GameWindow* current = qobject_cast<GameWindow*>(m_stack->currentWidget());
    GameWindow* target  = nullptr;

    // 1. Определяем, куда идем
    if      (nextSceneId == "village")    { target = m_village;    }
    else if (nextSceneId == "intro")      { target = m_intro;      }
    else if (nextSceneId == "blacksmith") { target = m_blacksmith; }
    else if (nextSceneId == "witch")      { target = m_witch;      }
    else if (nextSceneId == "forest")     { target = m_forest;     }
    else if (nextSceneId == "gates")      { target = m_gates;      }

    // Сюда будешь добавлять новые локации: else if (nextSceneId == "forest") target = m_forest;

    if (target && current) {
        //передаем состояние (флаги, предметы)
        target ->loadGame(*current->getState());

        // переключение сцены
        m_stack->setCurrentWidget(target);
        target ->onEnterScene();
        target ->setFocus();

        qDebug() << "Сцена успешно изменена на:" << nextSceneId;
    }
}
