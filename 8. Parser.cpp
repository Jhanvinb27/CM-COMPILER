#include <iostream>
#include <stdexcept>
#include "lexer.h"
#include "symbols.h"
#include "inter.h"

using namespace std;

class Parser {
private:
    Lexer* lex;
    Token* look;
    Env* top = nullptr;
    int used = 0;

public:
    Parser(Lexer* l) {
        lex = l;
        move();
    }

    void move() {
        look = lex->scan();
    }

    void error(string s) {
        throw runtime_error("near line " + to_string(lex->line) + ": " + s);
    }

    void match(int t) {
        if (look->tag == t) {
            move();
        } else {
            error("syntax error");
        }
    }

    void program() {
        Stmt* s = block();
        int begin = s->newlabel();
        int after = s->newlabel();
        s->emitlabel(begin);
        s->gen(begin, after);
        s->emitlabel(after);
    }

    Stmt* block() {
        match('{');
        Env* savedEnv = top;
        top = new Env(top);
        decls();
        Stmt* s = stmts();
        match('}');
        top = savedEnv;
        return s;
    }

    void decls() {
        while (look->tag == Tag::BASIC) {
            Type* p = type();
            Token* tok = look;
            match(Tag::ID);
            match(';');
            Id* id = new Id(static_cast<Word*>(tok), p, used);
            top->put(tok, id);
            used = used + p->width;
        }
    }

    Type* type() {
        Type* p = static_cast<Type*>(look);
        match(Tag::BASIC);
        if (look->tag != '[') return p;
        else return dims(p);
    }

    Type* dims(Type* p) {
        match('[');
        Token* tok = look;
        match(Tag::NUM);
        match(']');
        if (look->tag == '[')
            p = dims(p);
        return new Array(static_cast<Num*>(tok)->value, p);
    }

    // Other methods like stmt(), stmts(), expr(), etc., would be implemented similarly
};
