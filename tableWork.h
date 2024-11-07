#pragma once
#include <iostream>
#include <iomanip>
#include "parser.h"
#include "insert.h"
#include "rapidcsv.hpp"

using namespace std;

bool isLocked(string table, string scheme);
void locker(string table, string scheme);
bool tableExist(string table, Tables* head);
bool columnExist(string table, string columnName, Tables* head);
void separateDot(string message, string& table, string& column);
void crossJoin(string table1, string column1, string table2, string column2, JsonTable& jstab);
void crossJoinDepend(string table1, string column1, string table2, string column2, JsonTable& jstab);
void outputWithCondSingleT(JsonTable& jstab, string tableCond1, string tableCond2, string columnCond1, string columnCond2);
void outputWithCondSingleS(JsonTable& jstab, string tableCond1, string table, string columnCond1, string sCond);
void outputWithCondAnd(JsonTable& jstab, string tableCond1, string tableCond2, string columnCond1, string columnCond2, string tableCond3, string columnCond3, string sCond);
void outputWithCondOr(JsonTable& jstab, string tableCond1, string tableCond2, string columnCond1, string columnCond2, string tableCond3, string columnCond3, string sCond);
bool isDot(string message);
bool isDependenceTables(string table, string column, string tableCond, string columnCond, size_t csvNum, size_t row, JsonTable& jstab);
bool isDependenceString(string table, string column, string sCond, size_t csvNum, size_t row, JsonTable& jstab);