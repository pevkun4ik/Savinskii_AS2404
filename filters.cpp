#include "filters.h"
#include <iostream>
using namespace std;

bool CheckByRepair(const Pipe& pipe, const bool& status) {
    return pipe.GetRepair() == status;
}

bool CheckByUnusedPercent(const Station& station, const double& threshold) {
    if (station.GetNumberofWorkshops() == 0) return false;
    double percent = (station.GetNumberofWorkshops() - station.GetNumberofActiveWorkshops()) * 100.0 / station.GetNumberofWorkshops();
    return percent >= threshold;
}

bool CheckByNamePipe(const Pipe& pipe, const std::string& prefix) {
    return !pipe.GetName().empty() && pipe.GetName().rfind(prefix, 0) == 0;
}

bool CheckByNameStation(const Station& station, const std::string& prefix) {
    return !station.GetName().empty() && station.GetName().rfind(prefix, 0) == 0;
}

void FindByIsWorking(const unordered_map<int, Pipe>& pipes,
                     unordered_set<int>& selected_pipes,
                     bool is_working) {
    for (auto& [id, pipe] : pipes) {
        if (CheckByRepair(pipe, is_working)) selected_pipes.insert(id);
    }
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
                const std::string& prefix) {
    selected_pipes.clear();
    for (const auto& [id, pipe] : pipes) {
        if (!pipe.GetName().empty() && pipe.GetName().rfind(prefix, 0) == 0) {
            selected_pipes.insert(id);
        }
    }
}

void FindByName(const unordered_map<int, Station>& stations,
                unordered_set<int>& selected_stations,
                const std::string& prefix) {
    selected_stations.clear();
    for (const auto& [id, station] : stations) {
        if (!station.GetName().empty() && station.GetName().rfind(prefix, 0) == 0) {
            selected_stations.insert(id);
        }
    }
}

void FilterObjects(const unordered_map<int, Pipe>& pipes,
                   unordered_set<int>& selected_pipes,
                   FilterFunc<Pipe, bool> func,
                   const bool& param) {
    selected_pipes.clear();
    for (auto& [id, pipe] : pipes)
        if (func(pipe, param)) selected_pipes.insert(id);
}

void FilterObjects(const unordered_map<int, Station>& stations,
                   unordered_set<int>& selected_stations,
                   FilterFunc<Station, double> func,
                   const double& param) {
    selected_stations.clear();
    for (auto& [id, station] : stations)
        if (func(station, param)) selected_stations.insert(id);
}