#include "mainwindow.h"
#include <QPushButton>
#include <QMessageBox>
#include <QVBoxLayout>
#include "Expression.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Демонстрация Expression");
    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *layout = new QVBoxLayout(central);

    QPushButton *button = new QPushButton("Вычислить 3 + 4.5 * 5", this);
    layout->addWidget(button);

    connect(button, &QPushButton::clicked, this, &MainWindow::onTestExpression);
}

void MainWindow::onTestExpression()
{
    // Создаём выражение 3 + (4.5 * 5)
    Expression* sube = new BinaryOperation(new Number(4.5), '*', new Number(5));
    Expression* expr = new BinaryOperation(new Number(3), '+', sube);

    double result = expr->evaluate();

    QMessageBox::information(this, "Результат", QString::number(result));

    delete expr; // удаляет всё, включая sube
}
