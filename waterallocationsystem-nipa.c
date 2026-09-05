#include <stdio.h>

#define ZONES 6

int main()
{
    // Data: ID, Type, Requested, Minimum, Loss%, Waiting
    // Type: 1=Hospital, 2=Shelter, 3=Residential, 4=Emergency
    int id[ZONES] = {1, 2, 3, 4, 5, 6};
    int type[ZONES] = {1, 2, 3, 3, 2, 4};
    int req[ZONES] = {4000, 3500, 4500, 3800, 2600, 2000};
    int minReq[ZONES] = {3000, 2500, 2000, 1800, 1600, 1500};
    int loss[ZONES] = {2, 4, 8, 12, 5, 1};
    int wait[ZONES] = {0, 1, 2, 3, 1, 0};
    
    int allocated[ZONES] = {0, 0, 0, 0, 0, 0};
    int effective[ZONES] = {0, 0, 0, 0, 0, 0};
    int shortage[ZONES] = {0, 0, 0, 0, 0, 0};
    int status[ZONES] = {0, 0, 0, 0, 0, 0};
    int priority[ZONES] = {0, 0, 0, 0, 0, 0};
    
    int available = 13500;
    int choice, i, j, temp, code, found;
    int totalReq, totalMin;
    
    do
    {
        printf("\n\n");
        printf("1. Show Zone List\n");
        printf("2. Run Allocation\n");
        printf("3. Show Results\n");
        printf("4. Search Zone\n");
        printf("5. Change Water Amount\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        
        if(choice == 1)
        {
            printf("\nZONE LIST:\n");
            printf("Z01 - Municipal Hospital\n");
            printf("Z02 - Central Flood Shelter\n");
            printf("Z03 - Ward 3 Residential\n");
            printf("Z04 - Ward 5 Residential\n");
            printf("Z05 - School Emergency Shelter\n");
            printf("Z06 - Fire and Emergency Service\n");
        }
        
        else if(choice == 2)
        {
            // Calculate totals
            totalReq = 0;
            totalMin = 0;
            for(i = 0; i < ZONES; i++)
            {
                totalReq = totalReq + req[i];
                totalMin = totalMin + minReq[i];
            }
            
            printf("\nAvailable Water: %d litres\n", available);
            printf("Total Requested: %d litres\n", totalReq);
            printf("Total Minimum Required: %d litres\n\n", totalMin);
            
            // CASE 1: Enough for all
            if(available >= totalReq)
            {
                printf("Enough water for all requests!\n");
                for(i = 0; i < ZONES; i++)
                {
                    allocated[i] = req[i];
                    effective[i] = allocated[i] - (allocated[i] * loss[i] / 100);
                    shortage[i] = req[i] - effective[i];
                    status[i] = 1;  // Full
                }
                available = available - totalReq;
            }
            
            // CASE 2: Enough for minimum only
            else if(available >= totalMin)
            {
                printf("Enough for minimum requirements.\n");
                printf("Giving minimum to all zones first...\n");
                
                // Give minimum to all
                for(i = 0; i < ZONES; i++)
                {
                    allocated[i] = minReq[i];
                    effective[i] = allocated[i] - (allocated[i] * loss[i] / 100);
                    shortage[i] = req[i] - effective[i];
                    status[i] = 2;  // Minimum only
                }
                
                available = available - totalMin;
                
                // Calculate priority scores
                for(i = 0; i < ZONES; i++)
                {
                    priority[i] = 0;
                    
                    if(type[i] == 1 || type[i] == 4)
                        priority[i] = priority[i] + 50;
                    else if(type[i] == 2)
                        priority[i] = priority[i] + 30;
                    else
                        priority[i] = priority[i] + 10;
                    
                    priority[i] = priority[i] + (wait[i] * 5);
                    priority[i] = priority[i] + ((req[i] - minReq[i]) / 100);
                }
                
                // Sort by priority (highest first)
                for(i = 0; i < ZONES - 1; i++)
                {
                    for(j = i + 1; j < ZONES; j++)
                    {
                        if(priority[j] > priority[i])
                        {
                            temp = priority[i];
                            priority[i] = priority[j];
                            priority[j] = temp;
                            
                            temp = id[i];
                            id[i] = id[j];
                            id[j] = temp;
                            
                            temp = type[i];
                            type[i] = type[j];
                            type[j] = temp;
                            
                            temp = req[i];
                            req[i] = req[j];
                            req[j] = temp;
                            
                            temp = minReq[i];
                            minReq[i] = minReq[j];
                            minReq[j] = temp;
                            
                            temp = loss[i];
                            loss[i] = loss[j];
                            loss[j] = temp;
                            
                            temp = wait[i];
                            wait[i] = wait[j];
                            wait[j] = temp;
                            
                            temp = allocated[i];
                            allocated[i] = allocated[j];
                            allocated[j] = temp;
                            
                            temp = effective[i];
                            effective[i] = effective[j];
                            effective[j] = temp;
                            
                            temp = shortage[i];
                            shortage[i] = shortage[j];
                            shortage[j] = temp;
                            
                            temp = status[i];
                            status[i] = status[j];
                            status[j] = temp;
                        }
                    }
                }
                
                // Distribute remaining water
                printf("Distributing extra %d litres by priority...\n", available);
                for(i = 0; i < ZONES && available > 0; i++)
                {
                    int extra = req[i] - minReq[i];
                    
                    if(extra > 0)
                    {
                        if(available >= extra)
                        {
                            allocated[i] = allocated[i] + extra;
                            effective[i] = allocated[i] - (allocated[i] * loss[i] / 100);
                            shortage[i] = req[i] - effective[i];
                            status[i] = 1;
                            available = available - extra;
                            printf("Z0%d got full\n", id[i]);
                        }
                        else
                        {
                            allocated[i] = allocated[i] + available;
                            effective[i] = allocated[i] - (allocated[i] * loss[i] / 100);
                            shortage[i] = req[i] - effective[i];
                            available = 0;
                            printf("Z0%d got partial extra\n", id[i]);
                        }
                    }
                }
            }
            
            // CASE 3: Not enough for minimum
            else
            {
                printf("NOT ENOUGH WATER FOR MINIMUM!\n");
                printf("Available: %d, Minimum Required: %d\n", available, totalMin);
                
                // Calculate priority scores
                for(i = 0; i < ZONES; i++)
                {
                    priority[i] = 0;
                    
                    if(type[i] == 1 || type[i] == 4)
                        priority[i] = priority[i] + 50;
                    else if(type[i] == 2)
                        priority[i] = priority[i] + 30;
                    else
                        priority[i] = priority[i] + 10;
                    
                    priority[i] = priority[i] + (wait[i] * 5);
                }
                
                // Sort by priority
                for(i = 0; i < ZONES - 1; i++)
                {
                    for(j = i + 1; j < ZONES; j++)
                    {
                        if(priority[j] > priority[i])
                        {
                            temp = priority[i];
                            priority[i] = priority[j];
                            priority[j] = temp;
                            
                            temp = id[i];
                            id[i] = id[j];
                            id[j] = temp;
                            
                            temp = type[i];
                            type[i] = type[j];
                            type[j] = temp;
                            
                            temp = req[i];
                            req[i] = req[j];
                            req[j] = temp;
                            
                            temp = minReq[i];
                            minReq[i] = minReq[j];
                            minReq[j] = temp;
                            
                            temp = loss[i];
                            loss[i] = loss[j];
                            loss[j] = temp;
                            
                            temp = wait[i];
                            wait[i] = wait[j];
                            wait[j] = temp;
                            
                            temp = allocated[i];
                            allocated[i] = allocated[j];
                            allocated[j] = temp;
                            
                            temp = effective[i];
                            effective[i] = effective[j];
                            effective[j] = temp;
                            
                            temp = shortage[i];
                            shortage[i] = shortage[j];
                            shortage[j] = temp;
                            
                            temp = status[i];
                            status[i] = status[j];
                            status[j] = temp;
                        }
                    }
                }
                
                // Distribute by priority
                printf("Distributing by priority...\n");
                for(i = 0; i < ZONES && available > 0; i++)
                {
                    if(available >= minReq[i])
                    {
                        allocated[i] = minReq[i];
                        effective[i] = allocated[i] - (allocated[i] * loss[i] / 100);
                        shortage[i] = req[i] - effective[i];
                        status[i] = 2;
                        available = available - minReq[i];
                        printf("Z0%d got minimum\n", id[i]);
                    }
                    else if(available > 0)
                    {
                        allocated[i] = available;
                        effective[i] = allocated[i] - (allocated[i] * loss[i] / 100);
                        shortage[i] = req[i] - effective[i];
                        status[i] = 3;
                        printf("Z0%d got partial (%d litres)\n", id[i], available);
                        available = 0;
                    }
                    else
                    {
                        allocated[i] = 0;
                        effective[i] = 0;
                        shortage[i] = req[i];
                        status[i] = 4;
                        printf("Z0%d got nothing\n", id[i]);
                    }
                }
            }
            
            printf("\nAllocation complete!\n");
        }
        
        else if(choice == 3)
        {
            printf("\n===== RESULTS =====\n");
            
            int totalAlloc = 0;
            int totalEff = 0;
            int totalShort = 0;
            int fullCount = 0, minCount = 0, belowCount = 0, unservedCount = 0;
            
            for(i = 0; i < ZONES; i++)
            {
                printf("\nZ0%d: ", id[i]);
                
                if(type[i] == 1)
                    printf("Hospital ");
                else if(type[i] == 2)
                    printf("Shelter  ");
                else if(type[i] == 3)
                    printf("Resident ");
                else
                    printf("Service  ");
                
                printf("Req:%d Min:%d Alloc:%d Eff:%d Short:%d ",
                       req[i], minReq[i], allocated[i], effective[i], shortage[i]);
                
                if(status[i] == 1)
                {
                    printf("Status:FULL");
                    fullCount = fullCount + 1;
                }
                else if(status[i] == 2)
                {
                    printf("Status:MINIMUM");
                    minCount = minCount + 1;
                }
                else if(status[i] == 3)
                {
                    printf("Status:BELOW MIN");
                    belowCount = belowCount + 1;
                }
                else
                {
                    printf("Status:UNSERVED");
                    unservedCount = unservedCount + 1;
                }
                
                totalAlloc = totalAlloc + allocated[i];
                totalEff = totalEff + effective[i];
                totalShort = totalShort + shortage[i];
            }
            
            printf("\n\n=== SUMMARY ===\n");
            printf("Total Allocated: %d\n", totalAlloc);
            printf("Total Effective: %d\n", totalEff);
            printf("Total Shortage: %d\n", totalShort);
            printf("Remaining Water: %d\n", available);
            printf("\nFULL: %d, MINIMUM: %d, BELOW MIN: %d, UNSERVED: %d\n",
                   fullCount, minCount, belowCount, unservedCount);
        }
        
        else if(choice == 4)
        {
            printf("\nEnter zone code (1-6): ");
            scanf("%d", &code);
            
            found = 0;
            for(i = 0; i < ZONES; i++)
            {
                if(id[i] == code)
                {
                    found = 1;
                    printf("\n=== ZONE FOUND ===\n");
                    printf("Zone: Z0%d\n", id[i]);
                    
                    if(type[i] == 1)
                        printf("Type: Hospital\n");
                    else if(type[i] == 2)
                        printf("Type: Shelter\n");
                    else if(type[i] == 3)
                        printf("Type: Residential\n");
                    else
                        printf("Type: Emergency Service\n");
                    
                    printf("Requested: %d\n", req[i]);
                    printf("Minimum: %d\n", minReq[i]);
                    printf("Loss: %d%%\n", loss[i]);
                    printf("Waiting: %d\n", wait[i]);
                    printf("Allocated: %d\n", allocated[i]);
                    printf("Effective: %d\n", effective[i]);
                    printf("Shortage: %d\n", shortage[i]);
                    
                    if(status[i] == 1)
                        printf("Status: FULL\n");
                    else if(status[i] == 2)
                        printf("Status: MINIMUM\n");
                    else if(status[i] == 3)
                        printf("Status: BELOW MINIMUM\n");
                    else
                        printf("Status: UNSERVED\n");
                }
            }
            
            if(found == 0)
                printf("Zone not found!\n");
        }
        
        else if(choice == 5)
        {
            printf("\nEnter new water amount: ");
            scanf("%d", &available);
            
            if(available >= 0)
                printf("Updated to %d litres\n", available);
            else
                printf("Invalid amount!\n");
        }
        
        else if(choice == 6)
        {
            printf("\nExiting... Thank you!\n");
        }
        
        else
        {
            printf("Invalid choice!\n");
        }
        
    } while(choice != 6);
    
    return 0;
}
