#ifndef TRAIN_H
#define TRAIN_H

#include <QString>
#include <QDate>
#include <QTime>

class Train
{
private:
    QDate date;          // дата отправления
    int number;          // номер рейса
    QString destination; // конечный пункт
    QTime time;          // время отправления
    int totalCoupe;      // всего купейных мест
    int totalPlatzkart;  // всего плацкартных мест
    int freeCoupe;       // свободных купейных
    int freePlatzkart;   // свободных плацкартных

public:
    Train();
    Train(const QDate& d, int num, const QString& dest, const QTime& t,
          int totalC, int totalP, int freeC, int freeP);

    // Геттеры
    QDate getDate() const { return date; }
    int getNumber() const { return number; }
    QString getDestination() const { return destination; }
    QTime getTime() const { return time; }
    int getTotalCoupe() const { return totalCoupe; }
    int getTotalPlatzkart() const { return totalPlatzkart; }
    int getFreeCoupe() const { return freeCoupe; }
    int getFreePlatzkart() const { return freePlatzkart; }

    // Сеттеры (для корректировки)
    void setDate(const QDate& d) { date = d; }
    void setNumber(int num) { number = num; }
    void setDestination(const QString& dest) { destination = dest; }
    void setTime(const QTime& t) { time = t; }
    void setTotalCoupe(int c) { totalCoupe = c; }
    void setTotalPlatzkart(int p) { totalPlatzkart = p; }
    void setFreeCoupe(int c) { freeCoupe = c; }
    void setFreePlatzkart(int p) { freePlatzkart = p; }

    // Вычисляемые поля
    int soldCoupe() const { return totalCoupe - freeCoupe; }
    int soldPlatzkart() const { return totalPlatzkart - freePlatzkart; }
    int totalFree() const { return freeCoupe + freePlatzkart; }

    // Сериализация в строку и из строки
    QString toString() const;
    static Train fromString(const QString& line);

    // Операторы сравнения для сортировки (по дате, времени, номеру)
    bool operator<(const Train& other) const;
};

#endif // TRAIN_H
