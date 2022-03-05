#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <cstringt.h>
#include <WinSock2.h>
#include <mysql.h>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <ctime>



#pragma comment(lib, "libmySQL.lib")
 
using namespace std;
#define MAX_LIST_SIZE 100
#define RNAME_LEN 20

MYSQL mysql;
MYSQL_RES* query_result;
MYSQL_ROW query_result_row;


class Customer {
    protected:
        string name;
        string mobile;
        bool name_error;
        bool mobile_error;
        bool already_exist_customer;
        int customer_no;
    public:
        Customer() {};
        Customer(string name, string mobile);
        void check_name();
        void check_mobile();
        void check_duplication();
        void validation();
        string signup_customer();
        string get_name();
};
Customer::Customer(string name, string mobile)
{
    this->name = name;
    this->mobile = mobile;
}
void Customer :: validation()
{
    check_name();
    check_mobile();
    check_duplication();
}
void Customer::check_name()
{
    int length = name.length();
    this->name_error = (length < 1 || length > 50);
}
void Customer::check_mobile()
{
    int length = mobile.length();
    this->mobile_error = (length < 1 || length > 20);
}
void Customer::check_duplication()
{
    char* query = (char*)malloc(sizeof(char) * 100);
    string tmp_query = "SELECT COUNT(1), no FROM customer WHERE name = '" + name + "' AND mobile = '" + mobile +"'";
    strcpy_s(query, 100, tmp_query.c_str());

    int state = mysql_query(&mysql, query);
    delete query;
    if (state != 0)
    {
        cout << "Mysql query error1 : " << mysql_error(&mysql) << endl;
        return;
    }

    query_result = mysql_store_result(&mysql);
    query_result_row = mysql_fetch_row(query_result);

    int result = stoi(query_result_row[0]);

    already_exist_customer = result > 0 ? true : false;
    if (already_exist_customer) customer_no = stoi(query_result_row[1]);

    mysql_free_result(query_result);
}
string Customer::signup_customer()
{
    char* query = (char*)malloc(sizeof(char) * 100);
    string tmp_query = "INSERT INTO customer(name, mobile) VALUE('" + name + "', '" + mobile + "')";
    strcpy_s(query, 100, tmp_query.c_str());
    //cout << query << endl;
    int state = mysql_query(&mysql, query);
    if (state != 0)
    {
        cout << "Mysql query error2 : " << mysql_error(&mysql) << endl;
        return "error";
    }

    strcpy_s(query, 50, "SELECT LAST_INSERT_ID()");
    state = mysql_query(&mysql, query);
    delete query;
    if (state != 0)
    {
        cout << "Mysql query error3 : " << mysql_error(&mysql) << endl;
        return "error";
    }
    query_result = mysql_store_result(&mysql);
    query_result_row = mysql_fetch_row(query_result);

    mysql_free_result(query_result);

    
    return query_result_row[0];
}

string Customer::get_name()
{
    return name;
}

class Account{
    protected:
        int id;
        int balance;
        bool id_error;
        int account_no;
    public:
        
        Account() {};
        Account(int id) {
            this->id = id;
        }
        Account(int id, int balance) {
            this->id = id;
            this->balance;
        }
        Account(int id, char* name, int balance) {
            this->id = id;
            this->balance = balance;
        }
        ~Account() {
        }
        
        void check_id() {
            id_error = id > 0 ? false : true;

            if (!id_error)
            {
                char* query = (char*)malloc(sizeof(char)*100);
                strcpy_s(query, 100, "SELECT COUNT(1) FROM account WHERE id = ");
                
                strcat_s(query, 100, to_string(id).c_str());
                
                int state = mysql_query(&mysql, query);
                delete query;
                if (state != 0)
                {
                    cout << "Mysql query error4 : " << mysql_error(&mysql) << endl;
                    return;
                }

                query_result = mysql_store_result(&mysql);
                query_result_row = mysql_fetch_row(query_result);

                int result = stoi(query_result_row[0]);
                
                id_error = result > 0 ? true : false;

                
                mysql_free_result(query_result);
            }
            
        }

