// File Main.cpp
#include <iostream>
#include "lexer.h"
#include "parser.h"

using namespace std;

int main() {
    try {
        Lexer lex;
        Parser parse(lex);
        parse.program();
        cout << endl;
    } catch (const exception &e) {
        cerr << "Exception: " << e.what() << endl;
    }
    return 0;
}
