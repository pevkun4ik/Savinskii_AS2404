#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <map>
#include <sstream>
#include <vector>
#include "Pipe.h"
#include "utils.h"

using namespace std;

int Pipe::nextId = 1;

// Сохранение трубы в файл
void Pipe::savePipe(ofstream &ofs) const {
    ofs << id << '\n'
        << km_mark << '\n'
        << length_km << '\n'
        << diameter_mm << '\n'
        << in_repair << '\n';
}

// Загрузка трубы из файла
bool Pipe::loadPipe(ifstream &ifs, Pipe &p) {
    if (!(ifs >> p.id)) return false;
    ifs.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!getline(ifs, p.km_mark)) return false;
    if (!(ifs >> p.length_km)) return false;
    if (!(ifs >> p.diameter_mm)) return false;
    if (!(ifs >> p.in_repair)) return false;
    ifs.ignore(numeric_limits<streamsize>::max(), '\n');
    return true;
}

// Ввод трубы с консоли
void Pipe::readPipeFromConsole() {
    km_mark = readLineNonEmpty("Введите километровую отметку (название трубы): ");
    length_km = readDouble("Введите длину трубы (км): ", 0.0, 1e6);
    diameter_mm = readInt("Введите диаметр трубы (мм): ", 1, 100000);
    in_repair = readInt("Труба в ремонте? (1 - да, 0 - нет): ", 0, 1);
}

// Переключение состояния ремонта
void Pipe::toggleRepair() {
    cout << "Текущее состояние: " << (in_repair ? "В ремонте" : "Не в ремонте") << "\n";
    in_repair = !in_repair;
    cout << "Новое состояние: " << (in_repair ? "В ремонте" : "Не в ремонте") << "\n";
}

// Печать трубы
void Pipe::print() const {
    cout << "=== Труба ===\n";
    cout << "ID: " << id << "\n";
    cout << "Километровая отметка: " << km_mark << "\n";
    cout << "Длина (км): " << length_km << "\n";
    cout << "Диаметр (мм): " << diameter_mm << "\n";
    cout << "В ремонте: " << (in_repair ? "Да" : "Нет") << "\n";
}

// Оператор ввода
istream& operator>>(istream& in, Pipe& p) {
    p.id = Pipe::nextId++;
    p.km_mark = readLineNonEmpty("Введите километровую отметку (название трубы): ");
    p.length_km = readDouble("Введите длину трубы (км, положительное число): ", 0.0, 1e6);
    p.diameter_mm = readInt("Введите диаметр (мм, положительное целое): ", 1, 100000);
    p.in_repair = readInt("Труба в ремонте? (1 - да, 0 - нет): ", 0, 1);
    return in;
}

// Оператор вывода
ostream& operator<<(ostream& out, const Pipe& pipe) {
    out << "Километровая отметка: " << pipe.km_mark << ";\n"
        << "Длина (км): " << pipe.length_km << ";\n"
        << "Диаметр (мм): " << pipe.diameter_mm << ";\n"
        << "В ремонте: " << (pipe.in_repair ? "Да" : "Нет") << ";\n\n";
    return out;
}

// Поиск труб по фильтрам
void Pipe::findByFilter(const map<int, Pipe>& pipes) {
    string kmMarkFilter = readLineNonEmpty("Введите часть названия для поиска (Enter - пропустить): ");
    int repairFilter = readInt("Фильтр по ремонту (0 - нет, 1 - да, 2 - любой): ", 0, 2);

    bool found = false;
    for (const auto& [id, p] : pipes) {
        bool matchName = kmMarkFilter.empty() || p.km_mark.find(kmMarkFilter) != string::npos;
        bool matchRepair = (repairFilter == 2) || (p.in_repair == (repairFilter == 1));

        if (matchName && matchRepair) {
            cout << "ID: " << id << "\n";
            p.print();
            cout << "\n";
            found = true;
        }
    }

    if (!found)
        cout << "Трубы, соответствующие фильтру, не найдены.\n";

    logAction("Поиск труб (фильтр: '" + kmMarkFilter + "', ремонт=" + to_string(repairFilter) + ")");
}

// Пакетное редактирование
void Pipe::batchEdit(map<int, Pipe>& pipes) {
    if (pipes.empty()) {
        cout << "Нет труб для редактирования.\n";
        return;
    }

    cout << "1. Изменить состояние всех труб\n";
    cout << "2. Изменить состояние выбранных труб по ID\n";
    int choice = readInt("Выберите действие: ", 1, 2);

    if (choice == 1) {
        int newState = readInt("Установить 'в ремонте'? (1 - да, 0 - нет): ", 0, 1);
        for (auto& [id, pipe] : pipes) pipe.in_repair = (newState == 1);
        cout << "Изменения применены ко всем трубам.\n";
        logAction("Пакетное редактирование всех труб");
    } else {
        cout << "Введите ID труб через пробел: ";
        string line;
        getline(cin, line);
        if (line.empty()) getline(cin, line);
        istringstream iss(line);
        vector<int> ids;
        int id;
        while (iss >> id) ids.push_back(id);

        int newState = readInt("Установить 'в ремонте'? (1 - да, 0 - нет): ", 0, 1);
        for (int id : ids) {
            auto it = pipes.find(id);
            if (it != pipes.end()) {
                it->second.in_repair = (newState == 1);
            } else {
                cout << "Труба с ID " << id << " не найдена.\n";
            }
        }
        logAction("Пакетное редактирование выбранных труб");
    }
}