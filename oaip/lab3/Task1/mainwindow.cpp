#include "mainwindow.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    resize(500, 300);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *layout = new QVBoxLayout(central);

    input = new QLineEdit;
    input->setPlaceholderText("Введите десятичное число (например, 66 или 3.14)");
    layout->addWidget(input);

    QPushButton *btn = new QPushButton("Преобразовать в двоичное");
    layout->addWidget(btn);

    result = new QLabel;
    result->setAlignment(Qt::AlignCenter);
    result->setWordWrap(true);
    layout->addWidget(result);

    connect(btn, &QPushButton::clicked, this, &MainWindow::convert);
}

QString MainWindow::integerToBinary(int n) const
{
    if (n == 0) return "0";
    if (n == 1) return "1";
    return integerToBinary(n / 2) + QString::number(n % 2);
}

QString MainWindow::fractionalToBinary(double frac, int depth) const
{
    if (depth == 0 || frac == 0.0) return "";
    frac *= 2;
    int bit = static_cast<int>(frac);
    if (bit >= 1) {
        return "1" + fractionalToBinary(frac - 1.0, depth - 1);
    } else {
        return "0" + fractionalToBinary(frac, depth - 1);
    }
}

void MainWindow::convert()
{
    bool ok;
    double value = input->text().toDouble(&ok);
    if (!ok) {
        QMessageBox::critical(this, "Ошибка", "Введите корректное число!");
        result->clear();
        return;
    }

    // Отделяем целую и дробную части
    int integerPart = static_cast<int>(value);
    double fractionalPart = value - integerPart;

    // Преобразуем целую часть
    QString binaryInt = integerToBinary(integerPart);

    // Преобразуем дробную часть (максимум 10 битов)
    QString binaryFrac = fractionalToBinary(fractionalPart, 10);
    if (binaryFrac.isEmpty()) binaryFrac = "";

    QString output = "Двоичное: " + binaryInt;
    if (!binaryFrac.isEmpty())
        output += "." + binaryFrac;

    result->setText(output);
}
