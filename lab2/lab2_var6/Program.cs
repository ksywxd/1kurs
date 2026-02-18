using System;

namespace Task1
{
    class Program
    {
        static void Main()
        {
            Task1();

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
                        Task1();
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

        static void Task1()
        {
            bool valid = false;

            while (!valid)
            {
                Console.Write("Vvedite dvuhznachnoe chislo: ");
                string? input = Console.ReadLine();
                if (int.TryParse(input, out int num))
                {
                    if (num < 10 || num > 99)
                    {
                        Console.WriteLine("Error: vvedite dvuhznachnoe chislo");
                    }
                    else
                    {
                        valid = true;

                        int first = num / 10;
                        int second = num % 10;

                        Console.WriteLine($"First num: {first}");
                        Console.WriteLine($"Second num: {second}");

                        if (first > second)
                        {
                            Console.WriteLine("The first is greater");
                        }
                        else if (first < second)
                        {
                            Console.WriteLine("The second is greater");
                        }
                        else
                        {
                            Console.WriteLine("Equal");
                        }
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