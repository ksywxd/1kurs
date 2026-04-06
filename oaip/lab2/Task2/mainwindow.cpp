#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QDateEdit>
#include <QTimeEdit>
#include <QTextEdit>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QHeaderView>
#include <QLabel>
#include <QLocale>
#include <QMap>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Железнодорожные кассы");
    resize(900, 700);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    // --- Ввод данных для добавления/редактирования ---
    QGroupBox *inputBox = new QGroupBox("Данные рейса");
    QGridLayout *inputLayout = new QGridLayout(inputBox);
    inputLayout->addWidget(new QLabel("Дата:"), 0, 0);
    m_dateEdit = new QDateEdit(QDate::currentDate());
    m_dateEdit->setCalendarPopup(true);
    inputLayout->addWidget(m_dateEdit, 0, 1);

    inputLayout->addWidget(new QLabel("Номер рейса:"), 0, 2);
    m_numberEdit = new QLineEdit;
    inputLayout->addWidget(m_numberEdit, 0, 3);

    inputLayout->addWidget(new QLabel("Пункт назначения:"), 1, 0);
    m_destEdit = new QLineEdit;
    inputLayout->addWidget(m_destEdit, 1, 1);

    inputLayout->addWidget(new QLabel("Время отправления:"), 1, 2);
    m_timeEdit = new QTimeEdit(QTime::currentTime());
    inputLayout->addWidget(m_timeEdit, 1, 3);

    inputLayout->addWidget(new QLabel("Всего купе:"), 2, 0);
    m_totalCoupeEdit = new QLineEdit;
    inputLayout->addWidget(m_totalCoupeEdit, 2, 1);

    inputLayout->addWidget(new QLabel("Всего плацкарт:"), 2, 2);
    m_totalPlatzkartEdit = new QLineEdit;
    inputLayout->addWidget(m_totalPlatzkartEdit, 2, 3);

    inputLayout->addWidget(new QLabel("Свободно купе:"), 3, 0);
    m_freeCoupeEdit = new QLineEdit;
    inputLayout->addWidget(m_freeCoupeEdit, 3, 1);

    inputLayout->addWidget(new QLabel("Свободно плацкарт:"), 3, 2);
    m_freePlatzkartEdit = new QLineEdit;
    inputLayout->addWidget(m_freePlatzkartEdit, 3, 3);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    m_addBtn = new QPushButton("Добавить");
    m_editBtn = new QPushButton("Изменить выбранный");
    m_deleteBtn = new QPushButton("Удалить выбранный");
    m_sortBtn = new QPushButton("Сортировать");
    m_refreshBtn = new QPushButton("Обновить таблицу");
    btnLayout->addWidget(m_addBtn);
    btnLayout->addWidget(m_editBtn);
    btnLayout->addWidget(m_deleteBtn);
    btnLayout->addWidget(m_sortBtn);
    btnLayout->addWidget(m_refreshBtn);
    inputLayout->addLayout(btnLayout, 4, 0, 1, 4);

    mainLayout->addWidget(inputBox);

    // --- Таблица для отображения данных ---
    m_table = new QTableWidget;
    m_table->setColumnCount(8);
    m_table->setHorizontalHeaderLabels(QStringList() << "Дата" << "№ рейса" << "Назначение"
                                                     << "Время" << "Купе(всего)" << "Плац(всего)"
                                                     << "Своб. купе" << "Своб. плац");
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    mainLayout->addWidget(m_table);

    // --- Функции отчётов ---
    QGroupBox *funcBox = new QGroupBox("Специальные функции");
    QHBoxLayout *funcLayout = new QHBoxLayout(funcBox);
    m_functionCombo = new QComboBox;
    m_functionCombo->addItem("Свободные места по каждому рейсу и типу");
    m_functionCombo->addItem("Расписание по дням недели");
    m_functionCombo->addItem("Список рейсов в пункт назначения");
    m_executeFuncBtn = new QPushButton("Выполнить");
    funcLayout->addWidget(m_functionCombo);
    funcLayout->addWidget(m_executeFuncBtn);
    mainLayout->addWidget(funcBox);

    m_resultEdit = new QTextEdit;
    m_resultEdit->setReadOnly(true);
    mainLayout->addWidget(m_resultEdit);

    // --- Кнопки работы с файлами ---
    QHBoxLayout *fileLayout = new QHBoxLayout;
    QPushButton *openBtn = new QPushButton("Открыть файл");
    QPushButton *saveBtn = new QPushButton("Сохранить файл");
    fileLayout->addWidget(openBtn);
    fileLayout->addWidget(saveBtn);
    mainLayout->addLayout(fileLayout);

    // Подключение сигналов
    connect(openBtn, &QPushButton::clicked, this, &MainWindow::onOpenFile);
    connect(saveBtn, &QPushButton::clicked, this, &MainWindow::onSaveFile);
    connect(m_addBtn, &QPushButton::clicked, this, &MainWindow::onAdd);
    connect(m_editBtn, &QPushButton::clicked, this, &MainWindow::onEdit);
    connect(m_deleteBtn, &QPushButton::clicked, this, &MainWindow::onDelete);
    connect(m_sortBtn, &QPushButton::clicked, this, &MainWindow::onSort);
    connect(m_refreshBtn, &QPushButton::clicked, this, &MainWindow::onRefreshTable);
    connect(m_executeFuncBtn, &QPushButton::clicked, this, &MainWindow::onExecuteFunction);
}

