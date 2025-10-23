#include "filters.h"
#include <iostream>
using namespace std;

bool CheckByRepair(const Pipe& pipe, const bool& status) {
    return pipe.GetRepair() == status;
}

void FindByIsWorking(const unordered_map<int, Pipe>& pipes,
                     unordered_set<int>& selected_pipes,
                     bool is_working) {
    for (auto& [id, pipe] : pipes) {
        if (CheckByRepair(pipe, is_working)) selected_pipes.insert(id);
    }
}

bool CheckByUnusedPercent(const Station& station, const double& threshold) {
    if (station.GetNumberofWorkshops() == 0) return false;
    double percent = (station.GetNumberofWorkshops() - station.GetNumberofActiveWorkshops()) * 100.0 / station.GetNumberofWorkshops();
    return percent >= threshold;
}

void FindByUnusedWorkshops(const unordered_map<int, Station>& stations,
                           unordered_set<int>& selected_stations,
                           double threshold) {
    for (auto& [id, station] : stations) {
        if (CheckByUnusedPercent(station, threshold)) selected_stations.insert(id);
    }
}

void FindByName(const unordered_map<int, Pipe>& pipes,
                unordered_set<int>& selected_pipes,
                const string& name) {
    for (auto& [id, pipe] : pipes) {
        if (pipe.GetName().find(name) != string::npos) selected_pipes.insert(id);
    }
}

void FindByName(const unordered_map<int, Station>& stations,
                unordered_set<int>& selected_stations,
                const string& name) {
    for (auto& [id, station] : stations) {
        if (station.GetName().find(name) != string::npos) selected_stations.insert(id);
    }
}