        bool make_account(string customer_no)
        {
            //계좌 등록
            char* query = (char*)malloc(sizeof(char) * 200);
            string tmp_query = "INSERT INTO account(id, balance, customer_no) VALUE(" + to_string(id) + ", " + to_string(balance) + ", " + customer_no + ")";
            strcpy_s(query, 200, tmp_query.c_str());
            
            int state = mysql_query(&mysql, query);

            

            if (state != 0)
            {
                cout << "Mysql query error5 : " << mysql_error(&mysql) << endl;
                return false;
            }


            //이체 기록 테이블 생성
            tmp_query = "CREATE TABLE " + customer_no + "_" + to_string(id) + "(type ENUM('D','W','T'), remitter INT, receiver INT, money INT NOT NULL DEFAULT 0, result INT NOT NULL DEFAULT 0, date VARCHAR(20) NOT NULL, cdate INT NOT NULL)";
            
            strcpy_s(query, 200, tmp_query.c_str());

            state = mysql_query(&mysql, query);

            delete query;

            if (state != 0)
            {
                cout << "Mysql query error6 : " << mysql_error(&mysql) << endl;
                return false;
            }


            
            return true;
        }

        int get_balance() {
            char* query = (char*)malloc(sizeof(char) * 100);
            strcpy_s(query, 100, "SELECT balance FROM account WHERE id = ");

            strcat_s(query, 100, to_string(id).c_str());

            int state = mysql_query(&mysql, query);
            delete query;
            if (state != 0)
            {
                cout << "Mysql query error7 : " << mysql_error(&mysql) << endl;
                return 0;
            }

            query_result = mysql_store_result(&mysql);
            query_result_row = mysql_fetch_row(query_result);

            int result = stoi(query_result_row[0]);


            mysql_free_result(query_result);

            return result;
        }

        bool get_id_error()
        {
            return id_error;
        }

        int take_customer_no()
        {
            char* query = (char*)malloc(sizeof(char) * 100);
            strcpy_s(query, 100, "SELECT customer_no FROM account WHERE id = ");

            strcat_s(query, 100, to_string(id).c_str());

            int state = mysql_query(&mysql, query);
            delete query;
            if (state != 0)
            {
                cout << "Mysql query error20 : " << mysql_error(&mysql) << endl;
                return -1;
            }

            query_result = mysql_store_result(&mysql);
            query_result_row = mysql_fetch_row(query_result);

            int result = stoi(query_result_row[0]);


            mysql_free_result(query_result);

            return result;
        }

        bool check_exist_id()
        {
            char* query = (char*)malloc(sizeof(char) * 100);
            strcpy_s(query, 100, "SELECT COUNT(1) FROM account WHERE id = ");

            strcat_s(query, 100, to_string(id).c_str());

            int state = mysql_query(&mysql, query);
            delete query;
            if (state != 0)
            {
                cout << "Mysql query error4 : " << mysql_error(&mysql) << endl;
                return false;
            }

            query_result = mysql_store_result(&mysql);
            query_result_row = mysql_fetch_row(query_result);

            int result = stoi(query_result_row[0]);

            


            mysql_free_result(query_result);

            return result > 0 ? true : false;
        }

};

class Bank :public Account, public Customer {
private:
    Customer *c;
    Account *a;
public:
    Bank() {};
    Bank(int id);
    Bank(Customer *c, Account *a);
    Bank(string name, string mobile, int id, int balance);//계좌 설계를 위한 생성자
    void set(string name, string mobile, int id, int balance);
    string check_error();
    bool make();
    void take();
    void destroy();
    void deposit(int money);
    void withdraw(int money);
    void transfer(int money, int receiver, Account*& a);
    string get_time();
};

Bank::Bank(int id)
{
    this->id = id;
}

Bank::Bank(Customer *c, Account *a)
{
    this->c = c;
    this->a = a;
    this->c->validation();
    this->a->check_id();
}