MainWindow::~MainWindow() {}

void MainWindow::loadFromFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл");
        return;
    }
    m_trains.clear();
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        try {
            Train t = Train::fromString(line);
            m_trains.append(t);
        } catch (const std::exception& e) {
            QMessageBox::warning(this, "Ошибка", "Строка пропущена: " + line + "\n" + e.what());
        }
    }
    file.close();
    updateTable(m_trains);
    m_currentFile = fileName;
}

void MainWindow::saveToFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось сохранить файл");
        return;
    }
    QTextStream out(&file);
    for (const Train& t : m_trains) {
        out << t.toString() << "\n";
    }
    file.close();
    m_currentFile = fileName;
}

void MainWindow::updateTable(const QVector<Train>& data)
{
    m_table->setRowCount(data.size());
    for (int i = 0; i < data.size(); ++i) {
        const Train& t = data[i];
        m_table->setItem(i, 0, new QTableWidgetItem(t.getDate().toString("dd.MM.yyyy")));
        m_table->setItem(i, 1, new QTableWidgetItem(QString::number(t.getNumber())));
        m_table->setItem(i, 2, new QTableWidgetItem(t.getDestination()));
        m_table->setItem(i, 3, new QTableWidgetItem(t.getTime().toString("hh:mm")));
        m_table->setItem(i, 4, new QTableWidgetItem(QString::number(t.getTotalCoupe())));
        m_table->setItem(i, 5, new QTableWidgetItem(QString::number(t.getTotalPlatzkart())));
        m_table->setItem(i, 6, new QTableWidgetItem(QString::number(t.getFreeCoupe())));
        m_table->setItem(i, 7, new QTableWidgetItem(QString::number(t.getFreePlatzkart())));
    }
}

void MainWindow::onOpenFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть файл", "",
                                                    "Текстовые файлы (*.txt)");
    if (fileName.isEmpty()) return;
    loadFromFile(fileName);
}

void MainWindow::onSaveFile()
{
    if (m_currentFile.isEmpty()) {
        QString fileName = QFileDialog::getSaveFileName(this, "Сохранить файл", "",
                                                        "Текстовые файлы (*.txt)");
        if (fileName.isEmpty()) return;
        m_currentFile = fileName;
    }
    saveToFile(m_currentFile);
}

void MainWindow::onAdd()
{
    bool ok;
    int num = m_numberEdit->text().toInt(&ok);
    if (!ok) { QMessageBox::critical(this, "Ошибка", "Неверный номер рейса"); return; }
    int totalC = m_totalCoupeEdit->text().toInt(&ok);
    if (!ok) { QMessageBox::critical(this, "Ошибка", "Неверное число купе"); return; }
    int totalP = m_totalPlatzkartEdit->text().toInt(&ok);
    if (!ok) { QMessageBox::critical(this, "Ошибка", "Неверное число плацкарт"); return; }
    int freeC = m_freeCoupeEdit->text().toInt(&ok);
    if (!ok || freeC > totalC) { QMessageBox::critical(this, "Ошибка", "Некорректное число свободных купе"); return; }
    int freeP = m_freePlatzkartEdit->text().toInt(&ok);
    if (!ok || freeP > totalP) { QMessageBox::critical(this, "Ошибка", "Некорректное число свободных плацкарт"); return; }

    Train t(m_dateEdit->date(), num, m_destEdit->text(), m_timeEdit->time(),
            totalC, totalP, freeC, freeP);
    m_trains.append(t);
    updateTable(m_trains);
    m_resultEdit->append("Добавлен рейс: " + t.toString());
}

