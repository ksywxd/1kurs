#include "pausemenu.h"
#include <QKeyEvent>

PauseMenu::PauseMenu(QWidget *parent) : QWidget(parent)
{
    // полупрозрачный черный фон
    setAttribute(Qt::WA_TranslucentBackground, false);
    this->resize(parent->size());

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    QPushButton *resumeBtn = new QPushButton("Продолжить", this);
    QPushButton *saveBtn   = new QPushButton("Сохранить",  this);
    QPushButton *exitBtn   = new QPushButton("В меню",     this);

    QString style =
        "QPushButton { background-color: Chocolate;"
        "   color: white;"
        "   font-weight: bold;"
        "   font-size: 20px;"
        "   padding: 15px;"
        "   min-width: 250px;"
        "   border-radius: 10px; }"
        "QPushButton:hover {background-color: rgb(139, 69, 19);"
        "}";

    resumeBtn->setStyleSheet(style);
    saveBtn  ->setStyleSheet(style);
    exitBtn  ->setStyleSheet(style);

    layout->addWidget(resumeBtn);
    layout->addSpacing(15);
    layout->addWidget(saveBtn);
    layout->addSpacing(15);
    layout->addWidget(exitBtn);

    //коннекты
    connect(resumeBtn, &QPushButton::clicked, this, &PauseMenu::resumeClicked);
    connect(saveBtn,   &QPushButton::clicked, this, &PauseMenu::saveClicked);
    connect(exitBtn,   &QPushButton::clicked, this, &PauseMenu::backToMenuClicked);

}

void PauseMenu::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        emit resumeClicked();
        event->accept();
        return;
    }

    if (event->key() == Qt::Key_F11) {
        event->ignore();
        return;
    }

    QWidget::keyPressEvent(event);
}
