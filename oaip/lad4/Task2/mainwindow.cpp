#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QMessageBox>
#include <random>
#include <algorithm>
#include <cmath>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Интерполяционная сортировка и бинарное возведение в степень");
    resize(900, 700);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    // --- Блок ввода и генерации массива ---
    QHBoxLayout *sizeLayout = new QHBoxLayout;
    sizeLayout->addWidget(new QLabel("Размер массива:"));
    sizeSpin = new QSpinBox;
    sizeSpin->setRange(1, 500);
    sizeSpin->setValue(10);
    sizeLayout->addWidget(sizeSpin);
    genBtn = new QPushButton("Сгенерировать случайный массив");
    sizeLayout->addWidget(genBtn);
    sizeLayout->addStretch();
    mainLayout->addLayout(sizeLayout);

    mainLayout->addWidget(new QLabel("Элементы массива (через пробел):"));
    arrayEdit = new QLineEdit;
    arrayEdit->setPlaceholderText("например: 5 2 8 1 9");
    mainLayout->addWidget(arrayEdit);

    sortBtn = new QPushButton("Интерполяционная сортировка (с пошаговым выводом)");
    mainLayout->addWidget(sortBtn);

    outputEdit = new QTextEdit;
    outputEdit->setReadOnly(true);
    outputEdit->setMinimumHeight(300);
    mainLayout->addWidget(outputEdit);

    QHBoxLayout *searchLayout = new QHBoxLayout;
    searchLayout->addWidget(new QLabel("Искомое число:"));
    searchEdit = new QLineEdit;
    searchLayout->addWidget(searchEdit);
    searchLayout->addWidget(new QLabel("Модуль:"));
    modEdit = new QLineEdit;
    modEdit->setText("1000000007");
    searchLayout->addWidget(modEdit);
    searchBtn = new QPushButton("Бинарный поиск и возведение в степень");
    searchLayout->addWidget(searchBtn);
    searchLayout->addStretch();
    mainLayout->addLayout(searchLayout);

    resultLabel = new QLabel;
    resultLabel->setAlignment(Qt::AlignCenter);
    resultLabel->setWordWrap(true);
    mainLayout->addWidget(resultLabel);

    connect(genBtn, &QPushButton::clicked, this, &MainWindow::generateRandom);
    connect(sortBtn, &QPushButton::clicked, this, &MainWindow::interpolationSort);
    connect(searchBtn, &QPushButton::clicked, this, &MainWindow::binarySearchAndPow);
}

MainWindow::~MainWindow() {}

void MainWindow::generateRandom()
{
    int n = sizeSpin->value();
    if (n <= 0) return;
    originalArray.resize(n);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(-100, 100);
    for (int i = 0; i < n; ++i)
        originalArray[i] = dist(gen);
    QString str;
    for (int v : originalArray)
        str += QString::number(v) + " ";
    arrayEdit->setText(str.trimmed());
    outputEdit->clear();
    outputEdit->append("Сгенерирован массив: " + str);
    sortedArray.clear();
    resultLabel->clear();
}

void MainWindow::displayArray(const QVector<int>& arr, const QString& title)
{
    QString str = title + ": ";
    for (int v : arr)
        str += QString::number(v) + " ";
    outputEdit->append(str);
}

// Интерполяционная сортировка (вставками с интерполяционным поиском позиции)
void MainWindow::interpolationSort()
{
    QString text = arrayEdit->text().trimmed();
    if (!text.isEmpty()) {
        originalArray.clear();
        QStringList parts = text.split(' ', Qt::SkipEmptyParts);
        for (const QString& part : parts) {
            bool ok;
            int val = part.toInt(&ok);
            if (ok) originalArray.append(val);
        }
    }

    if (originalArray.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Массив пуст! Введите данные или сгенерируйте.");
        return;
    }

    outputEdit->clear();
    outputEdit->append("Начальный массив: " + arrayEdit->text());

    QVector<int> arr = originalArray;
    int n = arr.size();

    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int left = 0, right = i - 1;
        int pos = i;

        if (arr[left] <= key && key <= arr[right]) {
            while (left <= right && arr[left] != arr[right]) {
                int range = arr[right] - arr[left];
                if (range == 0) break;
                double ratio = static_cast<double>(key - arr[left]) / range;
                int mid = left + static_cast<int>(ratio * (right - left));
                if (mid < left) mid = left;
                if (mid > right) mid = right;
                if (arr[mid] == key) {
                    pos = mid;
                    break;
                } else if (arr[mid] < key) {
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            }
            for (pos = left; pos <= right; ++pos) {
                if (arr[pos] > key) break;
            }
        } else if (key < arr[left]) {
            pos = 0;
        } else {
            pos = i;
        }

        for (int j = i; j > pos; --j) {
            arr[j] = arr[j - 1];
        }
        arr[pos] = key;

        displayArray(arr, "После вставки элемента " + QString::number(key));
    }

    sortedArray = arr;
    outputEdit->append("Сортировка завершена.");
    displayArray(sortedArray, "Результат");
}

// Бинарный поиск в отсортированном массиве
static int binarySearch(const QVector<int>& arr, int target) {
    int left = 0, right = arr.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) return mid;
        else if (arr[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

int MainWindow::binpow(int digit, int powder, int mod) const
{
    if (mod == 1) return 0;
    long long result = 1;
    long long base = digit % mod;
    int exp = powder;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return static_cast<int>(result);
}

void MainWindow::binarySearchAndPow()
{
    if (sortedArray.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Массив не отсортирован. Сначала выполните сортировку.");
        return;
    }

    bool ok;
    int target = searchEdit->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Ошибка", "Введите целое число для поиска.");
        return;
    }
    int mod = modEdit->text().toInt(&ok);
    if (!ok || mod <= 0) {
        QMessageBox::warning(this, "Ошибка", "Введите положительное целое число в поле модуля.");
        return;
    }

    int idx = binarySearch(sortedArray, target);
    if (idx == -1) {
        resultLabel->setText(QString("Число %1 не найдено в массиве.").arg(target));
        return;
    }

    int length = sortedArray.size();
    int result = binpow(idx, length, mod);
    resultLabel->setText(QString("Индекс найденного элемента: %1\n"
                                 "Длина массива: %2\n"
                                 "(%1 ^ %2) mod %3 = %4")
                             .arg(idx).arg(length).arg(mod).arg(result));
}
