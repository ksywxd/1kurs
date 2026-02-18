using System;

namespace Task2
{
    class Program
    {
        static void Main()
        {
            Task2();

            bool exit = false;
            while (!exit)
            {
                Console.WriteLine("     MENU");
                Console.WriteLine("1. Continue");
                Console.WriteLine("0. Exit");

                string choice = Console.ReadLine()?.Trim() ?? "";

                switch (choice)
                {
                    case "1":
                        Task2();
                        break;
                    case "0":
                        exit = true;
                        break;
                    default:
                        Console.WriteLine("Incorrect input. Repeat");
                        break;
                }
            }
        }

        static void Task2()
        {
            bool valid = false;

            while (!valid)
            {
                Console.WriteLine("Vvedite koordinati tochki:");
                Console.Write("x: ");
                string? input1 = Console.ReadLine();
                Console.Write("y: ");
                string? input2 = Console.ReadLine();
                if (double.TryParse(input1, out double x) && double.TryParse(input2, out double y))
                {
                    double eps = 1e-9;

                    if (((x*x + y*y) < 15*15 - eps) && (y > Math.Abs(x) + eps))
                    {
                        valid = true;
                        Console.WriteLine("Da");
                    }
                    else if (((x * x + y * y) > 15*15 + eps) || (y < Math.Abs(x) - eps))
                    {
                        valid = true;
                        Console.WriteLine("Net");
                    }
                    else
                    {
                        valid = true;
                        Console.WriteLine("Na granice");
                    }
                }
                else
                {
                    Console.WriteLine("Incorrect input. Repeat");
                }
            }
        }
    }
}