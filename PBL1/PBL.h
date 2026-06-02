#ifndef PBL_H
#define PBL_H


#include <string>
using namespace std;

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
        string name;
        string phone;
    public:
        Person(string n = "", string p = "");
        virtual ~Person();

        virtual void displayInfo() const = 0;

        string getName() const;
        string getPhone() const;
    };

// ==========================================
// 3. LỚP KHÁCH HÀNG ĐA HÌNH VÀ KẾ THỪA
// ==========================================
class Customer : public Person {
    private:
        int customerID;
        static int totalCustomers;
    
    public:
        Customer(string n = "", string p = "");
        ~Customer();

        void displayInfo() const override;
        int getID() const;
        void updateInfo(string n, string p);

        static int getTotalCustomers();
    };


class Employee: public Person {
    private:
        string empID;
        string role; // "Manager" hoặc "Staff"
        double baseSalary;
        int shifts;

    public:
        Employee(string id = "", string n = "", string p = "", string r = "Staff", double salary = 0, int s = 0);
        // Tên, SDT, ID nhân viên, vai trò, lương cơ bản, số ca làm
        ~Employee();

        // Getters
        void displayInfo() const override;

        // Setters cho Quản lý
        void addShift() { shifts++; };
        void updateSalary(double newSalary) { baseSalary = newSalary; };

        // Tính lương cơ bản
        double calculatePay() const {
            double total = baseSalary * shifts;
            if (role == "Manager") total += 1000000; // Phụ cấp quản lý
            return total;
        }

        void displayEmployee() const {
            cout << left << setw(10) << empID << setw(20) << name 
                << setw(15) << role << setw(15) << baseSalary 
                << setw(10) << shifts << endl;
        }

        // Format chuỗi để ghi vào file
        string toFileString() const {
            return empID + "|" + name + "|" + role + "|" + to_string(baseSalary) + "|" + to_string(shifts);
        }
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
        void saveToFile(const string& filename); // FILE I/O
};

#endif // RESTAURANT_H