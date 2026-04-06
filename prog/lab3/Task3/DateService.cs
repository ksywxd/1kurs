using System;
using System.Globalization;

namespace Task3
{
    public class DateService
    {
        public string GetDay(DateTime date)
        {
            return date.ToString("dddd", CultureInfo.InvariantCulture);
        }

        public int GetDaysSpan(DateTime inputDate)
        {
            return Math.Abs((inputDate - DateTime.Today).Days);
        }
    }
}
