#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QElapsedTimer>
#include <QMessageBox>
#include <random>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Сортировки и бинарный поиск");
    resize(900, 600);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    // Строка ввода размера и генерации
    QHBoxLayout *sizeLayout = new QHBoxLayout;
    sizeLayout->addWidget(new QLabel("Размер массива:"));
    sizeSpin = new QSpinBox;
    sizeSpin->setRange(1, 100000);
    sizeSpin->setValue(10);
    sizeLayout->addWidget(sizeSpin);
    genBtn = new QPushButton("Сгенерировать случайный массив");
    sizeLayout->addWidget(genBtn);
    sizeLayout->addStretch();
    mainLayout->addLayout(sizeLayout);

    // Поле для ручного ввода массива (через пробел)
    mainLayout->addWidget(new QLabel("Введите элементы массива (через пробел):"));
    arrayEdit = new QLineEdit;
    arrayEdit->setPlaceholderText("например: 5 2 8 1 9");
    mainLayout->addWidget(arrayEdit);

    // Кнопки сортировок
    QHBoxLayout *sortLayout = new QHBoxLayout;
    heapBtn = new QPushButton("Heap sort");
    quickBtn = new QPushButton("Quick sort");
    mergeBtn = new QPushButton("Merge sort");
    sortLayout->addWidget(heapBtn);
    sortLayout->addWidget(quickBtn);
    sortLayout->addWidget(mergeBtn);
    sortLayout->addStretch();
    mainLayout->addLayout(sortLayout);

    // Поле для вывода результата
    resultEdit = new QTextEdit;
    resultEdit->setReadOnly(true);
    resultEdit->setMinimumHeight(200);
    mainLayout->addWidget(resultEdit);

    // Блок бинарного поиска
    QHBoxLayout *searchLayout = new QHBoxLayout;
    searchLayout->addWidget(new QLabel("Искомое число:"));
    searchEdit = new QLineEdit;
    searchLayout->addWidget(searchEdit);
    searchBtn = new QPushButton("Бинарный поиск");
    searchLayout->addWidget(searchBtn);
    searchLayout->addStretch();
    mainLayout->addLayout(searchLayout);

    searchResultLabel = new QLabel;
    searchResultLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(searchResultLabel);

    // Соединения
    connect(genBtn, &QPushButton::clicked, this, &MainWindow::generateRandom);
    connect(heapBtn, &QPushButton::clicked, this, &MainWindow::sortHeap);
    connect(quickBtn, &QPushButton::clicked, this, &MainWindow::sortQuick);
    connect(mergeBtn, &QPushButton::clicked, this, &MainWindow::sortMerge);
    connect(searchBtn, &QPushButton::clicked, this, static_cast<void (MainWindow::*)()>(&MainWindow::binarySearch));
}

MainWindow::~MainWindow() {}

void MainWindow::generateRandom()
{
    int n = sizeSpin->value();
    if (n <= 0) return;
    originalArray.resize(n);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(-1000, 1000);
    for (int i = 0; i < n; ++i)
        originalArray[i] = dist(gen);
    // Отображаем сгенерированный массив в поле ввода
    QString str;
    for (int v : originalArray)
        str += QString::number(v) + " ";
    arrayEdit->setText(str.trimmed());
    resultEdit->append("Сгенерирован массив из " + QString::number(n) + " элементов.");
}

void MainWindow::displayArray(const QVector<int>& arr, const QString& title)
{
    QString str = title + ": ";
    for (int v : arr)
        str += QString::number(v) + " ";
    resultEdit->append(str);
}

