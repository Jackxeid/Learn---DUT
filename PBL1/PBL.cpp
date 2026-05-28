#include "PBL.h"
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

// ================================ KHỞI TẠO THÀNH VIÊN TĨNH ================================
int Customer::totalCustomers = 0;



// ================================ ĐỊNH NGHĨA LỚP PERSON ================================
Person::Person(string n, string p) : name(n), phone(p) {}
// -----------------------------------------------------------------
Person::~Person() {}
// -----------------------------------------------------------------
string Person::getName() const { return name; }
// -----------------------------------------------------------------
string Person::getPhone() const { return phone; }
// -----------------------------------------------------------------



// ================================ ĐỊNH NGHĨA LỚP CUSTOMER ================================
Customer::Customer(string n, string p) : Person(n, p) {
    totalCustomers++;
    customerID = totalCustomers;
}
// -----------------------------------------------------------------
Customer::~Customer() {}
// -----------------------------------------------------------------
void Customer::displayInfo() const {
    cout << left << setw(10) << customerID << setw(20) << name << setw(15) << phone;
}
int Customer::getID() const { return customerID; }
// -----------------------------------------------------------------
void Customer::updateInfo(string n, string p) {
    name = n;
    phone = p;
}
// -----------------------------------------------------------------
int Customer::getTotalCustomers() { return totalCustomers; }



// ================================ ĐỊNH NGHĨA LỚP TABLE ================================
Table::Table(int id, int cap) : tableID(id), capacity(cap), isBooked(false), bookedBy(nullptr) {}
// -----------------------------------------------------------------
Table::~Table() {
    if (bookedBy != nullptr) {
        delete bookedBy;
    }
}
// -----------------------------------------------------------------
int Table::getTableID() const { return tableID; }
// -----------------------------------------------------------------
int Table::getCapacity() const { return capacity; }
// -----------------------------------------------------------------
bool Table::getStatus() const { return isBooked; }
// -----------------------------------------------------------------
Customer* Table::getCustomer() const { return bookedBy; }
// -----------------------------------------------------------------
bool Table::bookTable(Customer* c) {
    if (isBooked){
			return false;
		}

    bookedBy = new Customer(*c);
    isBooked = true;
    return true;
}
// -----------------------------------------------------------------
bool Table::bookTable(Customer* c, DateTime dt) {
    if (isBooked) {
        return false;
    }
    bookedBy = new Customer(*c);
    isBooked = true;
    bookTime = dt;
    return true;
}
// -----------------------------------------------------------------
void Table::freeTable() {
    if (isBooked) {
        delete bookedBy;
        bookedBy = nullptr;
        isBooked = false;
		}
}
// -----------------------------------------------------------------
void Table::displayTable() const {
    cout << "Bàn số: " << setw(5) << tableID << " | Chỗ ngồi: " << setw(5) << capacity 
				<< " | Trạng thái: " << (isBooked ? "Đã Đặt" : "Trống") << endl;
    if (isBooked && bookedBy != nullptr) {
        cout << "  -> Khách hàng: ";
        bookedBy->displayInfo();
        cout << endl;
    }
}



