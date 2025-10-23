#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
using namespace std;

class Pipe {
private:
    string name;
    double length;
    int diameter;
    bool repair;
    int id;
    static int current_pipeid;

public:
    Pipe();
    int GetId() const;
    string GetName() const;
    bool GetRepair() const;
    void ChangeRepair();
    void AddPipe();
    void ShowPipe() const;
    void pipe_save(ofstream& file) const;
    Pipe(ifstream& file);

    static int Get_currentid();
    static void Clear_currentid();
    static void set_currentid(const unordered_map<int, Pipe>& data);
};