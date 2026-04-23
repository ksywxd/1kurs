using System;
using System.Collections.Generic;
using System.Text;

namespace Task1
{
    public class Store
    {
        private readonly List<Product> _catalog = new List<Product>();
        private readonly List<Customer> _customers = new List<Customer>();

        public IReadOnlyList<Product> Catalog => _catalog;
        public IReadOnlyList<Customer> Customers => _customers;

        public void AddProduct(Product product)
        {
            if (product == null)
                throw new ArgumentNullException(nameof(product));

            Product existing = FindProductByName(product.Title);
            if (existing != null)
                throw new InvalidOperationException($"Товар с названием '{product.Title}' уже существует.");

            _catalog.Add(product);
        }

        public void RegisterOrder(string customerLastName, string productName)
        {
            if (string.IsNullOrWhiteSpace(customerLastName))
                throw new ArgumentException("Фамилия клиента не может быть пустой.");
            if (string.IsNullOrWhiteSpace(productName))
                throw new ArgumentException("Название товара не может быть пустым.");

            //поиск товара
            Product product = FindProductByName(productName);
            if (product == null)
                throw new KeyNotFoundException($"Товар '{productName}' не найден в каталоге.");

            //поиск клиента
            Customer customer = FindCustomerByLastName(customerLastName);
            if (customer == null)
            {
                customer = new Customer(customerLastName);
                _customers.Add(customer);
            }

            customer.AddPurchase(product);
        }

        public IReadOnlyList<Product> GetCustomerProducts(string lastName)
        {
            Customer customer = FindCustomerByLastName(lastName);
            if (customer == null)
                throw new KeyNotFoundException($"Клиент с фамилией '{lastName}' не найден.");
            return customer.LstProd;
        }

        public double GetCustomerTotal(string lastName)
        {
            Customer customer = FindCustomerByLastName(lastName);
            if (customer == null)
                throw new KeyNotFoundException($"Клиент с фамилией '{lastName}' не найден.");
            return customer.GetTotalSpent();
        }

        //методы поиска
        private Product? FindProductByName(string title)
        {
            foreach (Product product in _catalog)
            {
                if (product.Title.Equals(title, StringComparison.OrdinalIgnoreCase))
                    return product;
            }
            return null;
        }

        private Customer? FindCustomerByLastName(string lastName)
        {
            foreach (Customer customer in _customers)
            {
                if (customer.LastName.Equals(lastName, StringComparison.OrdinalIgnoreCase))
                    return customer;
            }
            return null;
        }
    }
}
