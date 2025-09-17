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

// ---------- Помошник ----------
void clearStdin() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string readLineNonEmpty(const std::string& prompt) {
    std::string s;
    while (true) {
        std::cout << prompt;
        if (!std::getline(std::cin, s)) {
            std::cin.clear();
            continue;
        }
        size_t start = s.find_first_not_of(" \t\r\n");
        size_t end = s.find_last_not_of(" \t\r\n");
        if (start == std::string::npos) s = "";
        else s = s.substr(start, end - start + 1);

        if (!s.empty()) return s;
        std::cout << "Ввод не может быть пустым. Повторите.\n";
    }
}

double readDouble(const std::string& prompt, double minVal = -1e300, double maxVal = 1e300) {
    while (true) {
        std::string s = readLineNonEmpty(prompt);
        try {
            size_t pos;
            double val = std::stod(s, &pos);
            if (pos != s.size()) throw std::invalid_argument("extra chars");
            if (val < minVal || val > maxVal) {
                std::cout << "Значение вне допустимого диапазона. Попробуйте снова.\n";
                continue;
            }
            return val;
        } catch (...) {
            std::cout << "Некорректный вещественный ввод. Попробуйте снова.\n";
        }
    }
}

int readInt(const std::string& prompt, int minVal = std::numeric_limits<int>::min(), int maxVal = std::numeric_limits<int>::max()) {
    while (true) {
        std::string s = readLineNonEmpty(prompt);
        try {
            size_t pos;
            long val = std::stol(s, &pos);
            if (pos != s.size()) throw std::invalid_argument("extra chars");
            if (val < minVal || val > maxVal) {
                std::cout << "Значение вне допустимого диапазона. Попробуйте снова.\n";
                continue;
            }
            return static_cast<int>(val);
        } catch (...) {
            std::cout << "Некорректный целочисленный ввод. Попробуйте снова.\n";
        }
    }
}

bool yesNo(const std::string& prompt) {
    while (true) {
        std::string s = readLineNonEmpty(prompt + " (y/n): ");
        if (s == "y" || s == "Y" || s == "yes" || s == "Yes") return true;
        if (s == "n" || s == "N" || s == "no" || s == "No") return false;
        std::cout << "Введите 'y' или 'n'.\n";
    }
}

// ---------- Операции с трубами ----------
void readPipeFromConsole(Pipe &p) {
    p.km_mark = readLineNonEmpty("Введите километровую отметку (название трубы): ");
    p.length_km = readDouble("Введите длину трубы (км, положительное число): ", 0.0, 1e6);
    p.diameter_mm = readInt("Введите диаметр (мм, положительное целое): ", 1, 100000);
    p.in_repair = yesNo("Труба в ремонте?");
}

void printPipe(const Pipe &p) {
    std::cout << "=== Труба ===\n";
    std::cout << "Километровая отметка: " << p.km_mark << "\n";
    std::cout << "Длина (км): " << p.length_km << "\n";
    std::cout << "Диаметр (мм): " << p.diameter_mm << "\n";
    std::cout << "В ремонте: " << (p.in_repair ? "Да" : "Нет") << "\n";
}

void togglePipeRepair(Pipe &p) {
    std::cout << "Текущее состояние: " << (p.in_repair ? "В ремонте" : "Не в ремонте") << "\n";
    p.in_repair = yesNo("Установить противоположный признак?");
    std::cout << "Новое состояние: " << (p.in_repair ? "В ремонте" : "Не в ремонте") << "\n";
}

// ---------- Оперции со станциями ----------
void readStationFromConsole(Station &s) {
    s.name = readLineNonEmpty("Введите название КС: ");
    s.total_workshops = readInt("Введите общее количество цехов (целое >=0): ", 0, 10000);
    s.running_workshops = 0;
    if (s.total_workshops > 0) {
        s.running_workshops = readInt("Введите количество работающих цехов (0..общее): ", 0, s.total_workshops);
    }
    s.station_class = readInt("Введите класс станции (целое, например 1..10): ", 0, 1000000);
}

void printStation(const Station &s) {
    std::cout << "=== КС ===\n";
    std::cout << "Название: " << s.name << "\n";
    std::cout << "Всего цехов: " << s.total_workshops << "\n";
    std::cout << "Работают цехов: " << s.running_workshops << "\n";
    std::cout << "Класс станции: " << s.station_class << "\n";
}

void startWorkshop(Station &s) {
    if (s.total_workshops == 0) {
        std::cout << "У станции нет цехов.\n";
        return;
    }
    if (s.running_workshops >= s.total_workshops) {
        std::cout << "Все цехи уже запущены.\n";
        return;
    }
    s.running_workshops++;
    std::cout << "Запущен 1 цех. Теперь работают: " << s.running_workshops << "\n";
}

void stopWorkshop(Station &s) {
    if (s.total_workshops == 0) {
        std::cout << "У станции нет цехов.\n";
        return;
    }
    if (s.running_workshops <= 0) {
        std::cout << "Нет работающих цехов, которые можно остановить.\n";
        return;
    }
    s.running_workshops--;
    std::cout << "Остановлен 1 цех. Теперь работают: " << s.running_workshops << "\n";
}

