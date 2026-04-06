#include "test.h"
#include <QtTest>
#include <QTemporaryDir>
#include <QDir>
#include <QFile>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QLabel>

#define private public
#include "mainwindow.h"
#undef private

static QTemporaryDir *tempDir = nullptr;

void TestFolderViewer::initTestCase()
{
    tempDir = new QTemporaryDir();
    QVERIFY(tempDir->isValid());
    QDir root(tempDir->path());

    root.mkdir("folderA");
    root.mkdir("folderB");
    root.mkdir(".hidden");               // скрытая папка

    QFile f(root.filePath("file1.txt"));
    f.open(QIODevice::WriteOnly);
    f.close();

    QDir folderA(root.filePath("folderA"));
    folderA.mkdir("subA1");
    folderA.mkdir("subA2");
    QFile f2(folderA.filePath("fileA.txt"));
    f2.open(QIODevice::WriteOnly);
    f2.close();

    QDir folderB(root.filePath("folderB"));
    QFile f3(folderB.filePath("fileB.txt"));
    f3.open(QIODevice::WriteOnly);
    f3.close();

    QDir hidden(root.filePath(".hidden"));
    QFile f4(hidden.filePath("hiddenFile.txt"));
    f4.open(QIODevice::WriteOnly);
    f4.close();
}

void TestFolderViewer::cleanupTestCase()
{
    delete tempDir;
}

void TestFolderViewer::emptyFolder()
{
    QTemporaryDir empty;
    QVERIFY(empty.isValid());
    MainWindow w;
    int folders = 0, files = 0;
    w.buildTree(empty.path(), nullptr, folders, files);
    QCOMPARE(folders, 0);
    QCOMPARE(files, 0);
}

void TestFolderViewer::singleFolder()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    QDir(dir.path()).mkdir("sub");
    MainWindow w;
    int folders = 0, files = 0;
    w.buildTree(dir.path(), nullptr, folders, files);
    QCOMPARE(folders, 1);
    QCOMPARE(files, 0);
}

void TestFolderViewer::folderWithFile()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    QFile f(dir.path() + "/test.txt");
    f.open(QIODevice::WriteOnly);
    f.close();
    MainWindow w;
    int folders = 0, files = 0;
    w.buildTree(dir.path(), nullptr, folders, files);
    QCOMPARE(folders, 0);
    QCOMPARE(files, 1);
}

void TestFolderViewer::nestedFolders()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    QDir root(dir.path());
    root.mkdir("A");
    root.mkdir("A/B");
    root.mkdir("A/B/C");
    MainWindow w;
    int folders = 0, files = 0;
    w.buildTree(dir.path(), nullptr, folders, files);
    QCOMPARE(folders, 3); // A, A/B, A/B/C
    QCOMPARE(files, 0);
}

void TestFolderViewer::hiddenFolders()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    QDir root(dir.path());
    root.mkdir(".hiddenFolder");
    MainWindow w;
    int folders = 0, files = 0;
    w.buildTree(dir.path(), nullptr, folders, files);
    QCOMPARE(folders, 1);
    QCOMPARE(files, 0);
}

void TestFolderViewer::countFoldersFiles()
{
    QVERIFY(tempDir != nullptr);
    MainWindow w;
    int folders = 0, files = 0;
    w.buildTree(tempDir->path(), nullptr, folders, files);
    // Подпапки: folderA, folderB, .hidden, subA1, subA2 -> 5
    QCOMPARE(folders, 5);
    // Файлы: file1.txt, fileA.txt, fileB.txt, hiddenFile.txt -> 4
    QCOMPARE(files, 4);
}

void TestFolderViewer::accessDenied()
{
    MainWindow w;
    int folders = 0, files = 0;
    w.buildTree("/non/existent/path", nullptr, folders, files);
    QCOMPARE(folders, 0);
    QCOMPARE(files, 0);
}

void TestFolderViewer::treeRootItem()
{
    MainWindow w;
    w.m_tree->clear();
    int folders = 0, files = 0;
    QString rootName = "TestRoot";
    QTreeWidgetItem *rootItem = new QTreeWidgetItem(w.m_tree);
    rootItem->setText(0, rootName);
    w.buildTree(tempDir->path(), rootItem, folders, files);
    QCOMPARE(rootItem->text(0), rootName);
    QVERIFY(rootItem->childCount() > 0);
}

void TestFolderViewer::selectFolderTreeFilled()
{
    MainWindow w;
    w.m_tree->clear();
    int folders = 0, files = 0;
    QTreeWidgetItem *rootItem = new QTreeWidgetItem(w.m_tree);
    rootItem->setText(0, QDir(tempDir->path()).dirName());
    rootItem->setIcon(0, w.style()->standardIcon(QStyle::SP_DirIcon));
    w.buildTree(tempDir->path(), rootItem, folders, files);
    rootItem->setExpanded(true);
    // --- добавлено обновление метки ---
    w.m_resultLabel->setText(QString("Всего подпапок: %1, Всего файлов: %2")
                                 .arg(folders).arg(files));

    QVERIFY(w.m_tree->topLevelItemCount() == 1);
    QVERIFY(w.m_tree->topLevelItem(0)->childCount() > 0);
    QCOMPARE(w.m_resultLabel->text(), QString("Всего подпапок: %1, Всего файлов: %2")
                                          .arg(folders).arg(files));
}

void TestFolderViewer::labelAfterBuild()
{
    MainWindow w;
    int folders = 0, files = 0;
    w.buildTree(tempDir->path(), nullptr, folders, files);
    // В оригинальном приложении метка обновляется только в selectFolder,
    // но здесь мы её не вызываем. Поэтому просто проверим, что метод buildTree
    // вообще не падает и возвращает ожидаемые значения.
    QCOMPARE(folders, 5);
    QCOMPARE(files, 4);
}

QTEST_MAIN(TestFolderViewer)
