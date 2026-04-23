#include <QTimer>
#include "introwindow.h"
#include <QDebug>

IntroWindow::IntroWindow(QWidget *parent) : GameWindow(parent)
{
    this->setStyleSheet("background-color: black; color: white; border: none;");
    this->setAttribute(Qt::WA_StyledBackground, true);

    m_mainLayout = new QVBoxLayout(this);    // главный layout
    setContentsMargins(0, 0, 0, 0);
    m_mainLayout -> setContentsMargins(0, 0, 0, 0);

    m_mainLayout->addStretch();

    //текст
    m_dialogLabel  = new QLabel(this);
    m_dialogLabel  -> setAlignment(Qt::AlignCenter);
    m_dialogLabel  -> setWordWrap(true);
    m_mainLayout   -> addWidget(m_dialogLabel);

    //кнопка перехода в деревню
    m_actionButton = new QPushButton(this);

    m_actionButton -> hide(); // изначально скрыта
    m_mainLayout   -> addWidget(m_actionButton, 0, Qt::AlignCenter);

    m_mainLayout->addStretch();

    //коннекты
    connect(m_actionButton, &QPushButton::clicked, this, &IntroWindow::onActionBtn);
}

void IntroWindow::onEnterScene()
{
    m_prologueParts.clear();
    m_prologueParts << "Много лет назад эти земли принадлежали барону фон Дугич..."
                    << "После смерти любимой жены он вышел из себя и обложил крестьян непомерными налогами..."
                    << "Однажды ночью жители не выдержали — ворвались в замок и убили барона."
                    << "Перед смертью он проклял своих убийц."
                    << "Говорят, его призрак до сих пор бродит по залам, не находя покоя."
                    << "Вы — странник, прибывший в деревню у подножия холма. Местные молят о помощи.";
    saveGame();
    m_prologueIndex = 0;
    m_inPrologue = true;
    m_dialogLabel -> setText(m_prologueParts[m_prologueIndex]);

    m_dialogLabel -> setStyleSheet(
        "color: white;"
        "font-size: 28px;"
        "font-family: 'Arial';"
        "font-weight: normal;"
        );

    m_actionButton->setStyleSheet(
        "QPushButton {"
        "   background-color: gray;"
        "   color: white;"
        "   font: bold 20px;"
        "   padding: 8px;"
        "   min-width: 150px;"
        "}"
        "QPushButton:hover { "
        "   background-color: darkgray; }"
        );

    m_actionButton -> hide();
    m_enter        -> show();

    QTimer::singleShot(0, this, [this]() {
        updateEnterPosition();
    });

    m_enter -> raise();

    qDebug() << "Размер IntroWindow:" << size();
    qDebug() << "Размер окна:" << topLevelWidget()->size();
}

void IntroWindow::enter()
{
    if (m_prologueIndex < m_prologueParts.size() - 1) {
        m_prologueIndex++;
        m_dialogLabel -> setText(m_prologueParts[m_prologueIndex]);
    } else {
        finishPrologue();
    }
}

void IntroWindow::finishPrologue()
{
    m_actionButton -> setText("В путь");
    m_actionButton -> show();
    m_enter        -> hide();
}

void IntroWindow::onActionBtn()
{
    getState()->setCurrentScene("village");
    emit sceneFinished("village");
}
