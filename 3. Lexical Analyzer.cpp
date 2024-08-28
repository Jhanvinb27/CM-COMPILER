// File Tag.h
#pragma once

class Tag {
public:
    static const int AND = 256, BASIC = 257, BREAK = 258, DO = 259, ELSE = 260, 
                     EQ = 261, FALSE = 262, GE = 263, ID = 264, IF = 265, 
                     INDEX = 266, LE = 267, MINUS = 268, NE = 269, NUM = 270, 
                     OR = 271, REAL = 272, TEMP = 273, TRUE = 274, WHILE = 275;
};
// File Token.h
#pragma once

#include <string>

class Token {
public:
    const int tag;
    Token(int t) : tag(t) {}
    virtual std::string toString() const {
        return std::string(1, (char)tag);
    }
};
// File Num.h
#pragma once

#include "Token.h"

class Num : public Token {
public:
    const int value;
    Num(int v) : Token(Tag::NUM), value(v) {}
    std::string toString() const override {
        return std::to_string(value);
    }
};
// File Word.h
#pragma once

#include "Token.h"

class Word : public Token {
public:
    std::string lexeme;
    Word(const std::string &s, int tag) : Token(tag), lexeme(s) {}

    std::string toString() const override {
        return lexeme;
    }

    static const Word 
        and, or, eq, ne, le, ge, 
        minus, True, False, temp;
};

// Definition of static members
const Word Word::and("&&", Tag::AND);
const Word Word::or("||", Tag::OR);
const Word Word::eq("==", Tag::EQ);
const Word Word::ne("!=", Tag::NE);
const Word Word::le("<=", Tag::LE);
const Word Word::ge(">=", Tag::GE);
const Word Word::minus("minus", Tag::MINUS);
const Word Word::True("true", Tag::TRUE);
const Word Word::False("false", Tag::FALSE);
const Word Word::temp("t", Tag::TEMP);
// File Lexer.h
#pragma once

#include <iostream>
#include <unordered_map>
#include "Token.h"
#include "Word.h"
#include "Num.h"

using namespace std;

class Lexer {
public:
    static int line;
    char peek = ' ';
    unordered_map<string, Word> words;

    void reserve(const Word &w) {
        words[w.lexeme] = w;
    }

    Lexer() {
        reserve(Word("if", Tag::IF));
        reserve(Word("else", Tag::ELSE));
        reserve(Word("while", Tag::WHILE));
        reserve(Word("do", Tag::DO));
        reserve(Word("break", Tag::BREAK));
        reserve(Word::True);
        reserve(Word::False);
        reserve(Word("int", Tag::BASIC));
        reserve(Word("char", Tag::BASIC));
        reserve(Word("bool", Tag::BASIC));
        reserve(Word("float", Tag::BASIC));
    }

    void readch() {
        peek = cin.get();
    }

    bool readch(char c) {
        readch();
        if (peek != c) return false;
        peek = ' ';
        return true;
    }

    Token scan() {
        // Skip whitespace
        for (;; readch()) {
            if (peek == ' ' || peek == '\t') continue;
            else if (peek == '\n') line++;
            else break;
        }

        // Recognize composite tokens
        switch (peek) {
            case '&': if (readch('&')) return Word::and; else return Token('&');
            case '|': if (readch('|')) return Word::or; else return Token('|');
            case '=': if (readch('=')) return Word::eq; else return Token('=');
            case '!': if (readch('=')) return Word::ne; else return Token('!');
            case '<': if (readch('=')) return Word::le; else return Token('<');
            case '>': if (readch('=')) return Word::ge; else return Token('>');
        }

        // Recognize numbers
        if (isdigit(peek)) {
            int v = 0;
            do {
                v = 10 * v + peek - '0';
                readch();
            } while (isdigit(peek));

            if (peek != '.') return Num(v);
            float x = v, d = 10;
            while (isdigit(readch())) {
                x += (peek - '0') / d;
                d *= 10;
            }
            return Real(x);
        }

        // Recognize identifiers or keywords
        if (isalpha(peek)) {
            string lexeme;
            do {
                lexeme += peek;
                readch();
            } while (isalnum(peek));

            if (words.find(lexeme) != words.end()) return words[lexeme];
            Word w(lexeme, Tag::ID);
            words[lexeme] = w;
            return w;
        }

        // Return any remaining characters as tokens
        Token tok(peek);
        peek = ' ';
        return tok;
    }
};

int Lexer::line = 1;
