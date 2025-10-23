#include "Station.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
using namespace std;

int Station::nextId = 0;

Station::Station() : name("None"), totalWorkshops(0), activeWorkshops(0), stationClass(0), id(0) {}

int Station::GetId() const { return id; }
string Station::GetName() const { return name; }
int Station::GetTotalWorkshops() const { return totalWorkshops; }
int Station::GetRunningWorkshops() const { return activeWorkshops; }
int Station::GetStationClass() const { return stationClass; }
void Station::SetRunningWorkshops(int newVal) { activeWorkshops = newVal; }

void Station::AddStation() {
    cout << "Station name: "; getline(cin >> ws, name);
    cout << "Total workshops: "; totalWorkshops = GetCorrectNumber(1, 1000);
    cout << "Active workshops: "; activeWorkshops = GetCorrectNumber(0, totalWorkshops);
    cout << "Station class: "; stationClass = GetCorrectNumber(1, 10);
    id = ++nextId;
}

void Station::cs_show() const {
    cout << "ID: " << id
         << "\nName: " << name
         << "\nTotal workshops: " << totalWorkshops
         << "\nActive workshops: " << activeWorkshops
         << "\nStation class: " << stationClass << endl;
}

void Station::cs_save(ofstream& file) const {
    file << "Station\n" << id << "\n" << name << "\n" << totalWorkshops << "\n"
         << activeWorkshops << "\n" << stationClass << endl;
}

Station::Station(ifstream& file) {
    file >> id; file.ignore(10000, '\n');
    getline(file >> ws, name);
    file >> totalWorkshops >> activeWorkshops >> stationClass;
}

int Station::GetNextId() { return nextId; }
void Station::ClearNextId() { nextId = 0; }
void Station::setNextId(const unordered_map<int, Station>& data) { nextId = Get_maxid(data); }