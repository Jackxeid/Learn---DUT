#include "PBL.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace std;

// ================================ KHỞI TẠO THÀNH VIÊN TĨNH & SINGLETON ================================
int Customer::totalCustomers = 0;
RestaurantManager* RestaurantManager::instance = nullptr; // Khởi tạo con trỏ cụm Singleton



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
    cout << left << setw(4) << customerID << " | Tên: " << setw(15) << name << " | SĐT: " << setw(15) << phone;
}
int Customer::getID() const { return customerID; }
// -----------------------------------------------------------------
void Customer::updateInfo(string n, string p) {
    name = n;
    phone = p;
}
// -----------------------------------------------------------------
int Customer::getTotalCustomers() { return totalCustomers; }



// ================================ ĐỊNH NGHĨA LỚP EMPLOYEE ================================
Employee::Employee(string id, string n, string p, string r, double salary, int s) 
    : Person(n, p), empID(id), role(r), baseSalary(salary), shifts(s) {}
// -----------------------------------------------------------------
Employee::~Employee() {}
// -----------------------------------------------------------------
string Employee::getID() const { return empID; }
string Employee::getRole() const { return role; }
double Employee::getBaseSalary() const { return baseSalary; }
int Employee::getShifts() const { return shifts; }
// -----------------------------------------------------------------
void Employee::addShift() { shifts++; }
void Employee::updateSalary(double newSalary) { baseSalary = newSalary; }
// -----------------------------------------------------------------
double Employee::calculatePay() const {
    double total = baseSalary * shifts;
    if (role == "Manager") total *= 1.5; // Phụ cấp trách nhiệm của Quản lý
    return total;
}
// -----------------------------------------------------------------
void Employee::displayEmployee() const {
    cout << left << setw(10) << empID << setw(20) << name 
        << setw(15) << role << setw(15) << fixed << setprecision(0) << baseSalary 
        << setw(10) << shifts << endl;
}
// -----------------------------------------------------------------
string Employee::toFileString() const {
    return empID + "|" + name + "|" + phone + "|" + role + "|" + to_string(baseSalary) + "|" + to_string(shifts);
}



