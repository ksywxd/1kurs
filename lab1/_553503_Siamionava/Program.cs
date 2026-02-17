using System;

class Program
{
    static void Main()
    {
        Console.Write("Vvedite delimoe:");
        double delimoe = double.Parse(Console.ReadLine());

        Console.Write("Vvedite delitel:");
        double delitel = double.Parse(Console.ReadLine());

        double res = delimoe / delitel;

        Console.WriteLine($"Result: {res}");
    }
}