#include <iostream>
#include <cstring>
using namespace std;

struct info{
    string name;
    string number;
    string address;
};

int main() {
    
    info GI;
    GI.name = "미입력";
    GI.number = "미입력";
    GI.address = "미입력";

    cout << "이름을 입력하시오: ";  
    cin >> GI.name;

    cout << "연락처를 입력하시오: ";  
    cin >> GI.number;

    cout << "주소를 입력하시오: ";
    cin >> GI.address;


    cout << "이름 : " << GI.name << endl;
    cout << "연락처 : " << GI.number << endl;
    cout << "주소 : " << GI.address << endl;
   return 0;
}