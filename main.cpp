#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <sstream>
#include <charconv>
#include <unordered_map>
#include "utils.h"
#include "Pipe.h"
#include "Station.h"
#include <chrono>
#include <format>


std::ofstream logfile;

void logAction(const std::string &msg) {
    if (logfile.is_open()) {
        logfile << msg << "\n";
        logfile.flush();
    }
}

bool loadFromFile(Pipe &p, Station &s, const std::string &filename, bool &hasPipe, bool &hasStation) 
{
    hasPipe = false;
    hasStation = false;
    std::ifstream ifs(filename);
    if (!ifs) {
        hasPipe = false;
        hasStation = false;
        return true;
    }

    std::string flag;
    if (!std::getline(ifs, flag)) 
        return true;
    hasPipe = (flag == "PIPE");

    if (hasPipe) {
        Pipe::loadPipe(ifs, p);
    }

    if (ifs.peek() != EOF) {
        Station::loadStation(ifs, s);
        hasStation = true;
    }
    return true;
}

bool saveToFile(const Pipe &p, const Station &s, const std::string &filename, bool hasPipe, bool hasStation) {
    std::ofstream ofs(filename);
    if (hasPipe) {
        ofs << "PIPE" << "\n";
        p.savePipe(ofs);
    } else {
        ofs << "NO_PIPE" << "\n";
    }
    if (hasStation) {
        s.saveStation(ofs);
    }
    std::cout << "Успешно сохранено в " << filename << "\n";
    return true;
}

int main() 
{
    Pipe pipe;
    Station st;
    bool hasPipe = false, hasStation = false;

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
                  << "0. Выход\n";

        int cmd = readInt("Выберите действие (номер): ", 0, 9);
        switch (cmd) {
            case 1:
                if (hasPipe && !readInt("Труба уже существует. Перезаписать? (1 - да, 0 - нет)", 0, 1)) 
                    break;
                pipe.readPipeFromConsole();
                hasPipe = true;

                logAction("Пользователь добавил/редактировал трубу.");
                logAction("Километровая отметка: " + pipe.getKmMark() +
                    ", Длина: " + std::to_string(pipe.getLength()) +
                    ", Диаметр: " + std::to_string(pipe.getDiameter()) +
                    ", В ремонте: " + (pipe.getInRepair() ? "Да" : "Нет"));

                break;
            case 2:
                if (hasStation && !readInt("КС уже существует. Перезаписать? (1 - да, 0 - нет)", 0, 1)) 
                    break;
                st.readStationFromConsole();
                hasStation = true;

                logAction("Пользователь добавил/редактировал КС.");
                logAction("Название: " + st.getName() +
                    ", Всего цехов: " + std::to_string(st.getTotalWorkshops()) +
                    ", Работающих цехов: " + std::to_string(st.getRunningWorkshops()) +
                    ", Класс станции: " + std::to_string(st.getStationClass()));
                    
                break;
            case 3:
                std::cout << (hasPipe ? (pipe.print(), "") : "Труба не задана.\n");
                std::cout << (hasStation ? (st.print(), "") : "КС не задана.\n");
                logAction("Пользователь просмотрел все объекты.");
                break;
            case 4:
                if (!hasPipe) {
                    std::cout << "Труба ещё не задана. Сначала добавьте трубу.\n";
                } else {
                    pipe.toggleRepair();
                    logAction("Пользователь изменил признак 'в ремонте' для трубы.");
                    logAction("Новое состояние трубы 'в ремонте': " + std::string(pipe.getInRepair() ? "Да" : "Нет"));
                }
                break;
            case 5:
                if (!hasStation) {
                    std::cout << "КС ещё не задана. Сначала добавьте КС.\n";
                } else {
                    std::cout << "1 - Запустить цех\n2 - Остановить цех\n";
                    int sub = readInt("Выберите: ", 1, 2);
                    st.manageWorkshop(sub == 1 ? 1 : -1);
                    if (sub == 1)
                        logAction("Пользователь запустил цех на КС.");
                    else
                        logAction("Пользователь остановил цех на КС.");
                    logAction("Текущее число работающих цехов: " + std::to_string(st.getRunningWorkshops()));
                }
                break;
            case 6:
                if (!hasPipe && !hasStation) {
                    std::cout << "Нет данных для сохранения.\n";
                    break;
                }
                {
                    std::string fn = readLineNonEmpty("Введите имя файла для сохранения: ");
                    saveToFile(pipe, st, fn, hasPipe, hasStation);
                    logAction("Пользователь сохранил данные в файл: " + fn);
                }
                break;
            case 7:
                {
                    std::string fn = readLineNonEmpty("Введите имя файла для загрузки: ");
                    Pipe tmpPipe; 
                    Station tmpSt;
                    bool tmpHasPipe = false, tmpHasStation = false;
                    if (loadFromFile(tmpPipe, tmpSt, fn, tmpHasPipe, tmpHasStation)) {
                        pipe = tmpPipe; 
                        st = tmpSt;
                        hasPipe = tmpHasPipe; 
                        hasStation = tmpHasStation;
                        logAction("Пользователь загрузил данные из файла: " + fn);
                    } else {
                        std::cout << "Не удалось загрузить объекты из файла.\n";
                        pipe = {};
                    }
                }
                break;
            case 0:
                std::cout << "Выход. До свидания.\n";
                return 0;
            default:
                std::cout << "Некорректный пункт меню.\n";
        }
    }
    return 0;
}