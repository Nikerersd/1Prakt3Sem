#pragma once
#include <iostream>
#include "parser.h"
#include "tableWork.h"
#include "rapidcsv.hpp"

using namespace std;

void cpColumns(const filesystem::path& fileIn, const filesystem::path& fileOut);
void insert(const string& command, JsonTable& jstab);