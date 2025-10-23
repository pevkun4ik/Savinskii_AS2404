#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>

class Pipe {
public:
    static int nextId; // публичный, для уникальной идентификации

    // данные
    int id = 0;
    std::string km_mark;
    double length_km = 0.0;
    int diameter_mm = 0;
    bool in_repair = false;

    Pipe() = default;

    // интерфейс
    int getId() const { return id; }
    void readPipeFromConsole();
    void toggleRepair(); // переключить признак "в ремонте"
    void print() const;

    // для логов/отчёта
    std::string getKmMark() const { return km_mark; }
    double getLength() const { return length_km; }
    int getDiameter() const { return diameter_mm; }
    bool getInRepair() const { return in_repair; }

    // ввод/вывод в файл
    void savePipe(std::ofstream& ofs) const;
    static bool loadPipe(std::ifstream& ifs, Pipe& p);

    // поиск/редактирование
    static void findByFilter(const std::map<int, Pipe>& pipes);    // поиск — не меняет коллекцию
    static void batchEdit(std::map<int, Pipe>& pipes);           // пакетное редактирование — изменяет

    friend std::istream& operator>>(std::istream& in, Pipe& p);
    friend std::ostream& operator<<(std::ostream& out, const Pipe& p);
};