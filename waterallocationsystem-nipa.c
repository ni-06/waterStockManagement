#include <stdio.h>

#define MAX_ZONES 6
#define MAX_NAME 30
double water_allocations[MAX_ZONES];
int zone_count = 0;
int main() {
printf("Enter the number of zones (max %d): ", MAX_ZONES);
scanf("%d", &zone_count);
return 0;
}