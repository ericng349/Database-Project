#ifndef PARSER_H
#define PARSER_H

#include "stokenizer.h"
#include "map.h"
#include "mmap.h"
#include "sql_state_machine.h"
#include <vector>
#include <string>



//typedef std::string string;

class Parser{

    public:
        Parser();
        void set_string(std::string command);
        MMap<std::string,std::string> get_parse_tree();
    private:
        vector<std::string> parsed_sentence;

};

Parser::Parser()
{

}

MMap<std::string,std::string> Parser::get_parse_tree()
{

    MMap<std::string,std::string> parseTree; //A MMap that holds the command,
                                             //fields, etc. (grouping the info)

    int _table[MAX_ROWS][MAX_COLUMNS];      //The state machine table

    int row = 0;                            // row in the table
    int current_index = 0;                  // column of table
    int state = 0;                          //current state

    //Generates a SQL state machine
    create_state_machine(_table);

    while(state != -1 && current_index < parsed_sentence.size())
    {
        //Takes the table, string vector of parsed words, row and current index,
        //and returns the state
        state = get_state(_table,parsed_sentence,row,current_index);

        switch(state)
        {
            case 1:     //Select command
            case 11:    //create/make command
            case 17:    //insert command
            case 22:    //batch
            {
                std::string command = toLower(parsed_sentence[current_index]);
                parseTree["command"] += command;
                break;
            }

            case 2:  //Star for Select machine
            case 3:  //Field names for Select machine
            case 15: //Fields for the Create/Make machine
            {
                std::string fields = toLower(parsed_sentence[current_index]);
                parseTree["fields"] += fields;
                break;
            }

            case 6:     //table name for select machine
            case 13:    //table name for create machine
            case 19:    //table name for insert machine
            {
                std::string table = toLower(parsed_sentence[current_index]);
                parseTree["table-name"] += table;
                break;
            }
            //If user enters a where clause
            case 7:     //String
            case 8:     //Relational
            case 9:     //String
            case 10:    //logic
            {
                std::string table = parsed_sentence[current_index];

                //Since the logical operator loops back to the same 3 sets
                //<FIELD_NAME> <RELATIONAL_OPERATOR> <VALUE> we don't want
                //the where, but we want the logical operator
                if(table != "where")
                    parseTree["where"] += table;
                break;
            }
            case 21:    //Values for Insert machine (what is being inserted)
            {
                std::string table = parsed_sentence[current_index];

                parseTree["values"] += table;
                break;
            }
        }

        //Update row and column
        row = state;
        current_index++;
    }

    //The while loop runs until the state is -1, which means it is in a
    //unsuccessful state, so when its unsuccessful say it was an unknown
    //command and clear tree
    if(state == -1)
    {
        parseTree.clear();
    }

    return parseTree;
}

void Parser::set_string(std::string command)
{    

    //incase there is something in parsed_sentence
    parsed_sentence.clear();

    //The stirng command converted to character array
    char s[command.size() + 1];
    strcpy(s,command.c_str());

    STokenizer stk(s);

    Token t;

    //The all too familiar golden while loop:
    stk>>t;

    while(!stk.done()){
        //process token here...

        if(t.type_string() != "SPACE")
            parsed_sentence.push_back(t.token_str());

        t = Token();
        stk>>t;
    }

/*
    for(int i = 0; i < parsed_sentence.size();i++)
        cout << parsed_sentence[i] << endl;
*/
}

#endif // PARSER_H
