#include "insert.h"

void cpColumns(const filesystem::path& fileIn, const filesystem::path& fileOut) {
    string columns;
    ifstream file(fileIn);
    if (!file.is_open()) {
      cerr << "Не удалось открыть файл: " << fileIn << endl;
      return;
    }
    file >> columns;
    file.close();
    ofstream fileO(fileOut);
    if (!fileO.is_open()) {
      cerr << "Не удалось открыть файл: " << fileOut << endl;
      return;
    }
    fileO << columns << endl;
    fileO.close();
}

void insert(const string& command, JsonTable& jstab) {
    istringstream iss(command);
    string message;
    iss >> message;
    iss >> message;
    if (message!="INTO") {
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

    iss >> message;
    if (message!="VALUES") {
        cerr << "Неверный ввод данных." << endl;
        return;
    }
    string values;
    while (iss >> message) {
        values += message;
    }
    if (values.front() != '(' || values.back()!= ')') {
        cerr << "Неверный ввод данных." << endl;
        return;
    }

    locker(table, jstab.scheme);
    filesystem::path pkPath = filesystem::current_path() / jstab.scheme / table / (table + "_pk_sequence.txt");
    ifstream file(pkPath);
    if (!file.is_open()) {
      cerr << "Не удалось открыть файл: " << pkPath << endl;
      return;
    }
    int primKey;
    file >> primKey;
    file.close();
    primKey++;
    ofstream fileO(pkPath);
    if (!fileO.is_open()) {
      cerr << "Не удалось открыть файл: " << pkPath << endl;
      return;
    }
    fileO << primKey;
    fileO.close();

    int csvCount = 1;
    while (true) {
        filesystem::path csvPath = filesystem::current_path() / jstab.scheme / table / (to_string(csvCount) + ".csv");
        ifstream file(csvPath);
        if (!file.is_open()) {
            ofstream fileO(csvPath);
            if (!fileO.is_open()) {
                cerr << "Не удалось открыть файл: " << csvPath << endl;
                return;
            }
            fileO.close();
        }
        else {
            file.close();
        }
        rapidcsv::Document csvdoc(csvPath.string());
        if (csvdoc.GetRowCount() < jstab.rowsCount) {
            break;
        }
        csvCount++;
    }
    filesystem::path csvFirstPath = filesystem::current_path() / jstab.scheme / table / "1.csv";
    filesystem::path csvEndPath = filesystem::current_path() / jstab.scheme / table / (to_string(csvCount) + ".csv");
    rapidcsv::Document doc(csvEndPath.string());
    if (doc.GetRowCount() == 0) {
        cpColumns(csvFirstPath, csvEndPath);
    }
    ofstream csvFile(csvEndPath, ios::app);
    if (!csvFile.is_open()) {
      cerr << "Не удалось открыть файл: " << csvEndPath << endl;
      return;
    }
    csvFile << primKey << ",";
    for (size_t i=1; i < values.size(); i++) {
        if (values[i] == '\'') {
            i++;
            while (values[i]!='\'') {
                csvFile << values[i];
                i++;
            }
            if (values[i+1] != ')') {
                csvFile << ",";
            }
            else {
                csvFile << endl;
            }
        }
    }
    csvFile.close();
    locker(table, jstab.scheme);
}