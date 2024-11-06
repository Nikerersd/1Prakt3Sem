#include "tableWork.h"

bool isLocked(string table, string scheme) {
    filesystem::path filePath = filesystem::current_path() / scheme / table / (table + "_lock.txt");
    ifstream file(filePath);
    if (!file.is_open()) {
      cerr << "Не удалось открыть файл." << endl;
      return true;
    }
    string lock;
    file >> lock;
    file.close();
    if (lock == "locked") return true;
    return false;
}

void locker(string table, string scheme) {
    filesystem::path filePath = filesystem::current_path() / scheme / table / (table + "_lock.txt");
    ifstream file(filePath);
    if (!file.is_open()) {
      cerr << "Не удалось открыть файл." << endl;
      return;
    }
    string lock;
    file >> lock;
    file.close();
    ofstream fileOut(filePath);
    if (!fileOut.is_open()) {
      cerr << "Не удалось открыть файл." << endl;
      return;
    }
    if (lock == "locked") {
        fileOut << "unlocked";
    }
    else {
        fileOut << "locked";
    }
    fileOut.close();
}

bool tableExist(string table, Tables* head) {
    Tables* current = head;
    while (current != nullptr) {
        if (current->name == table) {
            return true;
        }
        current = current->next;
    }
    return false;
}

