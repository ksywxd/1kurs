using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;
using System.Xml.Linq;

namespace Task1
{
    public class Product
    {
        public double Price { get; private set; }
        public string Title { get; private set; }
        public Category Category { get; private set; }

        public Product(string title, double price, Category category)
        {
            if (string.IsNullOrWhiteSpace(title))
                throw new ArgumentException("Название товара не может быть пустым.");
            if (price <= 0)
                throw new ArgumentException("Цена товара должна быть положительной.");

            Title = title;
            Price = price;
            Category = category;
        }

        public override string ToString() => $"{Title} ({Category}) - {Price:C}";
    }
}
