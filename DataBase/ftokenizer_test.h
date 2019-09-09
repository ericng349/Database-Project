#ifndef FTOKENIZER_TEST_H
#define FTOKENIZER_TEST_H

#include "ftokenizer.h"

void test_f_tokenize_simple(){
    Token t;
    FTokenizer ftk("solitude.txt");

    ftk>>t;
    int token_count = 0;

    while (ftk.more()){
        if (t.type_string()=="ALPHA" ){
            token_count++;
            cout<<setw(10)<<token_count
                <<setw(3)<<left<<":"<<setw(25)<<left<<t.token_str()
                <<t.type_string()<<endl;
        }
        ftk>>t;

    }
    cout<<"Tokens Found: "<<token_count<<endl;
    cout<<"=========="<<endl;

}

#endif // FTOKENIZER_TEST_H
