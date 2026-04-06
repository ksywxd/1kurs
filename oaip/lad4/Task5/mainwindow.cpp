#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QSpinBox>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Vector и Pair — демонстрация");
    resize(900, 500);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    // Информационная строка
    m_infoLabel = new QLabel;
    mainLayout->addWidget(m_infoLabel);

    // Горизонтальное расположение двух таблиц
    QHBoxLayout *tablesLayout = new QHBoxLayout;

    // Первая таблица (векторы int)
    QVBoxLayout *firstLayout = new QVBoxLayout;
    firstLayout->addWidget(new QLabel("Первая матрица (векторы целых чисел):"));
    m_tableFirst = new QTableWidget;
    m_tableFirst->setColumnCount(1);
    m_tableFirst->setHorizontalHeaderLabels(QStringList() << "Содержимое строки");
    m_tableFirst->horizontalHeader()->setStretchLastSection(true);
    firstLayout->addWidget(m_tableFirst);
    tablesLayout->addLayout(firstLayout);

    // Вторая таблица (векторы пар)
    QVBoxLayout *secondLayout = new QVBoxLayout;
    secondLayout->addWidget(new QLabel("Вторая матрица (векторы пар int:double):"));
    m_tableSecond = new QTableWidget;
    m_tableSecond->setColumnCount(1);
    m_tableSecond->setHorizontalHeaderLabels(QStringList() << "Содержимое строки");
    m_tableSecond->horizontalHeader()->setStretchLastSection(true);
    secondLayout->addWidget(m_tableSecond);
    tablesLayout->addLayout(secondLayout);

    mainLayout->addLayout(tablesLayout);

    // Панель управления главным вектором
    QHBoxLayout *controlLayout = new QHBoxLayout;
    QPushButton *btnAddRow = new QPushButton("Добавить строку");
    QPushButton *btnDeleteRow = new QPushButton("Удалить выбранную строку");
    QPushButton *btnClearAll = new QPushButton("Очистить всё");
    QPushButton *btnInfo = new QPushButton("Информация о векторе");
    controlLayout->addWidget(btnAddRow);
    controlLayout->addWidget(btnDeleteRow);
    controlLayout->addWidget(btnClearAll);
    controlLayout->addWidget(btnInfo);
    mainLayout->addLayout(controlLayout);

    // Панель выбора строки и позиции
    QHBoxLayout *selectLayout = new QHBoxLayout;
    selectLayout->addWidget(new QLabel("Номер строки:"));
    m_rowSpin = new QSpinBox;
    m_rowSpin->setRange(0, 100);
    selectLayout->addWidget(m_rowSpin);
    selectLayout->addWidget(new QLabel("Позиция внутри строки:"));
    m_posSpin = new QSpinBox;
    m_posSpin->setRange(0, 100);
    selectLayout->addWidget(m_posSpin);
    selectLayout->addStretch();
    mainLayout->addLayout(selectLayout);

    // Поле ввода значения
    QHBoxLayout *valueLayout = new QHBoxLayout;
    valueLayout->addWidget(new QLabel("Значение:"));
    m_valueEdit = new QLineEdit;
    m_valueEdit->setPlaceholderText("Для числа — просто число, для пары — 5:3.14");
    valueLayout->addWidget(m_valueEdit);
    valueLayout->addStretch();
    mainLayout->addLayout(valueLayout);

    // Панель операций над содержимым строк
    QGridLayout *editLayout = new QGridLayout;
    QPushButton *btnAddFirst = new QPushButton("Добавить число в конец строки (первая матрица)");
    QPushButton *btnAddSecond = new QPushButton("Добавить пару в конец строки (вторая матрица)");
    QPushButton *btnEditFirst = new QPushButton("Изменить число в строке по позиции");
    QPushButton *btnEditSecond = new QPushButton("Изменить пару в строке по позиции");
    editLayout->addWidget(btnAddFirst, 0, 0);
    editLayout->addWidget(btnAddSecond, 0, 1);
    editLayout->addWidget(btnEditFirst, 1, 0);
    editLayout->addWidget(btnEditSecond, 1, 1);
    mainLayout->addLayout(editLayout);

    // Подключение сигналов
    connect(btnAddRow, &QPushButton::clicked, this, &MainWindow::addRow);
    connect(btnDeleteRow, &QPushButton::clicked, this, &MainWindow::deleteRow);
    connect(btnClearAll, &QPushButton::clicked, this, &MainWindow::clearAll);
    connect(btnInfo, &QPushButton::clicked, this, &MainWindow::showInfo);
    connect(btnAddFirst, &QPushButton::clicked, this, &MainWindow::addNumberToFirst);
    connect(btnAddSecond, &QPushButton::clicked, this, &MainWindow::addPairToSecond);
    connect(btnEditFirst, &QPushButton::clicked, this, &MainWindow::editNumber);
    connect(btnEditSecond, &QPushButton::clicked, this, &MainWindow::editPair);

    updateTables();
    updateInfoLabel();
}

void MainWindow::updateInfoLabel()
{
    m_infoLabel->setText(QString("Всего строк: %1, ёмкость: %2, максимальный размер: %3")
                             .arg(m_data.size())
                             .arg(m_data.capacity())
                             .arg(m_data.max_size()));
}

