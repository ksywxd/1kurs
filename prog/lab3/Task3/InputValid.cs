using System;
using System.Globalization;

namespace Task3
{
	public class InputValid
	{
        public static DateTime GetValidDateString(string prompt)
        {
            while (true)
            {
                Console.Write(prompt);
                string input = Console.ReadLine() ?? "";

                /*if (DateTime.TryParseExact(input, "dd.MM.yyyy", CultureInfo.InvariantCulture, DateTimeStyles.None, out _))
                {
                    return input;
                }
                Console.WriteLine("Invalid input. Use the dd.mm.yyyy format (e.g., 01.01.2000)");*/
                string[] parts = input.Split('.');
                if (parts.Length != 3 ||
                    parts[0].Length != 2 || parts[1].Length != 2 || parts[2].Length != 4 ||
                    !int.TryParse(parts[0], out int day) ||
                    !int.TryParse(parts[1], out int month) ||
                    !int.TryParse(parts[2], out int year))
                {
                    Console.WriteLine("Format error. Use the dd.mm.yyyy format (e.g., 01.01.2000)");
                    continue;
                }

                if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1 || year > 9999)
                {
                    Console.WriteLine("Day must be from 1 to 31, month from 1 to 12, year from 1 to 9999");
                    continue;
                }

                try
                {
                    return new DateTime(year, month, day);
                }
                catch (ArgumentOutOfRangeException)
                {
                    Console.WriteLine("The entered date does not exist. Try again.");
                }
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