// ------------------- Heap sort -------------------
void MainWindow::heapSort(int* arr, int n)
{
    // Построение кучи (heapify)
    for (int i = n / 2 - 1; i >= 0; --i) {
        int root = i;
        while (true) {
            int largest = root;
            int left = 2 * root + 1;
            int right = 2 * root + 2;
            if (left < n && arr[left] > arr[largest]) largest = left;
            if (right < n && arr[right] > arr[largest]) largest = right;
            if (largest == root) break;
            std::swap(arr[root], arr[largest]);
            root = largest;
        }
    }
    // Извлечение элементов из кучи
    for (int i = n - 1; i > 0; --i) {
        std::swap(arr[0], arr[i]);
        int root = 0;
        int size = i;
        while (true) {
            int largest = root;
            int left = 2 * root + 1;
            int right = 2 * root + 2;
            if (left < size && arr[left] > arr[largest]) largest = left;
            if (right < size && arr[right] > arr[largest]) largest = right;
            if (largest == root) break;
            std::swap(arr[root], arr[largest]);
            root = largest;
        }
    }
}

// ------------------- Quick sort -------------------
int MainWindow::partition(int* arr, int low, int high)
{
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j] <= pivot) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void MainWindow::quickSort(int* arr, int low, int high)
{
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// ------------------- Merge sort -------------------
void MainWindow::merge(int* arr, int l, int m, int r)
{
    int n1 = m - l + 1;
    int n2 = r - m;
    int* L = new int[n1];
    int* R = new int[n2];
    for (int i = 0; i < n1; ++i) L[i] = arr[l + i];
    for (int j = 0; j < n2; ++j) R[j] = arr[m + 1 + j];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    delete[] L;
    delete[] R;
}

void MainWindow::mergeSort(int* arr, int l, int r)
{
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// ------------------- Binary search -------------------
int MainWindow::binarySearch(const int* arr, int size, int target)
{
    int left = 0, right = size - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) return mid;
        else if (arr[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

// ------------------- Слоты сортировок -------------------
void MainWindow::sortHeap()
{
    if (originalArray.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Массив пуст. Сначала сгенерируйте или введите данные.");
        return;
    }
    QVector<int> copy = originalArray;
    QElapsedTimer timer;
    timer.start();
    heapSort(copy.data(), copy.size());
    qint64 elapsed = timer.nsecsElapsed() / 1000; // микросекунды
    resultEdit->append(QString("Heap sort: %1 мкс").arg(elapsed));
    displayArray(copy, "Heap sort result");
}

void MainWindow::sortQuick()
{
    if (originalArray.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Массив пуст.");
        return;
    }
    QVector<int> copy = originalArray;
    QElapsedTimer timer;
    timer.start();
    quickSort(copy.data(), 0, copy.size() - 1);
    qint64 elapsed = timer.nsecsElapsed() / 1000;
    resultEdit->append(QString("Quick sort: %1 мкс").arg(elapsed));
    displayArray(copy, "Quick sort result");
}

void MainWindow::sortMerge()
{
    if (originalArray.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Массив пуст.");
        return;
    }
    QVector<int> copy = originalArray;
    QElapsedTimer timer;
    timer.start();
    mergeSort(copy.data(), 0, copy.size() - 1);
    qint64 elapsed = timer.nsecsElapsed() / 1000;
    resultEdit->append(QString("Merge sort: %1 мкс").arg(elapsed));
    displayArray(copy, "Merge sort result");
}

void MainWindow::binarySearch()
{
    if (originalArray.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Массив пуст.");
        return;
    }
    bool ok;
    int target = searchEdit->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Ошибка", "Введите целое число для поиска.");
        return;
    }
    // Для бинарного поиска массив должен быть отсортирован. Сделаем копию и отсортируем (например, быстрой сортировкой)
    QVector<int> sorted = originalArray;
    quickSort(sorted.data(), 0, sorted.size() - 1);
    int idx = binarySearch(sorted.data(), sorted.size(), target);
    if (idx == -1)
        searchResultLabel->setText(QString("Число %1 не найдено.").arg(target));
    else
        searchResultLabel->setText(QString("Число %1 найдено на позиции %2 (в отсортированном массиве).").arg(target).arg(idx));
}
