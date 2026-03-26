#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "date.h"

QT_BEGIN_NAMESPACE
class QPushButton;
class QLabel;
class QTextEdit;
class QComboBox;
class QLineEdit;
class QTableWidget;
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onCurrentSystemDateClicked();
    void onNextDayClicked();
    void onPrevDayClicked();
    void onIsLeapClicked();
    void onWeekNumberClicked();
    void onDaysTillBirthdayClicked();
    void onDurationClicked();
    void onApplyOperationClicked();

    void onOpenFileClicked();
    void onAddDateClicked();
    void onEditDateClicked();
    void onSaveFileClicked();

private:
    Date getBirthdayFromUI() const;
    Date getSecondDateFromUI() const;
    void showResult(const QString &msg);
    void showError(const QString &err);
    void updateCurrentDateLabel();
    void loadDatesFromFile(const QString &filePath);
    void updateTable();
    void saveAllDatesToFile(const QString &filePath);

    // Виджеты существующего функционала
    QLabel *m_currentDateLabel;
    QPushButton *m_currentDateBtn;
    QPushButton *m_nextDayBtn;
    QPushButton *m_prevDayBtn;
    QPushButton *m_isLeapBtn;
    QPushButton *m_weekNumBtn;
    QLineEdit *m_birthDayEdit;
    QLineEdit *m_birthMonthEdit;
    QLineEdit *m_birthYearEdit;
    QPushButton *m_daysTillBirthdayBtn;
    QLineEdit *m_secondDayEdit;
    QLineEdit *m_secondMonthEdit;
    QLineEdit *m_secondYearEdit;
    QPushButton *m_durationBtn;
    QComboBox *m_operationCombo;
    QPushButton *m_applyOperationBtn;
    QTextEdit *m_resultEdit;

    // Новые виджеты
    QTableWidget *m_table;
    QPushButton *m_openFileBtn;
    QPushButton *m_addDateBtn;
    QPushButton *m_editDateBtn;
    QPushButton *m_saveFileBtn;
    QLineEdit *m_newDateEdit;
    QLineEdit *m_editDateEdit;

    QVector<Date> m_dates;
    QString m_currentFilePath;
    int m_operationCallCount;

    Date m_currentDate;   // <-- ДОБАВЛЕНА СТРОКА
};

#endif // MAINWINDOW_H
