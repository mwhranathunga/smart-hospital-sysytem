#include <stdio.h>

//Functions
void displaySpecialties();
void displayWards();
void initBeds();
int findFreeBed(int ward);
void displayBedStatus(int ward);
int getIntInRange(int min, int max);
void registerPatient();
float calcWaitTime(int specIDX);
float calcSurcharge(int triage, float fee);
float calcWardCost(int days, int wIDX);
float calcDiscount(int age, float gross);
void printBill(int i);
void printPriorityList();
void generationReports();
void saveBedsToFile();
void loadBedsFromFile();

//Doctor Specialties
int specialityID[4] = {1, 2, 3, 4};
char specialtyName[4][30] = {"General Practice (OPD)", "Paediatrics", "Cardiology", "Neurology"};
float baseFee[4] = {1500.00, 2500.00, 4500.00, 5000.00};
int consultMins[4] = {15,20,30,30};
int dailyCap[4] = {30, 20, 12, 10};

//Hospital Wards
int wardID[4] = {1, 2, 3, 4};
char wardName[4][30] = {"General Ward", "Paediatrics Ward", "Surgical Ward", "ICU"};
float bedRate [4] = {3000.00, 6000.00, 12000.00, 25000.00};
int bedCapacity[4] = {20, 10, 10, 5};

int bedOccupancy[4][20];

//Patient Records
#define MAX_PATIENTS 100

char patientName[MAX_PATIENTS][50];
int patientAge[MAX_PATIENTS];
int triageLevel[MAX_PATIENTS];
int specialtyIdx[MAX_PATIENTS];
int isAdmitted[MAX_PATIENTS];
int wardIdx[MAX_PATIENTS];
int bedNumber[MAX_PATIENTS];
int daysAdmitted[MAX_PATIENTS];

float waitTime[MAX_PATIENTS];
float surcharge[MAX_PATIENTS];
float wardCost[MAX_PATIENTS];
float grossTotal[MAX_PATIENTS];
float discount[MAX_PATIENTS];
float finalAmount[MAX_PATIENTS];

int patientCount = 0;
int queueCount[4] = {0, 0, 0, 0};


