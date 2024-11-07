#include <iostream>
#include "select.h"

void select(string command, JsonTable& jstab) {
    istringstream iss(command);
    string message;
    iss >> message;
    iss >> message;
    string table1, column1;
    separateDot(message, table1, column1);
    if (!tableExist(table1, jstab.head)) {
        cerr << "Данной таблицы ("<< table1 << ") нет в списке." << endl;
        return;
    }
    if (!columnExist(table1, column1, jstab.head)) {
        cerr << "Данной колонки ("<< column1 << ") нет в таблице." << endl;
        return;
    }
    iss >> message;
    string table2, column2;
    separateDot(message, table2, column2);
    if (!tableExist(table2, jstab.head)) {
        cerr << "Данной таблицы ("<< table2 << ")нет в списке." << endl;
        return;
    }
    if (!columnExist(table2, column2, jstab.head)) {
        cerr << "Данной колонки ("<< column2 << ") нет в таблице." << endl;
        return;
    }

    string command2;
    cout << "<< ";
    getline(cin, command2);
    istringstream iss2(command2);
    string message2;
    iss2 >> message2;
    if (message2 != "FROM") {
        cerr << "Неверный ввод данных." << endl;
        return;
    }

    iss2 >> message2;
    message2.pop_back();
    if (message2 != table1) {
        cerr << "Неверный ввод данных." << endl;
        return;
    }

    iss2 >> message2;
    if (message2 != table2) {
        cerr << "Неверный ввод данных." << endl;
        return;
    }

    string command3;
    cout << "<< ";
    getline(cin, command3);
    istringstream iss3(command3);
    string message3;
    iss3 >> message3;
    if (message3 == "") {
        crossJoin(table1, column1, table2, column2, jstab);
        return;
    }
    else if (message3 != "WHERE") {
        cerr << "Неверный ввод данных." << endl;
        return;
    }
    else {
        iss3 >> message3;
        string tableCond1, columnCond1;
        separateDot(message3, tableCond1, columnCond1);
        if (!tableExist(tableCond1, jstab.head)) {
            cerr << "Данной таблицы ("<< tableCond1 << ") нет в списке." << endl;
            return;
        }
        if (!columnExist(tableCond1, columnCond1, jstab.head)) {
            cerr << "Данной колонки ("<< columnCond1 << ") нет в таблице." << endl;
            return;
        }

        iss3 >> message3;
        if (message3 != "=") {
            cerr << "Неверный ввод данных." << endl;
            return;
        }

        iss3 >> message3;
        if (isDot(message3) == true) {
            string tableCond2, columnCond2;
            separateDot(message3, tableCond2, columnCond2);
            if (tableCond1 != table1) {
                cerr << "Неверный ввод данных." << endl;
                return;
            }
            if (tableCond2 != table2) {
                cerr << "Неверный ввод данных." << endl;
                return;
            }
            if (!tableExist(tableCond2, jstab.head)) {
                cerr << "Данной таблицы ("<< tableCond2 << ") нет в списке." << endl;
                return;
            }
            if (!columnExist(tableCond2, columnCond2, jstab.head)) {
                cerr << "Данной колонки ("<< columnCond2 << ") нет в таблице." << endl;
                return;
            }

            iss3 >> message3;
            if (message3 != "AND" && message3 != "OR") {
                condSingleT(jstab, tableCond1, tableCond2, columnCond1, columnCond2);
                crossJoinDepend(table1, column1, table2, column2, jstab);
                size_t icsv = 1;
                while (true) {
                    filesystem::path csvPathRes1 = filesystem::current_path() / jstab.scheme / tableCond1 / ("res_" + to_string(icsv) + ".csv");
                    filesystem::path csvPathRes2 = filesystem::current_path() / jstab.scheme / tableCond2 / ("res_" + to_string(icsv) + ".csv");
                    if (!filesystem::exists(csvPathRes2) && !filesystem::exists(csvPathRes1)) break;
                    if (filesystem::exists(csvPathRes1)) filesystem::remove(csvPathRes1);
                    if (filesystem::exists(csvPathRes2)) filesystem::remove(csvPathRes2);
                    icsv++;
                }
            }
            if (message3 == "AND") {
                iss3 >> message3;
                string tableCond3, columnCond3;
                separateDot(message3, tableCond3, columnCond3);
                if (tableCond3 != table1 && tableCond3 != table2) {
                    cerr << "Неверный ввод данных." << endl;
                    return;
                }
                if (!tableExist(tableCond3, jstab.head)) {
                    cerr << "Данной таблицы ("<< tableCond3 << ") нет в списке." << endl;
                    return;
                }
                if (!columnExist(tableCond3, columnCond3, jstab.head)) {
                    cerr << "Данной колонки ("<< columnCond3 << ") нет в таблице." << endl;
                    return;
                }
                iss3 >> message3;
                if (message3 != "=") {
                    cerr << "Неверный ввод данных." << endl;
                    return;
                }
                iss3 >> message3;
                string sCond;
                for (size_t i = 0; i < message3.size(); i++) {
                    if (message3[i] != '\'') {
                        sCond += message3[i];
                    }
                }
                condAnd(jstab, tableCond1, tableCond2, columnCond1, columnCond2, tableCond3, columnCond3, sCond);
                crossJoinDepend(table1, column1, table2, column2, jstab);
                size_t icsv = 1;
                while (true) {
                    filesystem::path csvPathRes1 = filesystem::current_path() / jstab.scheme / tableCond1 / ("res_" + to_string(icsv) + ".csv");
                    filesystem::path csvPathRes2 = filesystem::current_path() / jstab.scheme / tableCond2 / ("res_" + to_string(icsv) + ".csv");
                    if (!filesystem::exists(csvPathRes2) && !filesystem::exists(csvPathRes1)) break;
                    if (filesystem::exists(csvPathRes1)) filesystem::remove(csvPathRes1);
                    if (filesystem::exists(csvPathRes2)) filesystem::remove(csvPathRes2);
                    icsv++;
                }
            }
            if (message3 == "OR") {
                iss3 >> message3;
                string tableCond3, columnCond3;
                separateDot(message3, tableCond3, columnCond3);
                if (tableCond3 != table1 && tableCond3 != table2) {
                    cerr << "Неверный ввод данных." << endl;
                    return;
                }
                if (!tableExist(tableCond3, jstab.head)) {
                    cerr << "Данной таблицы ("<< tableCond3 << ") нет в списке." << endl;
                    return;
                }
                if (!columnExist(tableCond3, columnCond3, jstab.head)) {
                    cerr << "Данной колонки ("<< columnCond3 << ") нет в таблице." << endl;
                    return;
                }
                iss3 >> message3;
                if (message3 != "=") {
                    cerr << "Неверный ввод данных." << endl;
                    return;
                }
                iss3 >> message3;
                string sCond;
                for (size_t i = 0; i < message3.size(); i++) {
                    if (message3[i] != '\'') {
                        sCond += message3[i];
                    }
                }
                condOr(jstab, tableCond1, tableCond2, columnCond1, columnCond2, tableCond3, columnCond3, sCond);
                crossJoinDepend(table1, column1, table2, column2, jstab);
                size_t icsv = 1;
                while (true) {
                    filesystem::path csvPathRes1 = filesystem::current_path() / jstab.scheme / tableCond1 / ("res_" + to_string(icsv) + ".csv");
                    filesystem::path csvPathRes2 = filesystem::current_path() / jstab.scheme / tableCond2 / ("res_" + to_string(icsv) + ".csv");
                    if (!filesystem::exists(csvPathRes2) && !filesystem::exists(csvPathRes1)) break;
                    if (filesystem::exists(csvPathRes1)) filesystem::remove(csvPathRes1);
                    if (filesystem::exists(csvPathRes2)) filesystem::remove(csvPathRes2);
                    icsv++;
                }
            }
        }

        else {
            if (tableCond1 != table1 && tableCond1 != table2) {
                cerr << "Неверный ввод данных." << endl;
                return;
            }
            string sCond;
            for (size_t i = 0; i < message3.size(); i++) {
                if (message3[i] != '\'') {
                sCond += message3[i];
                }
            }
            iss3 >> message3;
            if (message3 != "AND" && message3 != "OR") {
                if (tableCond1 == table1) {
                    condSingleS(jstab, tableCond1, table2, columnCond1, sCond);
                    crossJoinDepend(table1, column1, table2, column2, jstab);
                    size_t icsv = 1;
                    while (true) {
                        filesystem::path csvPathRes1 = filesystem::current_path() / jstab.scheme / tableCond1 / ("res_" + to_string(icsv) + ".csv");
                        filesystem::path csvPathRes2 = filesystem::current_path() / jstab.scheme / table2 / ("res_" + to_string(icsv) + ".csv");
                        if (!filesystem::exists(csvPathRes2) && !filesystem::exists(csvPathRes1)) break;
                        if (filesystem::exists(csvPathRes1)) filesystem::remove(csvPathRes1);
                        if (filesystem::exists(csvPathRes2)) filesystem::remove(csvPathRes2);
                        icsv++;
                    }
                }
                if (tableCond1 == table2) {
                    condSingleS(jstab, tableCond1, table1, columnCond1, sCond);
                    crossJoinDepend(table1, column1, table2, column2, jstab);
                    size_t icsv = 1;
                    while (true) {
                        filesystem::path csvPathRes1 = filesystem::current_path() / jstab.scheme / tableCond1 / ("res_" + to_string(icsv) + ".csv");
                        filesystem::path csvPathRes2 = filesystem::current_path() / jstab.scheme / table1 / ("res_" + to_string(icsv) + ".csv");
                        if (!filesystem::exists(csvPathRes2) && !filesystem::exists(csvPathRes1)) break;
                        if (filesystem::exists(csvPathRes1)) filesystem::remove(csvPathRes1);
                        if (filesystem::exists(csvPathRes2)) filesystem::remove(csvPathRes2);
                        icsv++;
                    }
                }
            }
            if (message3 == "AND") {
                iss3 >> message3;
                string tableCond2, columnCond2;
                separateDot(message3, tableCond2, columnCond2);
                if (tableCond2 != table1) {
                    cerr << "Неверный ввод данных." << endl;
                    return;
                }
                if (!tableExist(tableCond2, jstab.head)) {
                    cerr << "Данной таблицы ("<< tableCond2 << ") нет в списке." << endl;
                    return;
                }
                if (!columnExist(tableCond2, columnCond2, jstab.head)) {
                    cerr << "Данной колонки ("<< columnCond2 << ") нет в таблице." << endl;
                    return;
                }
                iss3 >> message3;
                if (message3 != "=") {
                    cerr << "Неверный ввод данных." << endl;
                    return;
                }

                iss3 >> message3;
                string tableCond3, columnCond3;
                separateDot(message3, tableCond3, columnCond3);
                if (tableCond3 != table2) {
                    cerr << "Неверный ввод данных." << endl;
                    return;
                }
                if (!tableExist(tableCond3, jstab.head)) {
                    cerr << "Данной таблицы ("<< tableCond3 << ") нет в списке." << endl;
                    return;
                }
                if (!columnExist(tableCond3, columnCond3, jstab.head)) {
                    cerr << "Данной колонки ("<< columnCond3 << ") нет в таблице." << endl;
                    return;
                }

                condAnd(jstab, tableCond2, tableCond3, columnCond2, columnCond3, tableCond1, columnCond1, sCond);
                crossJoinDepend(table1, column1, table2, column2, jstab);
                size_t icsv = 1;
                while (true) {
                    filesystem::path csvPathRes1 = filesystem::current_path() / jstab.scheme / tableCond2 / ("res_" + to_string(icsv) + ".csv");
                    filesystem::path csvPathRes2 = filesystem::current_path() / jstab.scheme / tableCond3 / ("res_" + to_string(icsv) + ".csv");
                    if (!filesystem::exists(csvPathRes2) && !filesystem::exists(csvPathRes1)) break;
                    if (filesystem::exists(csvPathRes1)) filesystem::remove(csvPathRes1);
                    if (filesystem::exists(csvPathRes2)) filesystem::remove(csvPathRes2);
                    icsv++;
                }
            }
            if (message3 == "OR") {
                iss3 >> message3;
                string tableCond2, columnCond2;
                separateDot(message3, tableCond2, columnCond2);
                if (tableCond2 != table1) {
                    cerr << "Неверный ввод данных." << endl;
                    return;
                }
                if (!tableExist(tableCond2, jstab.head)) {
                    cerr << "Данной таблицы ("<< tableCond2 << ") нет в списке." << endl;
                    return;
                }
                if (!columnExist(tableCond2, columnCond2, jstab.head)) {
                    cerr << "Данной колонки ("<< columnCond2 << ") нет в таблице." << endl;
                    return;
                }
                iss3 >> message3;
                if (message3 != "=") {
                    cerr << "Неверный ввод данных." << endl;
                    return;
                }

                iss3 >> message3;
                string tableCond3, columnCond3;
                separateDot(message3, tableCond3, columnCond3);
                if (tableCond3 != table2) {
                    cerr << "Неверный ввод данных." << endl;
                    return;
                }
                if (!tableExist(tableCond3, jstab.head)) {
                    cerr << "Данной таблицы ("<< tableCond3 << ") нет в списке." << endl;
                    return;
                }
                if (!columnExist(tableCond3, columnCond3, jstab.head)) {
                    cerr << "Данной колонки ("<< columnCond3 << ") нет в таблице." << endl;
                    return;
                }

                condOr(jstab, tableCond2, tableCond3, columnCond2, columnCond3, tableCond1, columnCond1, sCond);
                crossJoinDepend(table1, column1, table2, column2, jstab);
                size_t icsv = 1;
                while (true) {
                    filesystem::path csvPathRes1 = filesystem::current_path() / jstab.scheme / tableCond2 / ("res_" + to_string(icsv) + ".csv");
                    filesystem::path csvPathRes2 = filesystem::current_path() / jstab.scheme / tableCond3 / ("res_" + to_string(icsv) + ".csv");
                    if (!filesystem::exists(csvPathRes2) && !filesystem::exists(csvPathRes1)) break;
                    if (filesystem::exists(csvPathRes1)) filesystem::remove(csvPathRes1);
                    if (filesystem::exists(csvPathRes2)) filesystem::remove(csvPathRes2);
                    icsv++;
                }
            }
        }
    }
}