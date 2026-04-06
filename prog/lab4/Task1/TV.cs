using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Reflection.Metadata;
using System.Text;

namespace Task1
{
    public class TV
    {
        private static double price = 0;
        private string model = "Unknown";
        private int id = 0;
        private static readonly Validator _validator = new Validator();

        public static double Price
        {
            get { return price; }
            set 
            {
                if (value < 0) throw new ArgumentException("Value below zero.");
                price = value; 
            }
        }

        public string Model 
        { 
            get { return model; } 
            set { model = value; }
        }

        public int ID
        {
            get { return id; }
            set { id = value; }
        }

        public TV(string newModel)
        {
            model = newModel;
        }

        public TV(string newModel, int newID)
        {
            model = newModel;
            id = newID;
        }
        
        public static void IncreasePrice(double amount)
        {
            _validator.ValidatePriceChange(amount, true, price);
            price += amount;
        }

        public static void DecreasePrice(double amount)
        {
            _validator.ValidatePriceChange(amount, false, price);
            price -= amount;
        }
    }
}