int main() {
    int choice;

    initBeds();

    loadBedsFromFile();

    do {
        printf("\n==== SMART HOSPITAL MENU ====\n");
        printf("1. Register Patient\n");
        printf("2. View Bed Status\n");
        printf("3. View Priority List\n");
        printf("4. Generate Reports\n");
        printf("5. Exit\n");

        choice = getIntInRange(1, 5);

        switch (choice) {
            case 1:
                registerPatient();
                break;
            case 2:
                displaySpecialties();
                displayWards();
                displayBedStatus(0);
                displayBedStatus(1);
                displayBedStatus(2);
                displayBedStatus(3);
                break;
            case 3:
                printf("Coming soon: View Priority List\n");
                break;
            case 4:
                generationReports();
                break;
            case 5:
                saveBedsToFile();
                printf("Exiting... Goodbye!\n");
                break;
        }
    } while (choice != 5);

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

void initBeds() {
    int w, b;
    for (w = 0; w < 4; w++) {
        for (b = 0; b < 20; b++) {
            bedOccupancy[w][b] = 0;
        }
    }
}

int findFreeBed(int ward) {
    int b;
    for (b = 0; b < bedCapacity[ward]; b++) {
        if (bedOccupancy[ward][b] == 0) {
            return b; 
        }
    }
    return -1; 
}

void displayBedStatus(int ward) {
    int b;
    printf("\n--- Bed Status for %s ---\n", wardName[ward]);
    for (b = 0; b < bedCapacity[ward]; b++) {
        printf("Bed #%02d: %s\n", b + 1, bedOccupancy[ward][b] == 0 ? "Available" : "Occupied");
    }
}

int getIntInRange(int min, int max) {
    int value;
    int status;
    do {
        printf("Enter a value between %d and %d: ", min, max);
        status = scanf("%d", &value);
         
        if (status !=1) {
            while (getchar() != '\n');
        }

    } while (status != 1 || value < min || value > max);
    return value;
}

void registerPatient() {
    int i = patientCount;
    int specID;

    printf("\n--- Register New Patient ---\n");

    printf("Patient Name: ");
    scanf(" %49[^\n]", patientName[i]);

    printf("Age: ");
    patientAge[i] = getIntInRange(0, 120);

    printf("Triage Level (1=Normal, 2=Urgent, 3=Critical): ");
    triageLevel[i] = getIntInRange(1, 3);

    printf("Specialty (1=General, 2=Paediatrics, 3=Cardiology, 4=Neurology): ");
    specID = getIntInRange(1, 4);
    specialtyIdx[i] = specID - 1;

    if (queueCount[specialtyIdx[i]] >= dailyCap[specialtyIdx[i]]) {
        printf("Note: Daily patient cap reached for this specialty today.\n");
    }

    printf("Admitted to ward? (1=Yes, 0=No): ");
    isAdmitted[i] = getIntInRange(0, 1);

    if (isAdmitted[i] == 1) {
        int wID, freeBed;

        printf("Ward (1=General, 2=Paediatric, 3=Surgical, 4=ICU): ");
        wID = getIntInRange(1, 4);
        wardIdx[i] = wID - 1;

        freeBed = findFreeBed(wardIdx[i]);

        if (freeBed == -1) {
            printf("Sorry, %s is full, Registering as outpatient instead.\n", wardName[wardIdx[i]]);
            isAdmitted[i] = 0;
            daysAdmitted[i] = 0;
        } else {
            bedNumber[i] = freeBed;
            bedOccupancy[wardIdx[i]][freeBed] = 1;
            printf("Days Admitted: ");
            daysAdmitted[i] = getIntInRange(1, 365);        
        }
        
    } else {
        wardIdx[i] = -1;
        daysAdmitted[i] = 0;
    }

    waitTime[i] = calcWaitTime(specialtyIdx[i]);
    surcharge[i] = calcSurcharge(triageLevel[i], baseFee[specialtyIdx[i]]);
    wardCost[i] = calcWardCost(daysAdmitted[i], wardIdx[i]);
    grossTotal[i] = baseFee[specialtyIdx[i]] + surcharge[i] + wardCost[i];
    discount[i] = calcDiscount(patientAge[i], grossTotal[i]);
    finalAmount[i] = grossTotal[i] - discount[i];


    patientCount++;
    queueCount[specialtyIdx[i]]++;

    printBill(i);
}

float calcWaitTime(int specIdx) {
    return queueCount[specIdx] * consultMins[specIdx];
}

float calcSurcharge(int triage, float fee) {
    if (triage == 2) {
        return fee * 0.20;
    } else if (triage == 3) {
        return fee * 0.50;
    }
    return 0;
}

float calcWardCost(int days, int wIdx) {
    if (days > 0) {
        return days * bedRate[wIdx];
    }
    return 0;
}

float calcDiscount(int age, float gross) {
    if (age < 5 || age > 65) {
        return gross * 0.15;
    }
    return 0;
}

void printBill(int i) {
    int patID = 1000 + i + 1;

    printf("\n====================================================\n");
    printf(" SMART HOSPITAL ADMISSION & BILL\n");
    printf("----------------------------------------------------------------------------------------\n");
    printf("Patient ID      : PAT-%d\n", patID);
    printf("Patient Name    : %s\n", patientName[i]);

    printf("Age             : %d Years", patientAge[i]);
    if (patientAge[i] < 5 || patientAge[i] > 65) {
        printf(" (15%% Subsidy Eligible)\n");
    } else {
        printf("\n");
    }

    printf("Specialty       : %s\n", specialtyName[specialtyIdx[i]]);

    if (isAdmitted[i] == 1) {
        printf("Assigned Ward   : %s (Bed #%02d)\n", wardName[wardIdx[i]], bedNumber[i] + 1);
    } else {
        printf("Assigned Ward   : Outpatient (OPD)\n");
    }

    printf("Urgency Level   : ");
    switch (triageLevel[i]) {
        case 1: printf("Level 1 (Normal)\n"); break;
        case 2: printf("Level 2 (Urgent)\n"); break;
        case 3: printf("Level 3 (Critical)\n"); break;
    }

    printf("----------------------------------------------------------------------------------------\n");
    printf("Base Consultation Fee   : LKR %.2f\n", baseFee[specialtyIdx[i]]);

    if (triageLevel[i] == 1) {
        printf("Emergency Surcharge     : LKR %.2f (0%%)\n", surcharge[i]);
    } else if (triageLevel[i] == 2) {
        printf("Emergency Surcharge     : LKR %.2f (20%%)\n", surcharge[i]);
    } else {
        printf("Emergency Surcharge     : LKR %.2f (50%%)\n", surcharge[i]);
    }

    printf("Ward Stay Cost (%d Days) : LKR %.2f\n", daysAdmitted[i], wardCost[i]);

    printf("----------------------------------------------------------------------------------------\n");
    printf("Gross Total Bill        : LKR %.2f\n", grossTotal[i]);
    printf("Age Subsidy Discount    : LKR -%.2f", discount[i]);
    if (discount[i] > 0) {
        printf(" (15%%)\n");
    } else {
        printf("\n");
    }

    printf("----------------------------------------------------------------------------------------\n");
    printf("Final Payable Amount    : LKR %.2f\n", finalAmount[i]);

    if (waitTime[i] == 0) {
        printf("Estimated Waiting Time  : 0.00 mins (Immediate Attention)\n");
    } else {
        printf("Estimated Waiting Time  : %.2f mins\n", waitTime[i]);
    }
    printf("====================================================\n");
}


void printPriorityList() {
    int order[MAX_PATIENTS];
    int i, j, temp;

    if (patientCount == 0) {
        printf("\nNo patients registered yet.\n");
        return;
    }

    for (i = 0; i < patientCount; i++) {
        order[i] = i;
    }

    for (i = 0; i < patientCount - 1; i++) {
        for (j = 0; j < patientCount - 1 - i; j++) {
            if (triageLevel[order[j]] < triageLevel[order[j + 1]]) {
                temp = order[j];
                order[j] = order[j + 1];
                order[j + 1] = temp;
            }
        }
    }

    printf("\n--- Patients by Priority ---\n");
    for (i = 0; i < patientCount; i++) {
        int p = order[i];
        printf("PAT-%d | %-20s | ", 1000 + p + 1, patientName[p]);
        switch (triageLevel[p]) {
            case 1: printf("Level 1 (Normal)  "); break;
            case 2: printf("Level 2 (Urgent)  "); break;
            case 3: printf("Level 3 (Critical)"); break;
        }
        printf(" | %-20s | LKR %.2f\n", specialtyName[specialtyIdx[p]], finalAmount[p]);
    }
}

void generationReports() {
    int i, w, b;
    int normalCount = 0, urgentCount = 0, criticalCount = 0;
    float totalRevenue = 0.0, totalDiscounts = 0.0;
    float highestBill = -1.0;
    int highestIdx = -1;

    if (patientCount == 0) {
        printf("\nNo patients registered yet.\n");
        return;

    }

    for (i = 0; i < patientCount; i++) {
        switch (triageLevel[i]) {
            case 1: normalCount++; break;
            case 2: urgentCount++; break;
            case 3: criticalCount++; break;
        }
        totalRevenue += finalAmount[i];
        totalDiscounts += discount[i];

        if (finalAmount[i] > highestBill) {
            highestBill = finalAmount[i];
            highestIdx = i;
        }
    } 

    printf("\n==== SUMMARY REPORT ====\n");
    printf("Total Patients Registered: %d\n", patientCount);
    printf(" Level 1 (Normal)   : %d\n", normalCount);
    printf(" Level 2 (Urgent)   : %d\n", urgentCount);
    printf(" Level 3 (Critical) : %d\n", criticalCount);

    printf("\n Total Revenue Earned    : LKR %.2f\n", totalRevenue);
    printf("Total Discounts Granted   : LKR %.2f\n", totalDiscounts);

    printf("\nBed Occupancy Per Ward;\n");
    for (w = 0; w < 4; w++) {
        int occupied = 0;
        for (b = 0; b < bedCapacity[w]; b++) {
            if (bedOccupancy[w][b] == 1) {
                occupied++;
            }
        }
        printf(" %-20s : %.1f%% (%d/%d beds)\n", wardName[w], (occupied * 100.0) / bedCapacity[w], occupied, bedCapacity[w]);

    }

    printf("\nHighest Paying Patient   : %s (LKR %.2f)\n", patientName[highestIdx], highestBill);
}


void saveBedsToFile() {
    FILE *fp = fopen("beds_status.txt", "w");
    int w, b;

    if (fp == NULL) {
        printf("Error: could not save bed status.\n");
        return;
    }

    for (w = 0; w < 4; w++) {
        for (b = 0; b < 20; b++) {
            fprintf(fp, "%d ", bedOccupancy[w][b]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}

void loadBedsFromFile() {
    FILE *fp = fopen("beds_status.txt", "r");
    int w, b;

    if (fp == NULL) {
        initBeds();
        return;
    }

    for (w = 0; w < 4; w++) {
        for (b = 0; b < 20; b++) {
            fscanf(fp, "%d", &bedOccupancy[w][b]);
        }
    }

    fclose(fp);
}