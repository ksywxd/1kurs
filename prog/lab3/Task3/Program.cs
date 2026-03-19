using System;
namespace Task3
{
    class Program
    {
        static void Main(string[] args)
        {
            DateService ds = new DateService();

            string day = ds.GetDay("27.11.2007");
            Console.WriteLine($"Day of the week: {day}");

            int daysBetween = ds.GetDaysSpan(27, 11, 2007);
            Console.WriteLine($"Days between today and 27.11.2007: {daysBetween}");
        }
    }
}