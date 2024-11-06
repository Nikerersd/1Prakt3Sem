#pragma once
#include <iostream>
#include "parser.h"
#include "rapidcsv.hpp"

using namespace std;

bool isLocked(string table, string scheme);
void locker(string table, string scheme);
bool tableExist(string table, Tables* head);
bool columnExist(string table, string columnName, Tables* head);
void splitDot(string message, string& table, string& column);