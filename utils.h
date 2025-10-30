#pragma once
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <string>
#include "Pipe.h"
#include "Station.h"

//using namespace std;

// void logInput(const std::string& input);

// // Функции-обёртки для безопасного логирования ввода пользователя
std::string inputString(const std::string& prompt);
int inputInt(const std::string& prompt, int min, int max);

// Лог-файл для действий
extern ofstream logfile;
void logAction(const std::string& message);

// Ввод корректного числа
template <typename T>
T GetCorrectNumber(T min, T max) {
    T value;
    while ((cin >> value).fail() || cin.peek() != '\n' || value < min || value > max) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "\nВведите число от " << min << " до " << max << ": ";
    }
    return value;
}

// Выбор всех объектов
template<typename T>
void SelectAll(const unordered_map<int, T>& objects, unordered_set<int>& selected) {
    selected.clear();
    for (const auto& [id, obj] : objects) 
        selected.insert(id);
}

// Выбор подмножества по ID
template<typename T>
void SelectById(const unordered_map<int, T>& objects, unordered_set<int>& selected) {
    selected.clear();
    while (true) {
        int id;
        cout << "Введите ID (0 для выхода): ";
        id = GetCorrectNumber(0, 10000);
        if (id == 0) break;
        if (objects.count(id)) //!!!
            selected.insert(id);
        else cout << "Такого ID нет.\n";
    }
}

// Загрузка объектов из файла
template<typename T>
void load(ifstream& file, unordered_map<int, T>& map) {
    T object(file);
    map[object.GetId()] = object;//!!!!
}

// Получение максимального ID
template<typename T>
int Get_maxid(const unordered_map<int, T>& objs) {
    int max_id = 0;
    for (auto& [id, obj] : objs)
        if (id > max_id) 
            max_id = id;
    return max_id;
}

// Сохранение всех объектов
void SaveAll(unordered_map<int, Pipe>& pipes, unordered_map<int, Station>& stations);
void ShowAll(const unordered_map<int, Pipe>& pipes, const unordered_map<int, Station>& stations);
void LoadAll(unordered_map<int, Pipe>& pipes, unordered_map<int, Station>& stations);

// Редактирование выбранных труб
void BatchEditPipes(unordered_map<int, Pipe>& pipes, unordered_set<int>& selected_pipes);

// Редактирование выбранных станций
void BatchEditStations(unordered_map<int, Station>& stations, unordered_set<int>& selected_stations);

// Изменение статуса выбранных труб
void ChangeSelectedPipesStatus(unordered_map<int, Pipe>& pipes, const unordered_set<int>& selected_pipes);