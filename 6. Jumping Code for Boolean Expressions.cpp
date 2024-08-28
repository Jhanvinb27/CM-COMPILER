#include <iostream>
#include <string>
#include <memory>

using namespace std;

class Expr {
public:
    virtual void jumping(int t, int f) = 0;
    void emit(const string &s) {
        cout << s << endl;
    }
    virtual ~Expr() = default;
};

class Constant : public Expr {
public:
    bool value;

    Constant(bool v) : value(v) {}

    static shared_ptr<Constant> True;
    static shared_ptr<Constant> False;

    void jumping(int t, int f) override {
        if (value && t != 0) {
            emit("goto L" + to_string(t));
        } else if (!value && f != 0) {
            emit("goto L" + to_string(f));
        }
    }
};

shared_ptr<Constant> Constant::True = make_shared<Constant>(true);
shared_ptr<Constant> Constant::False = make_shared<Constant>(false);

class Logical : public Expr {
protected:
    shared_ptr<Expr> expr1, expr2;

public:
    Logical(shared_ptr<Expr> x1, shared_ptr<Expr> x2) : expr1(move(x1)), expr2(move(x2)) {}

    virtual ~Logical() = default;
};

class Or : public Logical {
public:
    Or(shared_ptr<Expr> x1, shared_ptr<Expr> x2) : Logical(move(x1), move(x2)) {}

    void jumping(int t, int f) override {
        int label = t != 0 ? t : newlabel();
        expr1->jumping(label, 0);
        expr2->jumping(t, f);
        if (t == 0) emitlabel(label);
    }

private:
    int newlabel() {
        static int labels = 0;
        return ++labels;
    }

    void emitlabel(int label) {
        emit("L" + to_string(label) + ":");
    }
};

class And : public Logical {
public:
    And(shared_ptr<Expr> x1, shared_ptr<Expr> x2) : Logical(move(x1), move(x2)) {}

    void jumping(int t, int f) override {
        int label = f != 0 ? f : newlabel();
        expr1->jumping(0, label);
        expr2->jumping(t, f);
        if (f == 0) emitlabel(label);
    }

private:
    int newlabel() {
        static int labels = 0;
        return ++labels;
    }

    void emitlabel(int label) {
        emit("L" + to_string(label) + ":");
    }
};

class Not : public Logical {
public:
    Not(shared_ptr<Expr> x2) : Logical(move(x2), move(x2)) {}

    void jumping(int t, int f) override {
        expr1->jumping(f, t);
    }
};
