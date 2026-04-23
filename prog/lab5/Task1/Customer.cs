using System;
using System.Collections.Generic;
using System.Text;

namespace Task1
{
    public class Customer
    {
        private readonly List<Product> _lstProd = new List<Product>();

        public string LastName { get; private set; }

        public IReadOnlyList<Product> LstProd
        {
            get { return _lstProd; }
        }

        public Customer(string lastName)
        {
            if (string.IsNullOrWhiteSpace(lastName))
                throw new ArgumentException("Фамилия не может быть пустой.");
            LastName = lastName;
        }

        public void AddPurchase(Product product)
        {
            if (product == null)
                throw new ArgumentNullException(nameof(product));
            _lstProd.Add(product);
        }

        public double GetTotalSpent()
        {
            double total = 0.0;
            foreach (Product product in _lstProd)
            {
                total += product.Price;
            }
            return total;
        }
    }
}
