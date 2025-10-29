#include "utils.h"
#include "filters.h"
#include <fstream>
#include <iostream>

ofstream logfile; // определяем глобальный лог

void SaveAll(unordered_map<int, Pipe>& pipes, unordered_map<int, Station>& stations) {
    cout << "Введите имя файла для сохранения: ";
    string filename; getline(cin >> ws, filename);
    ofstream file(filename);
    if (!file.is_open()) { cout << "Ошибка открытия файла!\n"; return; }

    for (auto& [id, pipe] : pipes) pipe.pipe_save(file);
    for (auto& [id, station] : stations) station.cs_save(file);

    file.close();
    cout << "Данные сохранены.\n";
}

void ShowAll(const unordered_map<int, Pipe>& pipes, const unordered_map<int, Station>& stations) {
    if (pipes.empty()) cout << "Нет труб.\n";
    else for (auto& [id, pipe] : pipes) pipe.ShowPipe();

    if (stations.empty()) cout << "Нет станций.\n";
    else for (auto& [id, station] : stations) station.cs_show();
}

void LoadAll(unordered_map<int, Pipe>& pipes, unordered_map<int, Station>& stations) {
    cout << "Введите имя файла для загрузки: ";
    string filename; getline(cin >> ws, filename);
    ifstream file(filename);
    if (!file.is_open()) { cout << "Ошибка открытия файла!\n"; return; }

    pipes.clear();
    stations.clear();

    string line;
    while (file >> line) {
        if (line == "Pipe") load(file, pipes);
        if (line == "Station") load(file, stations);
    }
    file.close();

    Pipe::set_currentid(pipes);
    Station::setNextId(stations);

    cout << "Данные загружены.\n";
}

void BatchEditPipes(unordered_map<int, Pipe>& pipes, unordered_set<int>& selected_pipes) {
    while (true) {
        cout << "\n--- Фильтр для труб ---\n"
             << "1. Все трубы\n"
             << "2. По ID\n"
             << "3. По названию\n"
             << "4. По статусу 'в ремонте'\n"
             << "0. Назад\n"
             << "Выберите фильтр: ";
        int filter; cin >> filter;
        if (filter == 0) return;

        switch(filter) {
            case 1: SelectAll(pipes, selected_pipes); break;
            case 2: SelectById(pipes, selected_pipes); break;
            case 3: {
                string name; cout << "Введите название: "; cin.ignore(); getline(cin, name);
                FindByName(pipes, selected_pipes, name); break;
            }
            case 4: {
                bool status; cout << "1 - в работе, 0 - в ремонте: ";
                status = GetCorrectNumber(0,1);
                FindByIsWorking(pipes, selected_pipes, status); break;
            }
            default: cout << "Неверный выбор.\n"; continue;
        }

        // Редактирование выбранных труб
        while (true) {
            cout << "\n--- Редактирование выбранных труб ---\n"
                 << "1. Показать выбранные трубы\n"
                 << "2. Изменить статус всех выбранных\n"
                 << "3. Удалить выбранные трубы\n"
                 << "4. Выбрать подмножество по ID\n"
                 << "0. Назад\nВыберите действие: ";
            int cmd; cin >> cmd;
            if (cmd == 0) break;

            switch(cmd) {
                case 1: for(int id: selected_pipes) pipes[id].ShowPipe(); break;
                case 2: ChangeSelectedPipesStatus(pipes, selected_pipes); break;
                case 3:
                    for(int id: selected_pipes) { logAction("Удалена труба: " + pipes[id].GetName()); pipes.erase(id); }
                    selected_pipes.clear(); break;
                case 4: {
                    unordered_set<int> subset;
                    SelectById(pipes, subset);
                    selected_pipes = subset;
                    break;
                }
            }
        }
    }
}

void BatchEditStations(unordered_map<int, Station>& stations, unordered_set<int>& selected_stations) {
    while (true) {
        cout << "\n--- Фильтр для станций ---\n"
             << "1. Все станции\n"
             << "2. По ID\n"
             << "3. По названию\n"
             << "4. По проценту незадействованных цехов\n"
             << "0. Назад\nВыберите фильтр: ";
        int filter; cin >> filter;
        if(filter == 0) return;

        switch(filter) {
            case 1: SelectAll(stations, selected_stations); break;
            case 2: SelectById(stations, selected_stations); break;
            case 3: {
                string name; cout << "Введите название: "; cin.ignore(); getline(cin, name);
                FindByName(stations, selected_stations, name); break;
            }
            case 4: {
                double threshold; cout << "Введите минимальный % незадействованных цехов: ";
                threshold = GetCorrectNumber(0.0, 100.0);
                FindByUnusedWorkshops(stations, selected_stations, threshold); break;
            }
            default: cout << "Неверный выбор.\n"; continue;
        }

        while (true) {
            cout << "\n--- Редактирование выбранных станций ---\n"
                 << "1. Показать выбранные\n"
                 << "2. Изменить кол-во активных цехов\n"
                 << "3. Удалить выбранные\n"
                 << "4. Выбрать подмножество по ID\n"
                 << "0. Назад\nВыберите действие: ";
            int cmd; cin >> cmd;
            if(cmd == 0) break;

            switch(cmd) {
                case 1: for(int id: selected_stations) stations[id].cs_show(); break;
                case 2:
                    for(int id: selected_stations) {
                        int max_w = stations[id].GetNumberofWorkshops();
                        int new_active = GetCorrectNumber(0, max_w);
                        stations[id].SetRunningWorkshops(new_active);
                        logAction("Станция обновлена: " + stations[id].GetName());
                    }
                    break;
                case 3:
                    for(int id: selected_stations) { logAction("Станция удалена: " + stations[id].GetName()); stations.erase(id); }
                    selected_stations.clear(); break;
                case 4: {
                    unordered_set<int> subset;
                    SelectById(stations, subset);
                    selected_stations = subset; break;
                }
            }
        }
    }
}

void ChangeSelectedPipesStatus(unordered_map<int, Pipe>& pipes, const unordered_set<int>& selected_pipes) {
    for(int id: selected_pipes) {
        pipes[id].ChangeRepair();
        logAction("Изменен статус трубы: " + pipes[id].GetName());
    }
}

// Функция логирования действий
inline void logAction(const string& action) {
    if (!logfile.is_open()) {
        logfile.open("log.txt", ios::app);
    }
    auto now_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
    logfile << std::ctime(&now_time) << ": " << action << endl;
}