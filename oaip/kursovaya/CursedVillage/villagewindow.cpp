#include "villagewindow.h"
#include <QTimer>

VillageWindow::VillageWindow(QWidget *parent) : GameWindow(parent)
{
    setupBaseUI();

    m_backgroundPixmap.load(":/images/images/village.jpg");
    // m_characterLabel->setPixmap(QPixmap(":/images/char/zoiga.png"));

    m_blacksmithBtn = new QPushButton("Кузница",  this);
    m_witchBtn      = new QPushButton("Знахарня", this);
    m_forestBtn     = new QPushButton("Лес",      this);
    m_gatesBtn      = new QPushButton("Замок",    this);

    m_blacksmithBtn->setStyleSheet(styleSheet());
    m_witchBtn     ->setStyleSheet(styleSheet());
    m_forestBtn    ->setStyleSheet(styleSheet());
    m_gatesBtn     ->setStyleSheet(styleSheet());

    m_buttonsCol->addWidget(m_blacksmithBtn);
    m_buttonsCol->addWidget(m_witchBtn);
    m_buttonsCol->addWidget(m_forestBtn);
    m_buttonsCol->addWidget(m_gatesBtn);

    //коннекты
    connect(m_blacksmithBtn, &QPushButton::clicked, this, [this](){ goToScene("blacksmith"); });
    connect(m_witchBtn,      &QPushButton::clicked, this, [this](){ goToScene("witch"); });
    connect(m_forestBtn,     &QPushButton::clicked, this, [this](){ goToScene("forest"); });
    connect(m_gatesBtn,      &QPushButton::clicked, this, [this](){ goToScene("gates"); });
}

void VillageWindow::onEnterScene()
{
    m_isDialogFinished = getState()->getFlag("was_in_village");

    if (m_buttonsContainer->isVisible() && m_isDialogFinished) {
        return;
    }

    m_dialogParts.clear();
    m_dialogIndex = 0;

    if (!m_isDialogFinished) {
        // первый раз
        m_dialogParts << "Вы входите в деревню и стоите на деревенской площади. "
                      << "Тишина кажется подозрительной..."
                      << "Староста Зойга, пожилая женщина, подходит к вам."
                      << "Староста Зойга: Спасибо, что пришли!"
                      << "Барон не даёт нам житья. Ночью из замка доносятся крики."
                      << " Кузнец Крупеня и знахарка Даниэла могут навести на мысль, как с ним справиться."
                      << " Будьте осторожны!";

        typeText(m_dialogParts[m_dialogIndex], m_dialogLabel);
        m_enter->show();
        QTimer::singleShot(0, this, [this]() {
            updateEnterPosition();
        });

        hideActionButtons();
    } else {
        // уже были
        m_dialogParts << "Вы снова в деревне. Здесь ничего не изменилось.";
        saveGame();
        typeText(m_dialogParts[m_dialogIndex], m_dialogLabel);
        m_enter->hide();
        showActionButtons();
    }

    QTimer::singleShot(100, this, [this]() { updateEnterPosition(); });
    updateBackground();
}

void VillageWindow::enter()
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
        getState()->setFlag("was_in_village", true);
        saveGame();
        m_enter->hide();
        showActionButtons();
    }
}

void VillageWindow::showActionButtons()
{
    m_buttonsContainer->setVisible(true);
}

void VillageWindow::hideActionButtons()
{
    m_buttonsContainer->setVisible(false);
}

void VillageWindow::resetWindow() {
    m_isDialogFinished = false;
    m_dialogIndex = 0;
    m_dialogActive = false;
    hideActionButtons();
    if (m_dialogLabel) m_dialogLabel->clear();
}
