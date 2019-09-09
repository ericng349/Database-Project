#include "adjacency_matrix.h"

#include<iostream>

using namespace std;

/*
    Note: Since we're using all of column 0 as fail/success everything
          should be moved over by 1
 */
void init_table(int _table[][MAX_COLUMNS])
{
    for(unsigned int i = 0; i < MAX_ROWS; i++)
    {
        for(unsigned int j = 0; j < MAX_COLUMNS; j++)
        {
            _table[i][j] = -1;
        }
    }
}
/**
 * @brief mark_success
 * @param _table - the table
 * @param state - the row to mark
 * task: marks the first column of a specific row(the state) with a 1 to
 * represent success
 */
void mark_success(int _table[][MAX_COLUMNS], int state)
{
    _table[state][0] = 1;
}

/**
 * @brief mark_fail
 * @param _table - the table
 * @param state - the row to mark
 * task: marks the first column of a specific row(the state) with a 0 to
 * represent success
 */
void mark_fail(int _table[][MAX_COLUMNS], int state)
{
    _table[state][0] = 0;
}

/**
 * @brief is_success
 * @param _table - the table
 * @param state - the row to check
 * @return - true or false based on if the state is successful
 */
bool is_success(int _table[][MAX_COLUMNS], int state)
{
        return _table[state][0] == 1;
}

/**
 * @brief mark_cells
 * @param row - the row to mark the cell
 * @param _table - the table
 * @param from - the location of the character
 * @param to - the destination of the character
 * @param state - the state the characters are going to go to
 */
void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to, int state)
{
    for(int i = from; i <= to; i++)
    {
        _table[row][i + 1] = state;
    }
}

/**
 * @brief mark_cells
 * @param row - the row to mark the cell
 * @param _table - the table
 * @param columns - the characters to find
 * @param state - the state the characters are going to go to
 */
void mark_cells(int row, int _table[][MAX_COLUMNS], const char columns[], int state)
{
    for(int i = 0; i <strlen(columns); i++)
    {
        _table[row][columns[i] + 1] = state;
    }
}

/**
 * @brief mark_cell
 * @param row - the row to mark
 * @param table - the table
 * @param column - the column to mark
 * @param state - the state the cell is going to be in
 */
void mark_cell(int row, int table[][MAX_COLUMNS], int column, int state)
{
    table[row][column] = state;
}

/**
 * @brief print_table
 * @param _table - the table
 * task: prints the entire table
 */
void print_table(int _table[][MAX_COLUMNS])
{
    for(int i = 0; i < MAX_ROWS; i++)
    {
        cout<<i<<" " << endl;
        for(int j = 0; j < MAX_COLUMNS; j++)
        {
            cout<< _table[i][j];
        }
        cout<<endl;
    }
}

/**
 * @brief show_string
 * @param s - the string to look at
 * @param _pos - the current position in the string
 * task: displays the string and it's current position
 */
void show_string(char s[], int _pos)
{
    cout<<s<<endl;
    //Since position begins at 1 and index starts at 0 we add 1 to position
    cout<<setw(_pos + 1)<<'^'<<endl;
}
