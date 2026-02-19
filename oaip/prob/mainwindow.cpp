#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QPushButton>
#include <QSettings>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_truck(new DumpTruck(50, 150, this))
{
    ui->setupUi(this);
    readSettings();

    //connect(ui->leftButton, &QPushButton::clicked, this, &MainWindow::on_leftButton_clicked);
    //connect(ui->rightButton, &QPushButton::clicked, this, &MainWindow::on_rightButton_clicked);
    //connect(ui->raiseButton, &QPushButton::clicked, this, &MainWindow::on_raiseButton_clicked);

    connect(m_truck, &DumpTruck::positionChanged, this, &MainWindow::onTruckMoved);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_leftButton_clicked()
{
    m_truck->move(-20, 0);
}

void MainWindow::on_rightButton_clicked()
{
    m_truck->move(20, 0);
}

void MainWindow::on_raiseButton_clicked()
{
    m_truck->raiseBody();
}

void MainWindow::on_lowerButton_clicked()
{
    m_truck->lowerBody();
}

void MainWindow::onTruckMoved()
{
    update();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    // Кабина (синий прямоугольник)
    painter.setBrush(Qt::blue);
    painter.drawRect(m_truck->getX(), m_truck->getY(),
                     m_truck->getWidth(), m_truck->getHeight());

    // Кузов (красный прямоугольник с поворотом)
    painter.setBrush(Qt::red);
    painter.save();
    // Точка крепления кузова – справа от кабины
    painter.translate(m_truck->getX() + m_truck->getWidth(), m_truck->getY());
    painter.rotate(-m_truck->getBodyAngle()); // поворот вверх
    painter.drawRect(0, -40, 80, 40); // размер кузова
    painter.restore();

    // Колёса (чёрные круги)
    painter.setBrush(Qt::black);
    painter.drawEllipse(m_truck->getX() + 10, m_truck->getY() + m_truck->getHeight(), 30, 30);
    painter.drawEllipse(m_truck->getX() + 60, m_truck->getY() + m_truck->getHeight(), 30, 30);
}

void MainWindow::writeSettings()
{
    QSettings settings("MyUniversity", "DumpTruckSim");
    settings.setValue("truck/x", m_truck->getX());
    settings.setValue("truck/y", m_truck->getY());
    settings.setValue("truck/angle", m_truck->getBodyAngle());
}

void MainWindow::readSettings()
{
    QSettings settings("MyUniversity", "DumpTruckSim");
    int x = settings.value("truck/x", 50).toInt();
    int y = settings.value("truck/y", 150).toInt();
    int angle = settings.value("truck/angle", 0).toInt();

    // Перемещаем самосвал в сохранённую позицию
    m_truck->move(x - m_truck->getX(), y - m_truck->getY());
    m_truck->setBodyAngle(angle);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}
