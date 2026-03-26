using System;
namespace Task2
{
    class Program
    {
        static void Main()
        {
            Calculate myCalc = new();
            Console.Write("Enter any number: ");
            double input = 0.0;

            while (!double.TryParse(Console.ReadLine(), out input))
            {
                Console.WriteLine("Invalid input. Try again.");
            }

            myCalc.DoCalculate(input);     
        }
    }
}