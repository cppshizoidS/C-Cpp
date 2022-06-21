#include <iostream>
#include <string>
#include <typeinfo>

class Expression
{
public:
    virtual Expression *diff() = 0;
    virtual Expression *diff(std::string var) = 0;
    virtual Expression *simplify() = 0;
    virtual void print() = 0;
};

class Number: public Expression
{
public:
    double data;

public:
    Number(double d)
    {
        data = d;
    };
    Expression *diff()
    {
        Number *res = new Number(0);
        return res;
    }
    Expression *diff(std::string var)
    {
        Number *res = new Number(0);
        return res;
    }
    Expression *simplify()
    {
        return this;
    }
    void print()
    {
        if (data < 0)
            std::cout << "(" << data << ")";
        else
            std::cout << data;
    }
};

class Variable: public Expression
{
public:
    std::string data;

public:
    Variable(std::string d)
    {
        data = d;
    };

    Expression *diff()
    {
        Number *res = new Number(1);
        return res;
    }
    Expression *diff(std::string var)
    {
        if (data == var)
        {
            Number *res = new Number(1);
            return res;
        }
        else
        {
            Number *res = new Number(0);
            return res;
        }
    }
    Expression *simplify()
    {
        return this;
    }
    void print()
    {
        std::cout << data;
    }
};

class Add: public Expression
{
public:
    Expression *left;
    Expression *right;

public:
    Add(Expression *e1, Expression *e2)
    {
        left = e1;
        right = e2;
    }
    Expression *diff()
    {
        //(u+v)' = u' + v'
        Add *res = new Add(left->diff(), right->diff());
        return res->simplify();
    }
    Expression *diff(std::string var)
    {
        Add *res = new Add(left->diff(var), right->diff(var));
        return res->simplify();
    }
    Expression *simplify()
    {
        left = left->simplify();
        right = right->simplify();
        if (typeid(*left) == typeid(Number) && typeid(*right) == typeid(Number))
        {
            Number *res = new Number(dynamic_cast<Number*>(left)->data + dynamic_cast<Number*>(right)->data);
            return res;
        }
        if (typeid(*left) == typeid(Number) && dynamic_cast<Number*>(left)->data == 0)
            return right;
        if (typeid(*right) == typeid(Number) && dynamic_cast<Number*>(right)->data == 0)
            return left;
        return this;
    }
    void print()
    {
        std::cout << "(";
        left->print();
        std::cout << "+";
        right->print();
        std::cout << ")";
    }
};

class Sub: public Expression
{
public:
    Expression *left;
    Expression *right;

public:
    Sub(Expression *e1, Expression *e2)
    {
        left = e1;
        right = e2;
    }
    Expression *diff()
    {
        //(u-v)' = u' - v'
        Sub *res = new Sub(left->diff(), right->diff());
        return res->simplify();
    }
    Expression *diff(std::string var)
    {
        Sub *res = new Sub(left->diff(var), right->diff(var));
        return res->simplify();
    }
    Expression *simplify()
    {
        left = left->simplify();
        right = right->simplify();
        if (typeid(*left) == typeid(Number) && typeid(*right) == typeid(Number))
        {
            Number *res = new Number(dynamic_cast<Number*>(left)->data - dynamic_cast<Number*>(right)->data);
            return res;
        }
        if (typeid(*right) == typeid(Number) && dynamic_cast<Number*>(right)->data == 0)
        {
            return left;
        }
        return this;
    }
    void print()
    {
        std::cout << "(";
        left->print();
        std::cout << "-";
        right->print();
        std::cout << ")";
    }
};

class Mul: public Expression
{
public:
    Expression *left;
    Expression *right;

public:
    Mul (Expression *e1, Expression *e2)
    {
        left = e1;
        right = e2;
    }
    Expression *diff()
    {   //(uv)' = u'v + uv'
        Expression *res = new Add(new Mul(left->diff(), right), new Mul(left, right->diff()));
        return res->simplify();
    }
    Expression *diff(std::string var)
    {
        Expression *res = new Add(new Mul(left->diff(var), right), new Mul(left, right->diff(var)));
        return res->simplify();
    }
    Expression *simplify()
    {
        left = left->simplify();
        right = right->simplify();
        if (typeid(*left) == typeid(Number) && typeid(*right) == typeid(Number))
        {
            Number *res = new Number(dynamic_cast<Number*>(left)->data * dynamic_cast<Number*>(right)->data);
            return res;
        }
        if (typeid(*left) == typeid(Number))
        {
            switch ((int)dynamic_cast<Number*>(left)->data)
            {
            case 0:
            {
                return left;
                break;
            }
            case 1:
            {
                return right;
                break;
            }
            }
        }
        if (typeid(*right) == typeid(Number))
        {
            switch ((int)dynamic_cast<Number*>(right)->data)
            {
            case 0:
            {
                return right;
                break;
            }
            case 1:
            {
                return left;
                break;
            }
            }
        }
        return this;
    }
    void print()
    {
        std::cout << "(";
        left->print();
        std::cout << "*";
        right->print();
        std::cout << ")";
    }
};

