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

bool columnExist(string table, string columnName, Tables* head) {
    Tables* current = head;
    while (current != nullptr) {
        if (current->name == table) {
            Node* currentColumn = current->column;
            while (current != nullptr) {
                if (currentColumn->data == columnName) {
                    return true;
                }
                currentColumn = currentColumn->next;
            }
        }
        current = current->next;
    }
    return false;
}

void separateDot(string message, string& table, string& column) {
    bool isDot = false;
    for (size_t i=0; i < message.size(); i++) {
        if (message[i] == '.') {
            isDot = true;
            i++;
        }
        if (message[i] == ',') {
            continue;
        }
        if (isDot == false) table += message[i];
        else column += message[i];
    }
    if (isDot == false) {
        cerr << "Неверный ввод данных." << endl;
        return;
    }
}

void crossJoin(string table1, string column1, string table2, string column2, JsonTable& jstab) {
    size_t csvCount1 = 1;
    while (true) {
        filesystem::path csvPath1 = filesystem::current_path() / jstab.scheme / table1 / (to_string(csvCount1) + ".csv");
        ifstream file1(csvPath1);
        if (!file1.is_open()) {
            break;
        }
        file1.close();
        csvCount1++;
    }
    size_t csvCount2 = 1;
    while (true) {
        filesystem::path csvPath2 = filesystem::current_path() / jstab.scheme / table2 / (to_string(csvCount2) + ".csv");
        ifstream file2(csvPath2);
        if (!file2.is_open()) {
            break;
        }
        file2.close();
        csvCount2++;
    }
    cout << "|" << setw(10) << left << (table1 + "_pk") << " |" << setw(10) << left << column1 << " |";
    cout << setw(10) << left << (table2 + "_pk") << " |" << setw(10) << left << column2 << " |" << endl;
    for (size_t csvi1 = 1; csvi1 < csvCount1; csvi1++) {
        filesystem::path csvPath1 = filesystem::current_path() / jstab.scheme / table1 / (to_string(csvi1) + ".csv");
        rapidcsv::Document csvDoc1(csvPath1.string());
        int idxColumn1 = csvDoc1.GetColumnIdx(column1);
        size_t rowsCount1 = csvDoc1.GetRowCount();
        for (size_t i=0; i < rowsCount1; i++) {
            for (size_t csvi2 = 1; csvi2 < csvCount2; csvi2++) {
                filesystem::path csvPath2 = filesystem::current_path() / jstab.scheme / table2 / (to_string(csvi2) + ".csv");
                rapidcsv::Document csvDoc2(csvPath2.string());
                int idxColumn2 = csvDoc2.GetColumnIdx(column2);
                size_t rowsCount2 = csvDoc2.GetRowCount();
                for (size_t j=0; j < rowsCount2; j++) {
                    cout << "|" << setw(10) << left << csvDoc1.GetCell<string>(0, i) << " |" << setw(10) << left << csvDoc1.GetCell<string>(idxColumn1, i) << " |";
                    cout << setw(10) << left << csvDoc2.GetCell<string>(0, j) << " |" << setw(10) << left << csvDoc2.GetCell<string>(idxColumn2, j) << " |" << endl;
                }
            }
        }
    }
}

