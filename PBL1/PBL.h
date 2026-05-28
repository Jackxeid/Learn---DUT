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
    virtual ~Person();
    
    virtual void displayInfo() const = 0;
    
    std::string getName() const;
    std::string getPhone() const;
};

// ==========================================
// 3. LỚP KHÁCH HÀNG ĐA HÌNH VÀ KẾ THỪA
// ==========================================
class Customer : public Person {
private:
    int customerID;
    static int totalCustomers;
public:
    Customer(std::string n = "", std::string p = "");
    ~Customer();

    void displayInfo() const override;
    int getID() const;
    void updateInfo(std::string n, std::string p);
    
    static int getTotalCustomers();
};

// ==========================================
// 4. LỚP ĐỐI TƯỢNG BÀN ĂN (ENCAPSULATION)
// ==========================================
class Table {
private:
    int tableID;
    int capacity;
    bool isBooked;
    Customer* bookedBy;
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
// 5. LỚP QUẢN LÝ NHÀ HÀNG (CORE MANAGER)
// ==========================================
class RestaurantManager {
private:
    Table* tables;
    int totalTables;
    
		// Sơ đồ chỗ ngồi
    static const int ROWS = 5;
    static const int COLS = 5;
    int floorPlan[ROWS][COLS];

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