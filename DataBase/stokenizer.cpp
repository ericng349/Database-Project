#include "stokenizer.h"
#include <cstring>
#include <ctype.h>

int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];

/**
 * @brief STokenizer::STokenizer
 * task: constructor, initializes the table as well as the variables
 */
STokenizer::STokenizer()
{
    make_table(_table);
    _pos = 0;                  //current position in the string
    _last_success_state = 0;
    punctuation_counter = 0;
}

/**
 * @brief STokenizer::STokenizer
 * @param str - the string to place in buffer
 * task: constructor with 1 paramater, does the same thing, but initializes
 * the _buffer with str parameter
 */
STokenizer::STokenizer(char str[])
{
    make_table(_table);
    strcpy(_buffer,str);      //input string
    _pos = 0;            //current position in the string
    _last_success_state = 0;
    punctuation_counter = 0;
}

/**
 * @brief STokenizer::set_string
 * @param str - the place to place in buffer
 * task: resets all the variables as well as setting the buffer to the
 * string
 */
void STokenizer::set_string(char str[])
{
    strcpy(_buffer,str);      //input string
    _pos = 0;            //current position in the string
    _last_success_state = 0;
    punctuation_counter = 0;
}

/**
 * @brief STokenizer::done
 * @return whether the position is past the length of the buffer;
 */
bool STokenizer::done()
{
    return _pos > strlen(_buffer);
}

/**
 * @brief STokenizer::more
 * @return  - true or false depending if the position is past the length
 * of _buffer
 */
bool STokenizer::fail()
{
    return _pos <= strlen(_buffer);
}

/**
 * @brief STokenizer::make_table
 * @param _table - the table to create
 * task: takes in the table and sets the machines for the table
 */
