//===--- Lexer.cpp - The 'sa' Language Lexer --------------------*- C++ -*-===//
//
// This file implements the Lexer class.
//
//===----------------------------------------------------------------------===//

#include "frontend/Lexer.h"
#include "core/Token.h" // For tok::isKeyword
#include <unordered_map>

namespace sa {

// A map to look up keywords. This is the implementation detail of keyword recognition.
static const std::unordered_map<std::string_view, tok::TokenKind> KeywordMap = {
    {"fn",   tok::kw_fn},
    {"let",  tok::kw_let},
    {"void", tok::kw_void},
};

Lexer::Lexer(std::string_view source) : source(source) {}

Token Lexer::scanNextToken() {
    skipWhitespaceAndComments();

    start = current;

    if (isAtEnd()) {
        return makeToken(tok::eof);
    }

    char c = advance();

    // Check for identifiers and keywords
    if (isalpha(c) || c == '_') {
        return scanIdentifierOrKeyword();
    }

    // Check for string literals
    if (c == '"') {
        return scanStringLiteral();
    }

    // This is our main dispatcher.
    switch (c) {
        case '(': return makeToken(tok::l_paren);
        case ')': return makeToken(tok::r_paren);
        case '{': return makeToken(tok::l_brace);
        case '}': return makeToken(tok::r_brace);
        case ';': return makeToken(tok::semicolon);
        case '=': return makeToken(tok::equal);
        case ':': return makeToken(tok::colon);
        case '-':
            if (match('>')) {
                return makeToken(tok::arrow);
            }
            break; // We would handle 'minus' here in the future.
    }

    return makeErrorToken("Unexpected character.");
}

// --- Private Helper Methods ---

bool Lexer::isAtEnd() const {
    return current >= source.length();
}

char Lexer::advance() {
    return source[current++];
}

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return source[current];
}

char Lexer::peekNext() const {
    if (current + 1 >= source.length()) return '\0';
    return source[current + 1];
}

bool Lexer::match(char expected) {
    if (isAtEnd()) return false;
    if (source[current] != expected) return false;
    current++;
    return true;
}

Token Lexer::makeToken(tok::TokenKind kind) const {
    std::string_view lexeme = source.substr(start, current - start);
    return Token{kind, lexeme, line};
}

Token Lexer::makeErrorToken(const char* message) const {
    return Token{tok::unknown, message, line};
}

void Lexer::skipWhitespaceAndComments() {
    while (true) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                line++;
                advance();
                break;
            case '/':
                if (peekNext() == '/') {
                    // It's a line comment. Skip to the end of the line.
                    while (peek() != '\n' && !isAtEnd()) {
                        advance();
                    }
                } else {
                    // It's just a slash, not a comment.
                    return;
                }
                break;
            default:
                return;
        }
    }
}

Token Lexer::scanIdentifierOrKeyword() {
    while (isalnum(peek()) || peek() == '_') {
        advance();
    }

    std::string_view text = source.substr(start, current - start);
    
    // Check if the identifier is a keyword.
    if (KeywordMap.count(text)) {
        return makeToken(KeywordMap.at(text));
    }

    return makeToken(tok::identifier);
}

Token Lexer::scanStringLiteral() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') line++;
        advance();
    }

    if (isAtEnd()) {
        return makeErrorToken("Unterminated string.");
    }

    // Consume the closing quote.
    advance();
    return makeToken(tok::string_literal);
}

} // namespace sa