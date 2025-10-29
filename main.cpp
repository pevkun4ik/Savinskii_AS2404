#include "Pipe.h"
#include "Station.h"
#include "utils.h"
#include "filters.h"
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <chrono>
using namespace std;

int main() {
    unordered_map<int, Pipe> pipes;
    unordered_map<int, Station> stations;
    unordered_set<int> selected_pipes;
    unordered_set<int> selected_stations;

    // Создаем уникальный лог-файл с временем запуска
    auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
    logfile.open("log_" + to_string(now) + ".txt");

    while (true) {
        cout << "\n--- Главное меню ---\n"
             << "1. Добавить трубу\n"
             << "2. Добавить станцию\n"
             << "3. Показать все объекты\n"
             << "4. Сохранить в файл\n"
             << "5. Загрузить из файла\n"
             << "6. Пакетное редактирование труб\n"
             << "7. Пакетное редактирование станций\n"
             << "0. Выход\n"
             << "Выберите действие (номер): ";
        int cmd; 
        cin >> cmd;

        switch (cmd) {
            case 0: 
                if(logfile.is_open()) logfile.close(); 
                return 0;

            case 1: { 
                Pipe p; 
                p.AddPipe(); 
                pipes[p.GetId()] = p; 
                logAction("Добавлена труба: " + p.GetName()); 
                break; 
            }

            case 2: { 
                Station s; 
                s.AddStation(); 
                stations[s.GetId()] = s; 
                logAction("Добавлена станция: " + s.GetName()); 
                break; 
            }

            case 3: ShowAll(pipes, stations); break;
            case 4: SaveAll(pipes, stations); break;
            case 5: LoadAll(pipes, stations); break;
            case 6: BatchEditPipes(pipes, selected_pipes); break;
            case 7: BatchEditStations(stations, selected_stations); break;

            default: cout << "Неверный выбор. Попробуйте снова.\n"; break;
        }
    }
}