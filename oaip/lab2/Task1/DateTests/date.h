#ifndef DATE_H
#define DATE_H

#include <stdexcept>
#include <string>

class Date {
private:
    int day;
    int month;
    int year;

    static bool isLeapYear(int y);
    static int daysInMonth(int m, int y);
    static long long toDays(int y, int m, int d);
    static int dayOfWeek(int y, int m, int d);
    static int weekNumberISO(int y, int m, int d);

public:
    Date(int d, int m, int y);

    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }

    Date nextDay() const;
    Date previousDay() const;
    bool isLeap() const;
    short weekNumber() const;
    int daysTillYourBirthday(const Date& birthday) const;
    int duration(const Date& other) const;

    std::string toString() const;

    bool operator==(const Date& other) const;
    bool operator<(const Date& other) const;
};

#endif // DATE_H