void crossJoinDepend(string table1, string column1, string table2, string column2, JsonTable& jstab) {
    size_t csvCount1 = 1;
    while (true) {
        filesystem::path csvPath1 = filesystem::current_path() / jstab.scheme / table1 / ("res_" + to_string(csvCount1) + ".csv");
        ifstream file1(csvPath1);
        if (!file1.is_open()) {
            break;
        }
        file1.close();
        csvCount1++;
    }
    size_t csvCount2 = 1;
    while (true) {
        filesystem::path csvPath2 = filesystem::current_path() / jstab.scheme / table2 / ("res_" + to_string(csvCount2) + ".csv");
        ifstream file2(csvPath2);
        if (!file2.is_open()) {
            break;
        }
        file2.close();
        csvCount2++;
    }
    cout << "|" << setw(10) << left << (table1 + "_pk") << " |" << setw(10) << left << column1 << " |";
    cout << setw(10) << left << (table2 + "_pk") << " |" << setw(10) << left << column2 << " |" << endl;
    for (size_t csvi1 = 1; csvi1 < csvCount1; csvi1++) {
        filesystem::path csvPath1 = filesystem::current_path() / jstab.scheme / table1 / ("res_" + to_string(csvi1) + ".csv");
        rapidcsv::Document csvDoc1(csvPath1.string());
        int idxColumn1 = csvDoc1.GetColumnIdx(column1);
        size_t rowsCount1 = csvDoc1.GetRowCount();
        for (size_t i=0; i < rowsCount1; i++) {
            for (size_t csvi2 = 1; csvi2 < csvCount2; csvi2++) {
                filesystem::path csvPath2 = filesystem::current_path() / jstab.scheme / table2 / ("res_" + to_string(csvi2) + ".csv");
                rapidcsv::Document csvDoc2(csvPath2.string());
                int idxColumn2 = csvDoc2.GetColumnIdx(column2);
                size_t rowsCount2 = csvDoc2.GetRowCount();
                for (size_t j=0; j < rowsCount2; j++) {
                    cout << "|" << setw(10) << left << csvDoc1.GetCell<string>(0, i) << " |" << setw(10) << left << csvDoc1.GetCell<string>(idxColumn1, i) << " |";
                    cout << setw(10) << left << csvDoc2.GetCell<string>(0, j) << " |" << setw(10) << left << csvDoc2.GetCell<string>(idxColumn2, j) << " |" << endl;
                }
            }
        }
    }
}

void outputWithCondSingleT(JsonTable& jstab, string tableCond1, string tableCond2, string columnCond1, string columnCond2) {
    size_t i = 0;
    while (true) {
        size_t icsv = i/jstab.rowsCount + 1;
        size_t row = i%jstab.rowsCount;
        filesystem::path csvPathCond1 = filesystem::current_path() / jstab.scheme / tableCond1 / (to_string(icsv) + ".csv");
        filesystem::path csvPathCond2 = filesystem::current_path() / jstab.scheme / tableCond2 / (to_string(icsv) + ".csv");
        ifstream fileCond1(csvPathCond1);
        ifstream fileCond2(csvPathCond2);
        if (!fileCond1.is_open() || !fileCond2.is_open()) {
            break;
        }
        fileCond1.close();
        fileCond2.close();
        filesystem::path csvPathRes1 = filesystem::current_path() / jstab.scheme / tableCond1 / ("res_" + to_string(icsv) + ".csv");
        filesystem::path csvPathRes2 = filesystem::current_path() / jstab.scheme / tableCond2 / ("res_" + to_string(icsv) + ".csv");
        ifstream fileRes1(csvPathRes1);
        if (!fileRes1.is_open()) {
            ofstream fileRes1Out(csvPathRes1);
            if (!fileRes1Out.is_open()) {
                cerr << "Не удалось открыть файл: " << csvPathRes1 << endl;
                return;
            }
            fileRes1Out.close();
        }
        else {
            fileRes1.close();
        }

        ifstream fileRes2(csvPathRes2);
        if (!fileRes2.is_open()) {
            ofstream fileRes2Out(csvPathRes2);
            if (!fileRes2Out.is_open()) {
                cerr << "Не удалось открыть файл: " << csvPathRes2 << endl;
                return;
            }
            fileRes2Out.close();
        }
        else {
            fileRes2.close();
        }
        filesystem::path csvFirstPath1 = filesystem::current_path() / jstab.scheme / tableCond1 / "1.csv";
        filesystem::path csvEndPath1 = filesystem::current_path() / jstab.scheme / tableCond1 / ("res_" + to_string(icsv) + ".csv");
        rapidcsv::Document doc1(csvEndPath1.string());
        if (doc1.GetRowCount() == 0) {
            cpColumns(csvFirstPath1, csvEndPath1);
        }
        filesystem::path csvFirstPath2 = filesystem::current_path() / jstab.scheme / tableCond2 / "1.csv";
        filesystem::path csvEndPath2 = filesystem::current_path() / jstab.scheme / tableCond2 / ("res_" + to_string(icsv) + ".csv");
        rapidcsv::Document doc2(csvEndPath2.string());
        if (doc2.GetRowCount() == 0) {
            cpColumns(csvFirstPath2, csvEndPath2);
        }
        if (isDependenceTables(tableCond1, columnCond1, tableCond2, columnCond2, icsv, row, jstab)) {
            rapidcsv::Document docCond1(csvPathCond1.string());
            size_t columnsCount1 = docCond1.GetColumnCount();
            ofstream csvResFile1(csvPathRes1, ios::app);
            if (!csvResFile1.is_open()) {
            cerr << "Не удалось открыть файл: " << csvPathRes1 << endl;
            return;
            }
            for (size_t j=0; j < columnsCount1; j++) {
                if (j+1 != columnsCount1) {
                    string currentCell = docCond1.GetCell<string>(j, row);
                    csvResFile1 << currentCell << ",";
                }
                else {
                    string currentCell = docCond1.GetCell<string>(j, row);
                    csvResFile1 << currentCell;
                }
            }
            csvResFile1 << endl;
            csvResFile1.close();
            rapidcsv::Document docCond2(csvPathCond2.string());
            size_t columnsCount2 = docCond2.GetColumnCount();
            ofstream csvResFile2(csvPathRes2, ios::app);
            if (!csvResFile2.is_open()) {
            cerr << "Не удалось открыть файл: " << csvPathRes2 << endl;
            return;
            }
            for (size_t j=0; j < columnsCount2; j++) {
                if (j+1 != columnsCount2) {
                    string currentCell = docCond2.GetCell<string>(j, row);
                    csvResFile2 << currentCell << ",";
                }
                else {
                    string currentCell = docCond2.GetCell<string>(j, row);
                    csvResFile2 << currentCell;
                }
            }
            csvResFile2 << endl;
            csvResFile2.close();
        }
        i++;
    }
}

