#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Ханойские башни");
    resize(600, 400);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    // Верхняя панель: количество колец
    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(new QLabel("Количество колец:"));
    m_spinBox = new QSpinBox;
    m_spinBox->setRange(1, 20);
    m_spinBox->setValue(3);
    topLayout->addWidget(m_spinBox);
    m_button = new QPushButton("Решить");
    topLayout->addWidget(m_button);
    topLayout->addStretch();
    mainLayout->addLayout(topLayout);

    // Поле вывода шагов
    m_output = new QTextEdit;
    m_output->setReadOnly(true);
    mainLayout->addWidget(m_output);

    connect(m_button, &QPushButton::clicked, this, &MainWindow::solveHanoi);
}

void MainWindow::hanoi(int n, char from, char to, char aux, QTextEdit *output)
{
    if (n == 1) {
        output->append(QString("Переместить кольцо 1 с %1 на %2").arg(from).arg(to));
    } else {
        hanoi(n - 1, from, aux, to, output);
        output->append(QString("Переместить кольцо %1 с %2 на %3").arg(n).arg(from).arg(to));
        hanoi(n - 1, aux, to, from, output);
    }
}

void MainWindow::solveHanoi()
{
    int n = m_spinBox->value();
    m_output->clear();
    m_output->append(QString("Начальное состояние: %1 колец на стержне A").arg(n));
    m_output->append("Цель: переместить все кольца на стержень C");
    m_output->append("----------------------------------------");
    hanoi(n, 'A', 'C', 'B', m_output);
    m_output->append("----------------------------------------");
    m_output->append(QString("Всего перемещений: %1").arg((1 << n) - 1));
}
