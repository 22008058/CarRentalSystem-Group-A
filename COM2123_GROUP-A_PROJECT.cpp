
#include <iostream>
#include <fstream>
#include <algorithm> 
#include <cctype>
#include <string>
using namespace std;

//GROUP A PROJECT
/* MEMBERS
24071968 MATHUNJWA BF
22004719 RATSHILUMELA H
21014205 MASWIME R
22008058 MBATHA P
21012868 NOVELA HL
24007589 RAMBUDA M
20025383 NEVONDWE RJ
23014390 Mathalise M
*/

enum FuelType { PETROL, DIESEL, ELECTRIC };
enum Currency { ZAR, USD };

Currency selectedCurrency = ZAR;

string getFuelTypeName(FuelType fuelType) {
    switch(fuelType) {
        case PETROL: return "Petrol";
        case DIESEL: return "Diesel";
        case ELECTRIC: return "Electric";
        default: return "Unknown";
}
}


float convertCurrency(float amount) {
    const float rate = 0.055f; // Example: 1 ZAR = 0.055 USD
    return (selectedCurrency == USD) ? amount * rate : amount;
}

string getCurrencySymbol() {
    return (selectedCurrency == USD) ? "$" : "R";
}

union Fuel {
    FuelType fuelType;
};

struct Car {
    int ID;
    string model;
    float dailyRate;
    bool isAvailable;
    Fuel fuel;
};

struct Customer {
    string name;
    string licenseNumber;
    string contactNumber;
    int rentedCarID;
};

const int MAX_CARS = 10;
const int MAX_CUSTOMERS = 10;
Car cars[MAX_CARS];
Customer customers[MAX_CUSTOMERS];
int customerCount = 0;

void initializeCars() {
    cars[0] = {1, "Toyota_Corolla", 300.0f, true, {PETROL}};
    cars[1] = {2, "Volkswagen_Golf", 350.0f, true, {DIESEL}};
    cars[2] = {3, "Audi_E-Tron GT", 600.0f, true, {ELECTRIC}};
    cars[3] = {4, "Ford_Fiesta", 250.0f, true, {PETROL}};
    cars[4] = {5, "BMW_X5", 500.0f, true, {DIESEL}};
}

void addCustomer() {
    if (customerCount >= MAX_CUSTOMERS) return;
    cout << "Enter customer name: ";
    cin >> customers[customerCount].name;
    cout << "Enter license number: ";
    cin >> customers[customerCount].licenseNumber;
    customers[customerCount].rentedCarID = -1;
    customerCount++;
    string contact;
bool valid = false;
do {
    cout << "Enter contact number (max 10 digits, numbers only): ";
    cin >> contact;
    valid = (contact.length() <= 10) && all_of(contact.begin(), contact.end(), ::isdigit);
    if (!valid) {
        cout << "Invalid input. Please enter only digits and up to 10 characters.\n";
    }
} while (!valid);

customers[customerCount].contactNumber = contact;
}

void rentCar() {
    string license;
    int carID;
    cout << "Enter license number: ";
    cin >> license;
    cout << "Enter car ID to rent: ";
    cin >> carID;

    for (int i = 0; i < MAX_CARS; i++) {
        if (cars[i].ID == carID && cars[i].isAvailable) {
            for (int j = 0; j < customerCount; j++) {
                if (customers[j].licenseNumber == license) {
                    customers[j].rentedCarID = carID;
                    cars[i].isAvailable = false;
                    cout << "Car rented successfully.\n";
                    return;
                }
            }
        }
    }
    cout << "Car not available or customer not found.\n";
}

float standardPrice(float rate, int days) {
    return rate * days;
}

float discountedPrice(float rate, int days) {
    return rate * days * 0.9;
}

void returnCar(float (*priceCalc)(float, int)) {
    string license;
    int days;
    cout << "Enter license number: ";
    cin >> license;
    cout << "Enter number of rental days: ";
    cin >> days;

    for (int i = 0; i < customerCount; i++) {
        if (customers[i].licenseNumber == license && customers[i].rentedCarID != -1) {
            int carID = customers[i].rentedCarID;
            for (int j = 0; j < MAX_CARS; j++) {
                if (cars[j].ID == carID) {
                    float totalCost = priceCalc(cars[j].dailyRate, days);
                    float converted = convertCurrency(totalCost);
                    cars[j].isAvailable = true;
                    customers[i].rentedCarID = -1;
                    cout << "Return successful. Total cost: " << getCurrencySymbol() << converted << endl;
                    return;
                }
            }
        }
    }
    cout << "Customer not found or no car rented.\n";
}

void displayAvailableCars() {
    for (int i = 0; i < MAX_CARS; i++) {
        if (cars[i].isAvailable) {
            cout << "ID: " << cars[i].ID << ", Model: " << cars[i].model 
                 << ", Rate: " << getCurrencySymbol() << convertCurrency(cars[i].dailyRate) << ", Fuel: " << getFuelTypeName(cars[i].fuel.fuelType) << endl;
        }
    }
}

void displayCustomers() {
    for (int i = 0; i < customerCount; i++) {
        cout << "Name: " << customers[i].name << ", License: " << customers[i].licenseNumber 
             << ", Car ID: " << customers[i].rentedCarID << endl;
    }
}

void saveCarsToFile() {
    ofstream file("cars.txt");
    for (int i = 0; i < MAX_CARS; i++) {
        file << cars[i].ID << " " << cars[i].model << " " << cars[i].dailyRate << " "
             << cars[i].isAvailable << " " << cars[i].fuel.fuelType << endl;
    }
    file.close();
}

void loadCarsFromFile() {
    ifstream file("cars.txt");
    int id, fuel;
    string model;
    float rate;
    bool available;
    int index = 0;

    while (file >> id >> model >> rate >> available >> fuel) {
        cars[index++] = {id, model, rate, available, {(FuelType)fuel}};
    }
    file.close();
}

void menu() {
    int choice;
    do {
        cout << "\nCar Rental System\n";
        cout << "1. Add Customer\n2. Rent Car\n3. Return Car\n4. View Available Cars\n5. View Customers\n6. Exit\n";
        cout << "Choose option: ";
        cin >> choice;
        switch (choice) {
            case 1: addCustomer(); break;
            case 2: rentCar(); break;
            case 3:
                int option;
                cout << "1. Standard Price\n2. Discounted Price\n";
                cin >> option;
                if (option == 1)
                    returnCar(standardPrice);
                else
                    returnCar(discountedPrice);
                break;
            case 4: displayAvailableCars(); break;
            case 5: displayCustomers(); break;
            case 6: saveCarsToFile(); break;
            default: cout << "Invalid choice\n";
        }
    } while (choice != 6);
}

int main() {
    int currencyChoice;
    cout << "Choose your currency:\n1. Rands (ZAR)\n2. Dollars (USD)\nChoice: ";
    cin >> currencyChoice;
    selectedCurrency = (currencyChoice == 2) ? USD : ZAR;

    initializeCars();
    loadCarsFromFile();
    menu();
    return 0;
}
