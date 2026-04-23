#include "forestwindow.h"
#include <QTimer>

ForestWindow::ForestWindow(QWidget *parent) : GameWindow(parent)
{
    setupBaseUI();

    m_backgroundPixmap.load(":/images/images/forest.jpg");

    m_stumpBtn   = new QPushButton("Осмотреть старый пень", this);
    m_bushesBtn  = new QPushButton("Поискать в кустах",     this);
    m_villageBtn = new QPushButton("В деревню",             this);

    m_stumpBtn   ->setStyleSheet(styleSheet());
    m_bushesBtn  ->setStyleSheet(styleSheet());
    m_villageBtn ->setStyleSheet(styleSheet());

    m_buttonsCol->addWidget(m_stumpBtn);
    m_buttonsCol->addWidget(m_bushesBtn);
    m_buttonsCol->addWidget(m_villageBtn);

    //коннекты
    connect(m_stumpBtn,   &QPushButton::clicked, this, &ForestWindow::onStumpClicked);
    connect(m_bushesBtn,  &QPushButton::clicked, this, &ForestWindow::onBushesClicked);
    connect(m_villageBtn, &QPushButton::clicked, this, [this](){ goToScene("village"); });
}

void ForestWindow::onEnterScene()
{
    bool wasIn = getState()->getFlag("was_in_forest");

    m_dialogParts.clear();
    m_dialogIndex = 0;

    if (!wasIn) {
        // состояние 1: зашел впервые
        m_dialogParts << "Вы в лесу. Здесь тихо, пахнет прелой листвой."
                      << "Кажется, что за Вами кто-то наблюдает..."
                      << "Но это лишь ветер.";

        typeText(m_dialogParts[m_dialogIndex], m_dialogLabel);
        m_enter->show();
        QTimer::singleShot(0, this, [this]() {
            updateEnterPosition();
        });

        hideActionButtons();
    } else {
        // состояние 2: зашел не впервые
        m_dialogParts << "Вы в лесу. Здесь тихо, пахнет прелой листвой.";

        typeText(m_dialogParts[m_dialogIndex], m_dialogLabel);
        m_enter->hide();
        showActionButtons();
    }

    QTimer::singleShot(100, this, [this]() { updateEnterPosition(); });
    updateBackground();
}

void ForestWindow::enter()
{
    if (m_isDialogFinished) return;

    //скип
    if (m_typeTimer && m_typeTimer->isActive()) {
        m_typeTimer->stop();
        m_dialogLabel->setText(m_fullText);
        return;
    }

    if (m_dialogIndex < m_dialogParts.size() - 1) {
        m_dialogIndex++;
        typeText(m_dialogParts[m_dialogIndex], m_dialogLabel);
    }
    else {
        // текст закончился
        m_isDialogFinished = true;
        getState()->setFlag("was_in_forest", true);
        saveGame();
        m_enter->hide();
        showActionButtons();
    }
}

void ForestWindow::showActionButtons()
{
    m_buttonsContainer->setVisible(true);
}

void ForestWindow::hideActionButtons()
{
    m_buttonsContainer->setVisible(false);
}

void ForestWindow::resetWindow() {
    m_isDialogFinished = false;
    m_dialogIndex = 0;
    m_dialogActive = false;
    hideActionButtons();
    if (m_dialogLabel) m_dialogLabel->clear();
}

void ForestWindow::onStumpClicked()
{
    m_dialogParts.clear();
    m_dialogIndex = 0;

    if (!getState()->getFlag("is_moss_collected")) {
        getState()->setFlag("is_moss_collected", true);
        getState()->addItem("glowing_moss");
        saveGame();

        m_dialogParts << "Вы замечаете под старым пнём светящийся мох."
                      << "Он мягко мерцает в темноте, словно звёздная пыль."
                      << "Вы аккуратно собираете его в мешочек.";

        m_dialogIndex = 0;
        m_isDialogFinished = false; //чтоб ентер работал

        hideActionButtons();
        m_enter->show();

        typeText(m_dialogParts[m_dialogIndex], m_dialogLabel);
        updateEnterPosition();
    }
    else {
        m_dialogParts << "Здесь больше ничего нет.";
        m_isDialogFinished = true;
        typeText(m_dialogParts[m_dialogIndex], m_dialogLabel);
        m_enter->hide();
        showActionButtons();
    }
}

void ForestWindow::onBushesClicked()
{
    m_dialogParts.clear();
    m_dialogIndex = 0;

    if (!getState()->hasItem("old_medallion")) {
        getState()->addItem("old_medallion");
        saveGame();

        m_dialogParts << "В кустах что-то блеснуло. Раздвинув ветки, вы находите старый медальон с выцветшим гербом."
                      << "На обратной стороне едва различима гравировка: «Моему любимому навеки.»";

        m_dialogIndex = 0;
        m_isDialogFinished = false; //чтоб ентер работал

        hideActionButtons();
        m_enter->show();

        typeText(m_dialogParts[m_dialogIndex], m_dialogLabel);
        updateEnterPosition();
    }
    else {
        m_dialogParts << "Здесь только кто то насрал, а больше нихуя.";
        m_isDialogFinished = true;
        typeText(m_dialogParts[m_dialogIndex], m_dialogLabel);
        m_enter->hide();
        showActionButtons();
    }
}
