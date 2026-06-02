#ifndef PBL_H
#define PBL_H

#include <string>
#include <vector>
using namespace std;

// ================================ CẤU HÌNH HẰNG SỐ HỆ THỐNG ================================
const int ROWS = 5; // Số hàng của sơ đồ nhà hàng
const int COLS = 5; // Số cột của sơ đồ nhà hàng

// Cấu trúc thời gian hỗ trợ tính năng đặt lịch nâng cao
struct DateTime {
    int hour = 0;
    int minute = 0;
    int day = 1;
    int month = 1;
    int year = 2026;
};

// ================================ LỚP CƠ SỞ PERSON ================================
class Person {
protected:
    string name;
    string phone;
public:
    Person(string n = "", string p = "");
    virtual ~Person();
    string getName() const;
    string getPhone() const;
};

// ================================ LỚP CON CUSTOMER ================================
class Customer : public Person {
private:
    int customerID;
    static int totalCustomers; // Biến tĩnh đếm tổng số khách hàng
public:
    Customer(string n = "", string p = "");
    virtual ~Customer();
    void displayInfo() const;
    int getID() const;
    void updateInfo(string n, string p);
    static int getTotalCustomers();
};

// ================================ LỚP CON EMPLOYEE ================================
class Employee : public Person {
private:
    string empID;
    string role; // "Manager" hoặc "Staff"
    double baseSalary;
    int shifts;
public:
    Employee(string id = "", string n = "", string p = "", string r = "Staff", double salary = 0, int s = 0);
    virtual ~Employee();

    string getID() const;
    string getRole() const;
    double getBaseSalary() const;
    int getShifts() const;

    void addShift();
    void updateSalary(double newSalary);
    double calculatePay() const;
    void displayEmployee() const;
    string toFileString() const;
};

// ================================ LỚP HRMANAGER ================================
class HRManager {
private:
    vector<Employee*> staffList; // Mảng động đa hình quản lý danh sách nhân sự
    const string EMP_FILE = "employees.txt";
    const string PAYROLL_FILE = "payroll_report.txt";
    string trim(const string& str); // Hàm chuẩn hóa chuỗi
public:
    HRManager();
    ~HRManager();
    void loadEmployees();
    void saveEmployees();
    void exportPayroll();
    void displayAllStaff();
    string authenticate(string empID);
    
    // Các tính năng mở rộng phân quyền Admin
    void addEmployee();
    void removeEmployee();
    void manageSalaryAndShifts();
    void viewFeedbacks();
};

// ================================ LỚP ĐỐI TƯỢNG TABLE ================================
class Table {
private:
    int tableID;
    int capacity;
    bool isBooked;
    Customer* bookedBy; // Con trỏ liên kết đến đối tượng khách đặt
    DateTime bookTime;
public:
    Table(int id = 0, int cap = 0);
    ~Table();
    int getTableID() const;
    int getCapacity() const;
    bool getStatus() const;
    Customer* getCustomer() const;
    bool bookTable(Customer* c);
    bool bookTable(Customer* c, DateTime dt); // Nạp chồng hàm đặt bàn kèm thời gian
    void freeTable();
    void displayTable() const;
};

// ================================ LỚP ĐIỀU PHỐI RESTAURANT MANAGER (SINGLETON) ================================
class RestaurantManager {
private:
    int totalTables;
    Table* tables;                  // Mảng động chứa danh sách các bàn ăn
    int floorPlan[ROWS][COLS];      // Ma trận sơ đồ vị trí nhà hàng
    static RestaurantManager* instance; // Thực thể tĩnh duy nhất của Singleton

    // Đóng kín Constructor để ngăn chặn việc tạo đối tượng tự do bên ngoài
    RestaurantManager(int numTables);
public:
    ~RestaurantManager();
    // Phương thức tĩnh duy nhất để lấy con trỏ truy cập thực thể bộ quản lý
    static RestaurantManager* getInstance(int numTables = 13);
    
    void displayFloorPlan();
    void displayAllTables();
    void addReservation();
    void deleteReservation();
    void customerDeleteReservation(); // Tính năng bảo mật hủy bàn của khách
    void editReservation();
    void saveToFile(const string& filename);
};

// ================================ CÁC NGUYÊN MẪU HÀM GIAO DIỆN (INTERFACES) ================================
void sendFeedback();
void managerInterface(RestaurantManager* res, HRManager& hr);
void employeeInterface(RestaurantManager* res);
void customerInterface(RestaurantManager* res);

#endif // PBL_H