// ---------- Сохранить / удалить ----------
bool saveToFile(const Pipe &p, const Station &s, const std::string &filename) {
    std::ofstream ofs(filename);
    if (!ofs) {
        std::cout << "Не удалось открыть файл для записи: " << filename << "\n";
        return false;
    }
    ofs << "PIPE\n";
    ofs << p.km_mark << "\n";
    ofs << p.length_km << "\n";
    ofs << p.diameter_mm << "\n";
    ofs << (p.in_repair ? 1 : 0) << "\n";

    ofs << "STATION\n";
    ofs << s.name << "\n";
    ofs << s.total_workshops << "\n";
    ofs << s.running_workshops << "\n";
    ofs << s.station_class << "\n";

    ofs.close();
    if (!ofs) {
        std::cout << "Ошибка при записи в файл.\n";
        return false;
    }
    std::cout << "Успешно сохранено в " << filename << "\n";
    return true;
}

bool loadFromFile(Pipe &p, Station &s, const std::string &filename) {
    std::ifstream ifs(filename);
    if (!ifs) {
        std::cout << "Не удалось открыть файл для чтения: " << filename << "\n";
        return false;
    }
    std::string line;
    if (!std::getline(ifs, line) || line != "PIPE") {
        std::cout << "Формат файла неверный или отсутствует раздел PIPE.\n";
        return false;
    }
    if (!std::getline(ifs, p.km_mark)) return false;
    if (!std::getline(ifs, line)) return false;
    try { p.length_km = std::stod(line); } catch (...) { return false; }
    if (!std::getline(ifs, line)) return false;
    try { p.diameter_mm = std::stoi(line); } catch (...) { return false; }
    if (!std::getline(ifs, line)) return false;
    p.in_repair = (line == "1");

    if (!std::getline(ifs, line) || line != "STATION") {
        std::cout << "Формат файла неверный или отсутствует раздел STATION.\n";
        return false;
    }
    if (!std::getline(ifs, s.name)) return false;
    if (!std::getline(ifs, line)) return false;
    try { s.total_workshops = std::stoi(line); } catch (...) { return false; }
    if (!std::getline(ifs, line)) return false;
    try { s.running_workshops = std::stoi(line); } catch (...) { return false; }
    if (!std::getline(ifs, line)) return false;
    try { s.station_class = std::stoi(line); } catch (...) { return false; }

    // basic post-validation
    if (s.running_workshops < 0) s.running_workshops = 0;
    if (s.running_workshops > s.total_workshops) s.running_workshops = s.total_workshops;

    std::cout << "Успешно загружено из " << filename << "\n";
    return true;
}

// ---------- Меню ----------
void printMenu() {
    std::cout << "\n--- Меню ---\n";
    std::cout << "1. Добавить/редактировать Трубу\n";
    std::cout << "2. Добавить/редактировать КС\n";
    std::cout << "3. Просмотр всех объектов\n";
    std::cout << "4. Редактировать признак 'в ремонте' для трубы\n";
    std::cout << "5. Запуск/Остановка цеха на КС\n";
    std::cout << "6. Сохранить в файл\n";
    std::cout << "7. Загрузить из файла\n";
    std::cout << "0. Выход\n";
}

int main() {
    Pipe pipe;
    Station st;
    bool hasPipe = false;
    bool hasStation = false;

    while (true) {
        printMenu();
        int cmd = readInt("Выберите действие (номер): ", 0, 9);
        switch (cmd) {
            case 1: {
                if (hasPipe) {
                    if (!yesNo("Труба уже существует. Перезаписать?")) break;
                }
                readPipeFromConsole(pipe);
                hasPipe = true;
                break;
            }
            case 2: {
                if (hasStation) {
                    if (!yesNo("КС уже существует. Перезаписать?")) break;
                }
                readStationFromConsole(st);
                hasStation = true;
                break;
            }
            case 3: {
                if (hasPipe) printPipe(pipe);
                else std::cout << "Труба не задана.\n";
                if (hasStation) printStation(st);
                else std::cout << "КС не задана.\n";
                break;
            }
            case 4: {
                if (!hasPipe) {
                    std::cout << "Труба ещё не задана. Сначала добавьте трубу.\n";
                } else {
                    togglePipeRepair(pipe);
                }
                break;
            }
            case 5: {
                if (!hasStation) {
                    std::cout << "КС ещё не задана. Сначала добавьте КС.\n";
                } else {
                    std::cout << "1. Запустить цех\n2. Остановить цех\n";
                    int sub = readInt("Выберите: ", 1, 2);
                    if (sub == 1) startWorkshop(st); else stopWorkshop(st);
                }
                break;
            }
            case 6: {
                if (!hasPipe && !hasStation) {
                    std::cout << "Нет данных для сохранения.\n";
                    break;
                }
                std::string fn = readLineNonEmpty("Введите имя файла для сохранения: ");
                if (!hasPipe) {
                    // create empty pipe default?
                    Pipe tmp;
                    saveToFile(tmp, st, fn);
                } else if (!hasStation) {
                    Station tmp;
                    saveToFile(pipe, tmp, fn);
                } else {
                    saveToFile(pipe, st, fn);
                }
                break;
            }
            case 7: {
                std::string fn = readLineNonEmpty("Введите имя файла для загрузки: ");
                Pipe tmpPipe;
                Station tmpSt;
                if (loadFromFile(tmpPipe, tmpSt, fn)) {
                    pipe = tmpPipe;
                    st = tmpSt;
                    hasPipe = true;
                    hasStation = true;
                } else {
                    std::cout << "Не удалось загрузить объекты из файла.\n";
                }
                break;
            }
            case 0: {
                std::cout << "Выход. До свидания.\n";
                return 0;
            }
            default:
                std::cout << "Некорректный пункт меню.\n";
        }
    }

    return 0;
}