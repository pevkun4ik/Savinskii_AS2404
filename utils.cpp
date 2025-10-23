#include "utils.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
using namespace std;

ofstream logfile;

void logAction(const string& action) {
    auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
    string timestamp = ctime(&now); timestamp.pop_back();
    string entry = "[" + timestamp + "] " + action + "\n";
    cerr << entry;
    if (logfile.is_open()) logfile << entry;
}

void ShowAll(const unordered_map<int, Pipe>& pipes, const unordered_map<int, Station>& stations) {
    if (pipes.empty()) cout << "No pipes\n"; else for (auto& [_, p] : pipes) p.ShowPipe();
    if (stations.empty()) cout << "No stations\n"; else for (auto& [_, s] : stations) s.cs_show();
}

void SaveAll(unordered_map<int, Pipe>& pipes, unordered_map<int, Station>& stations) {
    cout << "Enter filename: "; string fn; getline(cin >> ws, fn);
    ofstream file(fn);
    if (!file.is_open()) { cout << "Cannot open file!\n"; return; }
    for (auto& [_, p] : pipes) p.pipe_save(file);
    for (auto& [_, s] : stations) s.cs_save(file);
    cout << "Saved!\n";
    logAction("Saved to file " + fn);
}

void LoadAll(unordered_map<int, Pipe>& pipes, unordered_map<int, Station>& stations) {
    cout << "Enter filename: "; string fn; getline(cin >> ws, fn);
    ifstream file(fn);
    if (!file.is_open()) { cout << "Cannot open file!\n"; return; }

    pipes.clear(); stations.clear();
    string line;
    while (file >> line) {
        if (line == "Pipe") { Pipe p(file); pipes[p.GetId()] = p; }
        else if (line == "Station") { Station s(file); stations[s.GetId()] = s; }
    }
    Pipe::set_currentid(pipes);
    Station::setNextId(stations);
    cout << "Loaded!\n";
    logAction("Loaded from file " + fn);
}

void BatchEditPipes(unordered_map<int, Pipe>& pipes, const unordered_set<int>& selected) {
    if (selected.empty()) { cout << "No pipes selected.\n"; return; }
    cout << "1. Toggle repair\n2. Delete pipes\nChoose action: ";
    int choice = GetCorrectNumber(1,2);
    for (int id : selected) {
        if (!pipes.count(id)) continue;
        if (choice == 1) { pipes[id].ChangeRepair(); logAction("Pipe ID " + to_string(id) + " repair toggled."); }
        else { pipes.erase(id); logAction("Pipe ID " + to_string(id) + " deleted."); }
    }
    cout << "Batch edit done.\n";
}

void FilterPipesByName(const unordered_map<int, Pipe>& pipes, unordered_set<int>& selected) {
    string name;
    cout << "Enter pipe name to search: "; getline(cin >> ws, name);
    selected.clear();
    for (auto& [id,p] : pipes) if (p.GetName().find(name) != string::npos) selected.insert(id);
}

void FilterPipesByRepair(const unordered_map<int, Pipe>& pipes, unordered_set<int>& selected) {
    cout << "Enter repair status (1/0): ";
    int r = GetCorrectNumber(0,1);
    selected.clear();
    for (auto& [id,p] : pipes) if (p.GetRepair() == r) selected.insert(id);
}

void FilterStationsByName(const unordered_map<int, Station>& stations, unordered_set<int>& selected) {
    string name;
    cout << "Enter station name to search: "; getline(cin >> ws, name);
    selected.clear();
    for (auto& [id,s] : stations) if (s.GetName().find(name) != string::npos) selected.insert(id);
}

void FilterStationsByUnused(const unordered_map<int, Station>& stations, unordered_set<int>& selected) {
    cout << "Enter minimum % of non-operating workshops: ";
    double pct; cin >> pct;
    selected.clear();
    for (auto& [id,s] : stations) {
        if (s.GetTotalWorkshops() == 0) continue;
        double unused = (s.GetTotalWorkshops() - s.GetRunningWorkshops()) * 100.0 / s.GetTotalWorkshops();
        if (unused >= pct) selected.insert(id);
    }
}