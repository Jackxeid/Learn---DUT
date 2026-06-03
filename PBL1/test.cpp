#include <iostream>
#include <fstream> // Cần thiết để làm việc với file

using namespace std;

int main() {
    // 1. Khởi tạo đối tượng ghi file
    ofstream fileOut("output.txt");

    // 2. Kiểm tra xem file có mở thành công không
    if (fileOut.is_open()) {
        // 3. Ghi dữ liệu vào file txt
        fileOut << "Xin chào! Đây là dòng đầu tiên." << endl;
        fileOut << "Kết quả tính toán: " << 5 + 10 << endl;

        // 4. Đóng file sau khi hoàn tất
        fileOut.close();
        cout << "Xuất file thành công!" << endl;
    } else {
        cout << "Không thể mở file để ghi!" << endl;
    }

    return 0;
}
