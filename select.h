#pragma once
#include <iostream>
#include "parser.h"
#include "insert.h"
#include "tableWork.h"
#include "rapidcsv.hpp"

using namespace std;

void select(string command, JsonTable& jstab);