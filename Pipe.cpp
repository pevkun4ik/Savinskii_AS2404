#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include "Pipe.h"
#include "utils.h"

using namespace std;

int Pipe::MaxID = 1;

// Сохраняем трубу в поток (член класса)
void Pipe::savePipe(ofstream &ofs) const {
    ofs << km_mark << '\n'
        << length_km << '\n'
        << diameter_mm << '\n'
        << in_repair << '\n';
}

// Статический загрузчик в переданный объект (без изменения позиционирования внешнего кода)
bool Pipe::loadPipe(ifstream &ifs, Pipe &p)
{
    ifs >> std::ws; // пропускаем пробельные символы перед getline
    if (!std::getline(ifs, p.km_mark)) return false;

    if (!(ifs >> p.length_km)) return false;
    if (!(ifs >> p.diameter_mm)) return false;
    if (!(ifs >> p.in_repair)) return false;

    // Удалим остаток строки после чтения чисел, чтобы следующий getline работал корректно
    ifs.ignore(numeric_limits<streamsize>::max(), '\n');

    return true;
}

// Чтение данных трубы с консоли (как метод объекта)
void Pipe::readPipeFromConsole()
{
    km_mark = readLineNonEmpty("Введите километровую отметку (название трубы): ");
    length_km = readDouble("Введите длину трубы (км, положительное число): ", 0.0, 1e6);
    diameter_mm = readInt("Введите диаметр (мм, положительное целое): ", 1, 100000);
    in_repair = readInt("Труба в ремонте? (1 - да, 0 - нет): ", 0, 1);
}

// Переключить признак ремонта (член класса)
void Pipe::toggleRepair()
{
    cout << "Текущее состояние: " << (in_repair ? "В ремонте" : "Не в ремонте") << "\n";
    in_repair = readInt("Установить противоположный признак? (1 - да, 0 - нет): ", 0, 1);
    cout << "Новое состояние: " << (in_repair ? "В ремонте" : "Не в ремонте") << "\n";
}

// Печать объекта
void Pipe::print() const
{
    cout << "=== Труба ===\n"
         << "Километровая отметка: " << km_mark << "\n"
         << "Длина (км): " << length_km << "\n"
         << "Диаметр (мм): " << diameter_mm << "\n"
         << "В ремонте: " << (in_repair ? "Да" : "Нет") << "\n";
}

// Оператор ввода (использует вспомогательные функции для удобства ввода в консоли)
std::istream& operator >> (std::istream& in, Pipe& p) {
    p.id = Pipe::MaxID++;
    p.km_mark = readLineNonEmpty("Введите километровую отметку (название трубы): ");
    p.length_km = readDouble("Введите длину трубы (км, положительное число): ", 0.0, 1e6);
    p.diameter_mm = readInt("Введите диаметр (мм, положительное целое): ", 1, 100000);
    p.in_repair = readInt("Труба в ремонте? (1 - да, 0 - нет): ", 0, 1);
    return in;
}

// Оператор вывода
std::ostream& operator << (std::ostream& out, const Pipe& pipe) {
    out << "Километровая отметка: " << pipe.km_mark << ";\n"
        << "Длина (км): " << pipe.length_km << ";\n"
        << "Диаметр (мм): " << pipe.diameter_mm << ";\n"
        << "В ремонте: " << (pipe.in_repair ? "Да" : "Нет") << ";\n\n";
    return out;
}