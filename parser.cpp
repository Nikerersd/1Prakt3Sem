#include "parser.h"

void createFiles(const json& structure, tableJson& tjs) { // создание полной директории и файлов
    tNode* tableHead = nullptr; // указатель на первый узел списка таблиц
    tNode* tableTail = nullptr; // указатель на последний узел списка таблиц

    for (const auto& table : structure.items()) { // цикл по всем элементам в структуре json
        tNode* newTable = new tNode{table.key(), nullptr, nullptr}; // создаём таблицу
        string lock = table.key() + "_lock.txt"; // создаём файл блокировки
        ofstream file(lock);
        if (!file.is_open()) {
            cerr << "Не удалось открыть файл." << endl;
        }
        file << "unlocked"; // по умолчанию разблокировано
        file.close();
        if (tableHead == nullptr) { // добавляем таблицу в список
            tableHead = newTable; // если список пустой, таблица будет и первой, и последней одновременно
            tableTail = newTable;
        }
        else {
            tableTail->next = newTable; // иначе добавляем новую таблицу в конец списка
            tableTail = newTable; 
        }

        string keyColumn = table.key() + "_pk"; // название специальной колонки
        Node* column_pk = new Node{keyColumn, nullptr}; // создаём список, где специальная колонка - первая
        newTable->column = column_pk; // присоединяем список колонок к таблице

        string csvFileName = table.key() + ".csv"; // создаём csv файл
        ofstream csvFile(csvFileName);
        if (!csvFile.is_open()) {
            cerr << "Не удалось создать файл: " << csvFileName << endl;
            return;
        }
        csvFile << keyColumn << ",";
        const auto& columns = table.value(); // запись колонок в файл, объект columns = названия
        for (size_t i = 0; i < columns.size(); i++) { 
            csvFile << columns[i].get<string>(); // записываем названия без кавычек
            Node* newColumn = new Node{columns[i], nullptr}; // создаём новую колонку
            if (newTable->column == nullptr) { // если в таблице ещё нет колонок
                newTable->column = newColumn;
            }
            else {
                Node* lastColumn = newTable->column;
                while (lastColumn->next != nullptr) { // ищем последнюю колонку
                    lastColumn = lastColumn->next;
                }
                lastColumn->next = newColumn; // добавляем новую колонку в конец
            }
            if (i < columns.size() - 1) { // для последнего значения не нужна запятая
                csvFile << ",";
            }
        }
        csvFile << endl;
        csvFile.close();
        cout << "Создан файл: " << csvFileName << endl;

        string pk = keyColumn + "_sequence.txt"; // создаём файл для хранения уникального первичного ключа
        ofstream filePk(pk);
        if (!filePk.is_open()) {
            cerr << "Не удалось открыть файл.\n";
        }
        filePk << "0";
        filePk.close();
    }
    tjs.tablehead = tableHead;
}

void parsing(tableJson& tjs) { // парсинг схемы
    string filename = "schema.json"; // название файла
    ifstream file(filename); // открываем
    if (!file.is_open()) {
        cerr << "Не удалось открыть файл: " << filename << endl;
        return;
    }
    string json_content; // содержимое
    string line; // строка
    while(getline(file, line)) { // чтение из файла
        json_content += line;
    }
    file.close();

    json jparsed; // хранение результата парсинга
    jparsed = json::parse(json_content); // парсинг в объект jparsed

    tjs.schemeName = jparsed["name"]; // извлекаем имя схемы
    if (jparsed.contains("structure")) { // наполнение директории
        createFiles(jparsed["structure"], tjs);
    }
    tjs.tableSize = jparsed["tuples_limit"]; // вытаскиваем ограничения по строкам
}