void MainWindow::onEdit()
{
    int row = m_table->currentRow();
    if (row < 0 || row >= m_trains.size()) {
        QMessageBox::warning(this, "Предупреждение", "Выберите запись для редактирования");
        return;
    }
    bool ok;
    int num = m_numberEdit->text().toInt(&ok);
    if (!ok) { QMessageBox::critical(this, "Ошибка", "Неверный номер рейса"); return; }
    int totalC = m_totalCoupeEdit->text().toInt(&ok);
    if (!ok) { QMessageBox::critical(this, "Ошибка", "Неверное число купе"); return; }
    int totalP = m_totalPlatzkartEdit->text().toInt(&ok);
    if (!ok) { QMessageBox::critical(this, "Ошибка", "Неверное число плацкарт"); return; }
    int freeC = m_freeCoupeEdit->text().toInt(&ok);
    if (!ok || freeC > totalC) { QMessageBox::critical(this, "Ошибка", "Некорректное число свободных купе"); return; }
    int freeP = m_freePlatzkartEdit->text().toInt(&ok);
    if (!ok || freeP > totalP) { QMessageBox::critical(this, "Ошибка", "Некорректное число свободных плацкарт"); return; }

    m_trains[row].setDate(m_dateEdit->date());
    m_trains[row].setNumber(num);
    m_trains[row].setDestination(m_destEdit->text());
    m_trains[row].setTime(m_timeEdit->time());
    m_trains[row].setTotalCoupe(totalC);
    m_trains[row].setTotalPlatzkart(totalP);
    m_trains[row].setFreeCoupe(freeC);
    m_trains[row].setFreePlatzkart(freeP);

    updateTable(m_trains);
    m_resultEdit->append("Изменён рейс (строка " + QString::number(row+1) + ")");
}

void MainWindow::onDelete()
{
    int row = m_table->currentRow();
    if (row < 0 || row >= m_trains.size()) {
        QMessageBox::warning(this, "Предупреждение", "Выберите запись для удаления");
        return;
    }
    m_trains.remove(row);
    updateTable(m_trains);
    m_resultEdit->append("Удалена строка " + QString::number(row+1));
}

void MainWindow::onSort()
{
    std::sort(m_trains.begin(), m_trains.end());
    updateTable(m_trains);
    m_resultEdit->append("Список отсортирован по дате, времени, номеру");
}

void MainWindow::onRefreshTable()
{
    updateTable(m_trains);
}

void MainWindow::onExecuteFunction()
{
    int idx = m_functionCombo->currentIndex();
    m_resultEdit->clear();

    switch (idx) {
    case 0: // Свободные места по каждому рейсу и типу
        for (const Train& t : m_trains) {
            m_resultEdit->append(QString("Рейс %1 (%2 %3): свободно купе %4, плацкарт %5")
                                     .arg(t.getNumber())
                                     .arg(t.getDate().toString("dd.MM.yyyy"))
                                     .arg(t.getTime().toString("hh:mm"))
                                     .arg(t.getFreeCoupe())
                                     .arg(t.getFreePlatzkart()));
        }
        break;
    case 1: // Расписание по дням недели
    {
        QMap<QString, QVector<Train>> schedule;
        for (const Train& t : m_trains) {
            QString day = QLocale::system().dayName(t.getDate().dayOfWeek());
            schedule[day].append(t);
        }
        for (auto it = schedule.begin(); it != schedule.end(); ++it) {
            m_resultEdit->append("=== " + it.key() + " ===");
            for (const Train& t : it.value()) {
                m_resultEdit->append(QString("  %1 %2 -> %3 в %4")
                                         .arg(t.getDate().toString("dd.MM.yyyy"))
                                         .arg(t.getNumber())
                                         .arg(t.getDestination())
                                         .arg(t.getTime().toString("hh:mm")));
            }
        }
        break;
    }
    case 2: // Список рейсов в пункт назначения
    {
        QMap<QString, QPair<int, int>> stats;
        for (const Train& t : m_trains) {
            stats[t.getDestination()].first += t.totalFree();
            stats[t.getDestination()].second += t.soldCoupe() + t.soldPlatzkart();
        }
        for (auto it = stats.begin(); it != stats.end(); ++it) {
            m_resultEdit->append(QString("%1: свободно %2 мест, продано %3 билетов")
                                     .arg(it.key())
                                     .arg(it.value().first)
                                     .arg(it.value().second));
        }
        break;
    }
    default:
        break;
    }
}