Bank::Bank(string name, string mobile, int id, int balance)
{
    /*Customer c(name, mobile);
    c.validation();

    Account a(id, balance);
    a.check_id();*/
    this->name = name;
    this->mobile = mobile;
    this->id = id;
    this->balance = balance;

    validation();
    check_id();

}

void Bank::set(string name, string mobile, int id, int balance)
{
    Customer c(name, mobile);
    c.validation();

    Account a(id, balance);
    a.check_id();
}

string Bank::check_error()
{
    if (name_error) return "이름이 입력되지 않았거나 50자가 넘었습니다.";
    if (mobile_error) return "전화번호가 입력되지 않았거나 20자가 넘었습니다.";
    //if (already_exist_customer) return "1";
    if (id_error) return "이미 존재하는 계좌번호입니다.";
    return "";
}

bool Bank::make()
{
    int choice = 0;
    string customer_no = "error";
    bool result = false;

    if (already_exist_customer) {
        cout << "이미 존재하는 고객입니다. 확인 부탁드립니다." << endl;
        cout << "등록한 적이 있는 고객이 맞습니까?(예-1, 아니요-2)";
        cin >> choice;
        if (choice == 2) {
            cout << "이미 존재하는 고객과 정보가 동일합니다. 고객의 이름 뒤에 번호 뒷자리를 붙여 다시 등록해주세요." << endl;
            return false;
        }

        customer_no = this->customer_no;
    }

    

    if (choice == 0) {
        customer_no = signup_customer();

        if (customer_no == "error") return false;
    }

    result = make_account(customer_no);

    if (result == false) {
        return false;
    }

    return true;
}

void Bank::take()
{
    if (id < 1) {
        cout << "존재하지 않는 계좌번호입니다." << endl;
        return;
    }


    char* query = (char*)malloc(sizeof(char) * 100);
    strcpy_s(query, 100, "SELECT * FROM account WHERE id = ");

    strcat_s(query, 100, to_string(id).c_str());

    int state = mysql_query(&mysql, query);
    delete query;
    if (state != 0)
    {
        cout << "Mysql query error8 : " << mysql_error(&mysql) << endl;
        return;
    }

    query_result = mysql_store_result(&mysql);
    query_result_row = mysql_fetch_row(query_result);

    if (query_result_row != NULL)
    {
        account_no = stoi(query_result_row[0]);
        balance = stoi(query_result_row[2]);
        customer_no = stoi(query_result_row[3]);
    }
    
    if (customer_no == NULL || account_no == NULL) {
        cout << "존재하지 않는 계좌번호입니다." << endl;
        mysql_free_result(query_result);
        return;
    }


    query = (char*)malloc(sizeof(char) * 100);
    strcpy_s(query, 100, "SELECT * FROM customer WHERE no = ");

    strcat_s(query, 100, to_string(customer_no).c_str());

    state = mysql_query(&mysql, query);
    delete query;
    if (state != 0)
    {
        cout << "Mysql query error9 : " << mysql_error(&mysql) << endl;
        mysql_free_result(query_result);
        return;
    }

    query_result = mysql_store_result(&mysql);
    query_result_row = mysql_fetch_row(query_result);

    if (query_result_row != NULL)
    {
        name = query_result_row[1];
        mobile = query_result_row[2];
    }


    mysql_free_result(query_result);
}

void Bank::destroy()
{
    //계좌 파기
    char* query = (char*)malloc(sizeof(char) * 100);
    string tmp_query = "DELETE FROM account WHERE id = ";
    strcpy_s(query, 100, tmp_query.c_str());
    strcat_s(query, 100, to_string(id).c_str());
    
    int state = mysql_query(&mysql, query);

    if (state != 0)
    {
        cout << "Mysql query error10 : " << mysql_error(&mysql) << endl;
        return;
    }

    //이체 기록 테이블 삭제
    tmp_query = "DROP TABLE "+to_string(customer_no) + "_" + to_string(id);
    strcpy_s(query, 100, tmp_query.c_str());

    state = mysql_query(&mysql, query);

    delete query;

    if (state != 0)
    {
        cout << "Mysql query error11 : " << mysql_error(&mysql) << endl;
        return;
    }


    cout << id << " 계좌가 파기되었습니다." << endl;

}

