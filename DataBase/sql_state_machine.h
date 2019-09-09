#ifndef SQL_STATE_MACHINE_H
#define SQL_STATE_MACHINE_H

#include "adjacency_matrix.h"
#include "helper_functions.h"
#include <vector>
#include <cctype>
#include "map.h"

enum key{ZERO,SELECT,STRING,COMMA,STAR,FROM,WHERE,RELATIONAL_OPERATOR,QUOTE,LOGICAL_OPERATOR,
         CREATE,TABLE,FIELDS,
         INSERT,INTO,VALUES,
         BATCH};


void create_state_machine(int _table[][MAX_COLUMNS])
{


    //Everything is shifted over by one because the zero column is being used
    init_table(_table);

     //Moves to the correct state/command
     mark_cell(0, _table,SELECT, 1);      //State[0] -> State[1] select machine
     mark_cell(0, _table,CREATE, 11);     //State[0] -> State[11] create machine
     mark_cell(0, _table,INSERT, 17);     //State[0] -> State[17] insert machine
     mark_cell(0, _table,BATCH,22);

    //========================== SELECT MACHINE ============================
    mark_cell(1, _table, STAR,2);                //state[1] -> state[2]
                                                 //select -> *
    //Select comma is state right before star
    mark_cell(1, _table,STRING, 3);              //state[1] -> state[3]
                                                 //select -> fields

    mark_cell(3, _table,COMMA, 1);               //state[3] -> state[1]
                                                 //field -> , (incase there are
                                                 //multiple fields)

    mark_cell(2, _table, FROM,5);                //state[2] -> state[5]
                                                 // * -> from

    mark_cell(3, _table, FROM,5);                //state[2] -> state[5]
                                                 // field -> from

    mark_cell(5, _table,STRING,6);               //state[5] -> state[6]
                                                 //from -> table_name

    //Where continuation
    mark_cell(6, _table,WHERE,7);                 //state[6] -> state[7]
                                                  //table_name -> where

    mark_cell(7, _table,STRING,8);                //state[7] -> state[8]
                                                  //where -> lhs_operand

    mark_cell(8, _table,RELATIONAL_OPERATOR,9);   //state[8] -> state[9]
                                                  //lhs_operand ->
                                                  //relational_operator

    mark_cell(9, _table,STRING,10);               //state[9] -> state[10]
                                                  //relational_operator
                                                  //-> rhs_operand

    mark_cell(10, _table,LOGICAL_OPERATOR,7);     //state[10] -> state[7]
                                                  //rhs_operator ->
                                                  //logical_operator

    //======================== Create/Make Machine ==========================
    mark_cell(11, _table,TABLE,12);               //state[11] -> state[12]
                                                  //create/make -> table

    mark_cell(12, _table,STRING,13);              //state[12] -> state[13]
                                                  //table -> table_name

    mark_cell(13, _table,FIELDS,14);             //state[13] -> state[14]
                                                 //table -> fields

    mark_cell(14, _table,STRING,15);             //state[14] -> state[15]
                                                 //fields -> field_names

    mark_cell(15, _table,COMMA,14);              //state[15] -> state[14]
                                                 //field_names -> ,

    //======================== Insert Machine ==========================
    mark_cell(17, _table,INTO,18);               //state[17] -> state[18]
                                                 //insert -> into

    mark_cell(18, _table,STRING,19);             //state[18] -> state[19]
                                                 //into -> table_name

    mark_cell(19, _table,VALUES,20);             //state[19] -> state[20]
                                                 //table_name -> values

    mark_cell(20, _table,STRING,21);             //state[20] -> state[21]
                                                 //values -> field

    mark_cell(21, _table,COMMA,20);              //state[21] -> state[20]
                                                 //field -> ,



    //============= Mark Success/Fail ==================================
    //Only succesfful state is if it reaches FROM_FIELD
    mark_fail(_table, 0);
    mark_fail(_table,1);
    mark_fail(_table,2);
    mark_fail(_table,3);
    mark_fail(_table,4);
    mark_fail(_table,5);
    mark_success(_table,6);

    //Where
    mark_fail(_table, 7);
    mark_fail(_table,8);
    mark_fail(_table,9);
    mark_success(_table,10);

    //Create/Make
    mark_fail(_table, 11);
    mark_fail(_table,12);
    mark_fail(_table,13);
    mark_fail(_table,14);
    mark_success(_table,15);

    //Insert Machine
    mark_fail(_table, 17);
    mark_fail(_table,18);
    mark_fail(_table,19);
    mark_fail(_table,20);
    mark_success(_table,21);

    //Batch
    mark_success(_table,22);

}

int get_state(int _table[][MAX_COLUMNS],vector<std::string>
                              parsed_sentence,int row,int current_index)
{

    Map<std::string,long> key_words;
    long col;


    //Mapping the keywords

    key_words["select"] = SELECT;

    key_words["*"] = STAR;
    key_words[","] = COMMA;

    key_words["from"] = FROM;
    //Optional condition, state
    key_words["where"] = WHERE;
    //Relational operators: [ = | > | < | >= | <= ]
    key_words["="] = RELATIONAL_OPERATOR;
    key_words[">"] = RELATIONAL_OPERATOR;
    key_words["<"] = RELATIONAL_OPERATOR;
    key_words[">="] = RELATIONAL_OPERATOR;
    key_words["<="] = RELATIONAL_OPERATOR;
    key_words["and"] = LOGICAL_OPERATOR;
    key_words["or"] = LOGICAL_OPERATOR;

    key_words["create"] = CREATE;
    key_words["make"] = CREATE;
    key_words["table"] = TABLE;
    key_words["fields"] = FIELDS;

    key_words["insert"] = INSERT;
    key_words["into"] = INTO;
    key_words["values"] = VALUES;

    key_words["batch"] = BATCH;

    //If keywords already mapped ignore if its not there then its some
    //symbol or value or its a comma
    if(!key_words.contains(toLower(parsed_sentence[current_index])))
    {
        col = STRING;
    }
    else
    {
        col = key_words[toLower(parsed_sentence[current_index])];
    }

    return _table[row][col];
}

#endif // SQL_STATE_MACHINE_H
