#ifndef TEST_H
#define TEST_H

#include <QObject>

class TestFolderViewer : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();           // создаёт временную структуру
    void cleanupTestCase();        // удаляет временную структуру
    void emptyFolder();            // 1. пустая папка
    void singleFolder();           // 2. одна подпапка
    void folderWithFile();         // 3. один файл
    void nestedFolders();          // 4. вложенные папки
    void hiddenFolders();          // 5. скрытые папки (с точкой)
    void countFoldersFiles();      // 6. сложная структура
    void accessDenied();           // 7. несуществующий путь
    void treeRootItem();           // 8. корневой элемент дерева
    void selectFolderTreeFilled(); // 9. дерево заполняется после выбора папки
    void labelAfterBuild();        // 10. метка результата обновляется
};

#endif // TEST_H
