#include "mainwindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTreeWidget>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Просмотр папок и файлов");
    resize(800, 600);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *layout = new QVBoxLayout(central);

    m_selectBtn = new QPushButton("Выбрать папку");
    layout->addWidget(m_selectBtn);

    m_resultLabel = new QLabel("Результат: ");
    m_resultLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_resultLabel);

    m_tree = new QTreeWidget;
    m_tree->setHeaderLabel("Содержимое папки");
    layout->addWidget(m_tree);

    connect(m_selectBtn, &QPushButton::clicked, this, &MainWindow::selectFolder);
}

void MainWindow::buildTree(const QString &path, QTreeWidgetItem *parentItem, int &folders, int &files)
{
    QDir dir(path);
    if (!dir.exists()) return;

    QFileInfoList list = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
    for (const QFileInfo &info : list) {
        if (info.isDir()) {
            folders++;
            QTreeWidgetItem *item = new QTreeWidgetItem(parentItem);
            item->setText(0, info.fileName());
            item->setIcon(0, style()->standardIcon(QStyle::SP_DirIcon));
            buildTree(info.absoluteFilePath(), item, folders, files);
        } else if (info.isFile()) {
            files++;
            QTreeWidgetItem *item = new QTreeWidgetItem(parentItem);
            item->setText(0, info.fileName());
            item->setIcon(0, style()->standardIcon(QStyle::SP_FileIcon));
        }
    }
}

void MainWindow::selectFolder()
{
    QString folder = QFileDialog::getExistingDirectory(this, "Выберите главную папку");
    if (folder.isEmpty()) return;

    m_tree->clear();
    int folders = 0, files = 0;
    QTreeWidgetItem *rootItem = new QTreeWidgetItem(m_tree);
    rootItem->setText(0, QDir(folder).dirName());
    rootItem->setIcon(0, style()->standardIcon(QStyle::SP_DirIcon));
    buildTree(folder, rootItem, folders, files);
    rootItem->setExpanded(true);

    m_resultLabel->setText(QString("Всего подпапок: %1, Всего файлов: %2").arg(folders).arg(files));
}
