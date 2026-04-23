#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <QString>
#include <QSet>
#include <QMap>

class GameState
{
public:
    // текущая сцена
    QString getCurrentScene() const         { return m_currentScene; }
    void setCurrentScene(const QString& id) { m_currentScene = id; }

    // инвентарь
    bool hasItem(const QString& item) const     { return m_inventory.contains(item); }
    void addItem(const QString& item)           { m_inventory.insert(item); }
    void removeItem(const QString& item)        { m_inventory.remove(item); }
    const QSet<QString>& getInventory() const   { return m_inventory; }
    void setInventory(const QSet<QString>& inv) { m_inventory = inv; }

    //проверка нескольких предметов
    bool hasAll(const QStringList& items) const {
        for (const QString& item : items) {
            if (!m_inventory.contains(item)) return false;
        }
        return true;
    }

    //проверка на хотя бы 1 предмет
    bool hasAny(const QStringList& items) const {
        for (const QString& item : items) {
            if (m_inventory.contains(item)) return true;
        }
        return false;
    }

    // флаги
    void setFlag(const QString& flag, bool value = true) { m_flags[flag] = value; }
    void setFlags(const QMap<QString, bool>& f)          { m_flags = f; }
    bool getFlag(const QString& flag) const              { return m_flags.value(flag, false); }
    const QMap<QString, bool>& getFlags() const          { return m_flags; }

    // сброс для новой игры
    void clear() {
        m_currentScene.clear();
        m_inventory.clear();
        m_flags.clear();
    }

private:
    QString             m_currentScene;
    QSet<QString>       m_inventory;
    QMap<QString, bool> m_flags;
};

#endif // GAMESTATE_H
