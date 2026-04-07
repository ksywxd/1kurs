using System;

namespace Task1
{
    class Program
    {
        static void Main()
        {
            TV.Price  = 500;
            Shop shop = Shop.Instance;
            shop.Name = "TV SHOP";

            MenuHandler menu = new MenuHandler(shop);
            menu.Run();
        }
    }
}