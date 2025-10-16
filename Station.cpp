#include <iostream>
#include <fstream>
#include <limits>
#include "Station.h"
#include "utils.h"

using namespace std;

int Station::MaxID = 1;

void Station::saveStation(std::ofstream& ofs) const {
    ofs << name << "\n"
        << total_workshops << "\n"
        << running_workshops << "\n"
        << station_class << "\n";
}

bool Station::loadStation(std::ifstream& ifs, Station& s) {
    std::getline(ifs >> std::ws, s.name);
    if (!(ifs >> s.total_workshops)) return false;
    if (!(ifs >> s.running_workshops)) return false;
    if (!(ifs >> s.station_class)) return false;

    if (s.running_workshops < 0) s.running_workshops = 0;
    if (s.running_workshops > s.total_workshops) s.running_workshops = s.total_workshops;

    return ifs.good();
}

void Station::print() const {
    std::cout << "=== КС ===\n"
              << "Название: " << name << "\n"
              << "Всего цехов: " << total_workshops << "\n"
              << "Работают цехов: " << running_workshops << "\n"
              << "Класс станции: " << station_class << "\n";
}

void Station::readStationFromConsole() {
    name = readLineNonEmpty("Введите название КС: ");
    total_workshops = readInt("Введите общее количество цехов (целое >=0): ", 0, std::numeric_limits<int>::max());
    if (total_workshops > 0) {
        running_workshops = readInt("Введите количество работающих цехов (0..общее): ", 0, total_workshops);
    } else {
        running_workshops = 0;
    }
    station_class = readInt("Введите класс станции (целое, например 1..10): ", 0, std::numeric_limits<int>::max());
}

void Station::manageWorkshop(int delta) {
    if (total_workshops == 0) {
        std::cout << "У станции нет цехов.\n";
        return;
    }
    int new_running = running_workshops + delta;
    if (new_running < 0) {
        std::cout << "Нет работающих цехов, которые можно остановить.\n";
        return;
    }
    if (new_running > total_workshops) {
        std::cout << "Все цехи уже запущены.\n";
        return;
    }
    running_workshops = new_running;
    std::cout << (delta > 0 ? "Запущен 1 цех. " : "Остановлен 1 цех. ") << "Теперь работают: " << running_workshops << "\n";
}

std::istream& operator>>(std::istream& in, Station& s) {
    s.name = readLineNonEmpty("Введите название КС: ");
    s.total_workshops = readInt("Введите общее количество цехов (целое >=0): ", 0, std::numeric_limits<int>::max());
    if (s.total_workshops > 0) {
        s.running_workshops = readInt("Введите количество работающих цехов (0..общее): ", 0, s.total_workshops);
    } else {
        s.running_workshops = 0;
    }
    s.station_class = readInt("Введите класс станции (целое, например 1..10): ", 0, std::numeric_limits<int>::max());
    return in;
}

std::ostream& operator<<(std::ostream& out, const Station& s) {
    out << "=== КС ===\n"
        << "Название: " << s.name << "\n"
        << "Всего цехов: " << s.total_workshops << "\n"
        << "Работают цехов: " << s.running_workshops << "\n"
        << "Класс станции: " << s.station_class << "\n";
    return out;
}