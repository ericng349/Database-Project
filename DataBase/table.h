#ifndef TABLE_H
#define TABLE_H

#include <string>
#include <cstring>
#include "record.h"
#include "stokenizer.h"
#include "mmap.h"
#include "map.h"
#include "rpn_functions.h"
//#include "record.h"

class Table{
    public:
        //default
        Table();
        //existing table with this name:
        Table(std::string name);
        //brand new table
        Table(std::string name, vector<std::string> field_list);

        void insert(vector<std::string> field_list);

        Table select_all(vector<std::string> where);
        Table select(vector<std::string> condition, vector<std::string> where);

        int get_field_size();

        int size();

        friend ostream& operator<<(ostream& outs,const Table& t);

    private:
        std::string _name;
        //list of fields
        vector<std::string> _field_list;

        //indicies: mmaps of data_records
        vector<MMap<std::string,long>> field_map;
        //The last position in the file
        vector<Record> list;

        string get_file_name(string file_type);
        vector<int> find_indicies(vector<std::string> original_list,
                                  vector<std::string> find_fields);

        void grab_records(Table &t, vector<long> find_records);
        void grab_records(Table& t, vector<long> find_records,
                                    vector<int> indicies);


        long pos_end = 0;
};

Table::Table()
{

}
/**
 * @brief Table::Table - read in the table and initialize the field list and
 *                       field map
 * @param name - name of the table
 */
Table::Table(std::string name)
{
    //name
    _name = name;

    string fields_file_name = get_file_name(".fields");
    string bin_file_name = get_file_name(".bin");

    fstream f;


    //Get the fields file
    Record temp;

    //sets up the fields file
    if(file_exists(fields_file_name.c_str()))
    {
        open_fileRW(f,fields_file_name.c_str());

        temp.read(f,0, _field_list);

        f.close();
    }

    //Get the binary file with the data
    if(file_exists(bin_file_name.c_str()))
    {
        open_fileRW(f,bin_file_name.c_str());
        //resets the end position
        pos_end = 0;

        //Finds the position of the end of the file
        f.seekg(0,f.end);
        int file_end_pos = f.tellg();
        f.seekg(0,f.beg);

        int recno = 0;                      //the current record number
        vector<std::string> field_values;   //a field value holder

        //reads in record until the end of the file
        while(pos_end != file_end_pos)
        {
            temp.read(f,recno,field_values);
            //throws the record into the list of records
            list.push_back(temp);

            //Initializes the vector for field_map
            if(field_map.size() < field_values.size())
            {
                for(int i = 0; i < field_values.size(); i++)
                {
                    field_map.push_back(MMap<string,long>());
                }
            }

            //Inserts the record number into the field map
            for(int i = 0; i < field_values.size(); i++)
            {
                field_map[i].insert(field_values[i],recno);
            }

            ++recno;
            field_values.clear();
            pos_end = f.tellg();
        }
    }
    else
    {
        cout << name << " files do not exist!" << endl;
    }

     f.close();
}
/**
 * @brief Table::Table - creates a table with the name and field_list
 * @param name - name of table
 * @param field_list - list of fields
 */
Table::Table(std::string name, vector<string> field_list)
{
    /* table does not exist.

     * i am given the names of lists.

     * create the files: table file and field name files

     * and indices.

    */

    //Initialize private variables
    _name = name;
    _field_list = field_list;

    string file_name = get_file_name(".fields");

    fstream f;
    open_fileW(f,file_name.c_str());

    //create a record
    Record temp(_field_list);

    temp.write(f);

    f.close();
}

/**
 * @brief Table::insert - inserts a field_list into the table
 * @param field_list - the field_list of strings
 */
void Table::insert(vector<std::string> field_list)
{
    //=========== Creating a record ===================================

    try
    {
        //create a record
        Record temp(field_list);

        //Open the file
        fstream f;
        //get the file name
        string file_name = get_file_name(".bin");
        //open file to write
        open_fileW(f,file_name.c_str());
        //Moves to last written position to write
        f.seekp(pos_end,f.beg);

        //============ Calculating record number ===========================
        //temp.write will return the beginning position of the write
        long recno = temp.write(f);
        //Last written position
        pos_end = f.tellp();

        //============ Inserting record into list ==========================
        //Insert record into list
        list.push_back(temp);

        if(field_map.size() < field_list.size())
        {
            for(int i = 0; i < field_list.size(); i++)
            {
                field_map.push_back(MMap<string,long>());
            }
        }

        //Inserts the record number into the field map
        for(int i = 0; i < field_list.size(); i++)
        {
            field_map[i].insert(field_list[i],recno);
        }

        //Inserts the field and the record number

        //============ saving list ==========================================
        save_list(list,file_name.c_str());

        f.close();
    }
    catch(int OVERSIZED)
    {
        cout << "Error! Field character amount exceeded!" << endl;

    }
}

/**
 * @brief Table::select_all - select all of the records, if there is no where
 *                            otherwise get the records that follow the
 *                            conditions
 * @param where - the conditions
 * @return a table with the records that match the conditions
 */
