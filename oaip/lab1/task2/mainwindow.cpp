#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QListWidget>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QMessageBox>
#include <QFormLayout>
#include <QCloseEvent>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include <cmath>
#include <stdexcept>

#include "Circle.h"
#include "Ellipse.h"
#include "Rectangle.h"
#include "Rhombus.h"
#include "Square.h"
#include "Triangle.h"
#include "EquilateralTriangle.h"
#include "Star.h"
#include "Hexagon.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    setupUi();
    loadShapes();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUi()
{
    auto* central = new QWidget;
    setCentralWidget(central);
    auto* mainLayout = new QHBoxLayout(central);

    // Left panel
    auto* leftPanel = new QWidget;
    auto* leftLayout = new QVBoxLayout(leftPanel);

    m_shapeTypeCombo = new QComboBox;
    m_shapeTypeCombo->addItems({
        "Circle", "Ellipse", "Rectangle", "Rhombus", "Square",
        "Triangle", "Equilateral Triangle", "Star (5 rays)", "Star (6 rays)", "Star (8 rays)",
        "Hexagon"
    });
    leftLayout->addWidget(new QLabel("Shape type:"));
    leftLayout->addWidget(m_shapeTypeCombo);

    m_paramGroup = new QGroupBox("Parameters");
    new QFormLayout(m_paramGroup); // set layout
    leftLayout->addWidget(m_paramGroup);

    auto* createBtn = new QPushButton("Create Shape");
    leftLayout->addWidget(createBtn);
    connect(createBtn, &QPushButton::clicked, this, &MainWindow::onCreateShape);

    m_shapeList = new QListWidget;
    leftLayout->addWidget(new QLabel("Shapes:"));
    leftLayout->addWidget(m_shapeList);
    connect(m_shapeList, &QListWidget::currentItemChanged, this, &MainWindow::onShapeSelected);

    auto* deleteBtn = new QPushButton("Delete Selected");
    leftLayout->addWidget(deleteBtn);
    connect(deleteBtn, &QPushButton::clicked, [this]() {
        if (m_currentShape) {
            m_canvas->removeShape(m_currentShape);
            m_currentShape.reset();
            updateShapeList();
            clearPropertyEditors();
        }
    });

    mainLayout->addWidget(leftPanel, 1);

    // Right panel
    auto* rightPanel = new QWidget;
    auto* rightLayout = new QVBoxLayout(rightPanel);

    m_canvas = new Canvas;
    m_canvas->setMinimumSize(400, 400);
    rightLayout->addWidget(m_canvas, 1);

    auto* propsGroup = new QGroupBox("Properties");
    auto* propsLayout = new QFormLayout(propsGroup);
    m_areaLabel = new QLabel("-");
    m_perimeterLabel = new QLabel("-");
    m_centerLabel = new QLabel("-");
    propsLayout->addRow("Area:", m_areaLabel);
    propsLayout->addRow("Perimeter:", m_perimeterLabel);
    propsLayout->addRow("Center of mass:", m_centerLabel);
    rightLayout->addWidget(propsGroup);

    auto* moveGroup = new QGroupBox("Move Center");
    auto* moveLayout = new QFormLayout(moveGroup);
    m_newCenterX = new QDoubleSpinBox;
    m_newCenterY = new QDoubleSpinBox;
    m_newCenterX->setRange(-1000, 1000);
    m_newCenterY->setRange(-1000, 1000);
    moveLayout->addRow("New X:", m_newCenterX);
    moveLayout->addRow("New Y:", m_newCenterY);
    auto* moveBtn = new QPushButton("Set Center");
    connect(moveBtn, &QPushButton::clicked, this, &MainWindow::onMoveCenter);
    moveLayout->addRow(moveBtn);
    rightLayout->addWidget(moveGroup);

    auto* animGroup = new QGroupBox("Animation");
    auto* animLayout = new QFormLayout(animGroup);
    m_translateDx = new QDoubleSpinBox;
    m_translateDy = new QDoubleSpinBox;
    m_translateDx->setRange(-500, 500);
    m_translateDy->setRange(-500, 500);
    animLayout->addRow("Translate dx:", m_translateDx);
    animLayout->addRow("Translate dy:", m_translateDy);
    auto* translateBtn = new QPushButton("Animate Translate");
    connect(translateBtn, &QPushButton::clicked, this, &MainWindow::onAnimateTranslate);
    animLayout->addRow(translateBtn);

    m_rotateAngle = new QDoubleSpinBox;
    m_rotateAngle->setRange(-360, 360);
    animLayout->addRow("Rotate angle:", m_rotateAngle);
    auto* rotateBtn = new QPushButton("Animate Rotate");
    connect(rotateBtn, &QPushButton::clicked, this, &MainWindow::onAnimateRotate);
    animLayout->addRow(rotateBtn);

    m_scaleFactor = new QDoubleSpinBox;
    m_scaleFactor->setRange(0.1, 10.0);
    m_scaleFactor->setValue(1.5);
    animLayout->addRow("Scale factor:", m_scaleFactor);
    auto* scaleBtn = new QPushButton("Animate Scale");
    connect(scaleBtn, &QPushButton::clicked, this, &MainWindow::onAnimateScale);
    animLayout->addRow(scaleBtn);

    m_animDuration = new QSpinBox;
    m_animDuration->setRange(100, 10000);
    m_animDuration->setValue(2000);
    m_animDuration->setSuffix(" ms");
    animLayout->addRow("Duration:", m_animDuration);

    rightLayout->addWidget(animGroup);
    mainLayout->addWidget(rightPanel, 2);
}

