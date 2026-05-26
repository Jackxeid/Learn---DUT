#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

// ==========================================
// 1. STRUCT & ENUM
// ==========================================
struct DateTime {
    int day, month, year;
    int hour, minute;
};

// ==========================================
// 2. TÍNH TRỪU TƯỢNG & KẾ THỪA
// ==========================================
// Lớp cơ sở trừu tượng Person
class Person {
protected:
    string name;
    string phone;
public:
    Person(string n = "", string p = "") : name(n), phone(p) {}
    virtual ~Person() {} // Hàm hủy ảo để tránh memory leak khi delete con trỏ lớp dẫn xuất
    
    // Hàm ảo thuần túy (Pure virtual function) -> Đa hình
    virtual void displayInfo() const = 0; 
    
    // Getters
    string getName() const { return name; }
    string getPhone() const { return phone; }
};

// Lớp Khách hàng kế thừa từ Person
class Customer : public Person {
private:
    int customerID;
    static int totalCustomers; // Biến tĩnh (Static variable)
public:
    Customer(string n = "", string p = "") : Person(n, p) {
        totalCustomers++;
        customerID = totalCustomers;
    }
    ~Customer() {}

    // Ghi đè hàm ảo (Override)
    void displayInfo() const override {
        cout << left << setw(10) << customerID 
						<< setw(20) << name 
						<< setw(15) << phone;
    }
    
    int getID() const { return customerID; }
    
    // Hàm tĩnh (Static function)
    static int getTotalCustomers() { return totalCustomers; }
    
    // Setter để chức năng SỬA sử dụng
    void updateInfo(string n, string p) {
        name = n;
        phone = p;
    }
};

int Customer::totalCustomers = 0; // Khởi tạo biến tĩnh

// ==========================================
// 3. LỚP ĐỐI TƯỢNG CƠ BẢN (ENCAPSULATION)
// ==========================================
class Table {
private:
    int tableID;
    int capacity;
    bool isBooked;
    Customer* bookedBy; // Con trỏ trỏ đến khách hàng đặt bàn
    DateTime bookTime;

public:
    Table(int id = 0, int cap = 4) : tableID(id), capacity(cap), isBooked(false), bookedBy(nullptr) {}
    ~Table() {
        if (bookedBy != nullptr) {
            delete bookedBy; // Giải phóng bộ nhớ con trỏ khách hàng
            bookedBy = nullptr;
        }
    }

    // Các hàm Getters / Setters (Đóng gói)
    int getTableID() const { return tableID; }
    int getCapacity() const { return capacity; }
    bool getStatus() const { return isBooked; }
    Customer* getCustomer() const { return bookedBy; }

    // HÀM QUÁ TẢI (Function Overloading) - Chức năng đặt bàn
    // Cách 1: Chỉ đặt bàn, không có thời gian cụ thể
    bool bookTable(Customer* c) {
        if (isBooked) return false;
        bookedBy = new Customer(*c); // Cấp phát động
        isBooked = true;
        return true;
    }
    
    // Cách 2: Đặt bàn có thời gian
    bool bookTable(Customer* c, DateTime dt) {
        if (isBooked) return false;
        bookedBy = new Customer(*c);
        isBooked = true;
        bookTime = dt;
        return true;
    }

    void freeTable() {
        if (isBooked) {
            delete bookedBy;
            bookedBy = nullptr;
            isBooked = false;
        }
    }

    void displayTable() const {
        cout << "Ban so: " << setw(5) << tableID << " | Cho ngoi: " << setw(5) << capacity 
						<< " | Trang thai: " << (isBooked ? "Da Dat" : "Trong") << endl;
        if (isBooked && bookedBy != nullptr) {
            cout << "  -> Khach hang: ";
            bookedBy->displayInfo();
            cout << endl;
        }
    }
};

// ==========================================
// 4. LỚP QUẢN LÝ TRUNG TÂM (CORE MANAGER)
// ==========================================
class RestaurantManager {
private:
    // Mảng 1 chiều động chứa các bàn (Sử dụng con trỏ)
    Table* tables;
    int totalTables;
    
