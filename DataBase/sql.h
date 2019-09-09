#ifndef SQL_H
#define SQL_H

#include "table.h"
#include "parser.h"
#include <iomanip>

//Continue working on file input

class SQL{
    public:
        SQL();
        void run();
    private:
        Map<std::string,Table> _tables;         //A map of the tables that were
                                                //created
        MMap<std::string,std::string> _ptree;   //Multimap of parsed command
        Parser parser;                          //the parser object
        std::string sql_command;                //the command the user enters
        std::string sql_table_name;             //the table name
        vector<std::string> sql_fields;         //the fields
        vector<std::string> sql_where;          //the where clause
        vector<std::string> sql_values;         //the values


        vector<std::string> file_names;         //the file names of newly
                                                //created tables

        ofstream outs;                          //The output file
};
/**
 * @brief SQL::SQL - the constructor for the sql class
 * The Sql constructor will load in the previous tables that were created
 */
SQL::SQL()
{
    fstream f;                  //Read in the files

    f.open("file_name.txt");        //Opens the file
    outs.open("output.txt");

    std::string name_of_file;       //Name of the files

    cout << "Tables loaded: " << endl;
    outs << "Tables loaded: " << endl;

    //read in the tables that have already been created
    while(f >> name_of_file)
    {
        _tables[name_of_file] = Table(name_of_file);
        file_names.push_back(name_of_file);
    }

    //Display the tables loaded
    if(file_names.empty())
    {
        cout << "No Tables!" << endl;
        outs << "No Tables!" << endl;
    }
    else
    {
        for(int i = 0; i < file_names.size(); i++)
        {
            cout << file_names[i] << endl;
            outs << file_names[i] << endl;
        }
    }

    cout << endl;
    outs << endl;

    //Close the file
    f.close();
}

/**
 * @brief SQL::run - runs the sql interactive program
 * The run is able to do 4 things,
 *      Select - grabs the records the user wants
 *      Create/Make - creates a new table if there isn't already a table with
 *                    that name
 *      Insert - adds in records that must have correct field size, and the
 *               fieldsnames must be less than 20
 *      Batch - reads in a file of commands
 */
