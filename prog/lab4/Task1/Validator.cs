using System;
using System.Collections.Generic;
using System.Text;
using System.Linq;

namespace Task1
{
    public class Validator
    {
        public void ValidTV(TV tv, IEnumerable<TV> existingTVs)
        {
            if (existingTVs == null)
                throw new ArgumentNullException("List cannot be null.");

            if (tv == null)
                throw new ArgumentNullException("TV cannot be null.");

            ValidModel(tv.Model, existingTVs);
            ValidID(tv.ID, existingTVs);
        }

        public void ValidModel(string  model, IEnumerable<TV> existingTVs)
        {
            if (existingTVs == null)
                throw new ArgumentNullException("List cannot be null.");

            if (string.IsNullOrWhiteSpace(model))
                throw new ArgumentException("Name of model cannot be empty.");

            if (existingTVs.Any(n => n.Model == model))
                throw new InvalidOperationException($"TV with name {model} already exists.");
        }

        public void ValidID(int id, IEnumerable<TV> existingTVs)
        {
            if (existingTVs == null)
                throw new ArgumentNullException("List cannot be null.");

            if (id < 0)
                throw new ArgumentException("ID cannot be below zero.");

            if (id != 0 && existingTVs.Any(t => t.ID == id))
                throw new InvalidOperationException($"TV with ID {id} already exists.");
        }

        public void ValidatePriceChange(double amount, bool isIncrease, double currentPrice)
        {
            if (amount < 0)
                throw new ArgumentException("Amount cannot be negative.");

            if (!isIncrease && currentPrice - amount < 0)
                throw new ArgumentException("Price cannot become negative.");
        }
    }

    public class InvalidMenuChoiceException : Exception
    {
        public InvalidMenuChoiceException(string message) : base(message) { }
    }

    public class InputValidationException : Exception
    {
        public InputValidationException(string message) : base(message) { }
    }
}