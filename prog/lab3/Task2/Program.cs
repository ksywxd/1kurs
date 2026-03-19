using System;
using Task2.Services;
namespace Task2
{
    class Program
    {
        static void Main()
        {
            Calculate myCalc = new Calculate();
            double input = 0.0;

            Console.Write("Enter any number: ");
            while (!double.TryParse(Console.ReadLine(), out input))
            {
                Console.WriteLine("Invalid input. Try again.");
            }

            double res = myCalc.doCalculate(input);     
        }
    }
}