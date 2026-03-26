using System;
namespace Task1
{
    class Program
    {
        static void Main()
        {
            Swap mySwap = new Swap();
            int input = 0;

            Console.Write("Enter a 3-digit num: ");

            while (!int.TryParse(Console.ReadLine(), out input) 
                   || Math.Abs(input) < 100 || Math.Abs(input) > 999)
            {
                Console.WriteLine("Invalid input. Try again.");
            }

            int res = mySwap.doSwap(input);
            Console.Write(res);
        }
    }
}