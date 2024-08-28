#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;

class Token {
    // Implementation of Token class (placeholder)
};

class Id {
    // Implementation of Id class (placeholder)
};

class Env {
private:
    unordered_map<Token, Id*> table;
    Env* prev;
    
public:
    Env(Env* n) : prev(n) {}
    
    void put(Token w, Id* i) {
        table[w] = i;
    }
    
    Id* get(Token w) {
        for (Env* e = this; e != nullptr; e = e->prev) {
            auto found = e->table.find(w);
            if (found != e->table.end()) {
                return found->second;
            }
        }
        return nullptr;
    }
};

class Type {
public:
    string lexeme;
    int tag;
    int width;

    Type(string s, int t, int w) : lexeme(s), tag(t), width(w) {}

    static Type Int, Float, Char, Bool;

    static bool numeric(const Type& p) {
        return p == Type::Char || p == Type::Int || p == Type::Float;
    }

    static Type max(const Type& p1, const Type& p2) {
        if (!numeric(p1) || !numeric(p2)) return Type("", -1, 0);
        if (p1 == Type::Float || p2 == Type::Float) return Type::Float;
        if (p1 == Type::Int || p2 == Type::Int) return Type::Int;
        return Type::Char;
    }
};

Type Type::Int = Type("int", 1, 4);
Type Type::Float = Type("float", 1, 8);
Type Type::Char = Type("char", 1, 1);
Type Type::Bool = Type("bool", 1, 1);

class Array : public Type {
public:
    Type of;
    int size;

    Array(int sz, Type p) : Type("[]", 2, sz * p.width), size(sz), of(p) {}

    string toString() const {
        return "[" + to_string(size) + "] " + of.lexeme;
    }
};