string Bank::get_time()
{
    // Get the current time 
    time_t curTime = time(NULL);

    // Convert the current time 
    struct tm* pLocal = NULL;

    //localtime_s(pLocal , &curTime);
    pLocal = localtime(&curTime);

    /*if (pLocal == NULL)
    {
        // Failed to convert the current time 
        return "";
    }*/

    // Print the current time 
    /*printf("%04d-%02d-%02dT%02d:%02d:%02d",
        pLocal->tm_year + 1900, pLocal->tm_mon + 1, pLocal->tm_mday,
        pLocal->tm_hour, pLocal->tm_min, pLocal->tm_sec);*/
    string time = to_string(pLocal->tm_year + 1900)+"-"+ to_string(pLocal->tm_mon + 1)+"-"+to_string(pLocal->tm_mday)+" "+to_string(pLocal->tm_hour)+":"+to_string(pLocal->tm_min)+":"+to_string(pLocal->tm_mday);
     

    
    return time;
}
void Bank::deposit(int money)
{
    //입금
    char* query = (char*)malloc(sizeof(char) * 200);
    string tmp_query = "UPDATE account SET balance = balance + ";
    strcpy_s(query, 200, tmp_query.c_str());
    strcat_s(query, 200, to_string(money).c_str());

    tmp_query = " WHERE id = ";
    strcat_s(query, 200, tmp_query.c_str());
    strcat_s(query, 200, to_string(id).c_str());

    int state = mysql_query(&mysql, query);

    

    if (state != 0)
    {
        cout << "Mysql query error12 : " << mysql_error(&mysql) << endl;
        return;
    }


    //계좌 금액을 가져옵니다.
    int after_balance = get_balance();

    //입금을 기록합니다.
    tmp_query = "INSERT INTO "+to_string(customer_no) + "_"+to_string(id) + "(type, money, result, date, cdate) VALUE('D', "+to_string(money) + ", "+ to_string(after_balance) +", '"+get_time() + "', UNIX_TIMESTAMP())";
    strcpy_s(query, 200, tmp_query.c_str());

    state = mysql_query(&mysql, query);

    delete query;

    if (state != 0)
    {
        cout << "Mysql query error13 : " << mysql_error(&mysql) << endl;
        return;
    }

    cout << "입금되었습니다." << endl;
}
void Bank::withdraw(int money)
{
    char* query = (char*)malloc(sizeof(char) * 200);
    string tmp_query = "UPDATE account SET balance = balance - ";
    strcpy_s(query, 200, tmp_query.c_str());
    strcat_s(query, 200, to_string(money).c_str());

    tmp_query = " WHERE id = ";
    strcat_s(query, 200, tmp_query.c_str());
    strcat_s(query, 200, to_string(id).c_str());

    int state = mysql_query(&mysql, query);

    //delete query;

    if (state != 0)
    {
        cout << "Mysql query error14 : " << mysql_error(&mysql) << endl;
        return;
    }


    //계좌 금액을 가져옵니다.
    int after_balance = get_balance();

    //출금을 기록합니다.
    tmp_query = "INSERT INTO " + to_string(customer_no) + "_" + to_string(id) + "(type, money, result, date, cdate) VALUE('W', " + to_string(money) + ", " + to_string(after_balance) + ", '" + get_time() + "', UNIX_TIMESTAMP())";
    strcpy_s(query, 200, tmp_query.c_str());

    state = mysql_query(&mysql, query);

    delete query;

    if (state != 0)
    {
        cout << "Mysql query error15 : " << mysql_error(&mysql) << endl;
        return;
    }


    cout << "출금되었습니다." << endl;
}

