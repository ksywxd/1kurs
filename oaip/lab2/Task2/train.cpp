#include "train.h"
#include <QStringList>

Train::Train() : date(1,1,2000), number(0), totalCoupe(0), totalPlatzkart(0), freeCoupe(0), freePlatzkart(0) {}

Train::Train(const QDate& d, int num, const QString& dest, const QTime& t,
             int totalC, int totalP, int freeC, int freeP)
    : date(d), number(num), destination(dest), time(t),
    totalCoupe(totalC), totalPlatzkart(totalP),
    freeCoupe(freeC), freePlatzkart(freeP) {}

QString Train::toString() const
{
    // Формат: дата|номер|пункт|время|всего_куп|всего_плац|своб_куп|своб_плац
    return date.toString("dd.MM.yyyy") + "|" +
           QString::number(number) + "|" +
           destination + "|" +
           time.toString("hh:mm") + "|" +
           QString::number(totalCoupe) + "|" +
           QString::number(totalPlatzkart) + "|" +
           QString::number(freeCoupe) + "|" +
           QString::number(freePlatzkart);
}

Train Train::fromString(const QString& line)
{
    QStringList parts = line.split('|');
    if (parts.size() != 8) {
        throw std::invalid_argument("Неверный формат строки (должно быть 8 полей)");
    }

    bool ok;
    QDate date = QDate::fromString(parts[0], "dd.MM.yyyy");
    if (!date.isValid()) {
        throw std::invalid_argument("Неверная дата");
    }

    int num = parts[1].toInt(&ok);
    if (!ok) {
        throw std::invalid_argument("Неверный номер рейса");
    }

    QString dest = parts[2];

    QTime time = QTime::fromString(parts[3], "hh:mm");
    if (!time.isValid()) {
        throw std::invalid_argument("Неверное время");
    }

    int totalC = parts[4].toInt(&ok);
    if (!ok) throw std::invalid_argument("Неверное число купе");
    int totalP = parts[5].toInt(&ok);
    if (!ok) throw std::invalid_argument("Неверное число плацкарт");
    int freeC = parts[6].toInt(&ok);
    if (!ok) throw std::invalid_argument("Неверное число свободных купе");
    int freeP = parts[7].toInt(&ok);
    if (!ok) throw std::invalid_argument("Неверное число свободных плацкарт");

    return Train(date, num, dest, time, totalC, totalP, freeC, freeP);
}

bool Train::operator<(const Train& other) const
{
    if (date != other.date)
        return date < other.date;
    if (time != other.time)
        return time < other.time;
    return number < other.number;
}
