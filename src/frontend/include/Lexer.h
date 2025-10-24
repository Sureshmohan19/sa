//===--- Lexer.h - The 'sa' Language Lexer --------------------*- C++ -*-===//
//
// This file defines the Lexer class, which is responsible for turning a
// stream of characters into a stream of tokens.
//
//===----------------------------------------------------------------------===//

#pragma once

#include "core/include/Token.h"
#include <string_view>

namespace sa {

class Lexer {
public:
    // Constructor: Initializes the lexer with the source code.
    Lexer(std::string_view source);

    // The main entry point for the lexer. Scans and returns the next token.
    Token scanNextToken();

private:
    // Advances the current position and returns the character that was consumed.
    char advance();

    // Peeks at the character at the current position without consuming it.
    char peek() const;

    // Peeks at the character *after* the current position.
    char peekNext() const;

    // Checks if the current character matches the expected one. If so, consumes it.
    bool match(char expected);

    // Checks if we have reached the end of the source file.
    bool isAtEnd() const;

    // Helper functions to create a token of a specific type.
    Token makeToken(tok::TokenKind kind) const;
    Token makeErrorToken(const char* message) const;

    // Helper functions for scanning specific token types.
    Token scanStringLiteral();
    Token scanIdentifierOrKeyword();

    // Helper to skip over characters that are not part of tokens.
    void skipWhitespaceAndComments();

    std::string_view source; // The full source code text.
    unsigned int start = 0;      // Start of the current lexeme being scanned.
    unsigned int current = 0;    // Current character we are looking at.
    unsigned int line = 1;       // Current line number for error reporting.
};

} // namespace sa