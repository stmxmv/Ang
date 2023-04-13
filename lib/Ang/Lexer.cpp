//
// Created by aojoie on 2/22/2023.
//

#include "Ang/Lexer.hpp"

void AN::KeywordFilter::addKeyword(std::string_view Keyword, AN::tok::TokenKind TokenCode) {
    HashTable.insert({Keyword, TokenCode});
}

void AN::KeywordFilter::initializeKeywords() {
#define KEYWORD(NAME, FLAGS) \
    addKeyword(#NAME, tok::kw_##NAME);
#include "Ang/TokenKinds.def"
}

namespace charinfo {

inline bool isASCII(char Ch) {
    return static_cast<unsigned char>(Ch) <= 127;
}

inline bool isVerticalWhitespace(char Ch) {
    return isASCII(Ch) && (Ch == '\r' || Ch == '\n');
}

inline bool isHorizontalWhitespace(char Ch) {
    return isASCII(Ch) && (Ch == ' ' || Ch == '\t' ||
                           Ch == '\f' || Ch == '\v');
}

inline bool isWhitespace(char Ch) {
    return isHorizontalWhitespace(Ch) ||
           isVerticalWhitespace(Ch);
}

inline bool isDigit(char Ch) {
    return isASCII(Ch) && Ch >= '0' && Ch <= '9';
}

inline bool isHexDigit(char Ch) {
    return isASCII(Ch) &&
           (isDigit(Ch) || (Ch >= 'A' && Ch <= 'F'));
}

inline bool isIdentifierHead(char Ch) {
    return isASCII(Ch) &&
           (Ch == '_' || (Ch >= 'A' && Ch <= 'Z') ||
            (Ch >= 'a' && Ch <= 'z'));
}

inline bool isIdentifierBody(char Ch) {
    return isIdentifierHead(Ch) || isDigit(Ch);
}

}// namespace charinfo

void AN::Lexer::next(AN::Token &result) {

start:
    /// skip all kinds of white space
    while (*curPtr && charinfo::isWhitespace(*curPtr)) {
        ++curPtr;
    }

    /// ignore comments
    if (*curPtr == '/') {
        if (*(curPtr + 1) == '/') {
            ignoreSingleLineComment();
            goto start;
        }
        if (*(curPtr + 1) == '*') {
            ignoreMultiLineComment();
            goto start;
        }
    }

    /// check if end of string
    if (!*curPtr) {
        result.setKind(tok::eof);
        return;
    }


    if (charinfo::isIdentifierHead(*curPtr)) {
        formIdentifier(result);
        return;
    }
    if (charinfo::isDigit(*curPtr)) {
        formNumber(result);
        return;
    }
    if (*curPtr == '\'') {
        formChar(result);
        return;
    }
    if (*curPtr == '"') {
        formString(result);
        return;
    }

    switch (*curPtr) {
#define CASE(ch, tok)                       \
    case ch:                                \
        formToken(result, curPtr + 1, tok); \
        return

        /// handle single character punctuator
        CASE('{', tok::left_curly_bracket);
        CASE('}', tok::right_curly_bracket);
        CASE('[', tok::left_square_bracket);
        CASE(']', tok::right_square_bracket);
        CASE('(', tok::left_bracket);
        CASE(')', tok::right_bracket);
        CASE(';', tok::semi);
        CASE('?', tok::question_mark);
        CASE('~', tok::bitwise_not);
        CASE(',', tok::comma);

#undef CASE

        /// handle multi character punctuator
        case '!':
            if (*(curPtr + 1) == '=') {
                formToken(result, curPtr + 2, tok::not_equal);
            } else {
                formToken(result, curPtr + 1, tok::logical_not);
            }
            return;
        case '#':
            if (*(curPtr + 1) == '#') {
                formToken(result, curPtr + 2, tok::double_hashtag);
            } else {
                formToken(result, curPtr + 1, tok::hashtag);
            }
            return;

        case ':':
            if (*(curPtr + 1) == ':') {
                formToken(result, curPtr + 2, tok::double_colon);
            } else {
                formToken(result, curPtr + 1, tok::colon);
            }
            return;

        case '.':
            if (*(curPtr + 1) == '.' && *(curPtr + 2) == '.') {
                formToken(result, curPtr + 3, tok::ellipsis);
            } else if (*(curPtr + 1) == '*') {
                formToken(result, curPtr + 2, tok::pointer_to_member1);
            } else {
                formToken(result, curPtr + 1, tok::period);
            }
            return;

        case '+':
            if (*(curPtr + 1) == '+') {
                formToken(result, curPtr + 2, tok::increment);
            } else if (*(curPtr + 1) == '=') {
                formToken(result, curPtr + 2, tok::plus_equal);
            } else {
                formToken(result, curPtr + 1, tok::plus);
            }

            return;

        case '-':
            if (*(curPtr + 1) == '-') {
                formToken(result, curPtr + 2, tok::decrement);
            } else if (*(curPtr + 1) == '=') {
                formToken(result, curPtr + 2, tok::minus_equal);
            } else if (*(curPtr + 1) == '>') {
                if (*(curPtr + 2) == '*') {
                    formToken(result, curPtr + 3, tok::pointer_to_member2);
                } else {
                    formToken(result, curPtr + 2, tok::member_access);
                }
            } else {
                formToken(result, curPtr + 1, tok::minus);
            }
            return;

        case '*':
            if (*(curPtr + 1) == '=') {
                formToken(result, curPtr + 2, tok::mul_equal);
            } else {
                formToken(result, curPtr + 1, tok::star);
            }
            return;

        case '/':
            if (*(curPtr + 1) == '=') {
                formToken(result, curPtr + 2, tok::division_equal);
            } else {
                formToken(result, curPtr + 1, tok::division);
            }
            return;

        case '%':
            if (*(curPtr + 1) == '=') {
                formToken(result, curPtr + 2, tok::mod_equal);
            } else {
                formToken(result, curPtr + 1, tok::mod);
            }
            return;

        case '^':
            if (*(curPtr + 1) == '=') {
                formToken(result, curPtr + 2, tok::xor_equal);
            } else {
                formToken(result, curPtr + 1, tok::bitwise_xor);
            }
            return;

        case '&':
            if (*(curPtr + 1) == '&') {
                formToken(result, curPtr + 2, tok::logical_and);
            } else if (*(curPtr + 1) == '=') {
                formToken(result, curPtr + 2, tok::and_equal);
            } else {
                formToken(result, curPtr + 1, tok::ampersand);
            }
            return;

        case '|':
            if (*(curPtr + 1) == '|') {
                formToken(result, curPtr + 2, tok::logical_or);
            } else if (*(curPtr + 1) == '=') {
                formToken(result, curPtr + 2, tok::or_equal);
            } else {
                formToken(result, curPtr + 1, tok::bitwise_or);
            }
            return;

        case '=':
            if (*(curPtr + 1) == '=') {
                formToken(result, curPtr + 2, tok::equal);
            } else {
                formToken(result, curPtr + 1, tok::assign);
            }
            return;

        case '<':
            if (*(curPtr + 1) == '<') {
                if (*(curPtr + 2) == '=') {
                    formToken(result, curPtr + 3, tok::left_shift_equal);
                } else {
                    formToken(result, curPtr + 2, tok::left_shift);
                }
            } else if (*(curPtr + 1) == '=') {
                formToken(result, curPtr + 2, tok::less_equal);
            } else {
                formToken(result, curPtr + 1, tok::less);
            }
            return;

        case '>':
            if (*(curPtr + 1) == '>') {
                if (*(curPtr + 2) == '=') {
                    formToken(result, curPtr + 3, tok::right_shift_equal);
                } else {
                    formToken(result, curPtr + 2, tok::right_shift);
                }
            } else if (*(curPtr + 1) == '=') {
                formToken(result, curPtr + 2, tok::greater_equal);
            } else {
                formToken(result, curPtr + 1, tok::greater);
            }
            return;

        default:
            formToken(result, curPtr + 1, tok::unknown);
            return;
    }
}


void AN::Lexer::formIdentifier(AN::Token &result) {
    const char *start = curPtr;
    const char *end   = curPtr + 1;
    while (charinfo::isIdentifierBody(*end)) {
        ++end;
    }

    std::string_view const identifier(start, end - start);

    formToken(result, end, keywordFilter.getKeyword(identifier, tok::identifier));
}

void AN::Lexer::formNumber(AN::Token &result) {
    const char *start = curPtr;
    const char *end   = curPtr + 1;

    bool point = false;

    // check hex start
    if (*start == '0' && *end == 'x') {
        ++end;
    }

    for (;;) {

        if (*end == '.') {
            if (!point) {
                point = true;
            } else {
                /// TODO illegal number report, double point
            }

        } else if (!charinfo::isDigit(*end)) {
            break;
        }

        ++end;
    }

    /// TODO check number literal suffix

    if (point) {
        formToken(result, end, tok::float_literal);
    } else {
        formToken(result, end, tok::integer_literal);
    }
}

void AN::Lexer::formChar(AN::Token &result) {
    const char *start = curPtr;
    const char *end   = curPtr + 1;

    while (*end && (*end != *start || *(end - 1) == '\\') && !charinfo::isVerticalWhitespace(*end)) {
        ++end;
    }

    /// TODO report error on *end is vertical whitespace

    /// *end = *start
    formToken(result, end + 1, tok::char_literal);
}

void AN::Lexer::formString(AN::Token &result) {
    const char *start = curPtr;
    const char *end   = curPtr + 1;

    while (*end && (*end != *start || *(end - 1) == '\\') && !charinfo::isVerticalWhitespace(*end)) {
        ++end;
    }
    /// TODO report error on *end is vertical whitespace

    /// *end = *start
    formToken(result, end + 1, tok::string_literal);
}

void AN::Lexer::ignoreSingleLineComment() {
    const char *end = curPtr + 2;
    while (*end && !charinfo::isVerticalWhitespace(*end)) {
        ++end;
    }
    curPtr = end;
}

void AN::Lexer::ignoreMultiLineComment() {
    const char *end = curPtr + 2;

    while (*end && (*end != '*' || *(end + 1) != '/')) {
        ++end;
    }

    if (*end == '*') {
        curPtr = end + 2;
        return;
    }

    // TODO report unterminated comment

    curPtr = end;
}


void AN::Lexer::formToken(AN::Token &Result, const char *TokEnd, AN::tok::TokenKind Kind) {
    size_t TokLen = TokEnd - curPtr;
    Result.ptr    = curPtr;

    Result.length = TokLen;
    Result.kind   = Kind;
    curPtr        = TokEnd;
}
