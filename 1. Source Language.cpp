#include <iostream>
#include <vector>
#include <string>

using namespace std;

enum TypeEnum { BASIC, ARRAY };

class Type {
public:
    string name;
    Type(string name) : name(name) {}
};

class Symbol {
public:
    string id;
    Type* type;
    Symbol(string id, Type* type) : id(id), type(type) {}
};

class Block {
public:
    vector<Symbol*> decls;
    vector<string> stmts;

    Block(vector<Symbol*> decls, vector<string> stmts) 
        : decls(decls), stmts(stmts) {}

    void addDecl(Symbol* symbol) {
        decls.push_back(symbol);
    }

    void addStmt(string stmt) {
        stmts.push_back(stmt);
    }

    void print() {
        cout << "{\n";
        for (auto& decl : decls) {
            cout << "  " << decl->type->name << " " << decl->id << ";\n";
        }
        for (auto& stmt : stmts) {
            cout << "  " << stmt << "\n";
        }
        cout << "}\n";
    }
};

int main() {
    Type basicType("int");
    Symbol* sym = new Symbol("x", &basicType);

    Block* block = new Block({}, {});
    block->addDecl(sym);
    block->addStmt("x = 5;");

    block->print();

    delete sym;
    delete block;

    return 0;
}




