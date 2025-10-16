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