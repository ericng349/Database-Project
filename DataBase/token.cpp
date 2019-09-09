#include "token.h"

/**
 * @brief Token::Token
 * task: constructor, initializes the token and type
 */
Token::Token()
{
    _token = "";
    //The default type of the token
    _type = 0;
}

/**
 * @brief Token::Token
 * @param str - the token
 * @param type - the token type
 * task: constructs a token object, but initializes the values of token and
 * type
 */
Token::Token(string str, int type)
{
    _token = str;
    _type = type;
}

ostream& operator <<(ostream& outs, const Token& t)
{
    return outs<<"|" << t._token << "|" << endl;
}

/**
 * @brief Token::type
 * @return - the type in number value
 */
int Token::type()
{
    return _type;
}

/**
 * @brief Token::type_string
 * @return - the token type in words
 */
string Token::type_string()
{
    switch(_type)
    {
        case 0: return "NUMBER";
                break;
        case 1: return "ALPHA";
                break;
        case 2: return "SPACE";
                break;
        case 3: return "PUNCTUATION";
                break;
        case 4: return "QUOTE";
                break;
        case 5: return "RELATIONAL";
                break;
        default: return "UNKNOWN";
    }
}

/**
 * @brief Token::token_str
 * @return - the token found
 */
string Token::token_str()
{
    return _token;
}
