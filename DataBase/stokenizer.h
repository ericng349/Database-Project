#ifndef STOKENIZER_H
#define STOKENIZER_H

#include "token.h"
#include "adjacency_matrix.h"

const static unsigned int MAX_BUFFER = 500;

class STokenizer
{
public:
    STokenizer();
    STokenizer(char str[]);
    bool done();            //true: there are no more tokens
    bool fail();            //true: there are more tokens
    //---------------
    //extract one token (very similar to the way cin >> works)
    friend STokenizer& operator >> (STokenizer& s, Token& t);

    //set a new string as the input string
    void set_string(char str[]);

private:
    //create table for all the tokens we will recognize
    //                      (e.g. doubles, words, etc.)
    void make_table(int _table[][MAX_COLUMNS]);

    //extract the longest string that match
    //     one of the acceptable token types
    bool get_token(int start_state, string& token);
    //---------------------------------
    char _buffer[MAX_BUFFER];       //input string
    unsigned int _pos;              //current position in the string
    int _last_success_state;     //current state of the machine
    static int _table[MAX_ROWS][MAX_COLUMNS];
    int punctuation_counter;       //makes sure there is only 1 punctuation
};

#endif // STOKENIZER_H