void outputWithCondSingleS(JsonTable& jstab, string tableCond1, string table, string columnCond1, string sCond) {
    size_t i = 0;
    while (true) {
        size_t icsv = i/jstab.rowsCount + 1;
        size_t row = i%jstab.rowsCount;
        filesystem::path csvPathCond1 = filesystem::current_path() / jstab.scheme / tableCond1 / (to_string(icsv) + ".csv");
        ifstream fileCond1(csvPathCond1);
        if (!fileCond1.is_open()) {
            break;
        }
        fileCond1.close();
        filesystem::path csvPathRes1 = filesystem::current_path() / jstab.scheme / tableCond1 / ("res_" + to_string(icsv) + ".csv");
        filesystem::path csvPathRes2 = filesystem::current_path() / jstab.scheme / table / ("res_" + to_string(icsv) + ".csv");
        ifstream fileRes1(csvPathRes1);
        if (!fileRes1.is_open()) {
            ofstream fileRes1Out(csvPathRes1);
            if (!fileRes1Out.is_open()) {
                cerr << "Не удалось открыть файл: " << csvPathRes1 << endl;
                return;
            }
            fileRes1Out.close();
        }
        else {
            fileRes1.close();
        }

        ifstream fileRes2(csvPathRes2);
        if (!fileRes2.is_open()) {
            ofstream fileRes2Out(csvPathRes2);
            if (!fileRes2Out.is_open()) {
                cerr << "Не удалось открыть файл: " << csvPathRes2 << endl;
                return;
            }
            fileRes2Out.close();
        }
        else {
            fileRes2.close();
        }
        filesystem::path csvFirstPath1 = filesystem::current_path() / jstab.scheme / tableCond1 / "1.csv";
        filesystem::path csvEndPath1 = filesystem::current_path() / jstab.scheme / tableCond1 / ("res_" + to_string(icsv) + ".csv");
        rapidcsv::Document doc1(csvEndPath1.string());
        if (doc1.GetRowCount() == 0) {
            cpColumns(csvFirstPath1, csvEndPath1);
        }
        filesystem::path csvFirstPath2 = filesystem::current_path() / jstab.scheme / table / "1.csv";
        filesystem::path csvEndPath2 = filesystem::current_path() / jstab.scheme / table / ("res_" + to_string(icsv) + ".csv");
        rapidcsv::Document doc2(csvEndPath2.string());
        if (doc2.GetRowCount() == 0) {
            cpColumns(csvFirstPath2, csvEndPath2);
        }
        if (isDependenceString(tableCond1, columnCond1, sCond, icsv, row, jstab)) {
            rapidcsv::Document docCond1(csvPathCond1.string());
            size_t columnsCount1 = docCond1.GetColumnCount();
            ofstream csvResFile1(csvPathRes1, ios::app);
            if (!csvResFile1.is_open()) {
            cerr << "Не удалось открыть файл: " << csvPathRes1 << endl;
            return;
            }
            for (size_t j=0; j < columnsCount1; j++) {
                if (j+1 != columnsCount1) {
                    string currentCell = docCond1.GetCell<string>(j, row);
                    csvResFile1 << currentCell << ",";
                }
                else {
                    string currentCell = docCond1.GetCell<string>(j, row);
                    csvResFile1 << currentCell;
                }
            }
            csvResFile1 << endl;
            csvResFile1.close();
            filesystem::path csvPath2 = filesystem::current_path() / jstab.scheme / table / (to_string(icsv) + ".csv");
            rapidcsv::Document docCond2(csvPath2.string());
            size_t columnsCount2 = docCond2.GetColumnCount();
            ofstream csvResFile2(csvPathRes2, ios::app);
            if (!csvResFile2.is_open()) {
            cerr << "Не удалось открыть файл: " << csvPathRes2 << endl;
            return;
            }
            for (size_t j=0; j < columnsCount2; j++) {
                if (j+1 != columnsCount2) {
                    string currentCell = docCond2.GetCell<string>(j, row);
                    csvResFile2 << currentCell << ",";
                }
                else {
                    string currentCell = docCond2.GetCell<string>(j, row);
                    csvResFile2 << currentCell;
                }
            }
            csvResFile2 << endl;
            csvResFile2.close();
        }
        i++;
    }
}

