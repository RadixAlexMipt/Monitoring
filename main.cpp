#include "monitoring.h"

int main() {
    
    Monitoring monitoring;

    monitoring.getConfig();
    monitoring.run();
    
    return 0;
}