void STokenizer::make_table(int _table[][MAX_COLUMNS])
{
    const char DIGITS[] = "0123456789";
    const char ALPHA[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdef"
                         "ghijklmnopqrstuvwxyz";
    const char SPACES[] = " \n\t";
    const char PUNCTUATIONS[] = ".?,!\\:";

    const char QUOTES[] = "\"";

    const char RELATIONAL_OPERATORS[] = "<>=";


    init_table(_table);

    mark_fail(_table, 0);

    //State 0 will be a general state that can determine which machine
    //afterwards
    mark_cells(0, _table, DIGITS, 1);    //state [0] --- DIGITS ---> [1]
    mark_cells(0, _table, ALPHA, 4);    //state [0] --- ALPHA ---> [4]
    mark_cells(0, _table, SPACES, 7);    //state [0] --- SPACES ---> [8]
    mark_cells(0, _table, PUNCTUATIONS, 8); //state [0] --- PUNCTUATIONS
                                            //---> [9]
    mark_cells(0, _table, QUOTES, 9); //state [0] --- QUOTES //---> [10]

    mark_cells(0, _table, RELATIONAL_OPERATORS, 11); //state [0] ---
                                                     //RELATIONAL_OPERATORS
                                                     //---> [12]

    //====================== Number Machine ================================
    mark_success(_table, 1);         //Mark states 1 and 3 as success states
    mark_fail(_table, 2);
    mark_success(_table, 3);

    mark_cells(1, _table, DIGITS, 1);    //state [1] --- DIGITS ---> [1]
    mark_cells(1, _table, '.', '.', 2);  //state [1] --- '.' ------> [2]

    mark_cells(2, _table, DIGITS, 3);    //state [2] --- DIGITS ---> [3]
    mark_cells(3, _table, DIGITS, 3);    //state [3] --- DIGITS ---> [3]

    //====================== Alphabet Machine ================================
    //Alphabets:
    mark_success(_table, 4);         //Mark states 4 as success states
    mark_fail(_table, 5);
    mark_success(_table, 6);

    mark_cells(4, _table, ALPHA, 4);    //state [4] --- ALPHA ---> [4]
    mark_cells(4, _table, '\'','\'', 5);//state [5] --- ''' ------> [6]

    mark_cells(5, _table, ALPHA, 6);    //state [6] --- ALPHA ---> [7]
    mark_cells(6, _table, ALPHA, 6);    //state [7] --- ALPHA ---> [7]

    //====================== Space Machine ================================
    //SPACES:
    mark_success(_table, 7);         //Mark states 8 as success states

    mark_cells(7, _table, SPACES, 7);    //state [8] --- SPACES ---> [8]

    //====================== Punctuation Machine ==========================
    //PUNCTUATIONS:
    //NOTE: Since we don't want multiple punctuations to be counted, if it
    //goes from state 0 to 9 that is the only success state

    mark_success(_table, 8);         //Mark states 9 as success states

    //====================== Quotation machine =====================
    mark_fail(_table, 9);         //Mark states 1 and 3 as success states
    mark_success(_table, 10);

    mark_cells(9, _table, ALPHA, 9);    //state [10] --- QUOTES ---> [10]
    mark_cells(9, _table, SPACES, 9);    //state [10] --- QUOTES ---> [10]
    mark_cells(9, _table, '.', '.', 9);  //state [1] --- '.' ------> [2]
    mark_cells(9, _table, DIGITS, 9);   //state [1] --- '.' ------> [2]
    mark_cells(9, _table, QUOTES, 10);    //state [10] --- QUOTES ---> [10]

    //====================== Relational Operators Machine ==================
    mark_fail(_table, 11);
    mark_success(_table,12);
    mark_success(_table, 13);

    mark_cells(11, _table, RELATIONAL_OPERATORS, 12);    //state [10] --- QUOTES ---> [10]
    mark_cells(12, _table, "=", 13);    //state [10] --- QUOTES ---> [10]

}

/**
 * @brief STokenizer::get_token
 * @param start_state - the state that the machine will start on (row)
 * @param token - the token that is found
 * @return - true or false based on whether a token was found
 */
bool STokenizer::get_token(int start_state, string &token)
{
    int current_state = start_state;
    int current_position = _pos;



    //Since we aren't using first column everything is shifted by 1
    int current_char = int(_buffer[current_position]) + 1;

    int last_pos = _pos;    //Saves the initial position before moving

    int next_state = _table[current_state][current_char]; //The state that
                                        //the current character will go to

    while(next_state != -1 && current_char != 0)
    {
        //A success would mean the first column in the next state is not 0
        if(is_success(_table,next_state))
        {
            //The position index goes up, and anything before it is valid
            _pos = current_position;
            _last_success_state = next_state;
        }

        //Moving through the sentence
        current_position++;

        //Again the current character will be +1 since the 1'st column is
        //used for something else
        current_char = int(_buffer[current_position]) + 1;

        //Simulate going to the next character in the _buffer
        current_state = next_state;
        next_state = _table[current_state][current_char];


    }

    //Goes to the next position
    _pos++;

    //Adds the token found to the string, if it's not empty
    if(strlen(_buffer) > 0)
    {
        //If it ends on  a quotation state, it means that we add to buffer
        //everything except the quotations which are in the first and last
        //spots
        if(current_state == 10)
        {
            for(unsigned int i = last_pos + 1;i < _pos - 1;i++)
            {
                token += _buffer[i];
            }
        }
        else
        {
            for(unsigned int i = last_pos;i < _pos;i++)
            {
                token += _buffer[i];
            }
        }
    }

    //If you input a empty string, then there can be no tokens found
    //therefore will be false, otherise if at least 1 character is found
    //it will return true
    return token.length() > 0;

}

/**
 * @brief operator >>
 * @param s - the STokenizer object
 * @param t - the Token object
 * @return - returns the STokenizer object, after it has set a token
 * if it has found one
 */
STokenizer& operator >> (STokenizer& s, Token& t)
{
    string str = t.token_str();

    //======== The numbers that represent the words ==========
    //Ex. 0 = STRING, 1 = ALPHA
    const int STR_NUMBER = 0;
    const int STR_ALPHA = 1;
    const int STR_SPACE = 2;
    const int STR_PUNC = 3;
    const int STR_QUOTE = 4;
    const int STR_REATIONAL = 5;

    const int STR_UNKNOWN = 6;


    const int START_STATE = 0;  //The start state for all the machines

    /*
     * NUMBER Success states: 1,3
     * ALPHA  Success states: 4,7
     * SPACE  Success states: 8
     * PUNCTUATION Success states: 9
     * QUOTES Sucess States: 10
     * UNKNOWN: any other state
    */
    if(s.get_token(START_STATE,str))
    {
        //_last_success_state will end on either a successful state, or a
        //non successful state, so depending on where it lands that is how it
        //will know which machine it is using
        if(s._last_success_state == 1 || s._last_success_state == 3)
        {
            Token temp(str,STR_NUMBER);
            t = temp;
            s.punctuation_counter = 0;
            s._last_success_state = 0;
        }
        else if(s._last_success_state == 4 || s._last_success_state == 6)
        {
            Token temp(str,STR_ALPHA);
            t = temp;
            s.punctuation_counter = 0;
            s._last_success_state = 0;
        }
        else if(s._last_success_state == 7)
        {
            Token temp(str,STR_SPACE);
            t = temp;
            s.punctuation_counter = 0;
            s._last_success_state = 0;
        }
        else if(s.punctuation_counter == 0 && s._last_success_state == 8)
        {
            if(s._last_success_state == 8)
            {
                Token temp(str,STR_PUNC);
                t = temp;
                s.punctuation_counter++;
                s._last_success_state = 0;
            }
        }
        else if(s._last_success_state == 10)
        {

                Token temp(str,STR_QUOTE);
                t = temp;
                s.punctuation_counter++;
                s._last_success_state = 0;
        }
        else if(s._last_success_state == 12 || s._last_success_state == 13)
        {

                Token temp(str,STR_REATIONAL);
                t = temp;
                s.punctuation_counter++;
                s._last_success_state = 0;
        }
        else
        {
            Token temp(str,STR_UNKNOWN);
            t = temp;
            s._last_success_state = 0;
        }
    }
    //If get token is false it means no tokens could be found
    else
    {
        cout<<"NO TOKENS FOUND!"<<endl;
    }

    //Returning the object
    return s;

}