// ================================ ĐỊNH NGHĨA LỚP HRMANAGER ================================
HRManager::HRManager() {
    loadEmployees(); // Tự động nạp dữ liệu ngay khi đối tượng quản lý được tạo
}
// -----------------------------------------------------------------
HRManager::~HRManager() {
    // Giải phóng bộ nhớ mảng con trỏ động
    for (auto emp : staffList) {
        delete emp;
    }
    staffList.clear();
}
// -----------------------------------------------------------------
string HRManager::trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first) return str;
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}
// -----------------------------------------------------------------
void HRManager::noEmployee() {
    // Nap mảng mac dinh theo yeu cau (Ma NV, Ten, SDT, Vai tro, Luong/ca, So ca mac dinh)
    staffList.push_back(new Employee("NV01", "Ngo Nguyen Khang", "123456789", "Manager", 500000, 10));
    staffList.push_back(new Employee("NV02", "Huynh Van Dat", "123456789", "Staff", 200000, 10));
    staffList.push_back(new Employee("NV03", "Le Nguyen Quoc Huy", "123456789", "Staff", 200000, 10));
    staffList.push_back(new Employee("NV04", "Le Anh Khoa", "123456789", "Staff", 200000, 10));
    staffList.push_back(new Employee("NV05", "Ha Huy An", "123456789", "Staff", 200000, 10));
}
// -----------------------------------------------------------------
void HRManager::loadEmployees() {
    ifstream inFile(EMP_FILE);
    
    // TRƯỜNG HỢP 1: File chưa từng tồn tại (Lần đầu chạy ứng dụng)
    if (!inFile) {
        cout << "[He thong] Khoi tao co so du lieu nhan su mac dinh ban dau...\n";
        // Tạo dữ liệu mẫu mặc định
        noEmployee();
        
        // Tu dong tao file va luu lai ngay lap tuc
        saveEmployees();
        return;
    }

    string line;
    while (getline(inFile, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string id, name, phone, role, salaryStr, shiftStr;

        // Tách chuỗi theo ký tự phân tách '|'
        getline(ss, id, '|');
        getline(ss, name, '|');
        getline(ss, phone, '|');
        getline(ss, role, '|');
        getline(ss, salaryStr, '|');
        getline(ss, shiftStr, '|');

        double salary = stod(trim(salaryStr));
        int shifts = stoi(trim(shiftStr));

        staffList.push_back(new Employee(trim(id), trim(name), trim(phone), trim(role), salary, shifts));
    }
    inFile.close();

    // TRƯỜNG HỢP 2: File ton tai nhung ai do da xoa het chuoi (File trong rong)
    if (staffList.empty()) {
        cout << "[He thong] File du lieu trong. Tu dong nap lai danh sach goc...\n";
        noEmployee();
        saveEmployees();
    }
}
// -----------------------------------------------------------------
void HRManager::saveEmployees() {
    ofstream outFile(EMP_FILE);
    
    // Xem file có thực sự mở được không
    if (!outFile) {
        cout << "[ERROR] Khong the mo file " << EMP_FILE << " de ghi dữ liệu! Kiếm tra lại quyền truy cập!\n";
        return;
    }

    // // Kiểm tra path file output
    // char absPath[4096];
    // if (_fullpath(absPath, EMP_FILE.c_str(), 4096) != NULL) {
    //     cout << "[CHECK] File cua ban thuc te dang nam tai: " << absPath << "\n";
    // }
    for (auto emp : staffList) {
        outFile << emp->toFileString() << endl;
    }

    outFile.close();
}
// -----------------------------------------------------------------
void HRManager::exportPayroll() {
    ofstream outFile(PAYROLL_FILE);
    if (!outFile) {
        cout << "[Loi] Khong the xuat file bao cao luong!\n";
        return;
    }
    outFile << "========================================================\n";
    outFile << "                 BANG LUONG THANH TOAN                  \n";
    outFile << "========================================================\n";
    outFile << left << setw(10) << "Ma NV" << setw(20) << "Ten Nhan Vien" 
            << setw(10) << "So ca" << setw(15) << "TONG LUONG" << "\n";
    outFile << "--------------------------------------------------------\n";
    
    for (auto emp : staffList) {
        outFile << left << setw(10) << emp->getID() 
                << setw(20) << emp->getName() 
                << setw(10) << emp->getShifts() 
                << fixed << setprecision(0) << emp->calculatePay() << "\n";
    }
    outFile << "========================================================\n";
    outFile.close();
    cout << "=> Da xuat file " << PAYROLL_FILE << " thanh cong!\n";
}
// -----------------------------------------------------------------
void HRManager::displayAllStaff() {
    cout << "\n-------------------------------------------------------------\n";
    cout << left << setw(10) << "Ma NV" << setw(20) << "Ten Nhan Vien" 
        << setw(15) << "Vai Tro" << setw(15) << "Luong/Ca" << setw(10) << "So Ca\n";
    cout << "-------------------------------------------------------------\n";
    for (auto emp : staffList) emp->displayEmployee();
    cout << "-------------------------------------------------------------\n";
}
// -----------------------------------------------------------------
string HRManager::authenticate(string empID) {
    for (auto emp : staffList) {
        if (emp->getID() == empID) {
            return emp->getRole();
        }
    }
    return "None";
}
// -----------------------------------------------------------------
// CHỨC NĂNG PHÁT TRIỂN THÊM CỦA QUẢN LÝ (Ý TƯỞNG THÊM MỚI NHÂN VIÊN)
void HRManager::addEmployee() {
    string id, name, phone, role;
    double salary;
    cout << "\n--- THEM NHAN VIEN MOI ---\n";
    cout << "Nhap Ma NV (VD: NV06): "; cin >> id;
    if (authenticate(id) != "None") {
        cout << "[Error] Ma nhan vien da ton tai!\n"; 
        return;
    }
    cin.ignore();
    cout << "Ho va Ten: "; getline(cin, name);
    cout << "So dien thoai: "; getline(cin, phone);
    cout << "Vai tro (Manager/Staff): "; cin >> role;
    if (role != "Manager" && role != "Staff") {
        cout << "[Error] Vai tro khong hop le! Vui lòng chon 'Manager' hoac 'Staff'.\n"; 
        return;
    }
    cout << "Luong co ban moi ca: "; cin >> salary;

    staffList.push_back(new Employee(id, name, phone, role, salary, 0));
    cout << "=> THEM NHAN VIEN THANH CONG!\n";
}
// -----------------------------------------------------------------
// CHỨC NĂNG SA THẢI NHÂN VIÊN
void HRManager::removeEmployee() {
    string id;
    cout << "\n--- SA THAI NHAN VIEN ---\n";
    cout << "Nhap Ma NV can sa thai: "; cin >> id;
    for (auto it = staffList.begin(); it != staffList.end(); ++it) {
        if ((*it)->getID() == id) {
            delete *it; // Xóa vùng nhớ động trước
            staffList.erase(it); // Xóa khỏi danh sách Vector
            cout << "=> DA XOA NHAN VIEN KHOI HE THONG!\n";
            return;
        }
    }
    cout << "[Error] Khong tim thay Ma nhan vien hop le!\n";
}
// -----------------------------------------------------------------
// CHỨC NĂNG TĂNG LƯƠNG HOẶC CHẤM CÔNG NHÂN VIÊN
void HRManager::manageSalaryAndShifts() {
    string id;
    int opt;
    cout << "\n--- DIU CHINH LUONG & CONG TRONG CA ---\n";
    cout << "Nhap Ma NV: "; cin >> id;
    for (auto emp : staffList) {
        if (emp->getID() == id) {
            cout << "Tên nhân viên: "; cout << emp->getName() << endl;
            cout << "1. Diem danh tang 1 ca lam (Check-in)\n2. Cap nhat muc luong cung\nChon: "; 
            cin >> opt;
            if (opt == 1) {
                emp->addShift();
                cout << "=> Ghi nhan ca lam viec thanh cong!\n";
            } else if (opt == 2) {
                double newSal;
                cout << "Nhap muc luong moi: "; cin >> newSal;
                emp->updateSalary(newSal);
                cout << "=> Cap nhat muc luong moi thanh cong!\n";
            }
            
            exportPayroll();

            return;
        }
    }
    cout << "[Error] Khong tim thay Ma nhan vien!\n";
}
// -----------------------------------------------------------------
// CHỨC NĂNG XEM FEEDBACK CỦA KHÁCH HÀNG TỪ FILE
void HRManager::viewFeedbacks() {
    ifstream inFile(FEEDBACK_FILE);
    cout << "\n=== DANH SACH PHAN HOI TU KHACH HANG ===\n";
    if (!inFile) {
        cout << "(Chua co phan hoi nao trong he thong)\n"; return;
    }
    string line;
    while (getline(inFile, line)) {
        cout << line << endl;
    }
    inFile.close();
}



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
    cout << "Bàn số: " << setw(3) << tableID << " | Chỗ ngồi: " << setw(3) << capacity 
                << " | Trạng thái: " << (isBooked ? "Đã Đặt" : "Trống") << endl;
    if (isBooked && bookedBy != nullptr) {
        cout << "   -> Thông tin: ";
        bookedBy->displayInfo();
        cout << endl;
    }
}



