#include "gateswindow.h"
#include <QTimer>

GatesWindow::GatesWindow(QWidget *parent) : GameWindow(parent)
{
    setupBaseUI();

    m_backgroundPixmap.load(":/images/images/gates.jpg");

    m_comeInBtn  = new QPushButton("Войти",          this);
    m_chestBtn   = new QPushButton("Открыть сундук", this);
    m_villageBtn = new QPushButton("В деревню",      this);

    m_comeInBtn  ->setStyleSheet(styleSheet());
    m_chestBtn   ->setStyleSheet(styleSheet());
    m_villageBtn ->setStyleSheet(styleSheet());

    m_buttonsCol->addWidget(m_comeInBtn);
    m_buttonsCol->addWidget(m_chestBtn);
    m_buttonsCol->addWidget(m_villageBtn);

    //коннекты
    connect(m_comeInBtn,  &QPushButton::clicked, this, &GatesWindow::onComeInClicked);
    connect(m_chestBtn,   &QPushButton::clicked, this, &GatesWindow::onChestClicked);
    connect(m_villageBtn, &QPushButton::clicked, this, [this](){ goToScene("village"); });
}

void GatesWindow::onEnterScene()
{
    bool wasIn = getState()->getFlag("was_in_gates");
    bool hasBlessed = getState()->hasItem("blessed_sword");

    m_dialogParts.clear();
    m_dialogIndex = 0;

    if (!wasIn) {
        // состояние 1: зашел впервые
        m_dialogParts << "Вы стоите перед воротами замка."
                      << "Изнутри доносится леденящий душу вой - то ли ветер, то ли призрак."
                      << "Герой: Без особого оружия здесь делать нечего. Призрак убьёт меня.";

        typeText(m_dialogParts[m_dialogIndex], m_dialogLabel);
        m_enter->show();
        QTimer::singleShot(0, this, [this]() {
            updateEnterPosition();
        });

        hideActionButtons();
    } else {
        // состояние 2: зашел не впервые
        m_dialogParts << (hasBlessed ? "Герой: Теперь я готов освободить жителей деревни от проклятия!"
                                     : "Герой: Без особого оружия здесь делать нечего. Призрак убьёт меня.");

        typeText(m_dialogParts[m_dialogIndex], m_dialogLabel);
        m_enter->hide();
        showActionButtons();
    }

    QTimer::singleShot(100, this, [this]() { updateEnterPosition(); });
    updateBackground();
}

void GatesWindow::enter()
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
        getState()->setFlag("was_in_gates", true);
        saveGame();
        m_enter->hide();
        showActionButtons();
    }
}

void GatesWindow::showActionButtons()
{
    m_buttonsContainer->setVisible(true);
}

void GatesWindow::hideActionButtons()
{
    m_buttonsContainer->setVisible(false);
}

void GatesWindow::resetWindow() {
    m_isDialogFinished = false;
    m_dialogIndex = 0;
    m_dialogActive = false;
    hideActionButtons();
    if (m_dialogLabel) m_dialogLabel->clear();
}

void GatesWindow::onComeInClicked()
{
    //go to castle scene
}

void GatesWindow::onChestClicked()
{

    if (!getState()->getFlag("is_sword_collected")) {
        getState()->setFlag("is_sword_collected", true);
        getState()->addItem("sword");
        saveGame();

        typeText("Вы находите старинный меч.", m_dialogLabel);
    }
    else {
        typeText("Здесь только паутина.", m_dialogLabel);
    }
}
