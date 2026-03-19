#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "Canvas.h"
#include "Animator.h"

QT_BEGIN_NAMESPACE
class QComboBox;
class QLineEdit;
class QPushButton;
class QListWidget;
class QListWidgetItem;
class QLabel;
class QGroupBox;
class QDoubleSpinBox;
class QSpinBox;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void onCreateShape();
    void onShapeSelected(QListWidgetItem* current, QListWidgetItem* previous);
    void onUpdateShape();
    void onMoveCenter();
    void onAnimateTranslate();
    void onAnimateRotate();
    void onAnimateScale();
    void onAnimationStep();
    void onAnimationFinished();

private:
    void setupUi();
    void updateShapeList();
    void showShapeProperties(std::shared_ptr<Shape> shape);
    void clearPropertyEditors();
    void saveShapes();
    void loadShapes();

    Canvas* m_canvas;
    QListWidget* m_shapeList;
    QComboBox* m_shapeTypeCombo;
    QGroupBox* m_paramGroup;
    QMap<QString, QDoubleSpinBox*> m_paramMap;
    QLabel* m_areaLabel;
    QLabel* m_perimeterLabel;
    QLabel* m_centerLabel;
    QDoubleSpinBox* m_newCenterX;
    QDoubleSpinBox* m_newCenterY;
    QDoubleSpinBox* m_translateDx;
    QDoubleSpinBox* m_translateDy;
    QDoubleSpinBox* m_rotateAngle;
    QDoubleSpinBox* m_scaleFactor;
    QSpinBox* m_animDuration;

    std::shared_ptr<Shape> m_currentShape;
    std::unique_ptr<Animator> m_animator;
    QMap<QListWidgetItem*, std::shared_ptr<Shape>> m_itemShapeMap;
};

#endif // MAINWINDOW_H
