using System;
using System.Collections.Generic;
using System.Text;
using System.Linq;

namespace Task1
{
    public static class Validator
    {
        public static void ValidTV(TV tv, IEnumerable<TV> existingTVs)
        {
            if (existingTVs == null) throw new ArgumentNullException("List cannot be null.");
            if (tv == null)          throw new ArgumentNullException("TV cannot be null.");

            ValidModel(tv.Model, existingTVs);
            ValidID(tv.ID, existingTVs);
        }

        public static void ValidModel(string  model, IEnumerable<TV> existingTVs)
        {
            if (existingTVs == null)                    throw new ArgumentNullException("List cannot be null.");
            if (string.IsNullOrWhiteSpace(model))       throw new ArgumentException("Name of model cannot be empty.");
            if (existingTVs.Any(n => n.Model == model)) throw new InvalidOperationException($"TV with name {model} already exists.");
        }

        public static void ValidID(int id, IEnumerable<TV> existingTVs)
        {
            if (existingTVs == null)                         throw new ArgumentNullException("List cannot be null.");
            if (id < 0)                                      throw new ArgumentException("ID cannot be below zero.");
            if (id != 0 && existingTVs.Any(t => t.ID == id)) throw new InvalidOperationException($"TV with ID {id} already exists.");
        }

        public static void ValidatePrice(double price)
        {
            if (price < 0) throw new ArgumentException("Price cannot be negative.");
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