void outputWithCondAnd(JsonTable& jstab, string tableCond1, string tableCond2, string columnCond1, string columnCond2, string tableCond3, string columnCond3, string sCond) {
    size_t i = 0;
    while (true) {
        size_t icsv = i/jstab.rowsCount + 1;
        size_t row = i%jstab.rowsCount;
        filesystem::path csvPathCond1 = filesystem::current_path() / jstab.scheme / tableCond1 / (to_string(icsv) + ".csv");
        filesystem::path csvPathCond2 = filesystem::current_path() / jstab.scheme / tableCond2 / (to_string(icsv) + ".csv");
        ifstream fileCond1(csvPathCond1);
        ifstream fileCond2(csvPathCond2);
        if (!fileCond1.is_open() || !fileCond2.is_open()) {
            break;
        }
        fileCond1.close();
        fileCond2.close();
        filesystem::path csvPathRes1 = filesystem::current_path() / jstab.scheme / tableCond1 / ("res_" + to_string(icsv) + ".csv");
        filesystem::path csvPathRes2 = filesystem::current_path() / jstab.scheme / tableCond2 / ("res_" + to_string(icsv) + ".csv");
        ifstream fileRes1(csvPathRes1);
        if (!fileRes1.is_open()) {
            ofstream fileRes1Out(csvPathRes1);
            if (!fileRes1Out.is_open()) {
                cerr << "Не удалось открыть файл: " << csvPathRes1 << endl;
                return;
            }
            fileRes1Out.close();
        }
        else {
            fileRes1.close();
        }

        ifstream fileRes2(csvPathRes2);
        if (!fileRes2.is_open()) {
            ofstream fileRes2Out(csvPathRes2);
            if (!fileRes2Out.is_open()) {
                cerr << "Не удалось открыть файл: " << csvPathRes2 << endl;
                return;
            }
            fileRes2Out.close();
        }
        else {
            fileRes2.close();
        }
        filesystem::path csvFirstPath1 = filesystem::current_path() / jstab.scheme / tableCond1 / "1.csv";
        filesystem::path csvEndPath1 = filesystem::current_path() / jstab.scheme / tableCond1 / ("res_" + to_string(icsv) + ".csv");
        rapidcsv::Document doc1(csvEndPath1.string());
        if (doc1.GetRowCount() == 0) {
            cpColumns(csvFirstPath1, csvEndPath1);
        }
        filesystem::path csvFirstPath2 = filesystem::current_path() / jstab.scheme / tableCond2 / "1.csv";
        filesystem::path csvEndPath2 = filesystem::current_path() / jstab.scheme / tableCond2 / ("res_" + to_string(icsv) + ".csv");
        rapidcsv::Document doc2(csvEndPath2.string());
        if (doc2.GetRowCount() == 0) {
            cpColumns(csvFirstPath2, csvEndPath2);
        }
        if (isDependenceTables(tableCond1, columnCond1, tableCond2, columnCond2, icsv, row, jstab) && isDependenceString(tableCond3, columnCond3, sCond, icsv, row, jstab)) {
            rapidcsv::Document docCond1(csvPathCond1.string());
            size_t columnsCount1 = docCond1.GetColumnCount();
            ofstream csvResFile1(csvPathRes1, ios::app);
            if (!csvResFile1.is_open()) {
            cerr << "Не удалось открыть файл: " << csvPathRes1 << endl;
            return;
            }
            for (size_t j=0; j < columnsCount1; j++) {
                if (j+1 != columnsCount1) {
                    string currentCell = docCond1.GetCell<string>(j, row);
                    csvResFile1 << currentCell << ",";
                }
                else {
                    string currentCell = docCond1.GetCell<string>(j, row);
                    csvResFile1 << currentCell;
                }
            }
            csvResFile1 << endl;
            csvResFile1.close();
            rapidcsv::Document docCond2(csvPathCond2.string());
            size_t columnsCount2 = docCond2.GetColumnCount();
            ofstream csvResFile2(csvPathRes2, ios::app);
            if (!csvResFile2.is_open()) {
            cerr << "Не удалось открыть файл: " << csvPathRes2 << endl;
            return;
            }
            for (size_t j=0; j < columnsCount2; j++) {
                if (j+1 != columnsCount2) {
                    string currentCell = docCond2.GetCell<string>(j, row);
                    csvResFile2 << currentCell << ",";
                }
                else {
                    string currentCell = docCond2.GetCell<string>(j, row);
                    csvResFile2 << currentCell;
                }
            }
            csvResFile2 << endl;
            csvResFile2.close();
        }
        i++;
    }
}

