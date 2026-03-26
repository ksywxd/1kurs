using System;
namespace Task1
{
    public class Swap
    {
        public int DoSwap(int num)
        {
            int n1 = num / 100;
            int n2 = (num / 10) % 10;
            int n3 = num % 10;

            return n3 * 100 + n2 * 10 + n1;
        }
    }
}