void SQL::run()
{
    bool debug = false;             //For debugging purposes

    std::string command;            //The SQL command that will be parsed and
                                    //read

    bool read_from_file = false;    //Whether or not the file is being read in

    ifstream f;                     //The file to read in

    int command_counter = 0;        //Counts up the command for output

    do
    {
        //Allows the user to enter their own command if commands are not being
        //read from files
        if(read_from_file == false || f.eof())
        {
            command.clear();    //Clear the command
            cin.clear();        //Clea the read in line

            cout << "Please enter a command: ";     //Get user input
            getline(cin,command);
        }
        else
        {
            command.clear();    //Clear previous commands if there are any
            getline(f,command); //Get new command from file

            //Check for comments
            while((command[0] == '/' && command[1] == '/') || command.empty()
                   && !(f.eof()))
            {
                cout << command << endl;
                outs << command << endl;
                command.clear();
                getline(f,command);
            }
            if(!(f.eof()))
            {
                cout << "[" << command_counter << "]" << " " << command << endl;
                outs << "[" << command_counter << "]" << " " << command << endl;
            }
            else
            {
                cout << "----------- END OF BATCH PROCESS --------" << endl;
                outs << "----------- END OF BATCH PROCESS --------" << endl;

                f.close();
            }
        }

        //================= PARSE TREE SETUP ==========================
        //do nothing if the command is empty or if it's exit
        if(!(command.empty()) && command != "exit")
        {
            parser.set_string(command); //Set up the parsert

            _ptree = parser.get_parse_tree();   //Get the parsed tree

            //View parse tree to debug
            if (debug && !_ptree.empty()){

                cout<<"here is the parse tree: "<<endl;

                cout<<_ptree<<endl;

            }

             //===================== TABLE SETUP =============================
            //here, figure out what the command is and

            //  what the parameters are for what table...
            //Sets up the command
            //The command is only one input so it will be the 0 index
            if(!(_ptree["command"].empty()))
                sql_command = _ptree["command"][0];

            //Gets the fields
            for(unsigned int i = 0; i < _ptree["fields"].size();i++)
                sql_fields.push_back(_ptree["fields"][i]);

            //Gets the where
            for(unsigned int i = 0; i < _ptree["where"].size();i++)
                sql_where.push_back(_ptree["where"][i]);


            //The table name is also one input so it will also be the 0 index
            if(!(_ptree["table-name"].empty()))
                sql_table_name = _ptree["table-name"][0];

            //Gets the values
            for(unsigned int i = 0; i < _ptree["values"].size();i++)
                sql_values.push_back(_ptree["values"][i]);


            //==================== COMMAND SETUP ===========================
            //convert the command to uppercase
            for(unsigned int i = 0; i < sql_command.size();i++)
                sql_command[i] = toupper(sql_command[i]);


            //Select command
            if (sql_command == "SELECT")
            {
                if(!(_tables.contains(sql_table_name)))
                {
                    cout << "THAT TABLE DOES NOT EXIST!" << endl;
                    outs << "THAT TABLE DOES NOT EXIST!" << endl;
                }
                else if(sql_table_name.empty())
                {
                    cout << "No Table provided!" << endl;
                    outs << "No Table provided!" << endl;
                    sql_command = "UNKNOWN";
                }
                //Checks if table exists
                else
                {

                    //Increment command counter
                    command_counter++;

                    if(sql_fields[0] == "*")
                    {
                        _tables["temp"] = _tables[sql_table_name].select_all
                                         (sql_where);
                    }
                    //Otherwise there are condition and fields
                    //everything else
                    else
                    {
                        _tables["temp"] = _tables[sql_table_name].select
                                           (sql_fields,sql_where);
                    }

                    //Output table name and amount of records
                    cout << "Table name: " << sql_table_name <<
                            ", records: " << _tables["temp"].size() << endl;
                    outs << "Table name: " << sql_table_name <<
                            ", records: " << _tables["temp"].size() << endl;

                    //Display table
                    cout << _tables["temp"] << endl;
                    outs << _tables["temp"] << endl;

                    cout << endl << endl << "SQL: DONE." << endl << endl;
                    outs << endl << endl << "SQL: DONE." << endl << endl;
                }

            }
            else if (sql_command == "MAKE" || sql_command == "CREATE")
            {
                //We don't want to overwrite the data base saved
                if(_tables.contains(sql_table_name))
                {
                    cout << "THERE IS ALREADY A TABLE WITH THAT NAME!" << endl;
                    outs << "THERE IS ALREADY A TABLE WITH THAT NAME!" << endl;
                }
                else if(sql_fields.empty())
                {
                    cout << "THE TABLE FIELDS SHOULD NOT BE EMPTY!" << endl;
                    outs << "THE TABLE FIELDS SHOULD NOT BE EMPTY!" << endl;
                }
                else if(sql_table_name.empty())
                {
                    cout << "No Table provided!" << endl;
                    outs << "No Table provided!" << endl;
                    sql_command = "UNKNOWN";
                }
                else
                {
                    //Increment command counter
                    command_counter++;

                    //Creates a new table with the field names from the command
                    Table new_table(sql_table_name,sql_fields);
                    //Maps the table
                    _tables[sql_table_name] = new_table;

                    //Get the new file name created
                    file_names.push_back(sql_table_name);

                    //Save the new table created into a file
                    ofstream f;

                    f.open("file_name.txt");

                    for(int i = 0; i < file_names.size(); i++)
                        f << file_names[i] << endl;

                    f.close();

                    cout << "Table " << sql_table_name << " has been created!"
                            << endl;
                    outs << "Table " << sql_table_name << " has been created!"
                            << endl;

                    cout << endl << "SQL: DONE." << endl << endl;
                    outs << endl << "SQL: DONE." << endl << endl;
                }
            }
            else if(sql_command == "INSERT")
            {
                //Check if the table contains the name
                if(!(_tables.contains(sql_table_name)))
                {
                    cout << "THAT TABLE DOES NOT EXIST!" << endl;
                    outs << "THAT TABLE DOES NOT EXIST!" << endl;
                }
                else if(sql_table_name.empty())
                {
                    cout << "No Table provided!" << endl;
                    outs << "No Table provided!" << endl;
                    sql_command = "UNKNOWN";
                }
                else if( _tables[sql_table_name].get_field_size() !=
                         sql_values.size())
                {
                    cout << "Values provided do not meet/exceed required field "
                            "length!" << endl;
                    outs << "Values provided do not meet/exceed required field "
                            "length!" << endl;
                }
                else
                {
                    //Increment command counter
                    command_counter++;

                    _tables[sql_table_name].insert(sql_values);

                    cout << "SQL::run: inserted." << endl << endl;
                    outs << "SQL::run: inserted." << endl << endl;
                }
            }
            else if(sql_command == "BATCH")
            {
                string file_name;       //Name of the file

                cout << "Please enter the name of the file you wish to read "
                        "in: ";
                outs << "Please enter the name of the file you wish to read "
                        "in: ";

                cin >> file_name;       //Gets the file name

                f.open(file_name);

                //Check to see if file opened correctly
                if(f.fail())
                {
                    cout << "failed to open file!" << endl;
                    outs << "failed to open file!" << endl;
                }
                else
                {
                    cout << "file opened successfully!" << endl;
                    read_from_file = true;
                }

                cout << "SQL: DONE." << endl << endl;
                outs << "SQL: DONE." << endl << endl;

            }
            else
            {
                cout << "COMMAND NOT RECOGNIZED!" << endl;
                outs << "COMMAND NOT RECOGNIZED!" << endl;
            }

            //Refresh the command and tables
            _ptree.clear();
            sql_command.clear();
            sql_table_name.clear();
            sql_fields.clear();
            sql_where.clear();
            sql_values.clear();
       }
    }

    //Checks if command is not exit
    while(command != "exit");

    //Close the files
    f.close();
    outs.close();

}


#endif // SQL_H
