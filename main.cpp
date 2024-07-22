#include <cmath>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <chrono>
#include <unistd.h>

#include "sensor.h"

class Monitorning {
public:
    void run();
};

void Monitorning::run() {
    


    while (true) {

        //
        
        sleep(1);
    }
}

void getConfig() {
    //TODO: query for list of sensors & fill it
}

int main() {
    std::cout << "Hi" << std::endl;
    Monitorning a; 
    a.run();
}