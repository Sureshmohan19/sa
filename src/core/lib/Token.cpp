#include "core/include/Token.h"

namespace sa {
namespace tok {

// This is the implementation for the getTokenName function declared in Token.h.
// Its job is to return a human-readable string for each token type,
// which is incredibly useful for debugging our lexer.
const char* getTokenName(TokenKind kind) {
    switch (kind) {
        case unknown: return "unknown";
        case eof: return "eof";
        case identifier: return "identifier";
        case string_literal: return "string_literal";
        case l_paren: return "l_paren";
        case r_paren: return "r_paren";
        case l_brace: return "l_brace";
        case r_brace: return "r_brace";
        case semicolon: return "semicolon";
        case equal: return "equal";
        case arrow: return "arrow";
        case colon: return "colon";
        case kw_fn: return "kw_fn";
        case kw_let: return "kw_let";
        case kw_void: return "kw_void";
        default: return "unnamed_token";
    }
}

} // namespace tok
} // namespace sa