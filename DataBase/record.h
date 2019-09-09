#ifndef RECORD_H
#define RECORD_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <string>
#include <cstring>
#include <iomanip>

using namespace std;

bool file_exists(const char filename[]);

void open_fileRW(fstream& f, const char filename[]) throw(char*);
void open_fileW(fstream& f, const char filename[]);


class Record{
public:
    Record(){
        //record[0] = NULL;
        recno = -1;

        for(int i = 0; i < ROW; i++)
        {
            for(int j = 0; j < COL; j++)
            {
                record[i][j] = '\0';
            }
        }
    }
    //Move this to a function in bottom after
    Record(vector<string> field_list){
        for(int i = 0; i < ROW; i++)
        {
            for(int j = 0; j < COL; j++)
            {
                record[i][j] = '\0';
            }
        }
        //Copy the items from the field list into the record

        //Check if the field_list is greater than row
        if(field_list.size() > ROW)
        {
            throw(ROW);
        }
        for(int i = 0; i < field_list.size();i++)
        {
            //In case the item in field is greater than fieldsize then it needs
            //to be increased
            if(field_list[i].size() > COL)
            {
                throw(COL);
            }
            strcpy(record[i],field_list[i].c_str());

        }


    }
    long write(fstream& outs);
    //reads in the file containing all the records
    long read(fstream& ins, long recno, vector<std::string>& field_list);

    //Reads part of the record that are in the indicies
    long read(fstream& ins, long recno, vector<std::string>& field_list,
              vector<int>& indicies);


    friend ostream& operator<<(ostream& outs,const Record& r);
private:
    int recno;
    const static int ROW = 15;
    const static int COL = 20;
    char record[ROW][COL];
};

/**
 * @brief Record::write - writes into the the file's last position the record
 * @param outs - the out file
 * @return - record number
 */
long Record::write(fstream &outs){
    //write to the end of the file.
    long pos;

    pos = outs.tellp();

    //comment: outs.write(&record[0], sizeof(record));
    for(int i = 0; i < ROW; i++)
    {
        outs.write(record[i], sizeof(record[i]));
    }

    return pos / sizeof(record);
}
/**
 * @brief Record::read - reads in from the file the record with the record
 *                       number
 * @param ins - the file to read from
 * @param recno - the record number to find
 * @param field_list - the field list
 * @return - a record number
 */
long Record::read(fstream &ins, long recno, vector<std::string>& field_list){
    long pos = recno * sizeof(record);
    ins.seekg(pos, ios_base::beg);

    //Problem was all char record[i] initialized to '\0' so we need to read in first

    for(int i = 0; i < ROW; ++i)
    {
        ins.read(record[i], sizeof(record[i]));
        //Grabs the fields that aren't empty
        if(record[i][0] != '\0')
        {
            field_list.push_back(record[i]);
        }
    }

    return recno;

}
/**
 * @brief Record::read - overloaded in order to get certain indicies in the
 *                       file, for example only last name fields
 * @param ins - the file to read in from
 * @param recno - the record number
 * @param field_list - the field list read in
 * @param indicies - the indicies of the fields to read in
 * @return a record number
 */
long Record::read(fstream& ins, long recno,vector<std::string>& field_list,
                  vector<int>& indicies)
{
    long pos = recno * sizeof(record);
    ins.seekg(pos, ios_base::beg);

    //Problem was all char record[i] initialized to '\0' so we need to read in
    //first
    for(int i = 0; i < ROW; ++i)
    {
        ins.read(record[i], sizeof(record[i]));
        //Grabs the fields that aren't empty
        for(int j = 0; j < indicies.size(); j++)
        {
            if(i == indicies[j])
            {
                field_list.push_back(record[i]);
            }
        }
    }

    return recno;
}

ostream& operator<<(ostream& outs,
                    const Record& r){

    for(int i = 0; i < r.ROW; i++)
    {
        //Size of pointer will only return 4 or 8 bytes, therefore
        //must calculate the size
        if(r.record[i][0] != '\0')
        {
            outs  << setw(20) << r.record[i];
        }
    }
    return outs;
}

//-------------------------------------------------
bool file_exists(const char filename[]){
    const bool debug = false;
    fstream ff;
    ff.open (filename,
        std::fstream::in | std::fstream::binary );
    if (ff.fail()){
        if (debug) cout<<"file_exists(): File does NOT exist: "<<
                         filename<<endl;
        return false;
    }
    if (debug) cout<<"file_exists(): File DOES exist: "<<filename<<endl;
    ff.close();
    return true;
}

void open_fileRW(fstream& f, const char filename[]) throw(char*){
    const bool debug = false;
    //openning a nonexistent file for in|out|app causes a fail()
    //  so, if the file does not exist, create it by openning it for
    //  output:
    if (!file_exists(filename)){
        f.open(filename, std::fstream::out|std::fstream::binary);
        if (f.fail()){
            cout<<"file open (RW) failed: with out|"<<filename<<"]"<<endl;
            throw("file RW failed  ");
        }
        else{
            if (debug) cout<<"open_fileRW: file created successfully: "<<
                             filename<<endl;
        }
    }
    else{
        f.open (filename,
            std::fstream::in | std::fstream::out| std::fstream::binary );
        if (f.fail()){
            cout<<"file open (RW) failed. ["<<filename<<"]"<<endl;
            throw("file failed to open.");
        }
    }

}
void open_fileW(fstream& f, const char filename[]){
    f.open (filename,
            std::fstream::out| std::fstream::binary );
    if (f.fail()){
        cout<<"file open failed: "<<filename<<endl;
        throw("file failed to open.");
    }

}
void save_list(vector<Record> list, string file_name){
    fstream f;
    open_fileW(f, file_name.c_str());
    for (int i= 0; i<list.size(); i++){
        list[i].write(f);
    }
    f.close();

}

#endif // RECORD_H
