#include "date.h"
#include <cmath>

bool Date::isLeapYear(int y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

int Date::daysInMonth(int m, int y) {
    static const int days[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (m == 2 && isLeapYear(y))
        return 29;
    return days[m-1];
}

long long Date::toDays(int y, int m, int d) {
    long long days = 0;
    for (int i = 1; i < y; ++i) {
        days += isLeapYear(i) ? 366 : 365;
    }
    for (int i = 1; i < m; ++i) {
        days += daysInMonth(i, y);
    }
    days += d;
    return days;
}

int Date::dayOfWeek(int y, int m, int d) {
    if (m < 3) {
        m += 12;
        y -= 1;
    }
    int K = y % 100;
    int J = y / 100;
    int h = (d + (13*(m+1))/5 + K + K/4 + J/4 + 5*J) % 7;
    return (h + 5) % 7 + 1; // 1=Monday ... 7=Sunday
}

int Date::weekNumberISO(int y, int m, int d) {
    int ordinal = 0;
    for (int i = 1; i < m; ++i) {
        ordinal += daysInMonth(i, y);
    }
    ordinal += d;

    int dowJan1 = dayOfWeek(y, 1, 1);
    int firstThursday = 4 - dowJan1;
    if (dowJan1 > 4) firstThursday += 7;
    int weekOfJan1 = (firstThursday <= 0) ? 1 : 0;

    if (ordinal < firstThursday) {
        int prevYear = y - 1;
        int lastDayPrevYear = isLeapYear(prevYear) ? 366 : 365;
        int dowLastDayPrev = dayOfWeek(prevYear, 12, 31);
        int lastThursday = 4 - dowLastDayPrev;
        if (dowLastDayPrev > 4) lastThursday += 7;
        return (lastDayPrevYear - lastThursday) / 7 + 1;
    } else {
        int week = (ordinal - firstThursday) / 7 + 1;
        if (week > 52) {
            int nextYear = y + 1;
            int dowNextJan1 = dayOfWeek(nextYear, 1, 1);
            int nextFirstThursday = 4 - dowNextJan1;
            if (dowNextJan1 > 4) nextFirstThursday += 7;
            if ((ordinal - firstThursday) >= (7 * 52) && nextFirstThursday <= 3) {
                week = 1;
            }
        }
        return week;
    }
}

Date::Date(int d, int m, int y) : day(d), month(m), year(y) {
    if (y < 1 || y > 9999)
        throw std::invalid_argument("Год должен быть от 1 до 9999");
    if (m < 1 || m > 12)
        throw std::invalid_argument("Месяц должен быть от 1 до 12");
    if (d < 1 || d > daysInMonth(m, y))
        throw std::invalid_argument("Некорректный день для данного месяца и года");
}

Date Date::nextDay() const {
    int d = day + 1;
    int m = month;
    int y = year;
    if (d > daysInMonth(m, y)) {
        d = 1;
        m++;
        if (m > 12) {
            m = 1;
            y++;
        }
    }
    return Date(d, m, y);
}

Date Date::previousDay() const {
    int d = day - 1;
    int m = month;
    int y = year;
    if (d < 1) {
        m--;
        if (m < 1) {
            m = 12;
            y--;
        }
        d = daysInMonth(m, y);
    }
    return Date(d, m, y);
}

bool Date::isLeap() const {
    return isLeapYear(year);
}

short Date::weekNumber() const {
    return static_cast<short>(weekNumberISO(year, month, day));
}

int Date::daysTillYourBirthday(const Date& birthday) const {
    // birthday: дата рождения (день, месяц, год)
    // Текущая дата: *this

    // День рождения в текущем году
    Date thisYearBirthday(birthday.getDay(), birthday.getMonth(), year);
    // День рождения в следующем году
    Date nextYearBirthday(birthday.getDay(), birthday.getMonth(), year + 1);
    // День рождения в году рождения
    Date birthYearBirthday(birthday.getDay(), birthday.getMonth(), birthday.getYear());

    // Если текущая дата раньше года рождения
    if (year < birthday.getYear()) {
        return static_cast<int>(toDays(birthYearBirthday.getYear(),
                                       birthYearBirthday.getMonth(),
                                       birthYearBirthday.getDay()) -
                                toDays(year, month, day));
    }
    // Если текущая дата в году рождения или позже
    if (*this < thisYearBirthday) {
        return static_cast<int>(toDays(thisYearBirthday.getYear(),
                                       thisYearBirthday.getMonth(),
                                       thisYearBirthday.getDay()) -
                                toDays(year, month, day));
    } else {
        return static_cast<int>(toDays(nextYearBirthday.getYear(),
                                       nextYearBirthday.getMonth(),
                                       nextYearBirthday.getDay()) -
                                toDays(year, month, day));
    }
}

int Date::duration(const Date& other) const {
    return static_cast<int>(toDays(year, month, day) - toDays(other.getYear(),
                                                              other.getMonth(),
                                                              other.getDay()));
}

std::string Date::toString() const {
    char buf[12];
    snprintf(buf, sizeof(buf), "%02d.%02d.%04d", day, month, year);
    return std::string(buf);
}

bool Date::operator==(const Date& other) const {
    return day == other.day && month == other.month && year == other.year;
}

bool Date::operator<(const Date& other) const {
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    return day < other.day;
}
