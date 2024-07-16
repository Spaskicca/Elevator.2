#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <mutex>
#include <chrono>
#include <string>
#include <thread>

using namespace std;

class Elevator {
public:
		string direction;
	    int level;
		string doors;
		vector<int> ElevatorLevels;
		const int weightLevel = 400; //weight in kg
		static const int speed = 3; //speed in sec

		Elevator(string direction, int level) {
			this->direction = direction;
			this->level = level;
			this->doors = "closed";
			
		}
		void CallToLevel(int ElevatorLevel) {
			if (find(ElevatorLevels.begin(), ElevatorLevels.end(), ElevatorLevel) == ElevatorLevels.end()) {
				ElevatorLevels.push_back(ElevatorLevel);
				if (direction == "stay") {
					if (level < ElevatorLevel) direction = "up";
					else if (level > ElevatorLevel) direction = "down";

					thread(&Elevator::Move, this, ElevatorLevel).detach();
				}
			}
		}
		void Move(int ElevatorLevel) {
			while (!ElevatorLevels.empty()) {
				this_thread::sleep_for(chrono::seconds(speed));
				if (direction == "up") level++;
				else if (direction == "down") level--;

				if (level == ElevatorLevels.front()) {
					
						this->TakeAPerson();
					ElevatorLevels.erase(ElevatorLevels.begin());
				}
			}
			direction = "stay";
		}

        void TakeAPerson() {
            direction = "stay";
            doors = "open";
            Print();
            this_thread::sleep_for(chrono::seconds(5));
            doors = "closed";
            Print();
        }

        static void Print();
};

vector<Elevator> elevators = { Elevator("stay", 17), Elevator("stay", 5), Elevator("stay", 12) };
mutex printMutex;

void Elevator::Print() {
    lock_guard<mutex> lock(printMutex);
    system("clear"); 
    for (int i = 18; i > 0; i--) {
        cout << "Level " << i << "  ";
        for (int e = 0; e < elevators.size(); e++) {
            if (elevators[e].level == i) {
                if (elevators[e].doors == "open") cout << "\\";
                else cout << " ";
                cout << "Monkey" << (e + 1);
                if (elevators[e].doors == "open") cout << "/";
                else cout << " ";
            }
            else {
                cout << "    ";
            }
            cout << "  ";
        }
        cout << endl;
    }
    for (int i = 0; i < elevators.size(); i++) {
        cout << "Monkey" << (i + 1) << "-" << elevators[i].level << " " << elevators[i].direction << endl;
    }
    cout << "Please type in floor level number. If you want to exit, type 0." << endl;
}

void Control() {
    Elevator::Print();
    int ElevatorLevels = 1;
    while (ElevatorLevels != 0) {
        cin >> ElevatorLevels;
        if (0 < ElevatorLevels && ElevatorLevels <= 18) {
            int distance = abs(ElevatorLevels - elevators[0].level);
            int chosenElevatorID = 0;
            for (int e = 0; e < elevators.size(); e++) {
                if ((ElevatorLevels >= elevators[e].level && (elevators[e].direction == "up" || elevators[e].direction == "stay") && distance >= abs(ElevatorLevels - elevators[e].level)) ||
                    (ElevatorLevels <= elevators[e].level && (elevators[e].direction == "down" || elevators[e].direction == "stay") && distance >= abs(ElevatorLevels - elevators[e].level))) {
                    distance = abs(ElevatorLevels - elevators[e].level);
                    chosenElevatorID = e;
                }
            }
            elevators[chosenElevatorID].CallToLevel(ElevatorLevels);
        }
    }
    exit(0);
}

int main() {
    Control();
    return 0;
}
