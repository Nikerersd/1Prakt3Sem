#include <iostream>
#include "delete.h"

void deleteRows(string command, JsonTable& jstab) {
    istringstream iss(command);
    string message;
    iss >> message;
    iss >> message;
    if (message != "FROM") {
        cerr << "Неверный ввод данных." << endl;
        return;
    }
    iss >> message;
    string table = message;
    if (!tableExist(table, jstab.head)) {
        cerr << "Таблицы: " << table << " не существует" << endl;
        return;
    }

    if (isLocked(table, jstab.scheme)) {
        cerr << "Файл заблокирован для редактирования" << endl;
        return;
    }
    string command2;
    cout << "<< ";
    getline(cin, command2);
    istringstream iss2(command2);
    string message2;
    iss2 >> message2;
    if (message2 != "WHERE") {
        cerr << "Неверный ввод данных." << endl;
        return;
    }
    iss2 >> message2;
    string table2;
    string column;
    separateDot(message2, table2, column);
    if (table2 != table) {
        cerr << "Неверный ввод данных." << endl;
        return;
    }
    if (!columnExist(table, column, jstab.head)) {
        cerr << "Данной колонки нет в таблице." << endl;
        return;
    }
    iss2 >> message2;
    if (message2 != "=") {
        cerr << "Неверный ввод данных." << endl;
        return;
    }
    iss2 >> message2;
    if (message2.front() != '\'' || message2.back() != '\'') {
        cerr << "Неверный ввод данных." << endl;
        return;
    }
    string value;
    for (size_t i=1; i < message2.size() - 1; i++) {
        value += message2[i];
    }

    locker(table, jstab.scheme);

    size_t csvCount = 1;
    while (true) {
        filesystem::path csvPath = filesystem::current_path() / jstab.scheme / table / (to_string(csvCount) + ".csv");
        ifstream file(csvPath);
        if (!file.is_open()) {
            break;
        }
        file.close();
        csvCount++;
    }
    for (size_t i = 1; i < csvCount; i++) {
        filesystem::path csvPath = filesystem::current_path() / jstab.scheme / table / (to_string(i) + ".csv");
        rapidcsv::Document csvDoc(csvPath.string());
        int index = csvDoc.GetColumnIdx(column);
        size_t countRows = csvDoc.GetRowCount();
        for (size_t j = 0; j < countRows; j++) {
            if (csvDoc.GetCell<string>(index, j) == value) {
                csvDoc.RemoveRow(j);
                countRows--;
                j--;
            }
        }
        csvDoc.Save(csvPath.string());
    }
    locker(table, jstab.scheme);
}