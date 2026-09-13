#include <stdio.h>

//Functions
void displaySpecialties();
void displayWards();

//Doctor Specialties
int specialityID[4] = {1, 2, 3, 4};
char specialtyName[4][30] = {"General Practice (OPD)", "Paediatrics", "Cardiology", "Neurology"};
float baseFee[4] = {1500.00, 2500.00, 4500.00, 5000.00};
int consultMins[4] = {15,20,30,30};
int dailyCap[4] = {30, 20, 12, 10};

//Hospital Wards
int wardID[4] = {1, 2, 3, 4};
char wardName[4][30] = {"General Ward", "Paediatrics Ward", "Surgical Ward", "ICU"};
float bedRate [4] = {3000.00, 60000.00,12000.00, 25000.00};
int bedCapacity[4] = {20, 10, 10, 5};

int main() {
    displaySpecialties();
    displayWards();
    
    return 0;
}



void displaySpecialties() {
    int i;
    printf("\n--- Doctor Specialties ---\n");
    for (i = 0; i < 4; i++) {
        printf("%d. %-25s | Fee: %8.2f | Time: %2d mins | Daily Cap: %2d\n", specialityID[i], specialtyName[i], baseFee[i], consultMins[i], dailyCap[i]);
    }
}

void displayWards() {
    int i;
    printf("\n--- Hospital Wards ---\n");
    for (i = 0; i < 4; i++) {
        printf("%d. %-25s | Rate: %8.2f/day | Capacity: %2d\n", wardID[i], wardName[i], bedRate[i], bedCapacity[i]);
    }
}