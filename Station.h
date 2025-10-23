#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <map>

class Station {
public:
    static int nextId; // уникальный ID для КС

    int id;
    std::string name;
    int total_workshops = 0;
    int running_workshops = 0;
    int station_class = 0;

    int getId() { return id; }

    void saveStation(std::ofstream& ofs) const;
    static bool loadStation(std::ifstream& ifs, Station& s);
    void readStationFromConsole();
    void manageWorkshop(int delta);
    void print() const;
    static void findByFilter(const std::map<int, Station>& stations);

    //для лога
    std::string getName() const { return name; }
    int getTotalWorkshops() const { return total_workshops; }
    int getRunningWorkshops() const { return running_workshops; }
    int getStationClass() const { return station_class; }

    friend std::istream& operator>>(std::istream& in, Station& s);
    friend std::ostream& operator<<(std::ostream& out, const Station& s);
};