void MainWindow::onCreateShape()
{
    try {
        std::shared_ptr<Shape> shape;
        QString type = m_shapeTypeCombo->currentText();

        if (type == "Circle") {
            shape = std::make_shared<Circle>(QPointF(200,200), 50);
        }
        else if (type == "Ellipse") {
            shape = std::make_shared<Ellipse>(QPointF(200,200), 80, 50, 0);
        }
        else if (type == "Rectangle") {
            shape = std::make_shared<Rectangle>(QPointF(200,200), 100, 60, 0);
        }
        else if (type == "Rhombus") {
            shape = std::make_shared<Rhombus>(QPointF(200,200), 100, 60, 0);
        }
        else if (type == "Square") {
            shape = std::make_shared<Square>(QPointF(200,200), 80, 0);
        }
        else if (type == "Triangle") {
            shape = std::make_shared<Triangle>(QPointF(150,150), QPointF(250,150), QPointF(200,250));
        }
        else if (type == "Equilateral Triangle") {
            shape = std::make_shared<EquilateralTriangle>(QPointF(200,200), 100, 0);
        }
        else if (type == "Star (5 rays)") {
            shape = std::make_shared<Star>(QPointF(200,200), 5, 80, 40, 0);
        }
        else if (type == "Star (6 rays)") {
            shape = std::make_shared<Star>(QPointF(200,200), 6, 80, 40, 0);
        }
        else if (type == "Star (8 rays)") {
            shape = std::make_shared<Star>(QPointF(200,200), 8, 80, 40, 0);
        }
        else if (type == "Hexagon") {
            shape = std::make_shared<Hexagon>(QPointF(200,200), 70, 0);
        }
        else {
            QMessageBox::warning(this, "Error", "Unknown shape type");
            return;
        }

        m_canvas->addShape(shape);
        updateShapeList();

        for (auto it = m_itemShapeMap.begin(); it != m_itemShapeMap.end(); ++it) {
            if (it.value() == shape) {
                m_shapeList->setCurrentItem(it.key());
                break;
            }
        }
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to create shape: %1").arg(e.what()));
    }
}

void MainWindow::updateShapeList()
{
    m_shapeList->clear();
    m_itemShapeMap.clear();

    for (const auto& shape : m_canvas->shapes()) {
        auto* item = new QListWidgetItem(shape->name());
        m_shapeList->addItem(item);
        m_itemShapeMap[item] = shape;
    }
}

void MainWindow::onShapeSelected(QListWidgetItem* current, QListWidgetItem* /*previous*/)
{
    if (!current) {
        m_currentShape.reset();
        clearPropertyEditors();
        return;
    }

    m_currentShape = m_itemShapeMap.value(current);
    if (m_currentShape) {
        showShapeProperties(m_currentShape);
    }
}

void MainWindow::showShapeProperties(std::shared_ptr<Shape> shape)
{
    m_areaLabel->setText(QString::number(shape->area()));
    m_perimeterLabel->setText(QString::number(shape->perimeter()));
    QPointF com = shape->centerOfMass();
    m_centerLabel->setText(QString("(%1, %2)").arg(com.x()).arg(com.y()));

    m_newCenterX->setValue(com.x());
    m_newCenterY->setValue(com.y());

    clearPropertyEditors();
    auto params = shape->parameters();
    auto* layout = qobject_cast<QFormLayout*>(m_paramGroup->layout());
    for (auto it = params.begin(); it != params.end(); ++it) {
        auto* spin = new QDoubleSpinBox;
        spin->setRange(-10000, 10000);
        spin->setDecimals(2);
        spin->setValue(it.value());
        layout->addRow(it.key() + ":", spin);
        m_paramMap[it.key()] = spin;
        connect(spin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::onUpdateShape);
    }
}

void MainWindow::clearPropertyEditors()
{
    auto* layout = qobject_cast<QFormLayout*>(m_paramGroup->layout());
    while (layout->rowCount() > 0)
        layout->removeRow(0);
    m_paramMap.clear();
}

void MainWindow::onUpdateShape()
{
    if (!m_currentShape) return;
    try {
        for (auto it = m_paramMap.begin(); it != m_paramMap.end(); ++it) {
            m_currentShape->setParameter(it.key(), it.value()->value());
        }
        m_canvas->update();
        m_areaLabel->setText(QString::number(m_currentShape->area()));
        m_perimeterLabel->setText(QString::number(m_currentShape->perimeter()));
        QPointF com = m_currentShape->centerOfMass();
        m_centerLabel->setText(QString("(%1, %2)").arg(com.x()).arg(com.y()));
    }
    catch (const std::exception& e) {
        QMessageBox::warning(this, "Error", QString("Failed to update parameter: %1").arg(e.what()));
    }
}

