#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <sstream>

struct Pipe {
    std::string km_mark;
    double length_km = 0.0;
    int diameter_mm = 0;
    bool in_repair = false;
};

struct Station {
    std::string name;
    int total_workshops = 0;
    int running_workshops = 0;
    int station_class = 0;
};

void clearStdin() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string readLineNonEmpty(const std::string& message) {
    std::string s;
    while (true) {
        std::cout << message;
        if (!std::getline(std::cin, s)) {
            std::cin.clear();
            continue;
        }
        size_t start = s.find_first_not_of(" \t\r\n");
        size_t end = s.find_last_not_of(" \t\r\n");
        s = (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
        if (!s.empty()) return s;
        std::cout << "Ввод не может быть пустым. Повторите.\n";
    }
}

double readDouble(const std::string& message, double minVal = -1e300, double maxVal = 1e300) {
    while (true) {
        std::string s = readLineNonEmpty(message);
        try {
            size_t pos;
            double val = std::stod(s, &pos);
            if (pos != s.size() || val < minVal || val > maxVal) throw std::invalid_argument("");
            return val;
        } catch (...) {
            std::cout << "Некорректный вещественный ввод. Попробуйте снова.\n";
        }
    }
}

int readInt(const std::string& message, int minVal = std::numeric_limits<int>::min(), int maxVal = std::numeric_limits<int>::max()) {
    while (true) {
        std::string s = readLineNonEmpty(message);
        try {
            size_t pos;
            long val = std::stol(s, &pos);
            if (pos != s.size() || val < minVal || val > maxVal) throw std::invalid_argument("");
            return static_cast<int>(val);
        } catch (...) {
            std::cout << "Некорректный целочисленный ввод. Попробуйте снова.\n";
        }
    }
}

bool yesNo(const std::string& message) {
    while (true) {
        std::string s = readLineNonEmpty(message + " (y/n): ");
        if (s == "y" || s == "Y" || s == "yes" || s == "Yes") return true;
        if (s == "n" || s == "N" || s == "no" || s == "No") return false;
        std::cout << "Введите 'y' или 'n'.\n";
    }
}

void readPipeFromConsole(Pipe &p) {
    p.km_mark = readLineNonEmpty("Введите километровую отметку (название трубы): ");
    p.length_km = readDouble("Введите длину трубы (км, положительное число): ", 0.0, 1e6);
    p.diameter_mm = readInt("Введите диаметр (мм, положительное целое): ", 1, 100000);
    p.in_repair = yesNo("Труба в ремонте?");
}

void printPipe(const Pipe &p) {
    std::cout << "=== Труба ===\n"
              << "Километровая отметка: " << p.km_mark << "\n"
              << "Длина (км): " << p.length_km << "\n"
              << "Диаметр (мм): " << p.diameter_mm << "\n"
              << "В ремонте: " << (p.in_repair ? "Да" : "Нет") << "\n";
}

void togglePipeRepair(Pipe &p) {
    std::cout << "Текущее состояние: " << (p.in_repair ? "В ремонте" : "Не в ремонте") << "\n";
    p.in_repair = yesNo("Установить противоположный признак?");
    std::cout << "Новое состояние: " << (p.in_repair ? "В ремонте" : "Не в ремонте") << "\n";
}

void readStationFromConsole(Station &s) {
    s.name = readLineNonEmpty("Введите название КС: ");
    s.total_workshops = readInt("Введите общее количество цехов (целое >=0): ", 0, 10000);
    s.running_workshops = (s.total_workshops > 0) ? readInt("Введите количество работающих цехов (0..общее): ", 0, s.total_workshops) : 0;
    s.station_class = readInt("Введите класс станции (целое, например 1..10): ", 0, 1000000);
}

void printStation(const Station &s) {
    std::cout << "=== КС ===\n"
              << "Название: " << s.name << "\n"
              << "Всего цехов: " << s.total_workshops << "\n"
              << "Работают цехов: " << s.running_workshops << "\n"
              << "Класс станции: " << s.station_class << "\n";
}

void manageWorkshop(Station &s, int delta) {
    if (s.total_workshops == 0) {
        std::cout << "У станции нет цехов.\n";
        return;
    }
    int new_running = s.running_workshops + delta;
    if (new_running < 0) {
        std::cout << "Нет работающих цехов, которые можно остановить.\n";
        return;
    }
    if (new_running > s.total_workshops) {
        std::cout << "Все цехи уже запущены.\n";
        return;
    }
    s.running_workshops = new_running;
    std::cout << (delta > 0 ? "Запущен 1 цех. " : "Остановлен 1 цех. ") << "Теперь работают: " << s.running_workshops << "\n";
}

bool saveToFile(const Pipe &p, const Station &s, const std::string &filename, bool hasPipe, bool hasStation) {
    std::ofstream ofs(filename);
    if (!ofs) {
        std::cout << "Не удалось открыть файл для записи: " << filename << "\n";
        return false;
    }
    if (hasPipe) {
        ofs << p.km_mark << "\n"
            << p.length_km << "\n"
            << p.diameter_mm << "\n"
            << (p.in_repair ? 1 : 0) << "\n";
    }
    if (hasStation) {
        ofs << s.name << "\n"
            << s.total_workshops << "\n"
            << s.running_workshops << "\n"
            << s.station_class << "\n";
    }
    if (!ofs) {
        std::cout << "Ошибка при записи в файл.\n";
        return false;
    }
    std::cout << "Успешно сохранено в " << filename << "\n";
    return true;
}

bool loadFromFile(Pipe &p, Station &s, const std::string &filename, bool &hasPipe, bool &hasStation) {
    hasPipe = false;
    hasStation = false;
    std::ifstream ifs(filename);
    if (!ifs) {
        std::cout << "Не удалось открыть файл для чтения: " << filename << "\n";
        return false;
    }
    if (ifs.peek() == EOF) {
        return false;
    }

    std::string line;

    if (!std::getline(ifs, p.km_mark)) return false;
    if (!std::getline(ifs, line)) return false;
    try { p.length_km = std::stod(line); } catch (...) { return false; }
    if (!std::getline(ifs, line)) return false;
    try { p.diameter_mm = std::stoi(line); } catch (...) { return false; }
    if (!std::getline(ifs, line)) return false;
    p.in_repair = (line == "1");
    hasPipe = true;

    if (ifs.peek() != EOF) {
        if (!std::getline(ifs, s.name)) return true; // no station, but pipe read, so return true
        if (!std::getline(ifs, line)) return true;
        try { s.total_workshops = std::stoi(line); } catch (...) { return true; }
        if (!std::getline(ifs, line)) return true;
        try { s.running_workshops = std::stoi(line); } catch (...) { return true; }
        if (!std::getline(ifs, line)) return true;
        try { s.station_class = std::stoi(line); } catch (...) { return true; }

        if (s.running_workshops < 0) s.running_workshops = 0;
        if (s.running_workshops > s.total_workshops) s.running_workshops = s.total_workshops;

        hasStation = true;
    }
    return true;
}

int main() {
    Pipe pipe;
    Station st;
    bool hasPipe = false, hasStation = false;

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
                if (hasPipe && !yesNo("Труба уже существует. Перезаписать?")) 
                    break;
                readPipeFromConsole(pipe);
                hasPipe = true;
                break;
            case 2:
                if (hasStation && !yesNo("КС уже существует. Перезаписать?")) 
                    break;
                readStationFromConsole(st);
                hasStation = true;
                break;
            case 3:
                std::cout << (hasPipe ? (printPipe(pipe), "") : "Труба не задана.\n");
                std::cout << (hasStation ? (printStation(st), "") : "КС не задана.\n");
                break;
            case 4:
                if (!hasPipe) 
                    std::cout << "Труба ещё не задана. Сначала добавьте трубу.\n";
                else 
                    togglePipeRepair(pipe);
                break;
            case 5:
                if (!hasStation) {
                    std::cout << "КС ещё не задана. Сначала добавьте КС.\n";
                } else {
                    std::cout << "1. Запустить цех\n2. Остановить цех\n";
                    int sub = readInt("Выберите: ", 1, 2);
                    manageWorkshop(st, sub == 1 ? 1 : -1);
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
                    } else {
                        std::cout << "Не удалось загрузить объекты из файла.\n";
                       // pipe = {};
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