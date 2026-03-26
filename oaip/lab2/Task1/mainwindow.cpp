#include "mainwindow.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QDate>
#include <QString>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QHeaderView>
#include <QRegularExpression>
#include <string>
#include <stdexcept>
#include <vector>

typedef Date (*DateOperation)(const Date&);

Date nextDayOp(const Date& d) {
    return d.nextDay();
}

Date prevDayOp(const Date& d) {
    return d.previousDay();
}

Date applyOperation(const Date& date, DateOperation op, void* param) {
    if (param) {
        int* counter = static_cast<int*>(param);
        (*counter)++;
    }
    return op(date);
}

static Date parseDate(const QString &str) {
    QStringList parts = str.split('.');
    if (parts.size() != 3) {
        throw std::invalid_argument("Неверный формат даты (должно быть dd.mm.yyyy)");
    }
    bool ok1, ok2, ok3;
    int d = parts[0].toInt(&ok1);
    int m = parts[1].toInt(&ok2);
    int y = parts[2].toInt(&ok3);
    if (!ok1 || !ok2 || !ok3) {
        throw std::invalid_argument("Дата содержит нечисловые символы");
    }
    return Date(d, m, y);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    m_operationCallCount(0),
    m_currentDate(QDate::currentDate().day(),
                  QDate::currentDate().month(),
                  QDate::currentDate().year())
{
    setWindowTitle("Date Operations");

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    // ======================== Старый функционал ========================
    // Текущая дата
    QGroupBox *currentBox = new QGroupBox("Текущая дата");
    QHBoxLayout *currentLayout = new QHBoxLayout(currentBox);
    m_currentDateBtn = new QPushButton("Обновить из системы");
    m_currentDateLabel = new QLabel();
    currentLayout->addWidget(m_currentDateLabel);
    currentLayout->addWidget(m_currentDateBtn);
    mainLayout->addWidget(currentBox);
    updateCurrentDateLabel();

    // Базовые операции
    QGroupBox *basicBox = new QGroupBox("Операции с датой");
    QHBoxLayout *basicLayout = new QHBoxLayout(basicBox);
    m_nextDayBtn = new QPushButton("Следующий день");
    m_prevDayBtn = new QPushButton("Предыдущий день");
    m_isLeapBtn = new QPushButton("Високосный?");
    m_weekNumBtn = new QPushButton("Номер недели");
    basicLayout->addWidget(m_nextDayBtn);
    basicLayout->addWidget(m_prevDayBtn);
    basicLayout->addWidget(m_isLeapBtn);
    basicLayout->addWidget(m_weekNumBtn);
    mainLayout->addWidget(basicBox);

    // Дней до дня рождения
    QGroupBox *birthdayBox = new QGroupBox("Дней до дня рождения");
    QHBoxLayout *birthdayLayout = new QHBoxLayout(birthdayBox);
    m_birthDayEdit = new QLineEdit;
    m_birthMonthEdit = new QLineEdit;
    m_birthYearEdit = new QLineEdit;
    m_daysTillBirthdayBtn = new QPushButton("Рассчитать");
    birthdayLayout->addWidget(new QLabel("День:"));
    birthdayLayout->addWidget(m_birthDayEdit);
    birthdayLayout->addWidget(new QLabel("Месяц:"));
    birthdayLayout->addWidget(m_birthMonthEdit);
    birthdayLayout->addWidget(new QLabel("Год:"));
    birthdayLayout->addWidget(m_birthYearEdit);
    birthdayLayout->addWidget(m_daysTillBirthdayBtn);
    mainLayout->addWidget(birthdayBox);

    // Разница между датами
    QGroupBox *durationBox = new QGroupBox("Разница между датами (текущая - другая)");
    QHBoxLayout *durationLayout = new QHBoxLayout(durationBox);
    m_secondDayEdit = new QLineEdit;
    m_secondMonthEdit = new QLineEdit;
    m_secondYearEdit = new QLineEdit;
    m_durationBtn = new QPushButton("Вычислить разницу");
    durationLayout->addWidget(new QLabel("День:"));
    durationLayout->addWidget(m_secondDayEdit);
    durationLayout->addWidget(new QLabel("Месяц:"));
    durationLayout->addWidget(m_secondMonthEdit);
    durationLayout->addWidget(new QLabel("Год:"));
    durationLayout->addWidget(m_secondYearEdit);
    durationLayout->addWidget(m_durationBtn);
    mainLayout->addWidget(durationBox);

    // Демонстрация параметров процедурного типа и void*
    QGroupBox *procBox = new QGroupBox("Демонстрация параметров процедурного типа и void*");
    QHBoxLayout *procLayout = new QHBoxLayout(procBox);
    m_operationCombo = new QComboBox;
    m_operationCombo->addItem("Следующий день", QVariant::fromValue<void*>(reinterpret_cast<void*>(nextDayOp)));
    m_operationCombo->addItem("Предыдущий день", QVariant::fromValue<void*>(reinterpret_cast<void*>(prevDayOp)));
    m_applyOperationBtn = new QPushButton("Применить операцию");
    procLayout->addWidget(m_operationCombo);
    procLayout->addWidget(m_applyOperationBtn);
    mainLayout->addWidget(procBox);

    // ======================== Новый функционал ========================
    QGroupBox *fileBox = new QGroupBox("Работа с файлом дат");
    QVBoxLayout *fileLayout = new QVBoxLayout(fileBox);

    // Кнопки
    QHBoxLayout *fileButtonsLayout = new QHBoxLayout;
    m_openFileBtn = new QPushButton("Открыть файл");
    m_addDateBtn = new QPushButton("Добавить дату");
    m_editDateBtn = new QPushButton("Изменить дату");
    m_saveFileBtn = new QPushButton("Сохранить файл");
    fileButtonsLayout->addWidget(m_openFileBtn);
    fileButtonsLayout->addWidget(m_addDateBtn);
    fileButtonsLayout->addWidget(m_editDateBtn);
    fileButtonsLayout->addWidget(m_saveFileBtn);
    fileLayout->addLayout(fileButtonsLayout);

    // Поля ввода
    QHBoxLayout *editLayout = new QHBoxLayout;
    editLayout->addWidget(new QLabel("Новая дата (dd.mm.yyyy):"));
    m_newDateEdit = new QLineEdit;
    editLayout->addWidget(m_newDateEdit);
    editLayout->addWidget(new QLabel("   Изменить на:"));
    m_editDateEdit = new QLineEdit;
    editLayout->addWidget(m_editDateEdit);
    fileLayout->addLayout(editLayout);

    // Таблица
    m_table = new QTableWidget;
    m_table->setColumnCount(3);
    m_table->setHorizontalHeaderLabels(QStringList() << "Исходная дата" << "Следующий день" << "Разница со следующей");
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    fileLayout->addWidget(m_table);

    mainLayout->addWidget(fileBox);

    // Результат
    m_resultEdit = new QTextEdit;
    m_resultEdit->setReadOnly(true);
    mainLayout->addWidget(m_resultEdit);

    // Подключение сигналов
    connect(m_currentDateBtn, &QPushButton::clicked, this, &MainWindow::onCurrentSystemDateClicked);
    connect(m_nextDayBtn, &QPushButton::clicked, this, &MainWindow::onNextDayClicked);
    connect(m_prevDayBtn, &QPushButton::clicked, this, &MainWindow::onPrevDayClicked);
    connect(m_isLeapBtn, &QPushButton::clicked, this, &MainWindow::onIsLeapClicked);
    connect(m_weekNumBtn, &QPushButton::clicked, this, &MainWindow::onWeekNumberClicked);
    connect(m_daysTillBirthdayBtn, &QPushButton::clicked, this, &MainWindow::onDaysTillBirthdayClicked);
    connect(m_durationBtn, &QPushButton::clicked, this, &MainWindow::onDurationClicked);
    connect(m_applyOperationBtn, &QPushButton::clicked, this, &MainWindow::onApplyOperationClicked);
    connect(m_openFileBtn, &QPushButton::clicked, this, &MainWindow::onOpenFileClicked);
    connect(m_addDateBtn, &QPushButton::clicked, this, &MainWindow::onAddDateClicked);
    connect(m_editDateBtn, &QPushButton::clicked, this, &MainWindow::onEditDateClicked);
    connect(m_saveFileBtn, &QPushButton::clicked, this, &MainWindow::onSaveFileClicked);
}

MainWindow::~MainWindow() {}

void MainWindow::updateCurrentDateLabel() {
    m_currentDateLabel->setText("Текущая дата: " + QString::fromStdString(m_currentDate.toString()));
}

Date MainWindow::getBirthdayFromUI() const {
    bool ok;
    int d = m_birthDayEdit->text().toInt(&ok);
    if (!ok) throw std::invalid_argument("Неверный день рождения");
    int m = m_birthMonthEdit->text().toInt(&ok);
    if (!ok) throw std::invalid_argument("Неверный месяц рождения");
    int y = m_birthYearEdit->text().toInt(&ok);
    if (!ok) throw std::invalid_argument("Неверный год рождения");
    return Date(d, m, y);
}

Date MainWindow::getSecondDateFromUI() const {
    bool ok;
    int d = m_secondDayEdit->text().toInt(&ok);
    if (!ok) throw std::invalid_argument("Неверный день");
    int m = m_secondMonthEdit->text().toInt(&ok);
    if (!ok) throw std::invalid_argument("Неверный месяц");
    int y = m_secondYearEdit->text().toInt(&ok);
    if (!ok) throw std::invalid_argument("Неверный год");
    return Date(d, m, y);
}

void MainWindow::showResult(const QString &msg) {
    m_resultEdit->append(msg);
}

void MainWindow::showError(const QString &err) {
    QMessageBox::critical(this, "Ошибка", err);
    m_resultEdit->append("Ошибка: " + err);
}

void MainWindow::onCurrentSystemDateClicked() {
    try {
        QDate sysDate = QDate::currentDate();
        m_currentDate = Date(sysDate.day(), sysDate.month(), sysDate.year());
        updateCurrentDateLabel();
        showResult("Текущая дата обновлена из системы: " + QString::fromStdString(m_currentDate.toString()));
    } catch (const std::exception &e) {
        showError(e.what());
    }
}

void MainWindow::onNextDayClicked() {
    try {
        Date next = m_currentDate.nextDay();
        showResult("Следующий день: " + QString::fromStdString(next.toString()));
    } catch (const std::exception &e) {
        showError(e.what());
    }
}

void MainWindow::onPrevDayClicked() {
    try {
        Date prev = m_currentDate.previousDay();
        showResult("Предыдущий день: " + QString::fromStdString(prev.toString()));
    } catch (const std::exception &e) {
        showError(e.what());
    }
}

void MainWindow::onIsLeapClicked() {
    try {
        bool leap = m_currentDate.isLeap();
        showResult(QString("Год %1 %2 високосным").arg(m_currentDate.getYear())
                       .arg(leap ? "является" : "не является"));
    } catch (const std::exception &e) {
        showError(e.what());
    }
}

void MainWindow::onWeekNumberClicked() {
    try {
        short wn = m_currentDate.weekNumber();
        showResult(QString("Номер недели: %1").arg(wn));
    } catch (const std::exception &e) {
        showError(e.what());
    }
}

void MainWindow::onDaysTillBirthdayClicked() {
    try {
        Date birthday = getBirthdayFromUI();
        int days = m_currentDate.daysTillYourBirthday(birthday);
        showResult(QString("Дней до дня рождения: %1").arg(days));
    } catch (const std::exception &e) {
        showError(e.what());
    }
}

void MainWindow::onDurationClicked() {
    try {
        Date other = getSecondDateFromUI();
        int diff = m_currentDate.duration(other);
        showResult(QString("Разница (текущая - другая) = %1 дней").arg(diff));
    } catch (const std::exception &e) {
        showError(e.what());
    }
}

void MainWindow::onApplyOperationClicked() {
    try {
        void* opPtr = m_operationCombo->currentData().value<void*>();
        if (!opPtr) {
            showError("Не удалось получить операцию");
            return;
        }
        DateOperation op = reinterpret_cast<DateOperation>(opPtr);
        Date result = applyOperation(m_currentDate, op, &m_operationCallCount);
        showResult(QString("Применена операция '%1', результат: %2, количество вызовов: %3")
                       .arg(m_operationCombo->currentText())
                       .arg(QString::fromStdString(result.toString()))
                       .arg(m_operationCallCount));
    } catch (const std::exception &e) {
        showError(e.what());
    }
}

// ======================== Работа с файлами ========================

void MainWindow::loadDatesFromFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::runtime_error("Не удалось открыть файл");
    }
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    // Разделяем по пробелам и переводам строк
    QRegularExpression re("\\s+");
    QStringList tokens = content.split(re, Qt::SkipEmptyParts);
    m_dates.clear();
    for (const QString &token : tokens) {
        try {
            Date d = parseDate(token);
            m_dates.push_back(d);
        } catch (const std::exception &e) {
            showResult("Ошибка при разборе даты \"" + token + "\": " + e.what());
        }
    }
    showResult("Загружено " + QString::number(m_dates.size()) + " дат из файла " + filePath);
    m_currentFilePath = filePath;
}

