#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>

class QPushButton;
class QLabel;
class QTreeWidget;
class QTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
private slots:
    void selectFolder();
private:
    void buildTree(const QString &path, QTreeWidgetItem *parentItem, int &folders, int &files);
    QPushButton *m_selectBtn;
    QLabel *m_resultLabel;
    QTreeWidget *m_tree;
};

#endif // MAINWINDOW_H
