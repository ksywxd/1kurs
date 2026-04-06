#ifndef EXPRESSION_H
#define EXPRESSION_H

class Expression {
public:
    virtual ~Expression() {}
    virtual double evaluate() const = 0;
};

class Number : public Expression {
private:
    double value;   // хранимое значение

public:
    Number(double v) : value(v) {}

    double evaluate() const override {
        return value;
    }
};

class BinaryOperation : public Expression {
private:
    Expression* left;   // левый операнд
    Expression* right;  // правый операнд
    char op;            // операция: '+', '-', '*', '/'

public:
    BinaryOperation(Expression* l, char oper, Expression* r)
        : left(l), right(r), op(oper) {}

    ~BinaryOperation() override {
        delete left;
        delete right;
    }

    // Реализация evaluate: вычисляет значение в зависимости от операции
    double evaluate() const override {
        double lval = left->evaluate();
        double rval = right->evaluate();
        switch (op) {
        case '+': return lval + rval;
        case '-': return lval - rval;
        case '*': return lval * rval;
        case '/':
            // Деление на ноль в данном задании не обрабатывается специально,
            // но для производственного кода следовало бы выбросить исключение.
            // По условию достаточно просто выполнить деление.
            return lval / rval;
        default:
            // Неизвестная операция – возвращаем 0 (можно также выбросить исключение)
            return 0.0;
        }
    }
};

#endif // EXPRESSION_H
