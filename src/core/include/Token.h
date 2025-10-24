//===--- Token.h - The 'sa' Language Token Interface ------------*- C++ -*-===//
//
// This file defines the Token interface for the 'sa' language.
//
//===----------------------------------------------------------------------===//

#pragma once

#include "core/include/TokenKind.h"
#include <string_view>
#include <string>

namespace sa {

    // Represents a single token from the source code. It contains the kind
    // of token, its text (lexeme), and its location for error reporting.
    struct Token {
        // The kind of the token (e.g., kw_fn, identifier).
        tok::TokenKind kind = tok::unknown;

        // A view of the token's text in the source buffer.
        // This avoids memory copies and is very efficient.
        std::string_view lexeme;

        // Location information for good error messages.
        unsigned int line = 0;
    };

} // namespace sa