void outputWithCondOr(JsonTable& jstab, string tableCond1, string tableCond2, string columnCond1, string columnCond2, string tableCond3, string columnCond3, string sCond) {
    size_t i = 0;
    while (true) {
        size_t icsv = i/jstab.rowsCount + 1;
        size_t row = i%jstab.rowsCount;
        filesystem::path csvPathCond1 = filesystem::current_path() / jstab.scheme / tableCond1 / (to_string(icsv) + ".csv");
        filesystem::path csvPathCond2 = filesystem::current_path() / jstab.scheme / tableCond2 / (to_string(icsv) + ".csv");
        ifstream fileCond1(csvPathCond1);
        ifstream fileCond2(csvPathCond2);
        if (!fileCond1.is_open() || !fileCond2.is_open()) {
            break;
        }
        fileCond1.close();
        fileCond2.close();
        filesystem::path csvPathRes1 = filesystem::current_path() / jstab.scheme / tableCond1 / ("res_" + to_string(icsv) + ".csv");
        filesystem::path csvPathRes2 = filesystem::current_path() / jstab.scheme / tableCond2 / ("res_" + to_string(icsv) + ".csv");
        ifstream fileRes1(csvPathRes1);
        if (!fileRes1.is_open()) {
            ofstream fileRes1Out(csvPathRes1);
            if (!fileRes1Out.is_open()) {
                cerr << "Не удалось открыть файл: " << csvPathRes1 << endl;
                return;
            }
            fileRes1Out.close();
        }
        else {
            fileRes1.close();
        }

        ifstream fileRes2(csvPathRes2);
        if (!fileRes2.is_open()) {
            ofstream fileRes2Out(csvPathRes2);
            if (!fileRes2Out.is_open()) {
                cerr << "Не удалось открыть файл: " << csvPathRes2 << endl;
                return;
            }
            fileRes2Out.close();
        }
        else {
            fileRes2.close();
        }
        filesystem::path csvFirstPath1 = filesystem::current_path() / jstab.scheme / tableCond1 / "1.csv";
        filesystem::path csvEndPath1 = filesystem::current_path() / jstab.scheme / tableCond1 / ("res_" + to_string(icsv) + ".csv");
        rapidcsv::Document doc1(csvEndPath1.string());
        if (doc1.GetRowCount() == 0) {
            cpColumns(csvFirstPath1, csvEndPath1);
        }
        filesystem::path csvFirstPath2 = filesystem::current_path() / jstab.scheme / tableCond2 / "1.csv";
        filesystem::path csvEndPath2 = filesystem::current_path() / jstab.scheme / tableCond2 / ("res_" + to_string(icsv) + ".csv");
        rapidcsv::Document doc2(csvEndPath2.string());
        if (doc2.GetRowCount() == 0) {
            cpColumns(csvFirstPath2, csvEndPath2);
        }
        if (isDependenceTables(tableCond1, columnCond1, tableCond2, columnCond2, icsv, row, jstab) || isDependenceString(tableCond3, columnCond3, sCond, icsv, row, jstab)) {
            rapidcsv::Document docCond1(csvPathCond1.string());
            size_t columnsCount1 = docCond1.GetColumnCount();
            ofstream csvResFile1(csvPathRes1, ios::app);
            if (!csvResFile1.is_open()) {
            cerr << "Не удалось открыть файл: " << csvPathRes1 << endl;
            return;
            }
            for (size_t j=0; j < columnsCount1; j++) {
                if (j+1 != columnsCount1) {
                    string currentCell = docCond1.GetCell<string>(j, row);
                    csvResFile1 << currentCell << ",";
                }
                else {
                    string currentCell = docCond1.GetCell<string>(j, row);
                    csvResFile1 << currentCell;
                }
            }
            csvResFile1 << endl;
            csvResFile1.close();
            rapidcsv::Document docCond2(csvPathCond2.string());
            size_t columnsCount2 = docCond2.GetColumnCount();
            ofstream csvResFile2(csvPathRes2, ios::app);
            if (!csvResFile2.is_open()) {
            cerr << "Не удалось открыть файл: " << csvPathRes2 << endl;
            return;
            }
            for (size_t j=0; j < columnsCount2; j++) {
                if (j+1 != columnsCount2) {
                    string currentCell = docCond2.GetCell<string>(j, row);
                    csvResFile2 << currentCell << ",";
                }
                else {
                    string currentCell = docCond2.GetCell<string>(j, row);
                    csvResFile2 << currentCell;
                }
            }
            csvResFile2 << endl;
            csvResFile2.close();
        }
        i++;
    }
}

