#include <stdio.h>

#define MAX_ZONES 6
#define MAX_NAME 30

int main() {
    // Using 2D arrays to store zone data
    // Row indices: 0=Z01, 1=Z02, 2=Z03, 3=Z04, 4=Z05, 5=Z06
    char zoneID[MAX_ZONES][4] = {"Z01", "Z02", "Z03", "Z04", "Z05", "Z06"};
    char serviceType[MAX_ZONES][30] = {
        "Municipal Hospital",
        "Central Flood Shelter",
        "Ward 3 Residential",
        "Ward 5 Residential",
        "School Emergency Shelter",
        "Fire and Emergency Service"
    };