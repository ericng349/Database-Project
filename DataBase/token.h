#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>

using namespace std;

class Token
{
public:
    Token();    //Creates a empty token
    Token(string str, int type);    //Initializes a token with specific
                                    //values
    friend ostream& operator <<(ostream& outs, const Token& t);

    int type();           //returns the token type
    string type_string(); //returns the token's type in words
    string token_str();   //returns the token found
private:
    string _token;
    int _type;
};

#endif // TOKEN_H
