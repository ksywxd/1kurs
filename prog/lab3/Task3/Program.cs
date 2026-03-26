using System;

namespace Task3
{
    class Program
    {
        static void Main()
        {
            DateService ds = new();

            string validStr = InputValid.GetValidDateString("Enter date (e.g., 01.01.2000): ");
            Console.WriteLine($"Day of the week: {ds.GetDay(validStr)}");

            Console.WriteLine("\nEnter the date in parts:");
            DateTime validIntDate = InputValid.GetValidDateInt();
            Console.WriteLine($"Days between dates: {ds.GetDaysSpan(validIntDate)}");
        }
    }
}