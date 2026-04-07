using System;
using System.Collections.Generic;
using System.Reflection;
using System.Text;
using System.Linq;

namespace Task1
{
    public sealed class Shop
    {
        private static Shop? _instance;
        private string _name = "Unknown";

        public string Name
        {
            get { return _name; }
            set { _name = value; }
        }

        private readonly List<TV> _tvList;

        private Shop()
        {
            _tvList = new List<TV>();
        }

        public static Shop Instance
        {
            get
            {
                if (_instance == null) { _instance = new Shop(); }
                return _instance;
            }
        }

        private int _purchaseCount = 0;

        public int Count
        {
            get { return _purchaseCount; }
        }

        private int GenerateUniqueId()
        {
            if (_tvList.Count == 0) return 1;

            int maxId = _tvList.Max(t => t.ID);
            return maxId + 1;
        }

        public void AddTV(TV tv)
        {
            Validator.ValidTV(tv, _tvList);

            if (tv.ID == 0)
            {
                int newId = GenerateUniqueId();
                tv = new TV(tv.Model, newId);
            }

            _tvList.Add(tv);
        }

        public void AddTV(string newModel)
        {
            Validator.ValidModel(newModel, _tvList);
            int newId = GenerateUniqueId();
            _tvList.Add(new TV(newModel, newId));
        }

        public void AddTV(string newModel, int newID)
        {
            Validator.ValidModel(newModel, _tvList);

            if (newID == 0) { newID = GenerateUniqueId(); }

            else { Validator.ValidID(newID, _tvList); }

            _tvList.Add(new TV(newModel, newID));
        }

        public void SellTV(string model)
        {
            TV? tv = _tvList.SingleOrDefault(t => t.Model.Equals(model, StringComparison.OrdinalIgnoreCase));

            if (tv == null)
                throw new InvalidOperationException($"TV '{model}' not found.");

            _tvList.Remove(tv);
            _purchaseCount++;
            Console.WriteLine($"Sold {tv.Model} (ID {tv.ID})");
        }

        public void SellTV(int id)
        {
            TV? tv = _tvList.SingleOrDefault(t => t.ID == id);

            if (tv == null)
                throw new InvalidOperationException($"TV with ID {id} not found.");

            _tvList.Remove(tv);
            _purchaseCount++;
            Console.WriteLine($"Sold {tv.Model} (ID {tv.ID})");
        }

        public void ShowInv()
        {
            Console.Clear();
            Console.WriteLine($"\nShop name: {Name}");
            Console.WriteLine($"Price: {TV.Price} $.");
            Console.WriteLine($"Sold: {_purchaseCount} pcs.");
            if (_tvList.Count == 0)
            {
                Console.WriteLine("The assortment is empty.");
                return;
            }
            Console.WriteLine("TVs in stock:");
            foreach (var tv in _tvList)
            {
                Console.WriteLine($"  ID: {tv.ID}, Model: {tv.Model}");
            }
        }

        public double GetTotalRevenue()
        {
            return TV.Price * _purchaseCount;
        }
    }
}
