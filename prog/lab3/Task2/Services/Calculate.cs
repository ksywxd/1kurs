using System;
namespace Task2
{
    public class Calculate
    {
        public void DoCalculate(double z)
        {
            double y = 0;
            double x = 0;
            int branch = 0;

            if (z > 0)
            {
                branch = 1;
                x = Math.Sqrt(z);
                y = Math.Cos(5 * x) + Math.Sin(0.2 * x) + Math.Exp(x);
            }
            else
            {
                branch = 2;
                x = 3 * Math.Pow(z, 3) - z - 5;
                y = Math.Cos(5 * x) + Math.Sin(0.2 * x) + Math.Exp(x);
            }

            Console.Write($"Result: {y}. Branch: {branch}.");  
        }
    }
}
