#include <stdio.h>

//Functions
void displaySpecialties();
void displayWards();
void initBeds();
int findFreeBed(int ward);
void displayBedStatus(int ward);
int getIntInRange(int min, int max);
void registerPatient();

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

int patientCount = 0;
int queueCount[4] = {0, 0, 0, 0};


int main() {
    int choice;

    initBeds();

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
                printf("Coming soon: Generate Reports\n");
                break;
            case 5:
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


    patientCount++;
    queueCount[specialtyIdx[i]]++;

    printf("\nPatient registered successfully. Total patients: %d\n", patientCount);
}