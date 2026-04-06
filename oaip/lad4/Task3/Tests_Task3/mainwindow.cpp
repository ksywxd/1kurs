#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QMessageBox>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Медианы троек");
    resize(600, 400);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *layout = new QVBoxLayout(central);

    layout->addWidget(new QLabel("Введите последовательность целых чисел (через пробел или запятую):"));

    inputEdit = new QLineEdit;
    inputEdit->setPlaceholderText("например: 1,5,1,4,5,6,2,1,3,4,4,4,5,7");
    layout->addWidget(inputEdit);

    computeBtn = new QPushButton("Вычислить медианы");
    layout->addWidget(computeBtn);

    outputEdit = new QTextEdit;
    outputEdit->setReadOnly(true);
    layout->addWidget(outputEdit);

    connect(computeBtn, &QPushButton::clicked, this, &MainWindow::computeMedians);
}

QVector<int> MainWindow::parseInput(const QString &text) const
{
    QVector<int> result;
    QString cleaned = text;
    cleaned.replace(',', ' ');
    QStringList parts = cleaned.split(' ', Qt::SkipEmptyParts);
    for (const QString &part : parts) {
        bool ok;
        int val = part.toInt(&ok);
        if (ok)
            result.append(val);
    }
    return result;
}

double MainWindow::medianOfThree(int a, int b, int c) const
{
    int arr[3] = {a, b, c};
    std::sort(arr, arr + 3);
    return arr[1]; // средний элемент
}

double MainWindow::averageOfTwo(int a, int b) const
{
    return (a + b) / 2.0;
}

void MainWindow::computeMedians()
{
    QString text = inputEdit->text().trimmed();
    if (text.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите последовательность чисел.");
        return;
    }

    QVector<int> a = parseInput(text);
    if (a.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось распознать числа.");
        return;
    }

    QVector<double> b;
    int n = a.size();
    int i = 0;
    while (i < n) {
        if (i + 2 < n) { // полная тройка
            double med = medianOfThree(a[i], a[i+1], a[i+2]);
            b.append(med);
            i += 3;
        } else if (i + 1 < n) { // два элемента
            double avg = averageOfTwo(a[i], a[i+1]);
            b.append(avg);
            i += 2;
        } else { // один элемент
            b.append(a[i]);
            i += 1;
        }
    }

    // Формируем строку вывода
    QString result;
    for (int j = 0; j < b.size(); ++j) {
        if (j > 0) result += ", ";
        // Выводим как целое, если значение целое, иначе с десятичной точкой
        if (b[j] == static_cast<int>(b[j]))
            result += QString::number(static_cast<int>(b[j]));
        else
            result += QString::number(b[j], 'f', 1);
    }

    outputEdit->setText("Вектор b = { " + result + " }");
}
