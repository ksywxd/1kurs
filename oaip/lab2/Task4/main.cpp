#include <iostream>
#include "Expression.h"

bool check_equals(Expression const* left, Expression const* right) {
    // Если любой указатель нулевой, они не могут указывать на объекты одного класса
    if (!left || !right) return false;
    return *reinterpret_cast<void const* const*>(left) == *reinterpret_cast<void const* const*>(right);
}

int main() {
    // Создаём объекты разных типов
    Expression* num1 = new Number(3.0);
    Expression* num2 = new Number(5.5);
    Expression* binOp = new BinaryOperation(new Number(2.0), '*', new Number(3.0));

    // Выводим результаты проверок
    std::cout << std::boolalpha;
    std::cout << "num1 & num2 (both Number): " << check_equals(num1, num2) << std::endl;        // true
    std::cout << "num1 & binOp (Number & BinaryOperation): " << check_equals(num1, binOp) << std::endl; // false
    std::cout << "binOp & binOp (same): " << check_equals(binOp, binOp) << std::endl;   // true

    // Освобождаем память
    delete num1;
    delete num2;
    delete binOp; // удалит и свои операнды

    return 0;
}
