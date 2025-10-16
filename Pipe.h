#pragma once
#include <string>
#include <iostream>
#include <fstream>

class Pipe {
    static int MaxID;
    int id;

    std::string km_mark;
    double length_km = 0.0;
    int diameter_mm = 0;
    bool in_repair = false;

public:

    int getId() { return id; }
    
    void readPipeFromConsole();
    void toggleRepair();
    void print() const;

    //для лога
    std::string getKmMark() const { return km_mark; }
    double getLength() const { return length_km; }
    int getDiameter() const { return diameter_mm; }
    bool getInRepair() const { return in_repair; }

    void savePipe(std::ofstream& ofs) const;
    static bool loadPipe(std::ifstream& ifs, Pipe& p);

    friend std::istream& operator>>(std::istream& in, Pipe& p);
    friend std::ostream& operator<<(std::ostream& out, const Pipe& p);
};