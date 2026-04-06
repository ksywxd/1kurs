#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "train.h"

QT_BEGIN_NAMESPACE
class QTableWidget;
class QLineEdit;
class QPushButton;
class QComboBox;
class QDateEdit;
class QTimeEdit;
class QTextEdit;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onOpenFile();
    void onSaveFile();
    void onAdd();
    void onDelete();
    void onEdit();
    void onSort();
    void onRefreshTable();
    void onExecuteFunction();          // <-- добавлен слот

private:
    void loadFromFile(const QString& fileName);
    void saveToFile(const QString& fileName);
    void updateTable(const QVector<Train>& data);

    QVector<Train> m_trains;

    // Виджеты
    QTableWidget *m_table;
    QLineEdit *m_numberEdit;
    QLineEdit *m_destEdit;
    QDateEdit *m_dateEdit;
    QTimeEdit *m_timeEdit;
    QLineEdit *m_totalCoupeEdit;
    QLineEdit *m_totalPlatzkartEdit;
    QLineEdit *m_freeCoupeEdit;
    QLineEdit *m_freePlatzkartEdit;
    QPushButton *m_addBtn;
    QPushButton *m_deleteBtn;
    QPushButton *m_editBtn;
    QPushButton *m_sortBtn;
    QPushButton *m_refreshBtn;
    QComboBox *m_functionCombo;
    QPushButton *m_executeFuncBtn;
    QTextEdit *m_resultEdit;

    QString m_currentFile;
};

#endif // MAINWINDOW_H
