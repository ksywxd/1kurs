#include "mainwindow.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Переворот числа (рекурсия)");
    resize(400, 200);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *layout = new QVBoxLayout(central);

    input = new QLineEdit;
    input->setPlaceholderText("Введите целое число без нулей");
    layout->addWidget(input);

    QPushButton *btn = new QPushButton("Перевернуть");
    layout->addWidget(btn);

    result = new QLabel;
    result->setAlignment(Qt::AlignCenter);
    result->setStyleSheet("font-size: 14pt; font-weight: bold;");
    layout->addWidget(result);

    connect(btn, &QPushButton::clicked, this, &MainWindow::reverseNumber);
}

// Рекурсивная проверка наличия нуля в десятичной записи числа
bool MainWindow::hasZero(int n) const
{
    if (n == 0) return false;   // дошли до конца – нулей не было
    if (n % 10 == 0) return true;
    return hasZero(n / 10);
}

// Рекурсивный переворот числа
int MainWindow::reverseRecursive(int n, int rev) const
{
    if (n == 0) return rev;
    return reverseRecursive(n / 10, rev * 10 + n % 10);
}

void MainWindow::reverseNumber()
{
    bool ok;
    int n = input->text().toInt(&ok);
    if (!ok) {
        QMessageBox::critical(this, "Ошибка", "Введите целое число!");
        result->clear();
        return;
    }
    if (n <= 0) {
        QMessageBox::critical(this, "Ошибка", "Введите положительное целое число!");
        result->clear();
        return;
    }
    if (hasZero(n)) {
        QMessageBox::critical(this, "Ошибка", "Число не должно содержать нулей в своей записи!");
        result->clear();
        return;
    }

    int reversed = reverseRecursive(n);
    result->setText("Перевёрнутое число: " + QString::number(reversed));
}
