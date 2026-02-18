using System;

namespace lab2_var6
{
    class Program
    {
        static void Main()
        {
            bool exit = false;
            while (!exit)
            {
                Console.WriteLine(" Lab2 var 6");
                Console.WriteLine("Choose task:");
                Console.WriteLine("1. Task1");
                Console.WriteLine("2. Task2");
                Console.WriteLine("0. Exit");

                string choice = Console.ReadLine()?.Trim() ?? "";

                switch (choice)
                {
                    case "1":
                        Task1();
                        break;
                    case "2":
                        //Task2();
                        break;
                    case "0":
                        exit = true;
                        break;
                    default:
                        Console.WriteLine("Incorrect input");
                        break;
                }
            }
        }

        static void Task1()
        {
            bool repeat = true;

            do 
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
                            Console.WriteLine("Equil");
                        }

                        bool validChoice = false;
                        do
                        {
                            Console.WriteLine("\n1. Repeat");
                            Console.WriteLine("0. Exit to main menu");

                            string choice = Console.ReadLine()?.Trim() ?? "";
                            switch (choice)
                            {
                                case "1":
                                    repeat = true;
                                    break;
                                case "0":
                                    repeat = false;
                                    break;
                                default:
                                    Console.WriteLine("Incorrect choice");
                                    break;
                            }
                        } while (!validChoice);
                    }
                }
                else
                {
                    Console.WriteLine("Incorrect input. Repeat");
                }
            } while (repeat);
        }
    }
}