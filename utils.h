#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "Pipe.h"
#include "Station.h"
using namespace std;

extern ofstream logfile;
void logAction(const string& action);

template<typename T>
T GetCorrectNumber(T min, T max) {
    T value;
    while ((cin >> value).fail() || cin.peek() != '\n' || value < min || value > max) {
        cin.clear(); cin.ignore(10000, '\n');
        cout << "Enter number from " << min << " to " << max << ": ";
    }
    return value;
}

template<typename T>
int Get_maxid(const unordered_map<int, T>& objs) {
    int id = 0;
    for (auto& [k, _] : objs) if (k > id) id = k;
    return id;
}

void ShowAll(const unordered_map<int, Pipe>& pipes, const unordered_map<int, Station>& stations);
void SaveAll(unordered_map<int, Pipe>& pipes, unordered_map<int, Station>& stations);
void LoadAll(unordered_map<int, Pipe>& pipes, unordered_map<int, Station>& stations);

void BatchEditPipes(unordered_map<int, Pipe>& pipes, unordered_set<int>& selected);

// Фильтры
void FilterPipesByName(const unordered_map<int, Pipe>& pipes, unordered_set<int>& selected);
void FilterPipesByRepair(const unordered_map<int, Pipe>& pipes, unordered_set<int>& selected);
void FilterStationsByName(const unordered_map<int, Station>& stations, unordered_set<int>& selected);
void FilterStationsByUnused(const unordered_map<int, Station>& stations, unordered_set<int>& selected);

// Выбор подмножества по ID
template<typename T>
void SelectById(const unordered_map<int, T>& set, unordered_set<int>& selected) {
    selected.clear();
    int id;
    while (true) {
        cout << "Enter ID (0 to stop): ";
        id = GetCorrectNumber(0, 10000);
        if (id == 0) break;
        if (set.count(id)) selected.insert(id);
    }
}

template<typename T>
void SelectAll(const unordered_map<int, T>& map, unordered_set<int>& selected) {
    selected.clear();
    for (const auto& [id, _] : map)
        selected.insert(id);
}