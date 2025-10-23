#include "Pipe.h"
#include "Station.h"
#include <fstream>
#include "utils.h"

void clearStdin() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string trim(const std::string& str) {
    auto start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    auto end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

std::string readLineNonEmpty(const std::string& message) {
    for (;;) {
        std::cout << message;
        std::string s;
        if (!std::getline(std::cin >> std::ws, s)) {
            std::cin.clear();
            continue;
        }
        s = trim(s);
        if (!s.empty()) return s;
        std::cout << "Ввод не может быть пустым. Повторите.\n";
    }
}

double readDouble(const std::string& message, double minVal, double maxVal) {
    double val;
    std::cout << message;
    for (;;) {
        if (std::cin >> val && val >= minVal && val <= maxVal) {
            return val;
        }
        clearStdin();
        std::cout << "Некорректный ввод числа с плавающей точкой. Попробуйте снова: ";
    }
}

int readInt(const std::string& message, int minVal, int maxVal) {
    int val;
    std::cout << message;
    for (;;) {
        if (std::cin >> val && val >= minVal && val <= maxVal) {
            return val;
        }
        clearStdin();
        std::cout << "Некорректный ввод целого числа. Попробуйте снова: ";
    }
}

std::ofstream logfile("log.txt", std::ios::app);

void logAction(const std::string& action) {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::string time_str = std::ctime(&now_c);
    time_str.pop_back(); // убрать \n

    std::string entry = "[" + time_str + "] " + action + "\n";

    // выводим в консоль (или в файл через redirect_output_wrapper)
    std::cerr << entry;
    if (logfile.is_open())
        logfile << entry;
}

bool saveToFile(const std::map<int, Pipe>& pipes, const std::map<int, Station>& stations, const std::string& filename) {
    std::ofstream ofs(filename);
    if (!ofs.is_open()) {
        std::cout << "Ошибка при открытии файла для записи.\n";
        return false;
    }

    ofs << pipes.size() << "\n";
    for (const auto& [id, p] : pipes)
        p.savePipe(ofs);

    ofs << stations.size() << "\n";
    for (const auto& [id, s] : stations)
        s.saveStation(ofs);

    logAction("Сохранение данных в файл: " + filename);
    return true;
}

bool loadFromFile(std::map<int, Pipe>& pipes, std::map<int, Station>& stations, const std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
        std::cout << "Ошибка при открытии файла для чтения.\n";
        return false;
    }

    pipes.clear();
    stations.clear();

    size_t pipeCount, stationCount;
    ifs >> pipeCount;
    for (size_t i = 0; i < pipeCount; ++i) {
        Pipe p;
        if (Pipe::loadPipe(ifs, p))
            pipes[p.getId()] = p;
    }

    ifs >> stationCount;
    for (size_t i = 0; i < stationCount; ++i) {
        Station s;
        if (Station::loadStation(ifs, s))
            stations[s.getId()] = s;
    }

    logAction("Загрузка данных из файла: " + filename);
    return true;
}