// ================================ ĐỊNH NGHĨA LỚP RESTAURANT MANAGER (SINGLETON PATTERN) ================================
// Thực thi hàm Singleton getInstance()
RestaurantManager* RestaurantManager::getInstance(int numTables) {
    if (instance == nullptr) {
        instance = new RestaurantManager(numTables);
    }
    return instance;
}

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
            if (floorPlan[i][j] == 0) cout << "[   ] ";
            else {
                int id = floorPlan[i][j] - 1;
                if (tables[id].getStatus()) cout << "[ X ] ";
                else cout << "[ " << floorPlan[i][j] << " ] ";
            }
        }
        cout << endl;
    }
}
// -----------------------------------------------------------------
void RestaurantManager::displayAllTables() {
    cout << "\n=== DANH SÁCH TẤT CẢ CÁC BÀN ===\n";
    for (int i = 0; i < totalTables; i++) tables[i].displayTable();
}
// -----------------------------------------------------------------
void RestaurantManager::addReservation() {
    int tableID;
    string name, phone;
    cout << "\n--- ĐẶT BÀN ---" << endl;
    displayFloorPlan();
    cout << "\nChú thích:\n"
        << "- [ id ] = Bàn trống\n"
        << "- [ X  ] = Bàn đã đặt\n"
        << "- [    ] = Lối đi\n";

    cout << "Nhập ID bàn: "; cin >> tableID;
    if (tableID < 1 || tableID > totalTables || tables[tableID - 1].getStatus()) {
        cout << "Không hợp lệ hoặc bàn đã bị đặt!\n"; return;
    }
    cin.ignore();
    cout << "Tên: "; getline(cin, name);
    cout << "SDT: "; getline(cin, phone);
    Customer newCust(name, phone);
    tables[tableID - 1].bookTable(&newCust);
    cout << "=> ĐẶT BÀN THÀNH CÔNG!\n";
}
// -----------------------------------------------------------------
void RestaurantManager::deleteReservation() {
    int tableID;
    cout << "\n--- HỦY ĐẶT BÀN ---" << endl;
    displayFloorPlan();
    cout << "Nhập ID bàn cần hủy: "; cin >> tableID;
    if (tableID < 1 || tableID > totalTables || !tables[tableID - 1].getStatus()) {
        cout << "ID không hợp lệ hoặc bàn đang trống!\n"; return;
    }
    tables[tableID - 1].freeTable();
    cout << "=> HỦY ĐẶT BÀN THÀNH CÔNG!\n";
}

