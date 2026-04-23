using System;
using System.Collections.Generic;
using System.Text;

namespace Task1
{
    public class StoreView
    {
        private readonly Store _store;

        public StoreView(Store store)
        {
            _store = store ?? throw new ArgumentNullException(nameof(store));
        }

        public void RunDemo()
        {
            Console.WriteLine("   Интернет-магазин\n");

            // 1. Добавление товаров
            Console.WriteLine("1. Добавление товаров в каталог:");
            try
            {
                _store.AddProduct(new Product("Ноутбук",  7500, Category.Electronics));
                _store.AddProduct(new Product("Футболка", 120,  Category.Clothing));
                _store.AddProduct(new Product("Книга",    850,  Category.Books));
                _store.AddProduct(new Product("Шоколад",  150,  Category.Food));
                Console.WriteLine("   Товары успешно добавлены.");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"   Ошибка каталога: {ex.Message}");
            }

            // Показываем каталог
            Console.WriteLine("\n   КАТАЛОГ ТАВАРОВ:");
            foreach (Product product in _store.Catalog)
            {
                Console.WriteLine($"   * {product}");
            }

            // 2. Регистрация заказов
            Console.WriteLine("\n2. РЕГИСТРАЦИЯ ЗАКАЗОВ:");
            try
            {
                _store.RegisterOrder("Иванов", "Ноутбук");
                _store.RegisterOrder("Иванов", "Книга");
                _store.RegisterOrder("Петров", "Футболка");
                _store.RegisterOrder("Сидоров", "Шоколад");
                _store.RegisterOrder("Сидоров", "Футболка");
                Console.WriteLine("   Заказы успешно оформлены.");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"   Ошибка заказа: {ex.Message}");
            }

            // 3. Вывод списка товаров по фамилии
            Console.WriteLine("\n3. СПИСОК ТОВАРОВ (по фамилии):");
            ShowCustomerProducts("Иванов");
            ShowCustomerProducts("Сидоров");

            // 4. Вывод суммы заказа по фамилии
            Console.WriteLine("\n4. СУММА ЗАКАЗА КЛИЕНТА (по фамилии):");
            ShowCustomerTotal("Иванов");
            ShowCustomerTotal("Петров");
            ShowCustomerTotal("Сидоров");
            ShowCustomerTotal("бабабой");

            // 5. Демонстрация обработки исключений
            Console.WriteLine("\n5. ПРОВЕРКА ИСКЛЮЧЕНИЙ:");
            try
            {
                Console.WriteLine("   Попытка добавить товар с отрицательной ценой...");
                _store.AddProduct(new Product("Ошибка", -100, Category.Other));
            }
            catch (Exception ex)
            {
                Console.WriteLine($"   Перехвачено исключение: {ex.Message}");
            }

            try
            {
                Console.WriteLine("   Попытка заказать несуществующий товар...");
                _store.RegisterOrder("Иванов", "Телевизор");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"   Перехвачено исключение: {ex.Message}");
            }

            try
            {
                Console.WriteLine("   Запрос данных несуществующего клиента...");
                _store.GetCustomerTotal("Неизвестный");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"   Перехвачено исключение: {ex.Message}");
            }

            Console.WriteLine("\nДемонстрация завершена.");
        }

        private void ShowCustomerProducts(string lastName)
        {
            try
            {
                var products = _store.GetCustomerProducts(lastName);
                Console.WriteLine($"   {lastName}:");
                if (products.Count == 0)
                {
                    Console.WriteLine("      (нет заказов)");
                }
                else
                {
                    foreach (Product product in products)
                    {
                        Console.WriteLine($"      * {product.Title} ({product.Price:C})");
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"   Ошибка для '{lastName}': {ex.Message}");
            }
        }

        private void ShowCustomerTotal(string lastName)
        {
            try
            {
                double total = _store.GetCustomerTotal(lastName);
                Console.WriteLine($"   {lastName}: {total:C}");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"   Ошибка для '{lastName}': {ex.Message}");
            }
        }
    }
}
