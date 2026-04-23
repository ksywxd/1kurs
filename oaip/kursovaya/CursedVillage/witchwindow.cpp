#include "witchwindow.h"
#include <QTimer>
#include <QResizeEvent>

WitchWindow::WitchWindow(QWidget *parent) : GameWindow(parent)
{
    setupBaseUI();

    m_backgroundPixmap.load(":/images/images/witch.jpg");
    m_characterLabel->setPixmap(QPixmap(":/images/images/witch.png"));

    m_glowingMossBtn  = new QPushButton("Светящийся мох", this);
    m_giveMossBtn     = new QPushButton("Отдать мох",     this);
    m_aboutBaronBtn   = new QPushButton("Про барона",     this);
    m_villageBtn      = new QPushButton("В деревню",      this);

    m_glowingMossBtn ->setStyleSheet(styleSheet());
    m_giveMossBtn    ->setStyleSheet(styleSheet());
    m_aboutBaronBtn  ->setStyleSheet(styleSheet());
    m_villageBtn     ->setStyleSheet(styleSheet());

    m_buttonsCol->addWidget(m_glowingMossBtn);
    m_buttonsCol->addWidget(m_giveMossBtn);
    m_buttonsCol->addWidget(m_aboutBaronBtn);
    m_buttonsCol->addWidget(m_villageBtn);

    //коннекты
    connect(m_glowingMossBtn, &QPushButton::clicked, this, &WitchWindow::onGlowingMossClicked);
    connect(m_giveMossBtn,    &QPushButton::clicked, this, &WitchWindow::onGiveMossClicked);
    connect(m_aboutBaronBtn,  &QPushButton::clicked, this, &WitchWindow::onAboutBaronClicked);
    connect(m_villageBtn,     &QPushButton::clicked, this, [this](){ goToScene("village"); });
}

void WitchWindow::onEnterScene()
{
    bool wasIn = getState()->getFlag("was_in_witch");
    bool hasSilverDust = getState()->hasItem("silver_dust");

    m_dialogParts.clear();
    m_dialogIndex = 0;

    if (!wasIn) {
        // состояние 1: зашел впервые
        m_dialogParts << "Вы в уютной избушке с травами, здесь пахнет мятой и сушёными грибами, "
                         "на полках – пузырьки и пучки трав."
                      << "Знахарка Даниэла: О, странник! Ты пришёл вовремя."
                      << "Я чувствую, что в замке творятся тёмные дела. Если ты собираешься туда, тебе понадобится защита."
                      << "Я могу приготовить особое средство, но для него нужен светящийся мох – он растёт в лесу у старого пня."
                      << "Принеси мне его, и я сделаю тебе серебряную пыль, которая поможет против нечисти.";

        typeText(m_dialogParts[m_dialogIndex], m_dialogLabel);
        m_enter->show();
        QTimer::singleShot(0, this, [this]() {
            updateEnterPosition();
        });

        hideActionButtons();
    } else {
        bool finishedQuest = getState()->hasAny(QStringList() << "silver_dust" << "blessed_sword");
        // состояние 2: зашел не впервые
        m_dialogParts << (finishedQuest ? "Знахарка Даниэла: ГЛ в бою!"
                                        : "Знахарка Даниэла: Приветики! Уже всё нашёл?");

        typeText(m_dialogParts[m_dialogIndex], m_dialogLabel);
        m_enter->hide();
        showActionButtons();
    }

    QTimer::singleShot(100, this, [this]() { updateEnterPosition(); });
    updateBackground();
}

void WitchWindow::enter()
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
        getState()->setFlag("was_in_witch", true);
        saveGame();
        m_enter->hide();
        showActionButtons();
    }
}

void WitchWindow::showActionButtons()
{
    m_buttonsContainer->setVisible(true);
}

void WitchWindow::hideActionButtons()
{
    m_buttonsContainer->setVisible(false);
}

void WitchWindow::resetWindow() {
    m_isDialogFinished = false;
    m_dialogIndex = 0;
    m_dialogActive = false;
    hideActionButtons();
    if (m_dialogLabel) m_dialogLabel->clear();
}

void WitchWindow::onGlowingMossClicked()
{
    if (getState()->getFlag("is_moss_collected")) {

        if (getState()->hasAny({"silver_dust", "blessed_sword"})) {
            typeText("У тебя уже есть пыль, иди сражайся!", m_dialogLabel);
            return;
        }

        if (getState()->hasItem("glowing_moss")) {
            typeText("Заебись, мох у тебя. Отдай мне его.", m_dialogLabel);
        }
    } else {
        typeText("Я ж сказала, иди в лесу под пнем чекни.", m_dialogLabel);
    }
}

void WitchWindow::onGiveMossClicked()
{
    if (getState()->getFlag("is_moss_collected")) {
        if (getState()->hasAny({"silver_dust", "blessed_sword"})) {
            typeText("Давай сюда, я приготовлю средство... Желаю удачи!", m_dialogLabel);
            return;
        }

        if (getState()->hasItem("glowing_moss")) {
            getState()->removeItem("glowing_moss");
            getState()->addItem("silver_dust");
            saveGame();
            typeText("Давай сюда, я приготовлю средство... Желаю удачи!", m_dialogLabel);
        }
    }
    else {
        typeText("Всё ок?? У тебя его нет.", m_dialogLabel);
    }
}

void WitchWindow::onAboutBaronClicked()
{
    m_dialogParts.clear();
    m_dialogParts << "Когда барон был жив, он часто приходил ко мне за травами для жены. Она болела, но я ничем не могла помочь."
                  << "После её смерти барон совсем обезумел. Говорят, он носил на груди её подарок — красивый медальон."
                  << "Я помню, как она при жизни приходила ко мне за травами, всегда улыбалась..."
                  << "А после похорон барон метался по лесу, что-то искал. Наверное, тот самый медальон обронил. Если найдёшь – может, и душа его успокоится.";

    m_dialogIndex = 0;
    m_isDialogFinished = false; //чтоб ентер работал

    hideActionButtons();
    m_enter->show();

    typeText(m_dialogParts[m_dialogIndex], m_dialogLabel);
    updateEnterPosition();
}
