#pragma once
#include <iostream>
#include "parser.h"
#include "tableWork.h"
#include "rapidcsv.hpp"

using namespace std;

void deleteRows(const string& command, JsonTable& jstab);