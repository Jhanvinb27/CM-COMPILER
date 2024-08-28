// Node class
#include <iostream>
#include <stdexcept>

class Node {
public:
    int lexline = 0;
    static int labels;

    Node() {
        lexline = Lexer::line;  // Assuming Lexer is a class with a static member `line`
    }

    void error(const std::string& s) {
        throw std::runtime_error("near line " + std::to_string(lexline) + ": " + s);
    }

    int newlabel() {
        return ++labels;
    }

    void emitlabel(int i) {
        std::cout << "L" << i << " : ";
    }

    void emit(const std::string& s) {
        std::cout << "\t" << s << std::endl;
    }
};

int Node::labels = 0;

//Expr class
#include <string>

class Expr : public Node {
public:
    Token op;
    Type type;

    Expr(Token tok, Type p) : op(tok), type(p) {}

    virtual Expr* gen() {
        return this;
    }

    virtual Expr* reduce() {
        return this;
    }

    void jumping(int t, int f) {
        emitjumps(toString(), t, f);
    }

    void emitjumps(const std::string& test, int t, int f) {
        if (t != 0 && f != 0) {
            emit("if " + test + " goto L" + std::to_string(t));
            emit("goto L" + std::to_string(f));
        } else if (t != 0) {
            emit("if " + test + " goto L" + std::to_string(t));
        } else if (f != 0) {
            emit("iffalse " + test + " goto L" + std::to_string(f));
        }
    }

    virtual std::string toString() {
        return op.toString();
    }
};

// Id Class
class Id : public Expr {
public:
    int offset;

    Id(Word id, Type p, int b) : Expr(id, p), offset(b) {}
};

// Op Class
class Op : public Expr {
public:
    Op(Token tok, Type p) : Expr(tok, p) {}

    Expr* reduce() override {
        Expr* x = gen();
        Temp t(type);
        emit(t.toString() + " = " + x->toString());
        return &t;
    }
};

// Arith Class
class Arith : public Op {
public:
    Expr *expr1, *expr2;

    Arith(Token tok, Expr* x1, Expr* x2) : Op(tok, nullptr), expr1(x1), expr2(x2) {
        type = Type::max(expr1->type, expr2->type);
        if (type == nullptr) {
            error("type error");
        }
    }

    Expr* gen() override {
        return new Arith(op, expr1->reduce(), expr2->reduce());
    }

    std::string toString() override {
        return expr1->toString() + " " + op.toString() + " " + expr2->toString();
    }
};

//Unary Class
#include <iostream>
#include <string>
#include "Type.h"
#include "Expr.h"

using namespace std;

class Unary : public Expr {
public:
    Expr* expr;

    Unary(Token tok, Expr* x) : Expr(tok, nullptr), expr(x) {
        type = Type::max(Type::Int, expr->type);
        if (type == nullptr) error("type error");
    }

    Expr* gen() {
        return new Unary(op, expr->reduce());
    }

    string toString() override {
        return op->toString() + " " + expr->toString();
    }
};

// Temp class
#include <iostream>
#include <string>
#include "Type.h"
#include "Expr.h"

using namespace std;

class Temp : public Expr {
    static int count;
    int number;

public:
    Temp(Type* p) : Expr(Word::temp, p) {
        number = ++count;
    }

    string toString() override {
        return "t" + to_string(number);
    }
};

int Temp::count = 0;