void MainWindow::updateTable() {
    m_table->setRowCount(m_dates.size());
    for (int i = 0; i < m_dates.size(); ++i) {
        const Date &d = m_dates[i];
        QTableWidgetItem *dateItem = new QTableWidgetItem(QString::fromStdString(d.toString()));
        m_table->setItem(i, 0, dateItem);

        Date next = d.nextDay();
        QTableWidgetItem *nextItem = new QTableWidgetItem(QString::fromStdString(next.toString()));
        m_table->setItem(i, 1, nextItem);

        if (i < m_dates.size() - 1) {
            int diff = m_dates[i+1].duration(d); // (следующая - текущая)
            QTableWidgetItem *diffItem = new QTableWidgetItem(QString::number(diff));
            m_table->setItem(i, 2, diffItem);
        } else {
            QTableWidgetItem *diffItem = new QTableWidgetItem("—");
            m_table->setItem(i, 2, diffItem);
        }
    }
}

void MainWindow::saveAllDatesToFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        throw std::runtime_error("Не удалось открыть файл для записи");
    }
    QTextStream out(&file);
    for (const Date &d : m_dates) {
        out << QString::fromStdString(d.toString()) << "\n";
    }
    file.close();
    showResult("Файл сохранён: " + filePath);
}

void MainWindow::onOpenFileClicked() {
    QString filePath = QFileDialog::getOpenFileName(this, "Выберите текстовый файл с датами", "",
                                                    "Текстовые файлы (*.txt)");
    if (filePath.isEmpty()) return;
    try {
        loadDatesFromFile(filePath);
        updateTable();
        showResult("Файл открыт: " + filePath);
    } catch (const std::exception &e) {
        showError(e.what());
    }
}