// CHỨC NĂNG PHÁT TRIỂN THÊM: HỦY ĐẶT BÀN BẢO MẬT (DÀNH CHO KHÁCH HÀNG)
void RestaurantManager::customerDeleteReservation() {
    int tableID;
    string inputPhone;
    displayFloorPlan();
    cout << "\n--- KHÁCH HÀNG TỰ HỦY ĐẶT BÀN (YÊU CẦU XÁC THỰC CHÍNH CHỦ) ---\n";
    cout << "Nhập ID bàn muốn hủy: "; cin >> tableID;
    if (tableID < 1 || tableID > totalTables || !tables[tableID - 1].getStatus()) {
        cout << "[Lỗi] Bàn trống hoặc ID không tồn tại!\n"; return;
    }
    cout << "Nhập số điện thoại đã dùng để đặt bàn: "; cin >> inputPhone;
    cin.ignore(); // Clear the input buffer
    // Đối chiếu chuỗi số điện thoại bảo mật
    if (tables[tableID - 1].getCustomer()->getPhone() == inputPhone) {
        tables[tableID - 1].freeTable();
        cout << "=> ĐÃ XÁC THỰC CHÍNH CHỦ! HỦY BÀN THÀNH CÔNG.\n";
    } else {
        cout << "[Từ chối hành động] Số điện thoại không trùng khớp với dữ liệu đặt bàn!\n";
    }
}

// -----------------------------------------------------------------
void RestaurantManager::editReservation() {
    int tableID;
    cout << "\n--- SỬA THÔNG TIN ĐẶT BÀN ---" << endl;
    displayFloorPlan();
    cout << "Nhập ID bàn: "; cin >> tableID;
    if (tableID < 1 || tableID > totalTables || !tables[tableID - 1].getStatus()) {
        cout << "ID không hợp lệ hoặc bàn đang trống!\n"; return;
    }
    string newName, newPhone;
    cin.ignore();
    cout << "Tên:"; getline(cin, newName);
    cout << "SDT: "; getline(cin, newPhone);
    tables[tableID - 1].getCustomer()->updateInfo(newName, newPhone);
    cout << "=> CẬP NHẬT THÔNG TIN THÀNH CÔNG!\n";
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
    cout << "=> ĐÃ LƯU DỮ LIỆU ĐẶT BÀN KHÁCH HÀNG!\n";
}

// CHỨC NĂNG THÊM MỚI: KHÁCH HÀNG GỬI PHẢN HỒI (FEEDBACK)
void sendFeedback() {
    string msg;
    cin.ignore();
    cout << "\nNhập ý kiến đóng góp của bạn về nhà hàng: ";
    getline(cin, msg);
    ofstream outFile(FEEDBACK_FILE, ios::app); // Mở ở chế độ Append ghi nối đuôi
    if (outFile) {
        outFile << "- " << msg << "\n";
        outFile.close();
        cout << "=> GỬI PHAN HOI THANH CONG! CAM ON BAN DA DONG GOP Y KIEN.\n";
    }
}



