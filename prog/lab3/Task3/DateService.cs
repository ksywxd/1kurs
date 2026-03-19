using System;
namespace Task3
{
    internal class DateService
    {
        public string GetDay(string date)
        {
            DateTime dt = DateTime.Parse(date);
            return dt.DayOfWeek.ToString();
        }

        public int GetDaysSpan(int day, int month, int year)
        {
            DateTime today = DateTime.Today;
            DateTime inputDate = new DateTime(year, month, day);

            return Math.Abs((inputDate - today).Days);
        }
    }
}
