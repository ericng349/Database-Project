#ifndef RPN_FUNCTIONS_H
#define RPN_FUNCTIONS_H

#include <string>
#include <cstring>
#include <vector>

#include "stack.h"
#include "queue.h"
#include "map.h"
#include "mmap.h"

#include <iostream>

using namespace std;

void get_union(vector<long> missing_records, vector<long>& records);

Map<std::string,int> setup_operator_map()
{
    //The presedence of the logical and relational operator
    enum Presedence{OR,AND,RELATIONAL_OPERATOR};
    //Order of presedence for the logical and relational operator
    //Look at operators that compare, then the and or
    Map<std::string,int> Operators;

    //Relational operators: [ = | > | < | >= | <= ]
    Operators["="] = RELATIONAL_OPERATOR;
    Operators[">"] = RELATIONAL_OPERATOR;
    Operators["<"] = RELATIONAL_OPERATOR;
    Operators[">="] = RELATIONAL_OPERATOR;
    Operators["<="] = RELATIONAL_OPERATOR;

    Operators["and"] = AND;
    Operators["or"] = OR;

    return Operators;
}
/**
 * @brief shunting_yard - converts the where into a post-fix notation through
 *                        the shunting yard alogrithm
 * @param where - the where clause
 * @return a postfix expression of the where clause
 */
vector<std::string> shunting_yard(vector<std::string> where)
{
    Queue<std::string> output_queue;
    Stack<std::string> operator_stack;

    vector<std::string> post_fix;

    Map<std::string,int> Operators = setup_operator_map();

    int index = 0;

    while(index  != where.size())
    {
        /*
        1.  While there are tokens to be read:
        2.        Read a token
        3.        If it's a number add it to queue
        4.        If it's an operator
        5.               While there's an operator on the top of the stack
                         with greater precedence:
        6.                       Pop operators from the stack onto the output
                                 queue
        7.               Push the current operator onto the stack
        8.        If it's a left bracket push it onto the stack
        9.        If it's a right bracket
        10.            While there's not a left bracket at the top of the
                       stack:
        11.                     Pop operators from the stack onto the output
                                queue.
        12.             Pop the left bracket from the stack and discard it
        13. While there are operators on the stack, pop them to the queue
        */
        //If it's not an operator
        if(!(Operators.contains(where[index])))
        {
            output_queue.push(where[index]);
        }
        else
        {
            //Check if the top of the stack is greater presedence than bottom
            while(!(operator_stack.empty()) && Operators[operator_stack.top()]
                                            >= Operators[where[index]])
            {
                std::string popped_item = operator_stack.pop();
                output_queue.push(popped_item);
            }

            operator_stack.push(where[index]);
        }
        index++;
    }
    //Place the rest of the items in the stack into the queue
    while(!(operator_stack.empty()))
    {
        std::string popped_item = operator_stack.pop();
        output_queue.push(popped_item);
    }

    for(Iterator<std::string> i = output_queue.begin(); i !=
                                  output_queue.end(); ++i)
        post_fix.push_back(i.operator *()._item);

    return post_fix;
}

/**
 *  Gets the index with the same field,
 *  ex. lhs_operand = first_name and first_name is index 1 of _field_list
 *  returns index 1.
 */
int get_index(std::string lhs_operand, vector<std::string> _field_list)
{
    int index = 0;  //the index in the field_list

    bool found = false;

    for(int i = 0; i < _field_list.size(); i++)
    {
        //finds the index of the field
        if(_field_list[i] == lhs_operand)
        {
            index = i;
            found = true;
        }
    }

    if(found)
        return index;
    else
        throw(index);
}
/**
 * @brief evaluate_relational_expression - gets the records that match the
 *                                         expression that is in postfix
 * @param lhs_operand - usually the field
 * @param rhs_operand - the value of the field
 * @param operator_string - the operation
 * @param field_map - the map of records
 * @param _field_list - the name of the field
 * @return records that match the expression
 */
