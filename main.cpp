#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <sstream>
#include <charconv>

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

double readDouble(const std::string & message, double minVal, double maxVal) {
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

int readInt(const std::string & message, int minVal, int maxVal) {
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

void readPipeFromConsole(Pipe &p) {
    p.km_mark = readLineNonEmpty("Введите километровую отметку (название трубы): ");
    p.length_km = readDouble("Введите длину трубы (км, положительное число): ", 0.0, 1e6);
    p.diameter_mm = readInt("Введите диаметр (мм, положительное целое): ", 1, 100000);
    p.in_repair = readInt("Труба в ремонте? (1 - да, 0 - нет): ", 0, 1);
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
    p.in_repair = readInt("Установить противоположный признак? (1 - да, 0 - нет)", 0, 1);
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

void savePipe(std::ofstream& ofs, const Pipe& p) {
    ofs << p.km_mark << "\n"
        << p.length_km << "\n"
        << p.diameter_mm << "\n"
        << (p.in_repair ? 1 : 0) << "\n";
}

void saveStation(std::ofstream& ofs, const Station& s) {
    ofs << s.name << "\n"
        << s.total_workshops << "\n"
        << s.running_workshops << "\n"
        << s.station_class << "\n";
}

bool loadPipe(std::ifstream& ifs, Pipe& p) {
    std::string line;
    if (!std::getline(ifs, p.km_mark)) return false;
    if (!std::getline(ifs, line)) return false;
    std::stringstream ss1(line); ss1 >> p.length_km;
    if (!std::getline(ifs, line)) return false;
    std::stringstream ss2(line); ss2 >> p.diameter_mm;
    if (!std::getline(ifs, line)) return false;
    p.in_repair = (line == "1");
    return true;
}

bool loadStation(std::ifstream& ifs, Station& s) {
    std::string line;
    std::getline(ifs, s.name);
    std::getline(ifs, line);
    std::stringstream(line) >> s.total_workshops;
    std::getline(ifs, line);
    std::stringstream(line) >> s.running_workshops;
    std::getline(ifs, line);
    std::stringstream(line) >> s.station_class;

    if (s.running_workshops < 0) s.running_workshops = 0;
    if (s.running_workshops > s.total_workshops) s.running_workshops = s.total_workshops;

    return true;
}

bool loadFromFile(Pipe &p, Station &s, const std::string &filename, bool &hasPipe, bool &hasStation) {
    hasPipe = false;
    hasStation = false;
    std::ifstream ifs(filename);
    if (!ifs) {
        hasPipe = false;
        hasStation = false;
        return true;
    }

    std::string flag;
    if (!std::getline(ifs, flag)) return true;
    hasPipe = (flag == "PIPE");

    if (hasPipe) {
        loadPipe(ifs, p);
    }

    if (ifs.peek() != EOF) {
        loadStation(ifs, s);
        hasStation = true;
    }
    return true;
}

bool saveToFile(const Pipe &p, const Station &s, const std::string &filename, bool hasPipe, bool hasStation) {
    std::ofstream ofs(filename);
    if (hasPipe) {
        ofs << "PIPE" << "\n";
        savePipe(ofs, p);
    } else {
        ofs << "NO_PIPE" << "\n";
    }
    if (hasStation) {
        saveStation(ofs, s);
    }
    std::cout << "Успешно сохранено в " << filename << "\n";
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
                if (hasPipe && !readInt("Труба уже существует. Перезаписать? (1 - да, 0 - нет)", 0, 1)) 
                    break;
                readPipeFromConsole(pipe);
                hasPipe = true;
                break;
            case 2:
                if (hasStation && !readInt("КС уже существует. Перезаписать? (1 - да, 0 - нет)", 0, 1)) 
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
                    std::cout << "1 - Запустить цех\n2 - Остановить цех\n";
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