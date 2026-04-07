using System;
using System.Linq;

namespace Task1
{
    public class MenuHandler
    {
        private readonly Shop _shop;

        public MenuHandler(Shop shop)
        {
            _shop = shop;
        }

        public void Run()
        {
            bool exit = false;
            while (!exit)
            {
                Console.Clear();
                Console.WriteLine($"   {Shop.Instance.Name}   ");
                Console.WriteLine($"Price: {TV.Price} $.");
                Console.WriteLine("1. Add TV");
                Console.WriteLine("2. Sell TV");
                Console.WriteLine("3. Show all TVs");
                Console.WriteLine("4. Show revenue and sales numbers");
                Console.WriteLine("5. Change price");
                Console.WriteLine("6. Exit");
                Console.Write("Select: ");

                string choice = Console.ReadLine() ?? "";
                try
                {
                    switch (choice)
                    {
                        case "1": AddTVMenu();       break;
                        case "2": SellTVMenu();      break;
                        case "3": _shop.ShowInv();   break;
                        case "4": ShowRevenue();     break;
                        case "5": ChangePriceMenu(); break;
                        case "6": exit = true;       break;
                        default: throw new InvalidMenuChoiceException($"Invalid choice: '{choice}'. Please enter 1-6.");
                    }
                }
                catch (Exception ex) 
                {
                    Console.WriteLine($"Error: {ex.Message}");
                }

                if (!exit)
                {
                    Console.WriteLine("\nPress any key to continue...");
                    Console.ReadKey();
                }
            }
        }

        private void AddTVMenu()
        {
            Console.Clear();
            Console.WriteLine("   Adding a TV   ");
            Console.Write("Enter model: ");
            string model = Console.ReadLine() ?? "";

            if (string.IsNullOrWhiteSpace(model))
                throw new InputValidationException("Model cannot be empty.");

            Console.Write("Enter ID (leave blank for automatic ID): ");
            string idInput = Console.ReadLine() ?? "";
            int id = 0;
            bool hasId = !string.IsNullOrWhiteSpace(idInput) && int.TryParse(idInput, out id);

            if (!string.IsNullOrWhiteSpace(idInput) && !hasId)
                throw new InputValidationException("ID must be a number.");

            if (hasId)
                _shop.AddTV(model, id);
            else
                _shop.AddTV(model);

            Console.WriteLine("TV added successfully.");
        }

        private void SellTVMenu()
        {
            Console.Clear();
            Console.WriteLine("   Selling TV   ");
            Console.WriteLine("1. By ID");
            Console.WriteLine("2. By model");
            Console.Write("Choose a method: ");
            string opt = Console.ReadLine() ?? "";

            if (opt == "1")
            {
                Console.Write("Enter ID: ");

                if (!int.TryParse(Console.ReadLine(), out int id))
                    throw new InputValidationException("ID must be a number.");

                _shop.SellTV(id);
            }

            else if (opt == "2")
            {
                Console.Write("Enter model: ");
                string model = Console.ReadLine() ?? "";

                if (string.IsNullOrWhiteSpace(model))
                    throw new InputValidationException("Model cannot be empty.");

                _shop.SellTV(model);
            }

            else { throw new InvalidMenuChoiceException("Invalid choice. Choose 1 or 2."); }
        }

        private void ShowRevenue()
        {
            Console.Clear();
            Console.WriteLine($"Number of sales: {_shop.Count}");
            Console.WriteLine($"Total revenue: {_shop.GetTotalRevenue():F2} $.");
        }

        private static void ChangePriceMenu()
        {
            Console.Clear();
            Console.WriteLine($"Current price: {TV.Price} $.");
            Console.WriteLine("1. Set new price");
            Console.WriteLine("2. Increase price by amount");
            Console.WriteLine("3. Decrease price by amount");
            Console.Write("Select: ");

            string choice = Console.ReadLine() ?? "";
            try
            {
                switch (choice)
                {
                    case "1":
                        Console.Write("Enter new price: ");
                        string input = Console.ReadLine() ?? "";

                        if (!double.TryParse(input, out double newPrice))
                            throw new InputValidationException("Price must be a number.");

                        TV.Price = newPrice;
                        Console.WriteLine($"Price set to {TV.Price} $.");
                        break;

                    case "2":
                        Console.Write("Enter amount to increase: ");
                        string incInput = Console.ReadLine() ?? "";

                        if (!double.TryParse(incInput, out double incAmount))
                            throw new InputValidationException("Amount must be a number.");

                        TV.IncreasePrice(incAmount);
                        Console.WriteLine($"Price increased by {incAmount}. New price: {TV.Price} $.");
                        break;

                    case "3":
                        Console.Write("Enter amount to decrease: ");
                        string decInput = Console.ReadLine() ?? "";

                        if (!double.TryParse(decInput, out double decAmount))
                            throw new InputValidationException("Amount must be a number.");

                        TV.DecreasePrice(TV.Price, decAmount);
                        Console.WriteLine($"Price decreased by {decAmount}. New price: {TV.Price} $.");
                        break;

                    default:
                        throw new InvalidMenuChoiceException("Invalid choice. Choose 1, 2 or 3.");
                }
            }
            catch (ArgumentException ex)
            {
                Console.WriteLine($"Error: {ex.Message}");
            }
        }
    }
}