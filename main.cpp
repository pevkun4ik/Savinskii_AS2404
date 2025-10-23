#include "Pipe.h"
#include "filters.h"
#include "Station.h"
#include "utils.h"
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <chrono>

using namespace std;

void BatchEditPipes(unordered_map<int, Pipe>& pipes, unordered_set<int>& selected_pipes) {
    while (true) {
        // Шаг 1: Выбор фильтра
        cout << "\n--- Фильтр для труб ---\n"
             << "1. Все трубы\n"
             << "2. По ID\n"
             << "3. По названию\n"
             << "4. По признаку 'в ремонте'\n"
             << "0. Назад\n"
             << "Выберите фильтр (номер): ";
        int filter_cmd; cin >> filter_cmd;

        switch (filter_cmd) {
            case 0: return;
            case 1: SelectAll(pipes, selected_pipes); break;
            case 2: SelectById(pipes, selected_pipes); break;
            case 3: {
                string name;
                cout << "Введите название трубы: ";
                cin.ignore();
                getline(cin, name);
                FindByName(pipes, selected_pipes, name);
                break;
            }
            case 4: {
                bool is_working;
                cout << "Выберите статус (1 - в работе, 0 - в ремонте): ";
                is_working = GetCorrectNumber(0,1);
                FindByIsWorking(pipes, selected_pipes, is_working);
                break;
            }
            default: cout << "Неверный выбор. Попробуйте снова.\n"; continue;
        }

        // Шаг 2: Редактирование выбранных труб
        while (true) {
            cout << "\n--- Редактирование выбранных труб ---\n"
                 << "1. Показать выбранные трубы\n"
                 << "2. Изменить статус 'в ремонте' всех выбранных\n"
                 << "3. Удалить выбранные трубы\n"
                 << "4. Выбрать подмножество по ID\n"
                 << "0. Вернуться к фильтру\n"
                 << "Выберите действие: ";
            int edit_cmd; cin >> edit_cmd;

            switch (edit_cmd) {
                case 0: goto filter_restart;
                case 1: 
                    for (int id : selected_pipes) pipes[id].ShowPipe();
                    break;
                case 2:
                    for (int id : selected_pipes) {
                        pipes[id].ChangeRepair();
                        logAction("Pipe status changed: " + pipes[id].GetName());
                    }
                    cout << "Статус изменен для выбранных труб.\n";
                    break;
                case 3: {
                    for (int id : selected_pipes) {
                        logAction("Pipe deleted: " + pipes[id].GetName());
                        pipes.erase(id);
                    }
                    selected_pipes.clear();
                    cout << "Выбранные трубы удалены.\n";
                    break;
                }
                case 4: {
                    unordered_set<int> subset;
                    SelectById(pipes, subset); // пользователь выбирает подмножество
                    selected_pipes = subset;
                    break;
                }
                default: cout << "Неверный выбор. Попробуйте снова.\n"; break;
            }
        }
        filter_restart:;
    }
}

int main() {
    unordered_map<int, Pipe> pipes;
    unordered_map<int, Station> stations;
    unordered_set<int> selected_pipes;

    auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
    logfile.open("log_" + to_string(now) + ".txt");

    while (true) {
        cout << "\n--- Главное меню ---\n"
             << "1. Добавить трубу\n"
             << "2. Добавить КС\n"
             << "3. Показать все объекты\n"
             << "4. Сохранить в файл\n"
             << "5. Загрузить из файла\n"
             << "6. Пакетное редактирование труб\n"
             << "0. Выход\n"
             << "Выберите действие (номер): ";
        int cmd; cin >> cmd;
        switch (cmd) {
            case 0: 
                if(logfile.is_open()) logfile.close(); 
                return 0;
            case 1: { 
                Pipe p; 
                p.AddPipe(); 
                pipes[p.GetId()] = p; 
                logAction("Pipe added: " + p.GetName()); 
                break; 
            }
            case 2: { 
                Station s; 
                s.AddStation(); 
                stations[s.GetId()] = s; 
                logAction("Station added: " + s.GetName()); 
                break; 
            }
            case 3: ShowAll(pipes, stations); break;
            case 4: SaveAll(pipes, stations); break;
            case 5: LoadAll(pipes, stations); break;
            case 6: BatchEditPipes(pipes, selected_pipes); break;
            default: cout << "Неверный выбор. Попробуйте снова.\n"; break;
        }
    }
}