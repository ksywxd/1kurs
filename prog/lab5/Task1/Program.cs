using System;

namespace Task1
{
    class Program
    {
        static void Main() 
        {
            Store store = new Store();
            StoreView view = new StoreView(store);
            view.RunDemo();

            Console.WriteLine("\nНажмите любую клавишу для выхода...");
            Console.ReadKey();

        }
    }
}