vector<long> evaluate_relational_expression(std::string lhs_operand,
                                            std::string rhs_operand,
                                            std::string operator_string,
                                vector<MMap<std::string,long>> field_map,
                                vector<std::string> _field_list)
{
    vector<long> records;

    //Incase for some reason the operator_string is capitalized
    for(int i = 0; i < operator_string.size(); i++)
        operator_string[i] = tolower(operator_string[i]);

    //If the operator is an =
    if(operator_string == "=")
    {
        int index;
        //This will test whether the left hand operator is valid, meaning if it
        //exists in the field, ex. last_name = jeff when field doesnt include
        //last_name
        try{
            index = get_index(lhs_operand,_field_list);
        }
        catch(int index)
        {
            cout << "Error! Left hand value not found in field!" << endl;
            records.clear();
            return records;
        }

        //Takes to correct field, then finds the item inside the multimap
        //returns a multipair after, then gets the vector of record numbers
        //and copies it into records
        for(int i = 0; i < field_map[index][rhs_operand].size();i++)
            records.push_back(field_map[index][rhs_operand][i]);

    }
    //if operator is an >
    else if(operator_string == ">")
    {
        int index;
        //This will test whether the left hand operator is valid, meaning if it
        //exists in the field
        try{
            index = get_index(lhs_operand,_field_list);
        }
        catch(int index)
        {
            cout << "Error! Left hand value not found in field!" << endl;
            records.clear();
            return records;
        }

        bool contained = true;  //Checks whether or not item is contained

        //Insert the rhs into the map so that it can be compared
        if(!(field_map[index].contains(rhs_operand)))
        {
            field_map[index].insert(rhs_operand,0);
            contained = false;
        }

        //Move to one after begin since we do not want =
        for(MMap<std::string,long>::Iterator it =
            ++field_map[index].lower_bound(rhs_operand);
            it != field_map[index].end(); it++)
        {
            //*it returns a MPair, then we get the value list, which is the
            //vector containing all the record numbers
            for(int i = 0; i < it.operator *().value_list.size();i++)
                records.push_back(it.operator *().value_list[i]);
        }

        if(!contained)
            field_map[index].erase(rhs_operand);
    }
    else if(operator_string == "<")
    {
        //This will test whether the left hand operator is valid, meaning if it
        //exists in the field
        int index;
        try{
            index = get_index(lhs_operand,_field_list);
        }
        catch(int index)
        {
            cout << "Error! Left hand value not found in field!" << endl;
            records.clear();
            return records;
        }

        bool contained = true;

        //Insert the rhs into the map so that it can be compared
        if(!(field_map[index].contains(rhs_operand)))
        {
            field_map[index].insert(rhs_operand,0);
            contained = false;
        }

        for(MMap<std::string,long>::Iterator it = field_map[index].begin();
                                             it != field_map[index].upper_bound
                                             (rhs_operand); it++)
        {
            //*it returns a MPair, then we get the value list, which is the
            //vector containing all the record numbers
            for(int i = 0; i < it.operator *().value_list.size();i++)
                records.push_back(it.operator *().value_list[i]);
        }

        if(!contained)
            field_map[index].erase(rhs_operand);
    }
    else if(operator_string == ">=")
    {
        //This will test whether the left hand operator is valid, meaning if it
        //exists in the field
        int index;
        try{
            index = get_index(lhs_operand,_field_list);
        }
        catch(int index)
        {
            cout << "Error! Left hand value not found in field!" << endl;
            records.clear();
            return records;
        }

        bool contained = true;
        MMap<std::string,long>::Iterator begin(NULL);

        //Insert the rhs into the map so that it can be compared
        if(!(field_map[index].contains(rhs_operand)))
        {
            field_map[index].insert(rhs_operand,0);
            contained = false;
            //If it is not contained in the field_map, then = will not apply
            begin = ++field_map[index].lower_bound(rhs_operand);
        }
        else
        {
            //If it is contained in the field map then = will apply
            begin = field_map[index].lower_bound(rhs_operand);
        }

        //Find where to begin it
        for(MMap<std::string,long>::Iterator it = begin;
                                             it != field_map[index].end(); it++)
        {
            //*it returns a MPair, then we get the value list, which is the
            //vector containing all the record numbers
            for(int i = 0; i < it.operator *().value_list.size();i++)
                records.push_back(it.operator *().value_list[i]);
        }

        //erase the rhs from the map so that it is no longer in the map if it
        //wasn't there before
        if(!contained)
            field_map[index].erase(rhs_operand);
    }
    else if(operator_string == "<=")
    {
        //This will test whether the left hand operator is valid, meaning if it
        //exists in the field
        int index;
        try{
            index = get_index(lhs_operand,_field_list);
        }
        catch(int index)
        {
            cout << "Error! Left hand value not found in field!" << endl;
            records.clear();
            return records;
        }
        bool contained = true;
        MMap<std::string,long>::Iterator end_it(NULL);

        //Insert the rhs into the map so that it can be compared
        if(!(field_map[index].contains(rhs_operand)))
        {
            field_map[index].insert(rhs_operand,0);
            contained = false;
            //If it is not contained in the field_map, then = will not apply
            end_it = field_map[index].upper_bound(rhs_operand);
        }
        else
        {
            //If it is contained in the field map then = will apply
            end_it = ++field_map[index].upper_bound(rhs_operand);
        }

        for(MMap<std::string,long>::Iterator it = field_map[index].begin();
                                             it != end_it; it++)
        {
            //*it returns a MPair, then we get the value list, which is the
            //vector containing all the record numbers
            for(int i = 0; i < it.operator *().value_list.size();i++)
                records.push_back(it.operator *().value_list[i]);
        }

        if(!contained)
            field_map[index].erase(rhs_operand);
    }

    return records;
}
/**
 * @brief evaluate_logical_expression - determins which logical expression to
 *                                      use and which records to get
 * @param lhs_operand   - a vector that contains the records evaluated from the
 *                        left hand where clause (last = "bob" and first =
 *                        "jim") record of last = "bob"
 * @param rhs_operand   - a vector that contains the records evaluated from the
 *                        right hand where clause (last = "bob" and first =
 *                        "jim") record of first = "jim"
 * @param operator_string - whether it is an "and"/"or"
 * @return - the records that match the condition of and/or
 */