// ================================ HỆ THỐNG MENU PHÂN QUYỀN (ROUTING GIAO DIỆN) ================================

void managerInterface(RestaurantManager* res, HRManager& hr) {
    int choice;
    do {
        cout << "\n=========================================\n";
        cout << "        MENU ĐIỀU HÀNH - QUẢN LÝ (MANAGER) \n";
        cout << "=========================================\n";
        cout << "1. Xem sơ đồ nhà hàng hiện tại\n";
        cout << "2. Xem danh sách trạng thái đặt bàn\n";
        cout << "3. Đặt bàn trực tiếp hộ khách\n";
        cout << "4. Hủy bàn bất kỳ (Quyền ADMIN)\n";
        cout << "5. Sửa đổi thông tin đặt lịch\n";
        cout << "6. Xem danh sách hồ sơ nhân viên\n";
        cout << "7. Thêm nhân viên mới vào biên chế\n";
        cout << "8. Sa thải / Xóa nhân viên khỏi hệ thống\n";
        cout << "9. Quản lý ca làm (Check-in) / Sửa lương\n";
        cout << "10. Xem hòm thư góp ý (Feedback khách)\n";
        cout << "11. Xuất file báo cáo lương tổng hợp (.txt)\n";
        cout << "12. Sao lưu dữ liệu đặt bàn khách hàng\n";
        cout << "0. Đăng xuất tài khoản Quản lý\n";
        cout << "=========================================\n";
        cout << "Nhập lựa chọn của bạn: "; cin >> choice;

        switch(choice) {
            case 1: res->displayFloorPlan(); break;
            case 2: res->displayAllTables(); break;
            case 3: res->addReservation(); break;
            case 4: res->deleteReservation(); break;
            case 5: res->editReservation(); break;
            case 6: hr.displayAllStaff(); break;
            case 7: hr.addEmployee(); break;
            case 8: hr.removeEmployee(); break;
            case 9: hr.manageSalaryAndShifts(); break;
            case 10: hr.viewFeedbacks(); break;
            case 11: hr.exportPayroll(); break;
            case 12: res->saveToFile(DATA_FILE); break;
            case 0: cout << "=> Đang thoát tài khoản quản lý và đồng bộ file nhân sự...\n"; break;
            default: cout << "Lựa chọn không hợp lệ!\n";
        }
        if(choice != 0) cout << "\n#######################################################\n";
    } while (choice != 0);
}

void employeeInterface(RestaurantManager* res) {
    int choice;
    do {
        cout << "\n=========================================\n";
        cout << "        MENU NGHIỆP VỤ - NHÂN VIÊN (STAFF) \n";
        cout << "=========================================\n";
        cout << "1. Xem sơ đồ nhà hàng\n";
        cout << "2. Xem trạng thái tất cả các bàn\n";
        cout << "3. Hỗ trợ khách đặt bàn mới\n";
        cout << "4. Sửa đổi thông tin khách hàng đặt lịch\n";
        cout << "5. Giải phóng bàn ăn (Khách thanh toán / Hủy)\n";
        cout << "6. Lưu dữ liệu biến động bàn ăn ra file\n";
        cout << "0. Đăng xuất tài khoản nhân viên\n";
        cout << "=========================================\n";
        cout << "Nhập lựa chọn của bạn: "; cin >> choice;

        switch(choice) {
            case 1: res->displayFloorPlan(); break;
            case 2: res->displayAllTables(); break;
            case 3: res->addReservation(); break;
            case 4: res->editReservation(); break;
            case 5: res->deleteReservation(); break;
            case 6: res->saveToFile(DATA_FILE); break;
            case 0: cout << "=> Đang đăng xuất tài khoản nhân viên...\n"; break;
            default: cout << "Lựa chọn không hợp lệ!\n";
        }
        if(choice != 0) cout << "\n#######################################################\n";
    } while (choice != 0);
}