void Bank::transfer(int money, int receiver, Account*& a)
{
    //계좌에서 이체할 금액을 감소합니다.
    char* query = (char*)malloc(sizeof(char) * 200);
    string tmp_query = "UPDATE account SET balance = balance - ";
    strcpy_s(query, 200, tmp_query.c_str());
    strcat_s(query, 200, to_string(money).c_str());

    tmp_query = " WHERE id = ";
    strcat_s(query, 200, tmp_query.c_str());
    strcat_s(query, 200, to_string(id).c_str());

    int state = mysql_query(&mysql, query);



    if (state != 0)
    {
        cout << "Mysql query error16 : " << mysql_error(&mysql) << endl;
        return;
    }

    //이체를 받을 계좌에 금액을 증가합니다.
    tmp_query = "UPDATE account SET balance = balance + ";
    strcpy_s(query, 200, tmp_query.c_str());
    strcat_s(query, 200, to_string(money).c_str());

    tmp_query = " WHERE id = ";
    strcat_s(query, 200, tmp_query.c_str());
    strcat_s(query, 200, to_string(receiver).c_str());

    state = mysql_query(&mysql, query);



    if (state != 0)
    {
        cout << "Mysql query error17 : " << mysql_error(&mysql) << endl;
        return;
    }


    //계좌 금액을 가져옵니다.
    int after_balance_of_id = get_balance();
    int after_balance_of_receiver = a->get_balance();

    //이체하는 계좌에 기록합니다.
    tmp_query = "INSERT INTO " + to_string(customer_no) + "_" + to_string(id) + "(type, remitter, receiver, money, result, date, cdate) VALUE('T', "+ to_string(id) + ", "+ to_string(receiver) + ", " + to_string(money) + ", " + to_string(after_balance_of_id) + ", '" + get_time() + "', UNIX_TIMESTAMP())";
    strcpy_s(query, 200, tmp_query.c_str());

    state = mysql_query(&mysql, query);

    //delete query;

    if (state != 0)
    {
        cout << "Mysql query error18 : " << mysql_error(&mysql) << endl;
        return;
    }

    //이체 받는 계좌에 기록합니다.
    tmp_query = "INSERT INTO " + to_string(a->take_customer_no()) + "_" + to_string(receiver) + "(type, remitter, receiver, money, result, date, cdate) VALUE('T', " + to_string(id) + ", " + to_string(receiver) + ", " + to_string(money) + ", " + to_string(after_balance_of_receiver) + ", '" + get_time() + "', UNIX_TIMESTAMP())";
    strcpy_s(query, 200, tmp_query.c_str());

    state = mysql_query(&mysql, query);

    delete query;

    if (state != 0)
    {
        cout << "Mysql query error19 : " << mysql_error(&mysql) << endl;
        return;
    }

    cout << "이체되었습니다." << endl;
}


void sql_init() {
mysql_init(&mysql);

    if (!mysql_real_connect(&mysql, "localhost", "root", "root", "bank", 3306, NULL, 0))
    {
        cout << "error" << endl;
        cout << mysql_error(&mysql);
    }
}

void PrintMenu();
void PrintMain();
void account_selected();
void MakeAccout();
bool DestroyAccount(Bank *b);
void Deposit(Bank*& b);
void Withdraw(Bank*& b);
void Inquire();
void MakeAccount();
void Transfer(Bank*& b);
enum { DESTROY = 1, DEPOSIT, WITHDRAW, INQUIRE, TRANSFER, BACK };
enum {MAKE = 1, SELECT, EXIT};


int main()
{

    sql_init();
    
    int choice;

    while (1)
    {
        PrintMain();
        cout << "선택: ";
        cin >> choice;

        switch (choice)
        {
        case MAKE:
            MakeAccount();
            break;
        case SELECT:
            account_selected();
            break;
        case EXIT:
            mysql_close(&mysql);
            return 0;
        default:
            cout << "올바른 메뉴를 입력하시오 " << endl;
            break;
        }
    }
    mysql_close(&mysql);
    return 0;
}

