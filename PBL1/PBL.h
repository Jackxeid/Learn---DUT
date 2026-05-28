#ifndef PBL_H
#define PBL_H

#include <string>

// ==========================================
// 1. STRUCT ĐỊNH NGHĨA THỜI GIAN
// ==========================================
struct DateTime {
    int day, month, year;
    int hour, minute;
};

// ==========================================
// 2. LỚP CƠ SỞ TRỪU TƯỢNG (ABSTRACTION)
// ==========================================
class Person {
protected:
    std::string name;
    std::string phone;
public:
    Person(std::string n = "", std::string p = "");
    virtual ~Person(); // Hàm hủy ảo chống leak bộ nhớ
    
    virtual void displayInfo() const = 0; // Hàm ảo thuần túy (Đa hình)
    
    std::string getName() const;
    std::string getPhone() const;
};

// ==========================================
// 3. LỚP KHÁCH HÀNG ĐA HÌNH VÀ KẾ THỪA
// ==========================================
class Customer : public Person {
private:
    int customerID;
    static int totalCustomers; // Thành viên tĩnh
public:
    Customer(std::string n = "", std::string p = "");
    ~Customer();

    void displayInfo() const override; // Ghi đè phương thức ảo
    int getID() const;
    void updateInfo(std::string n, std::string p);
    
    static int getTotalCustomers(); // Hàm tĩnh
};

// ==========================================
// 4. LỚP ĐỐI TƯỢNG BÀN ĂN (ENCAPSULATION)
// ==========================================
class Table {
private:
    int tableID;
    int capacity;
    bool isBooked;
    Customer* bookedBy; // Con trỏ quản lý đối tượng động
    DateTime bookTime;

public:
    Table(int id = 0, int cap = 4);
    ~Table();

    int getTableID() const;
    int getCapacity() const;
    bool getStatus() const;
    Customer* getCustomer() const;

    // Nạp chồng hàm (Function Overloading)
    bool bookTable(Customer* c);
    bool bookTable(Customer* c, DateTime dt);
    
    void freeTable();
    void displayTable() const;
};

// ==========================================
// 5. LỚP QUẢN LÝ TRUNG TÂM (CORE MANAGER)
// ==========================================
class RestaurantManager {
private:
    Table* tables; // Con trỏ quản lý mảng động 1 chiều
    int totalTables;
    
    static const int ROWS = 5;
    static const int COLS = 5;
    int floorPlan[ROWS][COLS]; // Mảng 2 chiều cố định sơ đồ

public:
    RestaurantManager(int numTables);
    ~RestaurantManager();

    void displayFloorPlan();
    void addReservation();    // CHỨC NĂNG: THÊM
    void deleteReservation(); // CHỨC NNG: XÓA
    void editReservation();   // CHỨC NĂNG: SỬA
    void displayAllTables();
    void saveToFile(const std::string& filename); // FILE I/O
};

#endif // RESTAURANT_H