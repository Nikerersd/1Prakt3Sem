#include <iostream>
#include "insert.h"
#include "delete.h"
#include "parser.h"
#include "tableWork.h"
#include "select.h"

using namespace std;

int main() {
    JsonTable jstab;
    parser(jstab);
    string command;
    while (true) {
        cout << "<< ";
        getline(cin, command);
        if (command == "") {
            continue;
        }

        istringstream iss(command);
        string firstMessage;
        iss >> firstMessage;

        if (firstMessage == "INSERT") {
            insert(command, jstab);
        }
        if (firstMessage == "DELETE") {
            deleteRows(command, jstab);
        }
        if (firstMessage == "SELECT") {
            select(command, jstab);
        }
        if (firstMessage == "EXIT") {
            return 1;
        }
    }
}