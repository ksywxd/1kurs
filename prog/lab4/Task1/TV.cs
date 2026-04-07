using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Reflection.Metadata;
using System.Text;

namespace Task1
{
    public class TV
    {
        private static double _price = 0;
        private string _model = "Unknown";
        private int _id = 0;

        public static double Price
        {
            get { return _price; }
            set 
            {
                Validator.ValidatePrice(value);
                _price = value; 
            }
        }

        public string Model 
        { 
            get { return _model; } 
            set { _model = value; }
        }

        public int ID
        {
            get { return _id; }
            set { _id = value; }
        }

        public TV(string newModel)
        {
            _model = newModel;
        }

        public TV(string newModel, int newID)
        {
            _model = newModel;
            _id = newID;
        }
        
        public static void IncreasePrice(double amount)
        {
            if (amount < 0) throw new ArgumentException("Amount must be positive.");
            _price += amount;
        }

        public static void DecreasePrice(double amount)
        {
            if (amount < 0) throw new ArgumentException("Amount must be positive.");
            if (_price - amount < 0) throw new ArgumentException("Invalid amount. Current price is less than the entered amount.");
            _price -= amount;
        }
    }
}
