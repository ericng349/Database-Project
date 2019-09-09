#ifndef Z_OUTPUT_H
#define Z_OUTPUT_H
/*
***** ALL RESULTS ARE VERIFIED ******
//---------------------------------------------------------------------
// normal run:
//---------------------------------------------------------------------
Run #1:
Tables loaded:
employee
student

Please enter a command: select * from employee
=========================================================================
Record Number       last                first               dep
=========================================================================
0                   Blow                Joe                 CS
1                   Johnson             Jimmy               Chemistry
2                   Yang                Bo                  CS
3                   Jackson             Billy               Math
4                   Johnson             Billy               Phys Ed
5                   Van Gogh            Jim Bob             Phys Ed



SQL: DONE.

Please enter a command: select * from employee where last <= Yang
=========================================================================
Record Number       last                first               dep
=========================================================================
0                   Blow                Joe                 CS
1                   Jackson             Billy               Math
2                   Johnson             Jimmy               Chemistry
3                   Johnson             Billy               Phys Ed
4                   Van Gogh            Jim Bob             Phys Ed
5                   Yang                Bo                  CS



SQL: DONE.

Please enter a command: exit
Press <RETURN> to close this window...


Tables loaded:
No Tables!

Please enter a command: batch
Please enter the name of the file you wish to read in: non-existent.txt
failed to open file!
SQL: DONE.

Please enter a command: Please enter a command: batch
Please enter the name of the file you wish to read in: _!sample.txt
file opened successfully!
SQL: DONE.

Run #2:

//****************************************************************************
//              CREATE AND INSERT
//****************************************************************************

//---- employee table ----------
[0] make table employee fields  last,           first,                  dep
Table employee has been created!

SQL: DONE.

[1] insert into employee values Blow,           Joe,                    CS
SQL::run: inserted.

[2] insert into employee values Johnson,        "Jimmy",                Chemistry
SQL::run: inserted.

[3] insert into employee values Yang,           Bo,                     CS
SQL::run: inserted.

[4] insert into employee values "Jackson",      Billy,                  Math
SQL::run: inserted.

[5] insert into employee values Johnson,        Billy,                  "Phys Ed"
SQL::run: inserted.

[6] insert into employee values "Van Gogh",     "Jim Bob",              "Phys Ed"
SQL::run: inserted.


[7] select * from employee
Table name: employee, records: 6
=========================================================================
Record Number       last                first               dep
=========================================================================
0                   Blow                Joe                 CS
1                   Johnson             Jimmy               Chemistry
2                   Yang                Bo                  CS
3                   Jackson             Billy               Math
4                   Johnson             Billy               Phys Ed
5                   Van Gogh            Jim Bob             Phys Ed



SQL: DONE.


//----- student table ----------
[8] make table student fields   fname,                  lname,          major,                          age
Table student has been created!

SQL: DONE.

[9] insert into student values  Flo,                    Yao,            CS,                             20
SQL::run: inserted.

[10] insert into student values         "Flo",                  "Jackson",      Math,                     21
SQL::run: inserted.

[11] insert into student values         Calvin,                 Woo,            Physics,                  22
SQL::run: inserted.

[12] insert into student values         "Anna Grace",   "Del Rio",      CS,                               22
SQL::run: inserted.

[13] select * from student
Table name: student, records: 4
=============================================================================================
Record Number       fname               lname               major               age
=============================================================================================
0                   Flo                 Yao                 CS                  20
1                   Flo                 Jackson             Math                21
2                   Calvin              Woo                 Physics             22
3                   Anna Grace          Del Rio             CS                  22



SQL: DONE.




//****************************************************************************
//              SIMPLE SELECT
//****************************************************************************

[14] select * from student
Table name: student, records: 4
=============================================================================================
Record Number       fname               lname               major               age
=============================================================================================
0                   Flo                 Yao                 CS                  20
1                   Flo                 Jackson             Math                21
2                   Calvin              Woo                 Physics             22
3                   Anna Grace          Del Rio             CS                  22



SQL: DONE.


//------- simple strings -------------------
[15] select * from student where lname = Jackson
Table name: student, records: 1
=============================================================================================
Record Number       fname               lname               major               age
=============================================================================================
0                   Flo                 Jackson             Math                21



SQL: DONE.


//----- quoted strings ---------------------
[16] select * from student where lname = "Del Rio"
Table name: student, records: 1
=============================================================================================
Record Number       fname               lname               major               age
=============================================================================================
0                   Anna Grace          Del Rio             CS                  22



SQL: DONE.


//-------- non-existing string ------------------
[17] select * from student where lname = "Does Not Exist"
Table name: student, records: 0
=============================================================================================
Record Number       fname               lname               major               age
=============================================================================================



SQL: DONE.


//****************************************************************************
//              RELATIONAL OPERATORS:
//****************************************************************************

//.................
//:Greater Than   :
//.................
[18] select * from student where lname > Yang
Table name: student, records: 1
=============================================================================================
Record Number       fname               lname               major               age
=============================================================================================
0                   Flo                 Yao                 CS                  20



SQL: DONE.



//. . . . . . . . . . . . . (Greater Than: Non-existing) . . . . . . . . . . . . . . . . . . . . .
[19] select * from student where age > 50
Table name: student, records: 0
=============================================================================================
Record Number       fname               lname               major               age
=============================================================================================



SQL: DONE.


//. . . . . . . . . . . . . (Greater than: last item) . . . . . . . . . . . . . . . . . .
[20] select * from student where age  > 53
Table name: student, records: 0
=============================================================================================
Record Number       fname               lname               major               age
=============================================================================================



SQL: DONE.


//. . . . . . . . . . . . . (Greater Than: Passed last item) . . . . . . . . . . . . . . . . . . . . .
[21] select * from student where age > 54
Table name: student, records: 0
=============================================================================================
Record Number       fname               lname               major               age
=============================================================================================



SQL: DONE.


//.................
//:Greater Equal  :
//.................
[22] select * from student where lname >= Yang
Table name: student, records: 1
=============================================================================================
Record Number       fname               lname               major               age
=============================================================================================
0                   Flo                 Yao                 CS                  20



SQL: DONE.

//. . . . . .  (Greater Equal non-existing: ) . . . . . . . . . . .
[23] select * from employee where last >= Jack
Table name: employee, records: 5
=========================================================================
Record Number       last                first               dep
=========================================================================
0                   Jackson             Billy               Math
1                   Johnson             Jimmy               Chemistry
2                   Johnson             Billy               Phys Ed
3                   Van Gogh            Jim Bob             Phys Ed
4                   Yang                Bo                  CS



SQL: DONE.



//.................
//:Less Than      :
//.................


//. . . . . . . . . . . . . (Less Than: Non-existing) . . . . . . . . . . . . . . . . . . . . .
[24] select * from student where lname < Jackson
Table name: student, records: 1
=============================================================================================
Record Number       fname               lname               major               age
=============================================================================================
0                   Anna Grace          Del Rio             CS                  22



SQL: DONE.


//. . . . . . . . . . . . . (Less than: first item) . . . . . . . . . . . . . . . . . .
[25] select * from student where age  < 20
Table name: student, records: 0
=============================================================================================
Record Number       fname               lname               major               age
=============================================================================================



SQL: DONE.



//. . . . . . . . . . . . . (Less Than: before first item) . . . . . . . . . . . . . . . . . . . . .

[26] select * from student where age  < 19
Table name: student, records: 0
=============================================================================================
Record Number       fname               lname               major               age
=============================================================================================



SQL: DONE.



//.................
//:Less Equal     :
//.................

[27] select * from student where lname <= Smith
Table name: student, records: 2
=============================================================================================
Record Number       fname               lname               major               age
=============================================================================================
0                   Anna Grace          Del Rio             CS                  22
1                   Flo                 Jackson             Math                21



SQL: DONE.


//. . . . . .  (Less Equal non-existing: ) . . . . . . . . . . .
[28] select * from employee where last <= Peach
Table name: employee, records: 4
=========================================================================
Record Number       last                first               dep
=========================================================================
0                   Blow                Joe                 CS
1                   Jackson             Billy               Math
2                   Johnson             Jimmy               Chemistry
3                   Johnson             Billy               Phys Ed



SQL: DONE.


//****************************************************************************
//              LOGICAL OPERATORS
//****************************************************************************


//.................
//:AND            :
//.................

[29] select * from student where fname = "Flo" and lname = "Yao"
Table name: student, records: 1
=============================================================================================
Record Number       fname               lname               major               age
=============================================================================================
0                   Flo                 Yao                 CS                  20



SQL: DONE.



//.................
//:OR            :
//.................
[30] select * from student where fname = Flo or lname = Jackson
Table name: student, records: 2
=============================================================================================
Record Number       fname               lname               major               age
=============================================================================================
0                   Flo                 Jackson             Math                21
1                   Flo                 Yao                 CS                  20



SQL: DONE.



//.................
//:OR AND         :
//.................
[31] select * from student where fname = Flo or major = CS and age <= 23
Table name: student, records: 3
=============================================================================================
Record Number       fname               lname               major               age
=============================================================================================
0                   Flo                 Yao                 CS                  20
1                   Anna Grace          Del Rio             CS                  22
2                   Flo                 Jackson             Math                21



SQL: DONE.



//.................
//:AND OR AND     :
//.................

[32] select * from student where age <30 and major=CS or major = Physics and lname = Jackson
Table name: student, records: 2
=============================================================================================
Record Number       fname               lname               major               age
=============================================================================================
0                   Flo                 Yao                 CS                  20
1                   Anna Grace          Del Rio             CS                  22



SQL: DONE.



//.................
//:OR AND OR      :
//.................

[33] select * from student where lname = Yang or major = CS and age < 23 or lname = Jackson
Table name: student, records: 3
=============================================================================================
Record Number       fname               lname               major               age
=============================================================================================
0                   Flo                 Jackson             Math                21
1                   Flo                 Yao                 CS                  20
2                   Anna Grace          Del Rio             CS                  22



SQL: DONE.











----------- END OF BATCH PROCESS --------
Please enter a command: Please enter a command: exit
Press <RETURN> to close this window...

Run #3:
Tables loaded:
employee
student

Please enter a command: select * from student
Table name: student, records: 4
=============================================================================================
Record Number       fname               lname               major               age
=============================================================================================
0                   Flo                 Yao                 CS                  20
1                   Flo                 Jackson             Math                21
2                   Calvin              Woo                 Physics             22
3                   Anna Grace          Del Rio             CS                  22



SQL: DONE.

Please enter a command: select fname, major where lname = Yao
COMMAND NOT RECOGNIZED!
Please enter a command: select fname, major from student where lname = Yao
Table name: student, records: 1
=====================================================
Record Number       fname               major
=====================================================
0                   Flo                 CS



SQL: DONE.

Please enter a command: exit
Press <RETURN> to close this window...

//---------------------------------------------------------------------
// help:
//---------------------------------------------------------------------



//---------------------------------------------------------------------
// no input:
//---------------------------------------------------------------------
//Run #1:
Tables loaded:
employee
student

Please enter a command: exit
Press <RETURN> to close this window...

//Run #2:
Tables loaded:
employee
student

Please enter a command: create
No Table provided!
COMMAND NOT RECOGNIZED!
Please enter a command: select
No Table provided!
COMMAND NOT RECOGNIZED!
Please enter a command: insert
No Table provided!
COMMAND NOT RECOGNIZED!
Please enter a command: exit
Press <RETURN> to close this window...


//---------------------------------------------------------------------
// bad input:
//---------------------------------------------------------------------
Run #1:
Tables loaded:
employee
student

Please enter a command: create table apple
THE TABLE FIELDS SHOULD NOT BE EMPTY!
Please enter a command: insert into apple
THAT TABLE DOES NOT EXIST!
Please enter a command: insert into
No Table provided!
COMMAND NOT RECOGNIZED!
Please enter a command: select
No Table provided!
COMMAND NOT RECOGNIZED!
Please enter a command: exit
Press <RETURN> to close this window...

Run #2:
Tables loaded:
employee
student

Please enter a command: apple
COMMAND NOT RECOGNIZED!
Please enter a command: orange
COMMAND NOT RECOGNIZED!
Please enter a command: insert into student
Values provided do not meet/exceed required field length!
Please enter a command: insert into student values
Values provided do not meet/exceed required field length!
Please enter a command: done
COMMAND NOT RECOGNIZED!
Please enter a command: exit
Press <RETURN> to close this window...

Run #3:

Tables loaded:
employee
student

Please enter a command: select random from student where lname = Yao
Table name: student, records: 0
=============================================================================================
Record Number       fname               lname               major               age
=============================================================================================



SQL: DONE.

Please enter a command: exit
Press <RETURN> to close this window...

*/
#endif // Z_OUTPUT_H
