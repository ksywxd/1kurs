using System;
using System.Globalization;

namespace Task3
{
    public class DateService
    {
        public string GetDay(string date)
        {
            if (DateTime.TryParseExact(date, "dd.MM.yyyy", CultureInfo.InvariantCulture, DateTimeStyles.None, out DateTime dt))
            {
                return dt.ToString("dddd", CultureInfo.InvariantCulture);
            }
            throw new FormatException("Invalid date format. Expected dd.mm.yyyy.");
        }

        public int GetDaysSpan(DateTime inputDate)
        {
            return Math.Abs((inputDate - DateTime.Today).Days);
        }
    }
}
