#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
using namespace std;

class Station {
private:
    string name;
    int totalWorkshops;
    int activeWorkshops;
    int stationClass;
    int id;
    static int nextId;

public:
    Station();
    int GetId() const;
    string GetName() const;
    int GetTotalWorkshops() const;
    int GetRunningWorkshops() const;
    int GetStationClass() const;
    void SetRunningWorkshops(int newVal);
    void AddStation();
    void cs_show() const;
    void cs_save(ofstream& file) const;
    Station(ifstream& file);
    int GetNumberofWorkshops() const { return totalWorkshops; }
    int GetNumberofActiveWorkshops() const { return activeWorkshops; }

    static int GetNextId();
    static void ClearNextId();
    static void setNextId(const unordered_map<int, Station>& data);
};