// ================================ ĐỊNH NGHĨA LỚP RESTAURANTMANAGER ================================
RestaurantManager::RestaurantManager(int numTables) {
    totalTables = numTables;
    tables = new Table[totalTables];
    for (int i = 0; i < totalTables; i++) {
        tables[i] = Table(i + 1, (i % 2 == 0) ? 4 : 6);
    }
    int currentTable = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (currentTable < totalTables && (i + j) % 2 == 0) {
                floorPlan[i][j] = tables[currentTable].getTableID();
                currentTable++;
            } else {
                floorPlan[i][j] = 0;
            }
        }
    }
}
// -----------------------------------------------------------------
RestaurantManager::~RestaurantManager() {
    delete[] tables;
}
// -----------------------------------------------------------------
void RestaurantManager::displayFloorPlan() {
    cout << "\n=== SƠ ĐỒ NHÀ HÀNG ===\n";
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
						int id = floorPlan[i][j] - 1;
						
						if (tables[id].getStatus()) {
							cout << "[ X ] ";
						} else {
							cout << "[ " << floorPlan[i][j] << " ] ";
						}
        }
        cout << endl;
    }
}
// -----------------------------------------------------------------
void RestaurantManager::addReservation() {
    int tableID;
    string name, phone;
    cout << "\n--- THEM DAT BAN ---" << endl;
    displayFloorPlan();
		cout << "Chú thích:\n"
		<< "- [ id ] = Bàn trống\n"
		<< "- [ X ] = Bàn đã đặt\n"
		<< "- [   ] = Lối đi\n";

    cout << "Nhap ID ban: "; cin >> tableID;
    if (tableID < 1 || tableID > totalTables || tables[tableID - 1].getStatus()) {
        cout << "Khong hop le hoac ban da bi dat!\n"; return;
    }
    cin.ignore();
    cout << "Ten khach: "; getline(cin, name);
    cout << "SDT: "; getline(cin, phone);
    Customer newCust(name, phone);
    tables[tableID - 1].bookTable(&newCust);
    cout << "=> DAT BAN THANH CONG!\n";
}
// -----------------------------------------------------------------
void RestaurantManager::deleteReservation() {
    int tableID;
    cout << "\n--- HUY DAT BAN ---" << endl;
    cout << "Nhap ID ban can huy: "; cin >> tableID;
    if (tableID < 1 || tableID > totalTables || !tables[tableID - 1].getStatus()) {
        cout << "ID khong hop le hoac ban dang trong!\n"; return;
    }
    tables[tableID - 1].freeTable();
    cout << "=> HUY DAT BAN THANH CONG!\n";
}
// -----------------------------------------------------------------
void RestaurantManager::editReservation() {
    int tableID;
    cout << "\n--- SUA THONG TIN DAT BAN ---" << endl;
    cout << "Nhap ID ban: "; cin >> tableID;
    if (tableID < 1 || tableID > totalTables || !tables[tableID - 1].getStatus()) {
        cout << "Khong hop le!\n"; return;
    }
    string newName, newPhone;
    cin.ignore();
    cout << "Ten moi: "; getline(cin, newName);
    cout << "SDT moi: "; getline(cin, newPhone);
    tables[tableID - 1].getCustomer()->updateInfo(newName, newPhone);
    cout << "=> CAP NHAT THANH CONG!\n";
}
// -----------------------------------------------------------------
void RestaurantManager::displayAllTables() {
    cout << "\n=== DANH SACH TRANG THAI BAN ===\n";
    for (int i = 0; i < totalTables; i++) tables[i].displayTable();
}
// -----------------------------------------------------------------
void RestaurantManager::saveToFile(const string& filename) {
    ofstream outFile(filename);
    if (!outFile) return;
    for (int i = 0; i < totalTables; i++) {
        if (tables[i].getStatus()) {
            outFile << tables[i].getTableID() << ","
                    << tables[i].getCustomer()->getName() << ","
                    << tables[i].getCustomer()->getPhone() << endl;
        }
    }
    outFile.close();
    cout << "=> DA LUU FILE!\n";
}



// ================================ SHOW MENU ================================
void showMenu() {
    cout << "\n=========================================\n";
    cout << "     HỆ THỐNG QUẢN LÝ ĐẶT BÀN NHÀ HÀNG   \n";
    cout << "=========================================\n";
    cout << "1. Xem sơ đồ nhà hàng\n";
    cout << "2. Xem danh sách tất cả các bàn\n";
    cout << "3. Đặt bàn mới\n";
    cout << "4. Hủy đặt bàn\n";
    cout << "5. Sửa thông tin khách hàng\n";
    cout << "6. Lưu dữ liệu (.txt)\n";
    cout << "0. Thoát chương trình\n";
    cout << "=========================================\n";
    cout << "Nhập lựa chọn của bạn: ";
}



// ================================ HÀM ĐIỀU HƯỚNG MAIN ================================
int main() {
    RestaurantManager bkRestaurant(12);
    int choice;
    do {
        showMenu();
        cin >> choice;
        if (choice == 1) bkRestaurant.displayFloorPlan();
        else if (choice == 2) bkRestaurant.displayAllTables();
        else if (choice == 3) bkRestaurant.addReservation();
        else if (choice == 4) bkRestaurant.deleteReservation();
        else if (choice == 5) bkRestaurant.editReservation();
        else if (choice == 6) bkRestaurant.saveToFile("Data.txt");
    } while (choice != 0);
    return 0;
}