class Div: public Expression
{
public:
    Expression *up;
    Expression *down;

public:
    Div(Expression *e1, Expression *e2)
    {
        up = e1;
        down = e2;
    }
    Expression *diff()
    {
        //(u/v)' = (u'v - uv')/(v*v)
        Expression *ans = new Div(new Sub(new Mul(up->diff(), down), new Mul(up, down->diff())), new Mul(down, down));
        return ans->simplify();
    }
    Expression *diff(std::string var)
    {
        Expression *ans = new Div(new Sub(new Mul(up->diff(var), down), new Mul(up, down->diff(var))), new Mul(down, down));
        return ans->simplify();
    }
    Expression *simplify()
    {
        up = up->simplify();
        down = down->simplify();
        if (typeid(*up) == typeid(Number) && typeid(*down) == typeid(Number))
        {
            Number *ans = new Number(dynamic_cast<Number*>(up)->data / dynamic_cast<Number*>(down)->data);
            return ans;
        }
        if (typeid(*up) == typeid(Number) && dynamic_cast<Number*>(up)->data == 0)
            return up;
        if (typeid(*down) == typeid(Number) && dynamic_cast<Number*>(down)->data == 1)
            return up;
        return this;
    }
    void print()
    {
        std::cout << "(";
        up->print();
        std::cout << "/";
        down->print();
        std::cout << ")";
    }
};

Expression *read_expression1(std::string s)
{
    std::cout << " - \"" << s << "\"\n";
    if (s == "")
    {
        Number *ans = new Number(0);
        return ans;
    }
    if (s[0] == '(')
    {
        int level = 0;
        for (int i=0; i<s.length(); i++)
        {
            switch (s[i])
            {
            case '(':
            {
                level++;
                break;
            }
            case ')':
            {
                level--;
                break;
            }
            case '+':
            {
                if (level == 1)
                {
                    Add *res = new Add(read_expression1(s.substr(1, i-1)), read_expression1(s.substr(i+1, s.length()-i-2)));
                    return res;
                }
                break;
            }
            case '-':
            {
                if (level == 1)
                {
                    Sub *res = new Sub(read_expression1(s.substr(1, i-1)), read_expression1(s.substr(i+1, s.length()-i-2)));
                    return res;
                }
                break;
            }
            case '*':
            {
                if (level == 1)
                {
                    Mul *res = new Mul(read_expression1(s.substr(1, i-1)), read_expression1(s.substr(i+1, s.length()-i-2)));
                    return res;
                }
                break;
            }
            case '/':
            {
                if (level == 1)
                {
                    Div *res = new Div(read_expression1(s.substr(1, i-1)), read_expression1(s.substr(i+1, s.length()-i-2)));
                    return res;
                }
                break;
            }
            }
        }
    }
    try
    {
        Number *res = new Number(std::stod(s));
        return res;
    }
    catch (std::invalid_argument)
    {
        Variable *res = new Variable(s);
        return res;
    }
};

Expression *read_expression(std::string s)
{
    Expression *ans = read_expression1(s);
    return ans->simplify();
}


int main()
{
    Number *n = new Number(15.25);
    Variable *v = new Variable("x");
    Add *aa = new Add(n, v);
    Sub *s = new Sub(v, new Number(0));

    n->print();
    std::cout << "\n";
    n->diff()->print();
    std::cout << "\n";
    v->print();
    std::cout << "\n";
    v->diff()->print();
    std::cout << "\n";
    aa->print();
    std::cout << "\n";
    aa->diff()->print();
    std::cout << "\n";
    s->print();
    std::cout << "\n";
    s->simplify()->print();
    std::cout << "\n";
    s->diff()->print();
    std::cout << "\n\n";

    Expression *a = new Add(new Add(new Add(new Number(15), new Variable("x")), new Variable("x")), new Sub(new Number(-4.23), new Variable("y")));
    a->print();
    std::cout << "\n";
    a->diff()->print();
    std::cout << "\n";
    a->diff("x")->print();
    std::cout << "\n";
    a->diff("y")->print();
    std::cout << "\n";
    a->diff("z")->print();
    std::cout << "\n\n";

    Expression *a4 = new Add(new Mul(new Number(-2), new Variable("x")), new Mul(new Variable("y"), new Number(0.25)));
    a4->print();
    std::cout << "\n";
    a4->diff("x")->print();
    std::cout << "\n";
    a4->diff("y")->print();
    std::cout << "\n";
    a4->diff()->print();
    std::cout << "\n\n";

    Expression *a2 = new Mul(new Mul(new Mul(new Number(3), new Variable("x")), new Variable("x")), new Variable("y"));
    a2->print();
    std::cout << "\n";
    a2->diff("x")->print();
    std::cout << "\n";
    a2->diff("x")->diff("x")->print();
    std::cout << "\n";
    a2->diff("x")->diff("x")->diff("x")->print();
    std::cout << "\n\n";

    Expression *a3 = new Div(new Number(1), new Variable("x"));
    a3->print();
    std::cout << "\n";
    a3->diff("x")->print();
    std::cout << "\n";
    a3->diff("x")->diff("x")->print();
    std::cout << "\n\n";

    Expression *s1 = read_expression("(y/((x*x)+(y*y)))");
    s1->print();
    std::cout << "\n";
    s1->diff("x")->print();
    std::cout << "\n";
}
