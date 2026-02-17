using System;

public class 553503_Siamionava
{
	public Class1()
	{
		static void Main(string[])
		{
			Consol.WriteLine("Введите делимое:");
			string first = Console.ReadLine();
			Consol.WriteLine("Введите делитель:");
			string second = Console.ReadLine();
			
			if (double.TryParse(first, out double delimoe && double.TryParse(second, out double delitel)
			{
				if (delitel != 0)
				{
					double result = 0;
					result = delitel / delitel;
					Console.WriteLine($"Результат: {result}");
				}
				else
				{
				Console.WriteLine("Деление на 0");
				}
			}
			else
			{
				Console.WriteLine("Введено не число")
			}
		}
	}
}
