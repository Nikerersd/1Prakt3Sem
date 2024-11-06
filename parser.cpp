#include <iostream>
#include "parser.h"

void removeDir(const filesystem::path& dirPath) {
  if (filesystem::exists(dirPath)) {
    filesystem::remove_all(dirPath);
  }
}

void createFiles(const filesystem::path& schemePath, const json& jsonStruct, JsonTable& jstable) {
  Tables* head = nullptr;
  Tables* tail = nullptr;

  for (const auto& table : jsonStruct.items()) {
    filesystem::path tableDir = schemePath / table.key();
    filesystem::create_directory(tableDir);
    if (!filesystem::exists(tableDir)) {
        cerr << "Не удалось создать директорию: " << tableDir << endl;
        return;
    }

    Tables* newTable = new Tables{table.key(), nullptr, nullptr};

    if (head == nullptr) {
      head = newTable;
      tail = newTable;
    }
    else {
      tail->next = newTable;
      tail = newTable;
    }

    filesystem::path lockDir = tableDir / (table.key() + "_lock.txt");
    ofstream file(lockDir);
    if (!file.is_open()) {
      cerr << "Не удалось открыть файл." << endl;
      return;
    }
    file << "unlocked";
    file.close();

    string pkColumn = table.key() + "_pk";
    Node* firstColumn = new Node{pkColumn, nullptr};

    Node* headColumn = firstColumn;
    Node* tailColumn = firstColumn;

    filesystem::path csvDir = tableDir / "1.csv";
    ofstream csvFile(csvDir);
    if (!csvFile.is_open()) {
      cerr << "Не удалось открыть файл." << endl;
      return;
    }
    csvFile << pkColumn << ",";
    
    const auto& columns = table.value();
    for (size_t i = 0; i < columns.size(); i++) {
      csvFile << columns[i].get<string>();
      Node* newColumn = new Node{columns[i].get<string>(), nullptr};
      tailColumn->next = newColumn;
      tailColumn = newColumn;
      if (i < columns.size() - 1) {
        csvFile << ",";
      }
    }
    csvFile << endl;
    csvFile.close();

    newTable->column = headColumn;

    filesystem::path sequence = tableDir / (pkColumn + "_sequence.txt");
    ofstream seqFile(sequence);
    if (!seqFile.is_open()) {
      cerr << "Не удалось открыть файл." << endl;
      return;
    }
    seqFile << "0";
    seqFile.close();
  }
  jstable.head = head;
}

void parser(JsonTable& jstab) {
    ifstream file("schema.json");
    if (!file.is_open()) {
        cerr << "Не удалось открыть файл: schema.json" << endl;
        return;
    }
    
    json jspars;
    file >> jspars;
    file.close();

    jstab.scheme = jspars["name"];
    filesystem::path schemePath = filesystem::current_path() / jstab.scheme;
    removeDir(schemePath); //Удаляем директорию с прошлого запуска
    filesystem::create_directory(schemePath);
    if (!filesystem::exists(schemePath)) {
        cerr << "Не удалось создать директорию: " << schemePath << endl;
        return;
    }

    if (jspars.contains("structure")) {
        createFiles(schemePath, jspars["structure"], jstab);
    }

    jstab.rowsCount = jspars["tuples_limit"];
}