void customerInterface(RestaurantManager* res) {
    int choice;
    do {
        cout << "\n=========================================\n";
        cout << "         CỔNG THÔNG TIN KHÁCH HÀNG       \n";
        cout << "=========================================\n";
        cout << "1. Xem sơ đồ vị trí bàn trống\n";
        cout << "2. Quét danh sách sức chứa các bàn\n";
        cout << "3. Thực hiện Đặt bàn trực tuyến\n";
        cout << "4. Yêu cầu hủy đặt bàn (Bảo mật SĐT)\n";
        cout << "5. Gửi đánh giá dịch vụ & Feedback đóng góp\n";
        cout << "0. Quay lại màn hình chính Gateway\n";
        cout << "=========================================\n";
        cout << "Nhập lựa chọn của bạn: "; cin >> choice;

        switch(choice) {
            case 1: res->displayFloorPlan(); break;
            case 2: res->displayAllTables(); break;
            case 3: res->addReservation(); break;
            case 4: res->customerDeleteReservation(); break;
            case 5: sendFeedback(); break;
            case 0: cout << "=> Đang chuyển hướng về cổng Gateway nhà hàng...\n"; break;
            default: cout << "Lựa chọn không hợp lệ!\n";
        }
        if(choice != 0) cout << "\n#######################################################\n";
    } while (choice != 0);
}



// ================================ HÀM ĐIỀU HƯỚNG MAIN (GATEWAY TRUNG TÂM) ================================
int main() {
    // 1. Áp dụng Singleton Pattern để khởi tạo thực thể bàn ăn duy nhất cho toàn hệ thống
    RestaurantManager* bkRestaurant = RestaurantManager::getInstance(13);
    
    // 2. Khởi tạo đối tượng quản lý tệp nhân sự
    HRManager hrSystem; 
    
    int accessRole;
    do {
        cout << "\n=======================================================\n";
        cout << "   XIN CHÀO! VUI LÒNG CHỌN VAI TRÒ ĐỂ TRUY CẬP HỆ THỐNG\n";
        cout << "=======================================================\n";
        cout << "1. Quản lý (Manager)\n";
        cout << "2. Nhân viên (Employee/Staff)\n";
        cout << "3. Khách hàng (Customer)\n";
        cout << "0. Tắt hệ thống dữ liệu nhà hàng\n";
        cout << "=======================================================\n";
        cout << "Nhập vai trò của bạn: "; cin >> accessRole;

        if (accessRole == 1) {
            string token;
            cout << "Yêu cầu nhập mã tài khoản quản lý (VD: NV01): "; cin >> token;
            string role = hrSystem.authenticate(token);
            if (role == "Manager") {
                cout << "\n[Xác thực thành công] Xin chào quản lý cấp cao!\n";
                managerInterface(bkRestaurant, hrSystem);
            } else {
                cout << "\n[Truy cập bị từ chối] Mã định danh không đúng hoặc bạn không có quyền hạn Quản lý!\n";
            }
        } 
        else if (accessRole == 2) {
            string token;
            cout << "Yêu cầu nhập mã tài khoản nhân viên (VD: NV02): "; cin >> token;
            string role = hrSystem.authenticate(token);
            if (role == "Staff") {
                cout << "\n[Xác thực thành công] Đăng nhập tài khoản Nhân viên làm việc!\n";
                employeeInterface(bkRestaurant);
            } else {
                cout << "\n[Truy cập bị từ chối] Mã nhân viên không tồn tại trên hệ thống dữ liệu ca trực!\n";
            }
        } 
        else if (accessRole == 3) {
            customerInterface(bkRestaurant);
        }
        
        cout << "\n#######################################################\n";
    } while (accessRole != 0);

    // Đồng bộ, lưu trữ toàn bộ tiến trình nhân sự và đóng tiến trình an toàn
    hrSystem.saveEmployees();
    bkRestaurant->saveToFile(DATA_FILE);
    
    // Giải phóng thực thể Singleton trước khi tắt hẳn ứng dụng
    delete bkRestaurant;
    
    cout << "=> HỆ THỐNG ĐÃ ĐỒNG BỘ DỮ LIỆU VÀ ĐÓNG CỬA AN TOÀN!\n";
    return 0;
}