#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Функция Аккермана");
    resize(400, 200);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    QHBoxLayout *inputLayout = new QHBoxLayout;
    mEdit = new QLineEdit;
    mEdit->setPlaceholderText("m (целое неотрицательное)");
    nEdit = new QLineEdit;
    nEdit->setPlaceholderText("n (целое неотрицательное)");
    inputLayout->addWidget(mEdit);
    inputLayout->addWidget(nEdit);
    mainLayout->addLayout(inputLayout);

    QPushButton *calcBtn = new QPushButton("Вычислить A(m,n)");
    mainLayout->addWidget(calcBtn);

    resultLabel = new QLabel;
    resultLabel->setAlignment(Qt::AlignCenter);
    resultLabel->setStyleSheet("font-size: 14pt; font-weight: bold;");
    mainLayout->addWidget(resultLabel);

    connect(calcBtn, &QPushButton::clicked, this, &MainWindow::compute);
}

unsigned long long MainWindow::ackermann(int m, int n) const
{
    if (m == 0)
        return n + 1;
    else if (n == 0)
        return ackermann(m - 1, 1);
    else
        return ackermann(m - 1, ackermann(m, n - 1));
}

void MainWindow::compute()
{
    bool okM, okN;
    int m = mEdit->text().toInt(&okM);
    int n = nEdit->text().toInt(&okN);
    if (!okM || !okN || m < 0 || n < 0) {
        QMessageBox::critical(this, "Ошибка", "Введите неотрицательные целые числа!");
        resultLabel->clear();
        return;
    }
    // Для больших m и n может быть переполнение стека или огромное время.
    // Добавим простое ограничение, чтобы не зависало.
    if (m > 3 && n > 5) {
        QMessageBox::warning(this, "Предупреждение", "Значения слишком большие, вычисление может занять много времени или вызвать переполнение стека.");
    }
    unsigned long long result = ackermann(m, n);
    resultLabel->setText(QString("A(%1, %2) = %3").arg(m).arg(n).arg(result));
}