Table Table::select_all(vector<std::string> where){

    /* open the file, grab ALL the stuff

     * and for now, just print it.

     * later: create a table with the info that

     * you extracted and return it.

     */

    Table t("temp",_field_list);

    vector<long> record_nums;   //The record numbers to place in temp

    //If there is a where clause
    if(!(where.empty()))
    {
        vector<std::string> post_fix_notation;

        post_fix_notation = shunting_yard(where);

        record_nums = read_expression(post_fix_notation,field_map,_field_list);
    }
    else
    {
        //If where is empty that means user wants all the records
        for(int i = 0; i < list.size(); i++)
            record_nums.push_back(i);
    }
    //Grabs the records that match the where
    grab_records(t, record_nums);

    return t;
}
/**
 * @brief Table::select - select the records that match the field and the where
 * @param condition - the fields that the user wants
 * @param where - the expression to follow
 * @return a table containing the records that match the condition and where
 *         clause
 */
Table Table::select(vector<std::string> condition, vector<std::string> where)
{
    Table t("temp",condition);

    //find the indicies that match the conditions to know which row
    //to look for in file.
    vector<int> indicies = find_indicies(_field_list,condition);

    vector<long> record_nums;

    //incase there were no matching fields found
    if(indicies.size() == 0)
    {
        t._field_list = _field_list;
        return t;
    }

    //If there is a where clause
    if(!(where.empty()))
    {
        vector<std::string> post_fix_notation;
        //create a post_fix notation by running through the shunting yard
        //algorithm
        post_fix_notation = shunting_yard(where);

        record_nums = read_expression(post_fix_notation,field_map,_field_list);
    }
    else
    {
        //If where is empty that means user wants all the records
        for(int i = 0; i < list.size(); i++)
            record_nums.push_back(i);
    }

    grab_records(t, record_nums, indicies);

    return t;
}
/**
 * @brief Table::grab_records - grabs the records that were requested
 * @param t - the table to insert records
 * @param find_records - the found records
 */
void Table::grab_records(Table& t, vector<long> find_records)
{
    string bin_file_name = get_file_name(".bin");

    fstream f;

    //Get the fields file
    Record temp;

    //technically only need this part

    //Get the binary file with the data
    if(file_exists(bin_file_name.c_str()))
    {
        open_fileRW(f,bin_file_name.c_str());

        //Reset the end position
        vector<std::string> temp_field_list;

        for(int i = 0; i < find_records.size();i++)
        {
            //Get the items from the file
            temp.read(f,find_records[i],temp_field_list);

            //insert it into the temporary file
            t.insert(temp_field_list);
            temp_field_list.clear();
        }
    }

    f.close();
}

/**
 * @brief Table::grab_records - grabs the records that were requested, but
 *                              for specific fields only
 * @param t - the table to insert them to
 * @param find_records - the records requested
 * @param indicies - the specific indicies the user wants
 */
void Table::grab_records(Table& t, vector<long> find_records,
                         vector<int> indicies)
{
    string bin_file_name = get_file_name(".bin");

    fstream f;

    //Get the fields file
    Record temp;

    if(file_exists(bin_file_name.c_str()))
    {
        open_fileRW(f,bin_file_name.c_str());

        vector<std::string> temp_field_list;

        for(int i = 0; i < find_records.size();i++)
        {
            //overloaded read to look for indicies (
            temp.read(f,find_records[i],temp_field_list,indicies);

            t.insert(temp_field_list);
            temp_field_list.clear();
        }
    }

    f.close();
}
/**
 * @brief Table::get_file_name - creates the file name by concatenating the
 *                               current file name with the file type
 * @param file_type - the type of file user wants to create, ex. .txt
 * @return - the file name
 */
string Table::get_file_name(string file_type)
{
    int file_name_size = _name.size() + file_type.size() + 1;

    char file_name[file_name_size];

    //initialize them to null
    for(int i = 0; i < file_name_size;i++)
        file_name[i] = '\0';

    //concatenate the filetype and name together
    strcat(file_name,_name.c_str());
    strcat(file_name,file_type.c_str());

    return file_name;
}

/**
 * @brief Table::find_indicies - (helper function for select) compares the
 *                                field the user wants with the original list
 *                                to grab the correct indicies
 * @param original_list - the original fields list
 * @param find_fields - the fields the user wants
 * @return - indicies of the fields the user wants
 */
vector<int> Table::find_indicies(vector<std::string> original_list,
                                 vector<std::string> find_fields)
{
    vector<int> indicies_found;

    for(int i = 0 ; i < original_list.size(); i++)
    {
        for(int j = 0; j < find_fields.size(); j++)
        {
            if(original_list[i] == find_fields[j])
                indicies_found.push_back(i);
        }
    }

    return indicies_found;
}

/**
 * @brief get_field_size - returns how many fields there are
 * @return the number of fields that are in the table
 */
int Table::get_field_size()
{
    return _field_list.size();
}

/**
 * @brief Table::size - returns the size of the table (amount of records)
 * @return the amount of records contained in the table
 */
int Table::size()
{
    return list.size();
}

/**
 * @brief operator << - overloaded output operator to display the table
 * @param outs - the ostream object
 * @param t - the table to display
 * @return A formatted table
 */
ostream& operator<<(ostream& outs,const Table& t)
{

    std::string line_divider(13 + (t._field_list.size() * 20),'=');

    outs << line_divider << endl;

    outs << setw(20) << left <<"Record Number";
    for(int i = 0; i < t._field_list.size(); i++)
    {
        outs << setw(20) << t._field_list[i];
    }
    outs << endl;
    outs << line_divider << endl;

    for(int i = 0; i < t.list.size(); i++)
    {
        outs << setw(20) << left << i  << t.list[i];
        outs << setw(0) << endl;
    }

    return outs;
}

#endif // TABLE_H