vector<long> evaluate_logical_expression(vector<long> lhs_operand,
                                         vector<long> rhs_operand,
                                         std::string operator_string)
{
    vector<long> records;   //vector of records that match expression

    //Incase for some reason the operator_string is capitalized
    for(int i = 0; i < operator_string.size(); i++)
        operator_string[i] = tolower(operator_string[i]);
    //and operator
    if(operator_string == "and")
    {
        //Check for intersections
        for(int i = 0; i < lhs_operand.size(); i++)
        {
            for(int j = 0; j < rhs_operand.size(); j++)
            {
                if(lhs_operand[i] == rhs_operand[j])
                    records.push_back(lhs_operand[i]);
            }
        }
    }
    //or operator
    else if(operator_string == "or")
    {
        //Check for union
        //First add all the records of the one with less records, then loop
        //through and add all the records that aren't inside yet

        //adds the smaller list of records first

        //left list of records smaller than right list
        if(lhs_operand.size() < rhs_operand.size())
        {
            //copy left list
            for(int i = 0; i < lhs_operand.size(); i++)
                records.push_back(lhs_operand[i]);
            //performs the union between the records and rhs operand
            get_union(rhs_operand,records);
        }
        else
        {
            //copy right list
            for(int i = 0; i < rhs_operand.size(); i++)
                records.push_back(rhs_operand[i]);

            //Add the remaining records from left list
            get_union(lhs_operand,records);
        }
    }
    else
    {
        cout << "logical operation unknown!" << endl;
    }

    return records;
}
/**
 * @brief read_expression - gets all the records that match the expression
 * @param post_fix_notation - the post fix notation of the where clause
 * @param field_map - the multimap of fields holding the records
 * @param _field_list - the list of fields
 * @return  all the records that match the expression
 */
vector<long> read_expression(vector<std::string> post_fix_notation,
                            vector<MMap<std::string,long>> field_map,
                            vector<std::string> _field_list)
{
     int index = 0;
     Map<std::string,int> Operators = setup_operator_map(); //Creates the map
                                                            //for operators
     Stack<std::string> operands;
     Stack<vector<long>> records;

     //loops until the whole where clause is evaluated (post fix notation holds
     //a bunch of where clauses along with and/or)
     while(index != post_fix_notation.size())
     {
         if(!(Operators.contains(post_fix_notation[index])))
         {
             operands.push(post_fix_notation[index]);
         }
         else
         {
            std::string operator_string = post_fix_notation[index];

            //Checks if operator is a relational one (enum 1)
            if(Operators[post_fix_notation[index]] == 2)
            {
                //Right hand operand comes out first, since filo for stack
                std::string rhs_operand = operands.pop();
                std::string lhs_operand = operands.pop();
                //after the evaluation, there will be a vector of record
                //numbers that contain the correct expression
                records.push(evaluate_relational_expression(lhs_operand,
                                                            rhs_operand,
                                     operator_string,field_map,_field_list));
            }
            //The and/or will use a different stack (enum 0)
            else if(Operators[post_fix_notation[index]] == 1 ||
                    Operators[post_fix_notation[index]] == 0)
            {

                vector<long> rhs_operand = records.pop();
                vector<long> lhs_operand = records.pop();
                records.push(evaluate_logical_expression(lhs_operand,
                                                         rhs_operand,
                                                         operator_string));
            }
         }
         index++;
     }

     return records.top();
}

/**
 * @brief get_union - gets the union of the records (removes duplicates)
 * @param missing_records - the records that are not yet in the official
 *                          records
 * @param records   - the records found that will be reported back to the
 *                    sql table
 */
void get_union(vector<long> missing_records, vector<long>& records)
{
    Map<long,long> current_records;

    current_records.clear();

    for(int i = 0; i < records.size(); i++)
        current_records[records[i]] = 0;

    for(int j = 0; j < missing_records.size(); j++)
    {
        //Checks if current record number is not inside records
        if(!(current_records.contains(missing_records[j])))
            records.push_back(missing_records[j]);
    }
}

#endif // RPN_FUNCTIONS_H