void MainWindow::onAddDateClicked() {
    QString newDateStr = m_newDateEdit->text().trimmed();
    if (newDateStr.isEmpty()) {
        showError("Введите дату для добавления");
        return;
    }
    try {
        Date newDate = parseDate(newDateStr);
        m_dates.push_back(newDate);
        if (!m_currentFilePath.isEmpty()) {
            QFile file(m_currentFilePath);
            if (file.open(QIODevice::Append | QIODevice::Text)) {
                QTextStream out(&file);
                out << QString::fromStdString(newDate.toString()) << "\n";
                file.close();
                showResult("Дата добавлена в конец файла: " + newDateStr);
            } else {
                showError("Не удалось открыть файл для добавления");
            }
        } else {
            showError("Сначала откройте файл");
        }
        updateTable();
        m_newDateEdit->clear();
    } catch (const std::exception &e) {
        showError(e.what());
    }
}

void MainWindow::onEditDateClicked() {
    int row = m_table->currentRow();
    if (row < 0 || row >= m_dates.size()) {
        showError("Выберите дату в таблице для изменения");
        return;
    }
    QString newDateStr = m_editDateEdit->text().trimmed();
    if (newDateStr.isEmpty()) {
        showError("Введите новую дату");
        return;
    }
    try {
        Date newDate = parseDate(newDateStr);
        m_dates[row] = newDate;
        if (!m_currentFilePath.isEmpty()) {
            saveAllDatesToFile(m_currentFilePath);
        } else {
            showError("Файл не открыт");
        }
        updateTable();
        m_editDateEdit->clear();
        showResult("Дата изменена на " + newDateStr);
    } catch (const std::exception &e) {
        showError(e.what());
    }
}

void MainWindow::onSaveFileClicked() {
    if (m_currentFilePath.isEmpty()) {
        QString newPath = QFileDialog::getSaveFileName(this, "Сохранить файл", "", "Текстовые файлы (*.txt)");
        if (newPath.isEmpty()) return;
        m_currentFilePath = newPath;
    }
    try {
        saveAllDatesToFile(m_currentFilePath);
    } catch (const std::exception &e) {
        showError(e.what());
    }
}
