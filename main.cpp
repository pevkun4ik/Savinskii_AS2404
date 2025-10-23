#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <sstream>
#include <charconv>
#include <unordered_map>
#include <map>
#include "utils.h"
#include "Pipe.h"
#include "Station.h"
#include <chrono>
#include <format>



int main() 
{
    std::map<int, Pipe> pipes;
    std::map<int, Station> stations;

    redirect_output_wrapper cerr_out(std::cerr);

    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%d_%m_%Y_%H_%M_%S"); 
    std::string time_str = oss.str();

    logfile.open("log_" + time_str + ".txt");
    if (logfile)
        cerr_out.redirect(logfile);

    std::cout << "\nЛог будет записываться в файл: log_" << time_str << ".txt\n";

    while (true) {
        std::cout << "\n--- Меню ---\n"
                  << "1. Добавить/редактировать Трубу\n"
                  << "2. Добавить/редактировать КС\n"
                  << "3. Просмотр всех объектов\n"
                  << "4. Редактировать признак 'в ремонте' для трубы\n"
                  << "5. Запуск/Остановка цеха на КС\n"
                  << "6. Сохранить в файл\n"
                  << "7. Загрузить из файла\n"
                  << "8. Удалить трубу\n"
                  << "9. Удалить КС\n"
                  << "10. Поиск объектов\n"
                  << "11. Пакетное редактирование труб\n"
                  << "0. Выход\n";

        int cmd = readInt("Выберите действие (номер): ", 0, 11);
        switch (cmd) {
            case 1:
            {
                int id = readInt("Введите ID трубы: ", 1, std::numeric_limits<int>::max());
                auto it = pipes.find(id);
                if (it != pipes.end()) {
                    if (!readInt("Труба с таким ID уже существует. Перезаписать? (1 - да, 0 - нет): ", 0, 1))
                        break;
                }
                Pipe p;
                p.readPipeFromConsole();
                pipes[id] = p;

                if (id >= Pipe::nextId) Pipe::nextId = id + 1;

                logAction("Пользователь добавил/редактировал трубу с ID " + std::to_string(id) + ".");
                logAction("Километровая отметка: " + p.getKmMark() +
                    ", Длина: " + std::to_string(p.getLength()) +
                    ", Диаметр: " + std::to_string(p.getDiameter()) +
                    ", В ремонте: " + (p.getInRepair() ? "Да" : "Нет"));
                break;
            }
            case 2:
            {
                int id = readInt("Введите ID КС: ", 1, std::numeric_limits<int>::max());
                auto it = stations.find(id);
                if (it != stations.end()) {
                    if (!readInt("КС с таким ID уже существует. Перезаписать? (1 - да, 0 - нет): ", 0, 1))
                        break;
                }
                Station s;
                s.readStationFromConsole();
                stations[id] = s;

                if (id >= Station::nextId) Station::nextId = id + 1;

                logAction("Пользователь добавил/редактировал КС с ID " + std::to_string(id) + ".");
                logAction("Название: " + s.getName() +
                    ", Всего цехов: " + std::to_string(s.getTotalWorkshops()) +
                    ", Работающих цехов: " + std::to_string(s.getRunningWorkshops()) +
                    ", Класс станции: " + std::to_string(s.getStationClass()));
                break;
            }
            case 3:
                if (pipes.empty())
                    std::cout << "Труб нет.\n";
                else {
                    std::cout << "Трубы:\n";
                    for (const auto& [id, p] : pipes) {
                        std::cout << "ID: " << id << "\n";
                        p.print();
                        std::cout << "\n";
                    }
                }
                if (stations.empty())
                    std::cout << "КС нет.\n";
                else {
                    std::cout << "КС:\n";
                    for (const auto& [id, s] : stations) {
                        std::cout << "ID: " << id << "\n";
                        s.print();
                        std::cout << "\n";
                    }
                }
                logAction("Пользователь просмотрел все объекты.");
                break;
            case 4:
            {
                if (pipes.empty()) {
                    std::cout << "Труб нет. Сначала добавьте трубу.\n";
                    break;
                }
                int id = readInt("Введите ID трубы для изменения признака 'в ремонте': ", 1, std::numeric_limits<int>::max());
                auto it = pipes.find(id);
                if (it == pipes.end()) {
                    std::cout << "Труба с таким ID не найдена.\n";
                    break;
                }
                it->second.toggleRepair();
                logAction("Пользователь изменил признак 'в ремонте' для трубы с ID " + std::to_string(id) + ".");
                logAction("Новое состояние трубы 'в ремонте': " + std::string(it->second.getInRepair() ? "Да" : "Нет"));
                break;
            }
            case 5:
            {
                if (stations.empty()) {
                    std::cout << "КС нет. Сначала добавьте КС.\n";
                    break;
                }
                int id = readInt("Введите ID КС для запуска/остановки цеха: ", 1, std::numeric_limits<int>::max());
                auto it = stations.find(id);
                if (it == stations.end()) {
                    std::cout << "КС с таким ID не найдена.\n";
                    break;
                }
                std::cout << "1 - Запустить цех\n2 - Остановить цех\n";
                int sub = readInt("Выберите: ", 1, 2);
                it->second.manageWorkshop(sub == 1 ? 1 : -1);
                if (sub == 1)
                    logAction("Пользователь запустил цех на КС с ID " + std::to_string(id) + ".");
                else
                    logAction("Пользователь остановил цех на КС с ID " + std::to_string(id) + ".");
                logAction("Текущее число работающих цехов: " + std::to_string(it->second.getRunningWorkshops()));
                break;
            }
            case 6:
                if (pipes.empty() && stations.empty()) {
                    std::cout << "Нет данных для сохранения.\n";
                    break;
                }
                {
                    std::string fn = readLineNonEmpty("Введите имя файла для сохранения: ");
                    saveToFile(pipes, stations, fn);
                    logAction("Пользователь сохранил данные в файл: " + fn);
                }
                break;
            case 7:
                {
                    std::string fn = readLineNonEmpty("Введите имя файла для загрузки: ");
                    std::map<int, Pipe> tmpPipes;
                    std::map<int, Station> tmpStations;
                    if (loadFromFile(tmpPipes, tmpStations, fn)) {
                        pipes = std::move(tmpPipes);
                        stations = std::move(tmpStations);
                        logAction("Пользователь загрузил данные из файла: " + fn);
                    } else {
                        std::cout << "Не удалось загрузить объекты из файла.\n";
                    }
                }
                break;
            case 8:
            {
                if (pipes.empty()) {
                    std::cout << "Труб нет для удаления.\n";
                    break;
                }
                int id = readInt("Введите ID трубы для удаления: ", 1, std::numeric_limits<int>::max());
                auto it = pipes.find(id);
                if (it == pipes.end()) {
                    std::cout << "Труба с таким ID не найдена.\n";
                    break;
                }
                pipes.erase(it);
                logAction("Пользователь удалил трубу с ID " + std::to_string(id) + ".");
                std::cout << "Труба с ID " << id << " удалена.\n";
                break;
            }
            case 9:
            {
                if (stations.empty()) {
                    std::cout << "КС нет для удаления.\n";
                    break;
                }
                int id = readInt("Введите ID КС для удаления: ", 1, std::numeric_limits<int>::max());
                auto it = stations.find(id);
                if (it == stations.end()) {
                    std::cout << "КС с таким ID не найдена.\n";
                    break;
                }
                stations.erase(it);
                logAction("Пользователь удалил КС с ID " + std::to_string(id) + ".");
                std::cout << "КС с ID " << id << " удалена.\n";
                break;
            }
            case 10: {
                std::cout << "1. Поиск труб\n2. Поиск КС\n";
                int sub = readInt("Выберите: ", 1, 2);
                if (sub == 1)
                    Pipe::findByFilter(pipes);
                else
                    Station::findByFilter(stations);
                break;
            }
            case 11: {
                Pipe::batchEdit(pipes);
                break;
            }
            case 0:
                if (logfile.is_open())
                    logfile.close();
                std::cout << "Выход. До свидания.\n";
                return 0;
            default:
                std::cout << "Некорректный пункт меню.\n";
        }
    }
    return 0;
}