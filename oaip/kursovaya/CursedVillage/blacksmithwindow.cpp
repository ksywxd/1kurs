#include "blacksmithwindow.h"
#include <QTimer>
#include <QResizeEvent>

BlacksmithWindow::BlacksmithWindow(QWidget *parent) : GameWindow(parent)
{
    setupBaseUI();

    m_backgroundPixmap.load(":/images/images/blacksmith.jpg");
    m_characterLabel->setPixmap(QPixmap(":/images/images/blacksmith.png"));

    m_silverDustBtn  = new QPushButton("Серебряная пыль",  this);
    m_baronsSwordBtn = new QPushButton("Меч барона",       this);
    m_enchantBtn     = new QPushButton("Зачаровать меч",   this);
    m_villageBtn     = new QPushButton("В деревню",        this);

    m_silverDustBtn ->setStyleSheet(styleSheet());
    m_baronsSwordBtn->setStyleSheet(styleSheet());
    m_enchantBtn    ->setStyleSheet(styleSheet());
    m_villageBtn    ->setStyleSheet(styleSheet());

    m_buttonsCol->addWidget(m_silverDustBtn);
    m_buttonsCol->addWidget(m_baronsSwordBtn);
    m_buttonsCol->addWidget(m_enchantBtn);
    m_buttonsCol->addWidget(m_villageBtn);

    //коннекты
    connect(m_silverDustBtn,  &QPushButton::clicked, this, &BlacksmithWindow::onSilverDustClicked);
    connect(m_baronsSwordBtn, &QPushButton::clicked, this, &BlacksmithWindow::onBaronsSwordClicked);
    connect(m_enchantBtn,     &QPushButton::clicked, this, &BlacksmithWindow::onEnchantClicked);
    connect(m_villageBtn,     &QPushButton::clicked, this, [this](){ goToScene("village"); });
}

void BlacksmithWindow::onEnterScene()
{
    bool wasIn = getState()->getFlag("was_in_blacksmith");
    bool hasBlessed = getState()->hasItem("blessed_sword");

    m_dialogParts.clear();
    m_dialogIndex = 0;

    if (!wasIn) {
        // состояние 1: зашел впервые
        m_dialogParts << "В кузнице жарко, пахнет металлом и углём."
                      << "Кузнец Крупеня: Слышал, ты к барону собрался? Храбрец!"
                      << "Обычный меч против призрака бесполезен. Нужно зачаровать клинок. "
                      << "Для этого требуется серебряная пыль."
                      << "А да..."
                      << "Когда-то при жизни барон заказывал у меня меч. "
                         "Красивый был клинок, лёгкий."
                      << "Рассказывал про свою жену..."
                      << "Она умерла от лихорадки, и барон с тех пор сам не свой стал. "
                      << "Жаль их обоих.";

        typeText(m_dialogParts[m_dialogIndex], m_dialogLabel);
        m_enter->show();
        QTimer::singleShot(0, this, [this]() {
            updateEnterPosition();
        });

        hideActionButtons();
    } else {
        // состояние 2: зашел не впервые
        m_dialogParts << (hasBlessed ? "Кузнец Крупеня: Береги себя, пусть этот меч принесет тебе победу."
                                     : "Кузнец Крупеня: Приветствую! Удалось найти что-нибудь?");

        typeText(m_dialogParts[m_dialogIndex], m_dialogLabel);
        m_enter->hide();
        showActionButtons();
    }

    QTimer::singleShot(100, this, [this]() { updateEnterPosition(); });
    updateBackground();
}

void BlacksmithWindow::enter()
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
        getState()->setFlag("was_in_blacksmith", true);
        saveGame();
        m_enter->hide();
        showActionButtons();
    }
}

void BlacksmithWindow::showActionButtons()
{
    m_buttonsContainer->setVisible(true);
}

void BlacksmithWindow::hideActionButtons()
{
    m_buttonsContainer->setVisible(false);
}

void BlacksmithWindow::resetWindow() {
    m_isDialogFinished = false;
    m_dialogIndex = 0;
    m_dialogActive = false;
    hideActionButtons();
    if (m_dialogLabel) m_dialogLabel->clear();
}

void BlacksmithWindow::onSilverDustClicked()
{
    if (getState()->hasItem("blessed_sword")) {
        typeText("Да ты ж уже нашёл её.", m_dialogLabel);
        return;
    }

    if (getState()->hasItem("silver_dust")) {
        typeText("Пыль уже у тебя.", m_dialogLabel);
    } else {
        typeText("Где её взять — не знаю, спроси у знахарки.", m_dialogLabel);
    }
}

void BlacksmithWindow::onBaronsSwordClicked()
{
    if (getState()->hasItem("blessed_sword")) {
        typeText("Ну куда ты тыкаешь.", m_dialogLabel);
        return;
    }

    if (getState()->getFlag("is_sword_collected")) {
        typeText("Меч уже у тебя.", m_dialogLabel);
    } else {
        typeText("Меч может лежать у входа в замок в сундуке.", m_dialogLabel);
    }
}

void BlacksmithWindow::onEnchantClicked()
{
    if (getState()->hasItem("blessed_sword")) {
        typeText("Твой меч уже зачарован серебряной пылью. Иди в замок!", m_dialogLabel);
        return;
    }

    // если есть все
    if (getState()->hasAll({"sword", "silver_dust"})) {
        getState()->removeItem("sword");
        getState()->removeItem("silver_dust");
        getState()->addItem("blessed_sword");
        getState()->setFlag("is_sword_enchanted", true);

        saveGame();
        typeText("Давай зачарую... Держи. Пусть он принесёт тебе удачу.", m_dialogLabel);
    }
    else {
        //если одного нет
        if (!getState()->hasAny({"sword", "silver_dust"})) {
            typeText("У тебя же нихуя нет. Идиот, принеси меч и пыль.", m_dialogLabel);
        } else if (!getState()->hasItem("sword")) {
            typeText("Пыль есть, а меч-то где?", m_dialogLabel);
        } else {
            typeText("Без серебряной пыли я не смогу зачаровать этот меч, дебил", m_dialogLabel);
        }
    }
}
