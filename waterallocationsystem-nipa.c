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
    int requested[MAX_ZONES] = {4000, 3500, 4500, 3800, 2600, 2000};
    int minimum[MAX_ZONES] = {3000, 2500, 2000, 1800, 1600, 1500};
    int lossPercent[MAX_ZONES] = {20, 4, 8, 12, 5, 1};
    int waitingCycles[MAX_ZONES] = {2, 1, 2, 3, 1, 0};
    int priority[MAX_ZONES] = {0, 0, 0, 0, 0, 0};
    int allocated[MAX_ZONES] = {0, 0, 0, 0, 0, 0};
    int effectiveDelivered[MAX_ZONES] = {0, 0, 0, 0, 0, 0};
    int shortage[MAX_ZONES] = {0, 0, 0, 0, 0, 0};
    char serviceCondition[MAX_ZONES][25] = {"", "", "", "", "", ""};
    
    int availableWater = 13500;
    int choice;
    int i, j, temp;
    int tempArray[MAX_ZONES];
    char tempStr[30];
    
    do {
        printf("\n========== WATER ALLOCATION SYSTEM ==========\n");
        printf("1. Calculate Priority & Allocate Water\n");
        printf("2. Search Zone Information\n");
        printf("3. Sort Zones by Service Condition\n");
        printf("4. Find Zone with Highest Shortage\n");
        printf("5. Display Complete Summary\n");
        printf("6. Test with Different Water Amounts\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                printf("\nEnter available water (litres): ");
                scanf("%d", &availableWater);
                // Reset allocated values
                for(i = 0; i < MAX_ZONES; i++) {
                    allocated[i] = 0;
                    effectiveDelivered[i] = 0;
                    shortage[i] = 0;
                }
                // Calculate priority and allocate
                calculatePriority(zoneID, serviceType, requested, minimum, 
                                lossPercent, waitingCycles, priority, availableWater);
                allocateWater(zoneID, serviceType, requested, minimum, 
                            lossPercent, waitingCycles, priority, allocated, 
                            effectiveDelivered, shortage, serviceCondition, &availableWater);
                displaySummary(zoneID, serviceType, requested, minimum, priority, 
                             allocated, effectiveDelivered, shortage, serviceCondition, availableWater);
                break;
                
            case 2:
                searchZone(zoneID, serviceType, requested, minimum, 
                          lossPercent, waitingCycles, priority, allocated, 
                          effectiveDelivered, shortage, serviceCondition);
                break;
                
            case 3:
                sortByServiceCondition(zoneID, serviceType, requested, minimum, 
                                      priority, allocated, effectiveDelivered, 
                                      shortage, serviceCondition);
                break;
                
            case 4:
                findHighestShortage(zoneID, serviceType, shortage, serviceCondition);
                break;
                
            case 5:
                displaySummary(zoneID, serviceType, requested, minimum, priority, 
                             allocated, effectiveDelivered, shortage, serviceCondition, availableWater);
                break;
                
            case 6:
                printf("\n--- TEST CASES ---\n");
                printf("\nCase 1: Original - 13,500 litres\n");
                availableWater = 13500;
                for(i = 0; i < MAX_ZONES; i++) {
                    allocated[i] = 0;
                    effectiveDelivered[i] = 0;
                    shortage[i] = 0;
                }
                calculatePriority(zoneID, serviceType, requested, minimum, 
                                lossPercent, waitingCycles, priority, availableWater);
                allocateWater(zoneID, serviceType, requested, minimum, 
                            lossPercent, waitingCycles, priority, allocated, 
                            effectiveDelivered, shortage, serviceCondition, &availableWater);
                displaySummary(zoneID, serviceType, requested, minimum, priority, 
                             allocated, effectiveDelivered, shortage, serviceCondition, availableWater);
                
                printf("\nCase 2: Sufficient - 25,000 litres\n");
                availableWater = 25000;
                for(i = 0; i < MAX_ZONES; i++) {
                    allocated[i] = 0;
                    effectiveDelivered[i] = 0;
                    shortage[i] = 0;
                }
                calculatePriority(zoneID, serviceType, requested, minimum, 
                                lossPercent, waitingCycles, priority, availableWater);
                allocateWater(zoneID, serviceType, requested, minimum, 
                            lossPercent, waitingCycles, priority, allocated, 
                            effectiveDelivered, shortage, serviceCondition, &availableWater);
                displaySummary(zoneID, serviceType, requested, minimum, priority, 
                             allocated, effectiveDelivered, shortage, serviceCondition, availableWater);
                
                printf("\nCase 3: Below Minimum - 8,000 litres\n");
                availableWater = 8000;
                for(i = 0; i < MAX_ZONES; i++) {
                    allocated[i] = 0;
                    effectiveDelivered[i] = 0;
                    shortage[i] = 0;
                }
                calculatePriority(zoneID, serviceType, requested, minimum, 
                                lossPercent, waitingCycles, priority, availableWater);
                allocateWater(zoneID, serviceType, requested, minimum, 
                            lossPercent, waitingCycles, priority, allocated, 
                            effectiveDelivered, shortage, serviceCondition, &availableWater);
                displaySummary(zoneID, serviceType, requested, minimum, priority, 
                             allocated, effectiveDelivered, shortage, serviceCondition, availableWater);
                break;
                
            case 7:
                printf("Exiting program...\n");
                break;
                
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while(choice != 7);
    
    return 0;
}

// Function to calculate priority
void calculatePriority(char zoneID[][4], char serviceType[][30], int requested[], 
                      int minimum[], int lossPercent[], int waitingCycles[], 
                      int priority[], int availableWater) {
    int i, j;
    int priorityScore[MAX_ZONES];
    int tempScore;
    char tempID[4];
    char tempService[30];
    int tempRequested, tempMinimum, tempLoss, tempWaiting, tempPriority;
    
    // Calculate priority scores
    for(i = 0; i < MAX_ZONES; i++) {
        priorityScore[i] = 0;
        
        // Check service type for emergency priority
        if(serviceType[i][0] == 'M' || serviceType[i][0] == 'F') {
            priorityScore[i] += 50;
        }
        
        // Check for shelter
        if(serviceType[i][0] == 'C' || serviceType[i][0] == 'S') {
            priorityScore[i] += 30;
        }
        
        // Add waiting cycles priority
        priorityScore[i] += waitingCycles[i] * 15;
        
        // Add efficiency factor (lower loss = higher priority)
        priorityScore[i] += (20 - lossPercent[i]);
        
        // Add need factor
        if(requested[i] / minimum[i] > 2) {
            priorityScore[i] += 10;
        }
    }
    
    // Sort by priority using bubble sort (descending)
    for(i = 0; i < MAX_ZONES - 1; i++) {
        for(j = 0; j < MAX_ZONES - i - 1; j++) {
            if(priorityScore[j] < priorityScore[j + 1]) {
                // Swap priority scores
                tempScore = priorityScore[j];
                priorityScore[j] = priorityScore[j + 1];
                priorityScore[j + 1] = tempScore;
                
                // Swap zone ID
                for(int k = 0; k < 4; k++) {
                    tempID[k] = zoneID[j][k];
                    zoneID[j][k] = zoneID[j + 1][k];
                    zoneID[j + 1][k] = tempID[k];
                }
                
                // Swap service type
                for(int k = 0; k < 30; k++) {
                    tempService[k] = serviceType[j][k];
                    serviceType[j][k] = serviceType[j + 1][k];
                    serviceType[j + 1][k] = tempService[k];
                }
                
                // Swap other attributes
                tempRequested = requested[j];
                requested[j] = requested[j + 1];
                requested[j + 1] = tempRequested;
                
                tempMinimum = minimum[j];
                minimum[j] = minimum[j + 1];
                minimum[j + 1] = tempMinimum;
                
                tempLoss = lossPercent[j];
                lossPercent[j] = lossPercent[j + 1];
                lossPercent[j + 1] = tempLoss;
                
                tempWaiting = waitingCycles[j];
                waitingCycles[j] = waitingCycles[j + 1];
                waitingCycles[j + 1] = tempWaiting;
            }
        }
    }
    
    // Assign priority numbers
    for(i = 0; i < MAX_ZONES; i++) {
        priority[i] = i + 1;
    }
    
    printf("\nPriority assigned based on:\n");
    printf("- Emergency service type\n");
    printf("- Waiting cycles\n");
    printf("- Distribution efficiency\n");
    printf("- Need urgency\n");
}

// Function to allocate water
void allocateWater(char zoneID[][4], char serviceType[][30], int requested[], 
                  int minimum[], int lossPercent[], int waitingCycles[], 
                  int priority[], int allocated[], int effectiveDelivered[], 
                  int shortage[], char serviceCondition[][25], int *availableWater) {
    int i;
    int totalMinimum = 0;
    int totalRequested = 0;
    
    // Calculate totals
    for(i = 0; i < MAX_ZONES; i++) {
        totalMinimum += minimum[i];
        totalRequested += requested[i];
    }
    
    // Case 1: Sufficient water for all
    if(*availableWater >= totalRequested) {
        for(i = 0; i < MAX_ZONES; i++) {
            allocated[i] = requested[i];
            effectiveDelivered[i] = allocated[i] * (100 - lossPercent[i]) / 100;
            shortage[i] = 0;
            // Set service condition
            for(int j = 0; j < 25; j++) {
                serviceCondition[i][j] = '\0';
            }
            char *cond = "Fully Satisfied";
            for(int j = 0; cond[j] != '\0'; j++) {
                serviceCondition[i][j] = cond[j];
            }
        }
        return;
    }
    
    // Case 2: Enough for minimums but not all
    if(*availableWater >= totalMinimum) {
        // Allocate minimum to all
        for(i = 0; i < MAX_ZONES; i++) {
            allocated[i] = minimum[i];
            *availableWater -= minimum[i];
        }
        
        // Distribute remaining
        for(i = 0; i < MAX_ZONES; i++) {
            int remaining = requested[i] - minimum[i];
            if(remaining > 0 && *availableWater > 0) {
                if(*availableWater >= remaining) {
                    allocated[i] += remaining;
                    *availableWater -= remaining;
                } else {
                    allocated[i] += *availableWater;
                    *availableWater = 0;
                }
            }
        }
        
        // Calculate delivery and shortage
        for(i = 0; i < MAX_ZONES; i++) {
            effectiveDelivered[i] = allocated[i] * (100 - lossPercent[i]) / 100;
            shortage[i] = requested[i] - allocated[i];
            
            // Set service condition
            for(int j = 0; j < 25; j++) {
                serviceCondition[i][j] = '\0';
            }
            
            if(shortage[i] == 0) {
                char *cond = "Fully Satisfied";
                for(int j = 0; cond[j] != '\0'; j++) {
                    serviceCondition[i][j] = cond[j];
                }
            } else if(effectiveDelivered[i] >= minimum[i]) {
                char *cond = "Minimum Satisfied";
                for(int j = 0; cond[j] != '\0'; j++) {
                    serviceCondition[i][j] = cond[j];
                }
            } else {
                char *cond = "Below Minimum";
                for(int j = 0; cond[j] != '\0'; j++) {
                    serviceCondition[i][j] = cond[j];
                }
            }
        }
        return;
    }
    
    // Case 3: Not enough for minimums
    if(*availableWater < totalMinimum) {
        for(i = 0; i < MAX_ZONES; i++) {
            if(*availableWater > 0) {
                // Priority for emergency services
                if(serviceType[i][0] == 'M' || serviceType[i][0] == 'F') {
                    if(*availableWater >= minimum[i]) {
                        allocated[i] = minimum[i];
                        *availableWater -= minimum[i];
                    } else {
                        allocated[i] = *availableWater;
                        *availableWater = 0;
                    }
                } else if(serviceType[i][0] == 'C' || serviceType[i][0] == 'S') {
                    int shelterAlloc = minimum[i] * 6 / 10;
                    if(*availableWater >= shelterAlloc) {
                        allocated[i] = shelterAlloc;
                        *availableWater -= shelterAlloc;
                    } else {
                        allocated[i] = *availableWater;
                        *availableWater = 0;
                    }
                } else {
                    int residentialAlloc = minimum[i] * 4 / 10;
                    if(*availableWater >= residentialAlloc) {
                        allocated[i] = residentialAlloc;
                        *availableWater -= residentialAlloc;
                    } else {
                        allocated[i] = *availableWater;
                        *availableWater = 0;
                    }
                }
            } else {
                allocated[i] = 0;
            }
            
            effectiveDelivered[i] = allocated[i] * (100 - lossPercent[i]) / 100;
            shortage[i] = requested[i] - allocated[i];
            
            // Set service condition
            for(int j = 0; j < 25; j++) {
                serviceCondition[i][j] = '\0';
            }
            
            if(allocated[i] == 0) {
                char *cond = "Unserved";
                for(int j = 0; cond[j] != '\0'; j++) {
                    serviceCondition[i][j] = cond[j];
                }
            } else if(effectiveDelivered[i] >= minimum[i]) {
                char *cond = "Minimum Satisfied";
                for(int j = 0; cond[j] != '\0'; j++) {
                    serviceCondition[i][j] = cond[j];
                }
            } else {
                char *cond = "Below Minimum";
                for(int j = 0; cond[j] != '\0'; j++) {
                    serviceCondition[i][j] = cond[j];
                }
            }
        }
    }
}

// Function to search for a zone
void searchZone(char zoneID[][4], char serviceType[][30], int requested[], 
               int minimum[], int lossPercent[], int waitingCycles[], 
               int priority[], int allocated[], int effectiveDelivered[], 
               int shortage[], char serviceCondition[][25]) {
    char searchId[4];
    int i, found = 0;
    
    printf("Enter Zone ID to search (e.g., Z01): ");
    scanf("%s", searchId);
    
    for(i = 0; i < MAX_ZONES; i++) {
        // Compare zone IDs
        int match = 1;
        for(int j = 0; j < 4; j++) {
            if(zoneID[i][j] != searchId[j]) {
                match = 0;
                break;
            }
        }
        
        if(match) {
            printf("\n=== Zone Information ===\n");
            printf("ID: %s\n", zoneID[i]);
            printf("Service Type: %s\n", serviceType[i]);
            printf("Requested: %d L\n", requested[i]);
            printf("Minimum: %d L\n", minimum[i]);
            printf("Loss: %d%%\n", lossPercent[i]);
            printf("Waiting Cycles: %d\n", waitingCycles[i]);
            printf("Priority: %d\n", priority[i]);
            printf("Allocated: %d L\n", allocated[i]);
            printf("Effective Delivered: %d L\n", effectiveDelivered[i]);
            printf("Shortage: %d L\n", shortage[i]);
            printf("Service Condition: %s\n", serviceCondition[i]);
            found = 1;
            break;
        }
    }
    
    if(!found) {
        printf("Zone not found!\n");
    }
}

// Function to sort by service condition
void sortByServiceCondition(char zoneID[][4], char serviceType[][30], int requested[], 
                           int minimum[], int priority[], int allocated[], 
                           int effectiveDelivered[], int shortage[], 
                           char serviceCondition[][25]) {
    int i, j;
    int order[MAX_ZONES];
    char tempID[4], tempService[30], tempCondition[25];
    int tempRequested, tempMinimum, tempPriority, tempAllocated, tempEffective, tempShortage;
    
    // Assign order values
    for(i = 0; i < MAX_ZONES; i++) {
        if(serviceCondition[i][0] == 'F') order[i] = 1;
        else if(serviceCondition[i][0] == 'M') order[i] = 2;
        else if(serviceCondition[i][0] == 'B') order[i] = 3;
        else if(serviceCondition[i][0] == 'U') order[i] = 4;
        else order[i] = 5;
    }
    
    // Bubble sort
    for(i = 0; i < MAX_ZONES - 1; i++) {
        for(j = 0; j < MAX_ZONES - i - 1; j++) {
            if(order[j] > order[j + 1]) {
                // Swap order
                int tempOrder = order[j];
                order[j] = order[j + 1];
                order[j + 1] = tempOrder;
                
                // Swap ID
                for(int k = 0; k < 4; k++) {
                    tempID[k] = zoneID[j][k];
                    zoneID[j][k] = zoneID[j + 1][k];
                    zoneID[j + 1][k] = tempID[k];
                }
                
                // Swap service type
                for(int k = 0; k < 30; k++) {
                    tempService[k] = serviceType[j][k];
                    serviceType[j][k] = serviceType[j + 1][k];
                    serviceType[j + 1][k] = tempService[k];
                }
                
                // Swap condition
                for(int k = 0; k < 25; k++) {
                    tempCondition[k] = serviceCondition[j][k];
                    serviceCondition[j][k] = serviceCondition[j + 1][k];
                    serviceCondition[j + 1][k] = tempCondition[k];
                }
                
                // Swap numeric values
                tempRequested = requested[j];
                requested[j] = requested[j + 1];
                requested[j + 1] = tempRequested;
                
                tempMinimum = minimum[j];
                minimum[j] = minimum[j + 1];
                minimum[j + 1] = tempMinimum;
                
                tempPriority = priority[j];
                priority[j] = priority[j + 1];
                priority[j + 1] = tempPriority;
                
                tempAllocated = allocated[j];
                allocated[j] = allocated[j + 1];
                allocated[j + 1] = tempAllocated;
                
                tempEffective = effectiveDelivered[j];
                effectiveDelivered[j] = effectiveDelivered[j + 1];
                effectiveDelivered[j + 1] = tempEffective;
                
                tempShortage = shortage[j];
                shortage[j] = shortage[j + 1];
                shortage[j + 1] = tempShortage;
            }
        }
    }
    
    printf("\nZones sorted by Service Condition:\n");
    for(i = 0; i < MAX_ZONES; i++) {
        printf("%s: %s\n", zoneID[i], serviceCondition[i]);
    }
}

// Function to find zone with highest shortage
void findHighestShortage(char zoneID[][4], char serviceType[][30], 
                        int shortage[], char serviceCondition[][25]) {
    int i;
    int maxShortage = 0;
    int maxIndex = 0;
    
    for(i = 0; i < MAX_ZONES; i++) {
        if(shortage[i] > maxShortage) {
            maxShortage = shortage[i];
            maxIndex = i;
        }
    }
    
    printf("\n=== Zone with Highest Shortage ===\n");
    printf("Zone ID: %s\n", zoneID[maxIndex]);
    printf("Service Type: %s\n", serviceType[maxIndex]);
    printf("Shortage: %d L\n", shortage[maxIndex]);
    printf("Service Condition: %s\n", serviceCondition[maxIndex]);
}

// Function to display summary
void displaySummary(char zoneID[][4], char serviceType[][30], int requested[], 
                   int minimum[], int priority[], int allocated[], 
                   int effectiveDelivered[], int shortage[], 
                   char serviceCondition[][25], int availableWater) {
    int i;
    int totalAllocated = 0;
    int totalEffective = 0;
    int totalShortage = 0;
    int fullySatisfied = 0, minSatisfied = 0, belowMin = 0, unserved = 0;
    
    // Calculate totals
    for(i = 0; i < MAX_ZONES; i++) {
        totalAllocated += allocated[i];
        totalEffective += effectiveDelivered[i];
        totalShortage += shortage[i];
        
        if(serviceCondition[i][0] == 'F') fullySatisfied++;
        else if(serviceCondition[i][0] == 'M') minSatisfied++;
        else if(serviceCondition[i][0] == 'B') belowMin++;
        else if(serviceCondition[i][0] == 'U') unserved++;
    }
    
    printf("\n========== WATER ALLOCATION SUMMARY ==========\n");
    printf("\n%-6s %-30s %-12s %-12s %-10s %-12s %-12s %-12s %-15s\n", 
           "Zone", "Service Type", "Requested", "Minimum", "Priority", 
           "Allocated", "Effective", "Shortage", "Condition");
    printf("--------------------------------------------------------------------------------------------------------\n");
    
    for(i = 0; i < MAX_ZONES; i++) {
        printf("%-6s %-30s %-12d %-12d %-10d %-12d %-12d %-12d %-15s\n",
               zoneID[i],
               serviceType[i],
               requested[i],
               minimum[i],
               priority[i],
               allocated[i],
               effectiveDelivered[i],
               shortage[i],
               serviceCondition[i]);
    }
    
    printf("\n========== TOTALS ==========\n");
    printf("Total Water Requested: %d L\n", 4000 + 3500 + 4500 + 3800 + 2600 + 2000);
    printf("Total Water Allocated: %d L\n", totalAllocated);
    printf("Total Effective Delivered: %d L\n", totalEffective);
    printf("Total Shortage: %d L\n", totalShortage);
    printf("Remaining Water: %d L\n", availableWater - totalAllocated);
    printf("\nService Condition Summary:\n");
    printf("  Fully Satisfied: %d zones\n", fullySatisfied);
    printf("  Minimum Satisfied: %d zones\n", minSatisfied);
    printf("  Below Minimum: %d zones\n", belowMin);
    printf("  Unserved: %d zones\n", unserved);
}