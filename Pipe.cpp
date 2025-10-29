#include "Pipe.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
using namespace std;

int Pipe::current_pipeid = 0;

Pipe::Pipe() : name("None"), length(0), diameter(0), repair(false), id(0) {}

int Pipe::GetId() const { return id; }
string Pipe::GetName() const { return name; }
bool Pipe::GetRepair() const { return repair; }

void Pipe::ChangeRepair() { repair = !repair; }

void Pipe::AddPipe() {
    cout << "Pipe name: "; getline(cin >> ws, name);
    cout << "Pipe length: "; length = GetCorrectNumber(1, 10000);
    cout << "Pipe diameter: "; diameter = GetCorrectNumber(1, 10000);
    cout << "Repair status (1/0): "; repair = GetCorrectNumber(0, 1);
    id = ++current_pipeid;
}

void Pipe::ShowPipe() const {
    cout << "PIPE ID: " << id << "\nName: " << name
         << "\nLength: " << length
         << "\nDiameter: " << diameter
         << "\nRepair: " << (repair ? "Yes" : "No") << "\n" << endl;
}

void Pipe::pipe_save(ofstream& file) const {
    file << "Pipe\n" << id << "\n" << name << "\n" << length << "\n" << diameter << "\n" << repair << endl;
}

Pipe::Pipe(ifstream& file) {
    file >> id; file.ignore(10000, '\n');
    getline(file >> ws, name);
    file >> length >> diameter >> repair;
}

int Pipe::Get_currentid() { return current_pipeid; }
void Pipe::Clear_currentid() { current_pipeid = 0; }
void Pipe::set_currentid(const unordered_map<int, Pipe>& data) { current_pipeid = Get_maxid(data); }