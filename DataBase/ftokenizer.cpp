#include "ftokenizer.h"

/**
 * @brief FTokenizer::FTokenizer
 * @param fname - file name to be read
 *
 * task: this function constructs a file tokenizer and initializes
 *       the file as well as reads in a block
 */
FTokenizer::FTokenizer(char* fname)
{
    _f.open(fname);
    _pos = 0;
    _blockPos = 0;
    _fail = true;

    //Read in a 500 byte block
    _f.read(_block,MAX_BLOCK);
    _block[MAX_BLOCK - 1] = '\0';

    //Keeps track of position in the file
    _pos += strlen(_block);

    //Set the 500 block into STokenizer
    _stk.set_string(_block);

}

/**
 * @brief FTokenizer::next_token
 * @return - a token if it is able to find one, if not then it will try
 *           to get 500 more words, and if that doesnt work then there
 *           are no more tokens
 */
Token FTokenizer::next_token()
{
    Token t; //The token found

    //checks if STokenizer has run out of words
    if(_stk.done())
    {
        //Gets a new 500 set of characters from the text file if it exists
        if(!get_new_block())
        {
            _fail = false;
            t = Token();
        }
    }
     _stk>>t;

    return t;
}

/**
 * @brief FTokenizer::more
 * @return  - whether or not there are any more text in the file
 */
bool FTokenizer::more()
{
    return _fail;
}

/**
 * @brief FTokenizer::pos
 * @return - the position in the file
 */
int FTokenizer::pos()
{
    return _pos;
}

/**
 * @brief FTokenizer::block_pos
 * @return - the position in the block that was read in
 */
int FTokenizer::block_pos()
{
    return _blockPos;
}

/**
 * @brief operator >>
 * @param f - FTokenizer object
 * @param t - the token found
 * @return the FTokenizer object
 */
FTokenizer& operator >> (FTokenizer& f, Token& t)
{

    //Gets the token found
    t = f.next_token();

    return f;
}

/**
 * Helper function
 * task: reads in 500 more words from the file if there are
 *       and sets it into the STokenizer.
 */
bool FTokenizer::get_new_block()
{
    //Reads in a block of 500 characters
    _f.read(_block,MAX_BLOCK);

    //Checks if it found anything
    if(_f.gcount() == 0)
    {
        _f.close();
        return false;
    }
    //Read does not append a null character at the end so this does it for it
    _block[_f.gcount()  - 1] = '\0';

    //Keeps track of position in the file
    _pos += strlen(_block);

    //Sets the string into STokenizser
    _stk.set_string(_block);

    return true;
}
