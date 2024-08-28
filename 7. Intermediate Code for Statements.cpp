#include <iostream>
#include <string>
using namespace std;

class Stmt {
public:
    static Stmt* Null;
    static Stmt* Enclosing;
    int after;

    Stmt() : after(0) {}
    virtual void gen(int b, int a) {
        // Placeholder for subclasses
    }
};

Stmt* Stmt::Null = new Stmt();
Stmt* Stmt::Enclosing = Stmt::Null;

class Expr {
public:
    virtual void jumping(int t, int f) = 0; // Generates jumping code for expressions
    static string type; // Placeholder for the type of expression
};

string Expr::type = "Bool"; // Assume boolean type for demonstration

class If : public Stmt {
    Expr* expr;
    Stmt* stmt;
public:
    If(Expr* x, Stmt* s) : expr(x), stmt(s) {
        if (expr->type != "Bool") {
            cerr << "boolean required in if" << endl;
        }
    }

    void gen(int b, int a) override {
        int label = newlabel(); // Label for the code for stmt
        expr->jumping(0, a); // Fall through on true, goto 'a' on false
        emitlabel(label);
        stmt->gen(label, a);
    }
};

class While : public Stmt {
    Expr* expr;
    Stmt* stmt;
public:
    While() : expr(nullptr), stmt(nullptr) {}

    void init(Expr* x, Stmt* s) {
        expr = x;
        stmt = s;
        if (expr->type != "Bool") {
            cerr << "boolean required in while" << endl;
        }
    }

    void gen(int b, int a) override {
        after = a; // Save label 'a'
        expr->jumping(0, a);
        int label = newlabel(); // Label for stmt
        emitlabel(label);
        stmt->gen(label, b);
        emit("goto L" + to_string(b)); // Jump to start of loop
    }
};

// Utility functions for label generation and emitting code
int newlabel() {
    static int label_count = 0;
    return ++label_count;
}

void emitlabel(int label) {
    cout << "L" << label << ":" << endl;
}

void emit(const string& code) {
    cout << code << endl;
}