void MainWindow::updateTables()
{
    int rows = static_cast<int>(m_data.size());
    m_tableFirst->setRowCount(rows);
    m_tableSecond->setRowCount(rows);

    for (int i = 0; i < rows; ++i) {
        const auto& firstVec = m_data[i].first;
        const auto& secondVec = m_data[i].second;

        QString firstText;
        for (size_t j = 0; j < firstVec.size(); ++j) {
            if (j > 0) firstText += ", ";
            firstText += QString::number(firstVec[j]);
        }
        if (firstText.isEmpty()) firstText = "(пусто)";
        m_tableFirst->setItem(i, 0, new QTableWidgetItem(firstText));

        QString secondText;
        for (size_t j = 0; j < secondVec.size(); ++j) {
            if (j > 0) secondText += ", ";
            secondText += QString("%1:%2").arg(secondVec[j].first).arg(secondVec[j].second);
        }
        if (secondText.isEmpty()) secondText = "(пусто)";
        m_tableSecond->setItem(i, 0, new QTableWidgetItem(secondText));
    }
}

void MainWindow::addRow()
{
    DataVector emptyFirst;
    DataPairVector emptySecond;
    m_data.push_back(MainPair(emptyFirst, emptySecond));
    updateTables();
    updateInfoLabel();
}

void MainWindow::deleteRow()
{
    int row = m_rowSpin->value();
    if (row < 0 || row >= static_cast<int>(m_data.size())) {
        QMessageBox::warning(this, "Ошибка", "Некорректный номер строки");
        return;
    }
    m_data.erase(m_data.cbegin() + row);
    updateTables();
    updateInfoLabel();
}

void MainWindow::clearAll()
{
    m_data.clear();
    updateTables();
    updateInfoLabel();
}

void MainWindow::showInfo()
{
    QMessageBox::information(this, "Информация о векторе",
                             QString("size = %1\ncapacity = %2\nmax_size = %3")
                                 .arg(m_data.size())
                                 .arg(m_data.capacity())
                                 .arg(m_data.max_size()));
}

void MainWindow::addNumberToFirst()
{
    int row = m_rowSpin->value();
    if (row < 0 || row >= static_cast<int>(m_data.size())) {
        QMessageBox::warning(this, "Ошибка", "Некорректный номер строки");
        return;
    }
    bool ok;
    int val = m_valueEdit->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Ошибка", "Введите целое число в поле значения.");
        return;
    }
    m_data[row].first.push_back(val);
    updateTables();
}

void MainWindow::addPairToSecond()
{
    int row = m_rowSpin->value();
    if (row < 0 || row >= static_cast<int>(m_data.size())) {
        QMessageBox::warning(this, "Ошибка", "Некорректный номер строки");
        return;
    }
    QString text = m_valueEdit->text().trimmed();
    QStringList parts = text.split(':');
    if (parts.size() != 2) {
        QMessageBox::warning(this, "Ошибка", "Введите пару в формате int:double, например 5:3.14");
        return;
    }
    bool ok1, ok2;
    int first = parts[0].toInt(&ok1);
    double second = parts[1].toDouble(&ok2);
    if (!ok1 || !ok2) {
        QMessageBox::warning(this, "Ошибка", "Неверный формат пары.");
        return;
    }
    m_data[row].second.push_back(DataPair(first, second));
    updateTables();
}

void MainWindow::editNumber()
{
    int row = m_rowSpin->value();
    int pos = m_posSpin->value();
    if (row < 0 || row >= static_cast<int>(m_data.size())) {
        QMessageBox::warning(this, "Ошибка", "Некорректный номер строки");
        return;
    }
    if (pos < 0 || pos >= static_cast<int>(m_data[row].first.size())) {
        QMessageBox::warning(this, "Ошибка", "Позиция вне диапазона. Сначала добавьте число.");
        return;
    }
    bool ok;
    int val = m_valueEdit->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Ошибка", "Введите целое число.");
        return;
    }
    m_data[row].first[pos] = val;
    updateTables();
}

void MainWindow::editPair()
{
    int row = m_rowSpin->value();
    int pos = m_posSpin->value();
    if (row < 0 || row >= static_cast<int>(m_data.size())) {
        QMessageBox::warning(this, "Ошибка", "Некорректный номер строки");
        return;
    }
    if (pos < 0 || pos >= static_cast<int>(m_data[row].second.size())) {
        QMessageBox::warning(this, "Ошибка", "Позиция вне диапазона. Сначала добавьте пару.");
        return;
    }
    QString text = m_valueEdit->text().trimmed();
    QStringList parts = text.split(':');
    if (parts.size() != 2) {
        QMessageBox::warning(this, "Ошибка", "Введите пару в формате int:double");
        return;
    }
    bool ok1, ok2;
    int first = parts[0].toInt(&ok1);
    double second = parts[1].toDouble(&ok2);
    if (!ok1 || !ok2) {
        QMessageBox::warning(this, "Ошибка", "Неверный формат пары.");
        return;
    }
    m_data[row].second[pos] = DataPair(first, second);
    updateTables();
}
