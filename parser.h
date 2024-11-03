#pragma once
#include <iostream>
#include <fstream>
#include "json.hpp" // json

#include "insert.h" // структура таблиц

using namespace std;
using json = nlohmann::json;

struct tableJson { 
    int tableSize; // размер колонок
    string schemeName; // имя схемы
    tNode* tablehead; // указатель на голову таблицы
};

void createFiles(const json& structure, tableJson& tjs); // создание полной директории и файлов
void parsing(tableJson& tjs); // парсинг схемы