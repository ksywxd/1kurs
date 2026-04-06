#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QSpinBox>
#include <QMessageBox>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Тестирование Vector (аналог std::vector)");
    resize(600, 500);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    m_infoLabel = new QLabel;
    mainLayout->addWidget(m_infoLabel);

    m_display = new QTextEdit;
    m_display->setReadOnly(true);
    m_display->setFontFamily("Courier");
    mainLayout->addWidget(m_display);

    QHBoxLayout *inputLayout = new QHBoxLayout;
    inputLayout->addWidget(new QLabel("Значение:"));
    m_valueEdit = new QLineEdit;
    inputLayout->addWidget(m_valueEdit);
    inputLayout->addWidget(new QLabel("Индекс:"));
    m_indexSpin = new QSpinBox;
    m_indexSpin->setRange(0, 1000);
    inputLayout->addWidget(m_indexSpin);
    inputLayout->addStretch();
    mainLayout->addLayout(inputLayout);

    QGridLayout *btnLayout = new QGridLayout;
    QPushButton *btnPushBack = new QPushButton("push_back");
    QPushButton *btnPopBack = new QPushButton("pop_back");
    QPushButton *btnInsert = new QPushButton("insert (значение по индексу)");
    QPushButton *btnErase = new QPushButton("erase (по индексу)");
    QPushButton *btnClear = new QPushButton("clear");
    QPushButton *btnResize = new QPushButton("resize (новый размер)");
    QPushButton *btnReserve = new QPushButton("reserve (ёмкость)");
    QPushButton *btnAssign = new QPushButton("assign (повторить значение n раз)");
    QPushButton *btnAt = new QPushButton("at (получить элемент)");
    QPushButton *btnFrontBack = new QPushButton("front / back");
    QPushButton *btnSwap = new QPushButton("swap (с вектором-соседом)");

    btnLayout->addWidget(btnPushBack, 0, 0);
    btnLayout->addWidget(btnPopBack, 0, 1);
    btnLayout->addWidget(btnInsert, 0, 2);
    btnLayout->addWidget(btnErase, 1, 0);
    btnLayout->addWidget(btnClear, 1, 1);
    btnLayout->addWidget(btnResize, 1, 2);
    btnLayout->addWidget(btnReserve, 2, 0);
    btnLayout->addWidget(btnAssign, 2, 1);
    btnLayout->addWidget(btnAt, 2, 2);
    btnLayout->addWidget(btnFrontBack, 3, 0);
    btnLayout->addWidget(btnSwap, 3, 1);
    mainLayout->addLayout(btnLayout);

    connect(btnPushBack, &QPushButton::clicked, this, &MainWindow::onPushBack);
    connect(btnPopBack, &QPushButton::clicked, this, &MainWindow::onPopBack);
    connect(btnInsert, &QPushButton::clicked, this, &MainWindow::onInsert);
    connect(btnErase, &QPushButton::clicked, this, &MainWindow::onErase);
    connect(btnClear, &QPushButton::clicked, this, &MainWindow::onClear);
    connect(btnResize, &QPushButton::clicked, this, &MainWindow::onResize);
    connect(btnReserve, &QPushButton::clicked, this, &MainWindow::onReserve);
    connect(btnAssign, &QPushButton::clicked, this, &MainWindow::onAssign);
    connect(btnAt, &QPushButton::clicked, this, &MainWindow::onAt);
    connect(btnFrontBack, &QPushButton::clicked, this, &MainWindow::onFrontBack);
    connect(btnSwap, &QPushButton::clicked, this, &MainWindow::onSwap);

    updateDisplay();
}

void MainWindow::updateDisplay()
{
    QString text;
    for (size_t i = 0; i < m_vec.size(); ++i)
        text += QString::number(m_vec[i]) + " ";
    if (text.isEmpty()) text = "(пусто)";
    m_display->setText(text);
    m_infoLabel->setText(QString("size = %1, capacity = %2, max_size = %3")
                             .arg(m_vec.size())
                             .arg(m_vec.capacity())
                             .arg(m_vec.max_size()));
}

void MainWindow::onPushBack()
{
    bool ok;
    int val = m_valueEdit->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Ошибка", "Введите целое число.");
        return;
    }
    m_vec.push_back(val);
    updateDisplay();
}

void MainWindow::onPopBack()
{
    if (m_vec.empty()) {
        QMessageBox::warning(this, "Ошибка", "Вектор пуст.");
        return;
    }
    m_vec.pop_back();
    updateDisplay();
}

void MainWindow::onInsert()
{
    bool ok;
    int val = m_valueEdit->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Ошибка", "Введите целое число.");
        return;
    }
    int idx = m_indexSpin->value();
    if (idx < 0 || idx > static_cast<int>(m_vec.size())) {
        QMessageBox::warning(this, "Ошибка", "Индекс вне диапазона.");
        return;
    }
    m_vec.insert(m_vec.cbegin() + idx, val);
    updateDisplay();
}

void MainWindow::onErase()
{
    int idx = m_indexSpin->value();
    if (idx < 0 || idx >= static_cast<int>(m_vec.size())) {
        QMessageBox::warning(this, "Ошибка", "Индекс вне диапазона.");
        return;
    }
    m_vec.erase(m_vec.cbegin() + idx);
    updateDisplay();
}

void MainWindow::onClear()
{
    m_vec.clear();
    updateDisplay();
}

void MainWindow::onResize()
{
    bool ok;
    int newSize = m_valueEdit->text().toInt(&ok);
    if (!ok || newSize < 0) {
        QMessageBox::warning(this, "Ошибка", "Введите неотрицательное целое число.");
        return;
    }
    m_vec.resize(static_cast<size_t>(newSize));
    updateDisplay();
}

void MainWindow::onReserve()
{
    bool ok;
    int newCap = m_valueEdit->text().toInt(&ok);
    if (!ok || newCap < 0) {
        QMessageBox::warning(this, "Ошибка", "Введите неотрицательное целое число.");
        return;
    }
    m_vec.reserve(static_cast<size_t>(newCap));
    updateDisplay();
}

void MainWindow::onAssign()
{
    bool ok;
    int count = m_valueEdit->text().toInt(&ok);
    if (!ok || count < 0) {
        QMessageBox::warning(this, "Ошибка", "Введите количество повторений (неотрицательное).");
        return;
    }
    int val = QInputDialog::getInt(this, "assign", "Значение для повторения:", 0);
    m_vec.assign(static_cast<size_t>(count), val);
    updateDisplay();
}

void MainWindow::onAt()
{
    int idx = m_indexSpin->value();
    try {
        int val = m_vec.at(static_cast<size_t>(idx));
        QMessageBox::information(this, "at", QString("vec[%1] = %2").arg(idx).arg(val));
    } catch (const std::out_of_range&) {
        QMessageBox::warning(this, "Ошибка", "Индекс вне диапазона.");
    }
}

void MainWindow::onFrontBack()
{
    if (m_vec.empty()) {
        QMessageBox::warning(this, "Ошибка", "Вектор пуст.");
        return;
    }
    QMessageBox::information(this, "front/back",
                             QString("front = %1, back = %2").arg(m_vec.front()).arg(m_vec.back()));
}

void MainWindow::onSwap()
{
    Vector<int> temp;
    temp.push_back(999);
    temp.push_back(888);
    m_vec.swap(temp);
    updateDisplay();
}