    // Mảng 2 chiều biểu diễn sơ đồ nhà hàng (Ví dụ: 5x5)
    static const int ROWS = 5;
    static const int COLS = 5;
    int floorPlan[ROWS][COLS]; 

public:
    RestaurantManager(int numTables) {
        totalTables = numTables;
        tables = new Table[totalTables]; // Cấp phát động mảng 1 chiều
        
        // Khởi tạo bàn với ID từ 1 -> totalTables
        for (int i = 0; i < totalTables; i++) {
            tables[i] = Table(i + 1, (i % 2 == 0) ? 4 : 6); // Bàn chẵn 4 chỗ, lẻ 6 chỗ
        }
        
        // Khởi tạo mảng 2 chiều sơ đồ (0 = không có bàn, >0 = ID bàn)
        int currentTable = 0;
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (currentTable < totalTables && (i + j) % 2 == 0) { // Sắp xếp bàn xen kẽ
                    floorPlan[i][j] = tables[currentTable].getTableID();
                    currentTable++;
                } else {
                    floorPlan[i][j] = 0;
                }
            }
        }
    }

    ~RestaurantManager() {
        delete[] tables; // Tránh memory leak
    }

    // Hiển thị mảng 2 chiều
    void displayFloorPlan() {
        cout << "\n=== SO DO NHA HANG (MANG 2 CHIEU) ===\n";
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (floorPlan[i][j] == 0) {
                    cout << "[   ] ";
                } else {
                    // Kiểm tra trạng thái bàn thông qua ID
                    int id = floorPlan[i][j] - 1;
                    if (tables[id].getStatus()) {
                        cout << "[ X ] "; // X là đã đặt
                    } else {
                        cout << "[" << setw(3) << floorPlan[i][j] << "] ";
                    }
                }
            }
            cout << endl;
        }
        cout << "Chu thich:\n";
				cout << "- [ id ] = Ban trong\n";
				cout << "- [ X ] = Ban da dat\n";
				cout << "- [   ] = Loi di\n";
    }

    // CHỨC NĂNG 1: THÊM (ĐẶT BÀN MỚI)
    void addReservation() {
        int tableID;
        string name, phone;
        
        cout << "\n--- THEM DAT BAN ---" << endl;
        displayFloorPlan();
        cout << "Nhap ID ban muon dat: "; cin >> tableID;
        
        if (tableID < 1 || tableID > totalTables) {
            cout << "ID ban khong hop le!\n"; return;
        }
        
        if (tables[tableID - 1].getStatus()) {
            cout << "Ban nay da duoc dat. Vui long chon ban khac!\n"; return;
        }

        cin.ignore();
        cout << "Nhap ten khach hang: "; getline(cin, name);
        cout << "Nhap so dien thoai: "; getline(cin, phone);
        
        Customer newCust(name, phone);
        tables[tableID - 1].bookTable(&newCust);
        cout << "=> DAT BAN THANH CONG!\n";
    }

    // CHỨC NĂNG 2: XÓA (HỦY ĐẶT BÀN)
    void deleteReservation() {
        int tableID;
        cout << "\n--- HUY DAT BAN ---" << endl;
        cout << "Nhap ID ban muon huy: "; cin >> tableID;
        
        if (tableID < 1 || tableID > totalTables) {
            cout << "ID ban khong hop le!\n"; return;
        }
        
        if (!tables[tableID - 1].getStatus()) {
            cout << "Ban nay hien dang trong, khong the huy!\n"; return;
        }
        
        tables[tableID - 1].freeTable();
        cout << "=> HUY DAT BAN THANH CONG!\n";
    }

    // CHỨC NĂNG 3: SỬA (CẬP NHẬT THÔNG TIN KHÁCH ĐẶT BÀN)
    void editReservation() {
        int tableID;
        cout << "\n--- SUA THONG TIN DAT BAN ---" << endl;
        cout << "Nhap ID ban can sua thong tin: "; cin >> tableID;
        
        if (tableID < 1 || tableID > totalTables) {
            cout << "ID ban khong hop le!\n"; return;
        }
        
        if (!tables[tableID - 1].getStatus()) {
            cout << "Ban nay chua duoc dat, khong co thong tin de sua!\n"; return;
        }
        
        string newName, newPhone;
        cin.ignore();
        cout << "Nhap ten khach hang moi: "; getline(cin, newName);
        cout << "Nhap so dien thoai moi: "; getline(cin, newPhone);
        
        Customer* currentCust = tables[tableID - 1].getCustomer();
        if (currentCust != nullptr) {
            currentCust->updateInfo(newName, newPhone);
            cout << "=> CAP NHAT THONG TIN THANH CONG!\n";
        }
    }

    // HIỂN THỊ DANH SÁCH BÀN
    void displayAllTables() {
        cout << "\n=== DANH SACH TRANG THAI BAN ===\n";
        for (int i = 0; i < totalTables; i++) {
            tables[i].displayTable();
        }
        cout << "Tong so khach hang da ghi nhan he thong: " << Customer::getTotalCustomers() << endl;
    }

    // ĐỌC/GHI FILE (FILE I/O)
    void saveToFile(const string& filename) {
        ofstream outFile(filename);
        if (!outFile) {
            cout << "Loi tao file!\n";
            return;
        }
        for (int i = 0; i < totalTables; i++) {
            if (tables[i].getStatus()) {
                outFile << tables[i].getTableID() << ","
                        << tables[i].getCustomer()->getName() << ","
                        << tables[i].getCustomer()->getPhone() << endl;
            }
        }
        outFile.close();
        cout << "=> DA LUU DU LIEU VAO FILE THANH CONG!\n";
    }
};

// ==========================================
// 5. HÀM MAIN & MENU ĐIỀU HƯỚNG
// ==========================================
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

int main() {
    // Khởi tạo nhà hàng với 12 bàn
    RestaurantManager bkRestaurant(12);
    int choice;

    do {
        showMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                bkRestaurant.displayFloorPlan();
                break;
            case 2:
                bkRestaurant.displayAllTables();
                break;
            case 3:
                bkRestaurant.addReservation();
                break;
            case 4:
                bkRestaurant.deleteReservation();
                break;
            case 5:
                bkRestaurant.editReservation();
                break;
            case 6:
                bkRestaurant.saveToFile("Data_DatBan.txt");
                break;
            case 0:
                cout << "Đang thoát hệ thống. Cảm ơn vì đã sử dụng dịch vụ!\n";
                break;
            default:
                cout << "Lựa chọn không hợp lệ. Vui lòng nhập lại!\n";
        }
    } while (choice != 0);

    return 0;
}