void MainWindow::onMoveCenter()
{
    if (!m_currentShape) return;
    try {
        m_currentShape->setCenterOfMass(QPointF(m_newCenterX->value(), m_newCenterY->value()));
        m_canvas->update();
        showShapeProperties(m_currentShape);
    }
    catch (const std::exception& e) {
        QMessageBox::warning(this, "Error", QString("Failed to move center: %1").arg(e.what()));
    }
}

void MainWindow::onAnimateTranslate()
{
    if (!m_currentShape) return;
    if (m_animator) return;

    QPointF delta(m_translateDx->value(), m_translateDy->value());
    m_animator = std::make_unique<Animator>(m_currentShape, Animator::Translate,
                                            QVariant::fromValue(delta), m_animDuration->value());
    connect(m_animator.get(), &Animator::step, this, &MainWindow::onAnimationStep);
    connect(m_animator.get(), &Animator::finished, this, &MainWindow::onAnimationFinished);
    m_animator->start();
}

void MainWindow::onAnimateRotate()
{
    if (!m_currentShape) return;
    if (m_animator) return;

    m_animator = std::make_unique<Animator>(m_currentShape, Animator::Rotate,
                                            m_rotateAngle->value(), m_animDuration->value());
    connect(m_animator.get(), &Animator::step, this, &MainWindow::onAnimationStep);
    connect(m_animator.get(), &Animator::finished, this, &MainWindow::onAnimationFinished);
    m_animator->start();
}

void MainWindow::onAnimateScale()
{
    if (!m_currentShape) return;
    if (m_animator) return;

    m_animator = std::make_unique<Animator>(m_currentShape, Animator::Scale,
                                            m_scaleFactor->value(), m_animDuration->value());
    connect(m_animator.get(), &Animator::step, this, &MainWindow::onAnimationStep);
    connect(m_animator.get(), &Animator::finished, this, &MainWindow::onAnimationFinished);
    m_animator->start();
}

void MainWindow::onAnimationStep()
{
    m_canvas->update();
    if (m_currentShape) {
        m_areaLabel->setText(QString::number(m_currentShape->area()));
        m_perimeterLabel->setText(QString::number(m_currentShape->perimeter()));
        QPointF com = m_currentShape->centerOfMass();
        m_centerLabel->setText(QString("(%1, %2)").arg(com.x()).arg(com.y()));
    }
}

void MainWindow::onAnimationFinished()
{
    m_animator.reset();
}

void MainWindow::saveShapes()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(path);
    QString fileName = path + "/shapes.json";

    QJsonArray shapesArray;
    for (const auto& shape : m_canvas->shapes()) {
        shapesArray.append(shape->toJson());
    }

    QJsonDocument doc(shapesArray);
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Could not save shapes to" << fileName;
        return;
    }
    file.write(doc.toJson());
    file.close();
}

void MainWindow::loadShapes()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString fileName = path + "/shapes.json";

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) {
        qWarning() << "Invalid shapes file";
        return;
    }

    QJsonArray shapesArray = doc.array();
    for (const QJsonValue& val : shapesArray) {
        QJsonObject obj = val.toObject();
        QString type = obj["type"].toString();

        std::shared_ptr<Shape> shape;
        if (type == "Circle") {
            shape = std::make_shared<Circle>(QPointF(0,0), 1.0);
        } else if (type == "Ellipse") {
            shape = std::make_shared<Ellipse>(QPointF(0,0), 1.0, 1.0, 0);
        } else if (type == "Rectangle") {
            shape = std::make_shared<Rectangle>(QPointF(0,0), 1.0, 1.0, 0);
        } else if (type == "Rhombus") {
            shape = std::make_shared<Rhombus>(QPointF(0,0), 1.0, 1.0, 0);
        } else if (type == "Square") {
            shape = std::make_shared<Square>(QPointF(0,0), 1.0, 0);
        } else if (type == "Triangle") {
            shape = std::make_shared<Triangle>(QPointF(0,0), QPointF(1,0), QPointF(0,1));
        } else if (type == "EquilateralTriangle") {
            shape = std::make_shared<EquilateralTriangle>(QPointF(0,0), 1.0, 0);
        } else if (type == "Star") {
            shape = std::make_shared<Star>(QPointF(0,0), 5, 1.0, 0.5, 0);
        } else if (type == "Hexagon") {
            shape = std::make_shared<Hexagon>(QPointF(0,0), 1.0, 0);
        } else {
            qWarning() << "Unknown shape type:" << type;
            continue;
        }

        shape->fromJson(obj);
        m_canvas->addShape(shape);
    }
    updateShapeList();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    saveShapes();
    event->accept();
}
