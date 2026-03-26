using System;
using System.Globalization;

namespace Task3
{
	public class InputValid
	{
        public static string GetValidDateString(string prompt)
        {
            while (true)
            {
                Console.Write(prompt);
                string input = Console.ReadLine() ?? "";

                if (DateTime.TryParseExact(input, "dd.MM.yyyy", CultureInfo.InvariantCulture, DateTimeStyles.None, out _))
                {
                    return input;
                }
                Console.WriteLine("Invalid input. Use the dd.mm.yyyy format (e.g., 01.01.2000)");
            }
        }

        public static DateTime GetValidDateInt()
        {
            while (true)
            {
                int d = GetInt("Day: ", 1, 31);
                int m = GetInt("Month: ", 1, 12);
                int y = GetInt("Year: ", 1, 9999);

                try
                {
                    return new DateTime(y, m, d);
                }
                catch (ArgumentOutOfRangeException)
                {
                    Console.WriteLine("The entered date does not exist. Try again.");
                }
            }
        }

        private static int GetInt(string prompt, int min, int max)
        {
            while (true)
            {
                Console.Write(prompt);
                if (int.TryParse(Console.ReadLine(), out int val) && val >= min && val <= max)
                {
                    return val;
                }

                Console.WriteLine($"Enter an integer between {min} and {max}");
            }
        }
    }
}