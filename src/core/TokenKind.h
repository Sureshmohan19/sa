//===--- TokenKind.h - Enum values for 'sa' Token Kinds ---------*- C++ -*-===//
//
// This file defines the sa::tok::TokenKind enum and support functions.
//
//===----------------------------------------------------------------------===//

#pragma once

namespace sa {
namespace tok {

    // Defines all the possible token kinds for the 'sa' language.
    enum TokenKind {
        #define TOK(X) X,
        #define PUNCTUATOR(X, Y) X,
        #define KEYWORD(X) kw_##X,
        #include "core/TokenKind.def"
        NUM_TOKENS
    };

    // Returns the spelling of a keyword, like "fn".
    const char* getKeywordSpelling(TokenKind kind);

    // Returns the spelling of a punctuator, like "->".
    const char* getPunctuatorSpelling(TokenKind kind);

    // Returns the internal name of a token, for debugging (e.g., "kw_fn").
    const char* getTokenName(TokenKind kind);

} // namespace tok
} // namespace sa