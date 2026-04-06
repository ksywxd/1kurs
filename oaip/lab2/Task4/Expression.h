#ifndef EXPRESSION_H
#define EXPRESSION_H

class Expression {
public:
    virtual ~Expression() {}
    virtual double evaluate() const = 0;
};

class Number : public Expression {
private:
    double value;
public:
    Number(double v) : value(v) {}
    double evaluate() const override { return value; }
};

class BinaryOperation : public Expression {
private:
    Expression* left;
    Expression* right;
    char op;
public:
    BinaryOperation(Expression* l, char oper, Expression* r)
        : left(l), right(r), op(oper) {}
    ~BinaryOperation() override {
        delete left;
        delete right;
    }
    double evaluate() const override {
        double lval = left->evaluate();
        double rval = right->evaluate();
        switch (op) {
        case '+': return lval + rval;
        case '-': return lval - rval;
        case '*': return lval * rval;
        case '/': return lval / rval;
        default: return 0.0;
        }
    }
};

#endif // EXPRESSION_H
