#include "villagewindow.h"

VillageWindow::VillageWindow(QWidget *parent) : GameWindow(parent)
{
    m_backgroundPixmap.load(":/images/images/village.jpg");
    setAutoFillBackground(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(30, 30, 30, 30); // отступы от краев экрана

    // место для картинки
    m_imageLabel = new QLabel(this);
    m_imageLabel->setAlignment(Qt::AlignCenter);
    layout      -> addWidget(m_imageLabel, 1); // 1 чтоб не растягивалась

    // место для диалога
    m_dialogLabel = new QLabel(this);
    m_dialogLabel->setWordWrap(true);      // перенос текста
    m_dialogLabel->setMinimumHeight(180);

    m_dialogLabel->setStyleSheet(
        "QLabel {"
        "   border: 4px solid Chocolate;"
        "   background-color: rgba(20, 20, 20, 200);"
        "   color: white;"
        "   font-size: 24px;"
        "   padding: 20px;"
        "   border-radius: 12px;"
        "}"
        );
    layout->addWidget(m_dialogLabel);
}

void VillageWindow::onEnterScene()
{
    m_dialogParts.clear();
    m_dialogIndex = 0;

    if (!getState()->getFlag("was_in_village")) {
        // первый раз
        m_dialogParts << "Вы входите в деревню и стоите на деревенской площади. "
                      << "Тишина кажется подозрительной..."
                      << "Староста Зойга, пожилая женщина, подходит к вам."
                      << "Староста Зойга: Спасибо, что пришли! Барон не даёт нам житья. Ночью из замка доносятся крики."
                        " Кузнец Крупеня и знахарка Даниэла могут навести на мысль, как с ним справиться."
                        " Будьте осторожны!";

        typeText(m_dialogParts[m_dialogIndex], m_dialogLabel);
        m_enter      ->show();
    } else {
        // уже были
        m_dialogParts << "Вы снова в деревне. Здесь ничего не изменилось.";

        typeText(m_dialogParts[m_dialogIndex], m_dialogLabel);
        m_enter      ->hide();
    }

    updateBackground();
}

void VillageWindow::enter()
{
    if (m_dialogIndex < m_dialogParts.size() - 1) {
        m_dialogIndex++;
        typeText(m_dialogParts[m_dialogIndex], m_dialogLabel);
    } else {
        // текст закончился
        getState()   ->setFlag("was_in_village", true);
        m_enter      ->hide();
        typeText(m_dialogParts[m_dialogIndex], m_dialogLabel);
    }
}