bool isDot(string message) {
    for (size_t i = 0; i < message[i]; i++) {
        if (message[i] == '.') {
            return true;
        }
    }
    return false;
}

bool isDependenceTables(string table, string column, string tableCond, string columnCond, size_t csvNum, size_t row, JsonTable& jstab) {
    filesystem::path csvPath = filesystem::current_path() / jstab.scheme / table / (to_string(csvNum) + ".csv");
    filesystem::path csvPathCond = filesystem::current_path() / jstab.scheme / tableCond / (to_string(csvNum) + ".csv");
    rapidcsv::Document doc(csvPath.string());
    rapidcsv::Document docCond(csvPathCond.string());
    int idxColumn = doc.GetColumnIdx(column);
    int idxColumnCond = docCond.GetColumnIdx(columnCond);
    int rowsCount = doc.GetRowCount();
    int rowsCountCond = docCond.GetRowCount();
    if (row >= rowsCount || row >= rowsCountCond) return false;
    if (doc.GetCell<string>(idxColumn, row) == docCond.GetCell<string>(idxColumnCond, row)) return true;
    else return false;
}

bool isDependenceString(string table, string column, string sCond, size_t csvNum, size_t row, JsonTable& jstab) {
    filesystem::path csvPath = filesystem::current_path() / jstab.scheme / table / (to_string(csvNum) + ".csv");
    rapidcsv::Document doc(csvPath.string());
    int idxColumn = doc.GetColumnIdx(column);
    int rowsCount = doc.GetRowCount();
    if (row >= rowsCount) return false;
    if (doc.GetCell<string>(idxColumn, row) == sCond) return true;
    else return false;
}