void account_selected()
{
    int id;
    cout << "==계좌 선택==" << endl;
    cout << "계좌 번호 : ";
    cin >> id;

    Bank  *b = new Bank(id);
    b->take();

    int choice;

    while (1)
    {
        PrintMenu();
        cout << "선택: ";
        cin >> choice;

        switch (choice)
        {
        case DESTROY:
            if (DestroyAccount(b)) {
                delete b;
                return;
            }
            break;
        case DEPOSIT:
            Deposit(b);
            break;
        case WITHDRAW:
            Withdraw(b);
            break;
        case INQUIRE:
            //Inquire(b);
            break;
        case TRANSFER:
            Transfer(b);
            break;
        case BACK:
            delete b;
            return;
        default:
            cout << "올바른 메뉴를 입력하시오 " << endl;
            break;
        }
    }
}

void PrintMenu()
{
    cout << "---MENU---" << endl;
    cout << "1. 계좌 파기 " << endl;
    cout << "2. 입금 " << endl;
    cout << "3. 출금 " << endl;
    cout << "4. 잔액 조회 " << endl;
    cout << "5. 이체 " << endl;
    cout << "6. 홈으로 가기 " << endl;
}

void PrintMain()
{
    cout << "---MAIN---" << endl;
    cout << "1. 계좌 개설 " << endl;
    cout << "2. 계좌 선택 " << endl;
    cout << "3. 프로그램 종료 " << endl;
}

void MakeAccount()
{
    int id;
    int balance;
    string name;
    string mobile;

    Bank* bank;

    while (1) {
        cout << "==계좌 개설==" << endl;
        cout << "이름 : ";
        cin >> name;
        cout << "전화번호 : ";
        cin >> mobile;
        cout << "계좌 번호 : ";
        cin >> id;
        cout << "입금액 : ";
        cin >> balance;

        bank = new Bank(name, mobile, id, balance);
        
        //Bank b(new Customer(name, mobile), new Account(id, balance));
        string error = bank->check_error();

        if (error == "") {
            break;
        }
        cout << error << endl;
        delete bank;
    }
   
    if (!bank->make())
    {
        cout << "등록 중에 오류가 발생했습니다." << endl;
        return;
    }
    
    cout << "등록되었습니다." << endl;
    
}

bool DestroyAccount(Bank *b){
    int choice;

    cout << "==계좌 파기==" << endl;
    cout << "계좌를 파기하시겠습니까?(예-1, 아니요-2)";
    cin >> choice;

    if (choice == 1) {
        b->destroy();
        return true;
    }

    return false;

}

void Deposit(Bank*& b)
{
    int money;
   
    cout << "==계좌 입금==" << endl;
    cout << "입금액 : ";
    cin >> money;

    if (money < 0)
    {
        cout << "입금액은 0보다 작을 수 없습니다. 다시 시도해주세요." << endl;
        return;
    }

    b->deposit(money);
   
}

void Withdraw(Bank*& b)
{
    int money;


    cout << "==계좌 출금==" << endl;
    cout << "출금액 : ";
    cin >> money;

    if (money < 0)
    {
        cout << "출금액은 0보다 작을 수 없습니다. 다시 시도해주세요." << endl;
        return;
    }

    b->withdraw(money);
    
}

void Inquire()
{
    /*for (int i = 0; i < list.size; i++)    {
        list.pArray[i]->Showalldata();
    }*/

}

void Transfer(Bank*& b)
{
    int money;
    int receiver;

    cout << "==계좌 이체==" << endl;
    cout << "이체 금액 : ";
    cin >> money;
    cout << "이체 받을 계좌번호 : ";
    cin >> receiver;

    Account *a = new Account(receiver);
    a->check_id();
    if (!a->check_exist_id())
    {
        cout << "존재하지 않는 계좌입니다. 다시 시도해주세요." << endl;
        return;
    }

    b->transfer(money, receiver, a);
}