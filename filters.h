#pragma once
#include <unordered_map>
#include <unordered_set>
#include <string>
#include "Pipe.h"
#include "Station.h"
#include "utils.h"

template<typename T, typename K>
using FilterFunc = bool(*)(const T&, const K&);

bool CheckByRepair(const Pipe& pipe, const bool& status);
void FindByIsWorking(const std::unordered_map<int, Pipe>& pipes,
                     std::unordered_set<int>& selected_pipes,
                     bool is_working);

bool CheckByUnusedPercent(const Station& station, const double& threshold);
void FindByUnusedWorkshops(const std::unordered_map<int, Station>& stations,
                           std::unordered_set<int>& selected_stations,
                           double threshold);

void FindByName(const std::unordered_map<int, Pipe>& pipes,
                std::unordered_set<int>& selected_pipes,
                const std::string& name);

void FindByName(const std::unordered_map<int, Station>& stations,
                std::unordered_set<int>& selected_stations,
                const std::string& name);