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
            //?????? ??????
            char* query = (char*)malloc(sizeof(char) * 200);
            string tmp_query = "INSERT INTO account(id, balance, customer_no) VALUE(" + to_string(id) + ", " + to_string(balance) + ", " + customer_no + ")";
            strcpy_s(query, 200, tmp_query.c_str());
            
            int state = mysql_query(&mysql, query);

            

            if (state != 0)
            {
                cout << "Mysql query error5 : " << mysql_error(&mysql) << endl;
                return false;
            }


            //?????? ?????? ????????? ??????
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
    Bank(string name, string mobile, int id, int balance);//?????? ????????? ?????? ?????????
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
    if (name_error) return "????????? ???????????? ???????????? 50?????? ???????????????.";
    if (mobile_error) return "??????????????? ???????????? ???????????? 20?????? ???????????????.";
    //if (already_exist_customer) return "1";
    if (id_error) return "?????? ???????????? ?????????????????????.";
    return "";
}

bool Bank::make()
{
    int choice = 0;
    string customer_no = "error";
    bool result = false;

    if (already_exist_customer) {
        cout << "?????? ???????????? ???????????????. ?????? ??????????????????." << endl;
        cout << "????????? ?????? ?????? ????????? ?????????????(???-1, ?????????-2)";
        cin >> choice;
        if (choice == 2) {
            cout << "?????? ???????????? ????????? ????????? ???????????????. ????????? ?????? ?????? ?????? ???????????? ?????? ?????? ??????????????????." << endl;
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
        cout << "???????????? ?????? ?????????????????????." << endl;
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
        cout << "???????????? ?????? ?????????????????????." << endl;
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
    //?????? ??????
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

    //?????? ?????? ????????? ??????
    tmp_query = "DROP TABLE "+to_string(customer_no) + "_" + to_string(id);
    strcpy_s(query, 100, tmp_query.c_str());

    state = mysql_query(&mysql, query);

    delete query;

    if (state != 0)
    {
        cout << "Mysql query error11 : " << mysql_error(&mysql) << endl;
        return;
    }


    cout << id << " ????????? ?????????????????????." << endl;

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
    //??????
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


    //?????? ????????? ???????????????.
    int after_balance = get_balance();

    //????????? ???????????????.
    tmp_query = "INSERT INTO "+to_string(customer_no) + "_"+to_string(id) + "(type, money, result, date, cdate) VALUE('D', "+to_string(money) + ", "+ to_string(after_balance) +", '"+get_time() + "', UNIX_TIMESTAMP())";
    strcpy_s(query, 200, tmp_query.c_str());

    state = mysql_query(&mysql, query);

    delete query;

    if (state != 0)
    {
        cout << "Mysql query error13 : " << mysql_error(&mysql) << endl;
        return;
    }

    cout << "?????????????????????." << endl;
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


    //?????? ????????? ???????????????.
    int after_balance = get_balance();

    //????????? ???????????????.
    tmp_query = "INSERT INTO " + to_string(customer_no) + "_" + to_string(id) + "(type, money, result, date, cdate) VALUE('W', " + to_string(money) + ", " + to_string(after_balance) + ", '" + get_time() + "', UNIX_TIMESTAMP())";
    strcpy_s(query, 200, tmp_query.c_str());

    state = mysql_query(&mysql, query);

    delete query;

    if (state != 0)
    {
        cout << "Mysql query error15 : " << mysql_error(&mysql) << endl;
        return;
    }


    cout << "?????????????????????." << endl;
}

void Bank::transfer(int money, int receiver, Account*& a)
{
    //???????????? ????????? ????????? ???????????????.
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

    //????????? ?????? ????????? ????????? ???????????????.
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


    //?????? ????????? ???????????????.
    int after_balance_of_id = get_balance();
    int after_balance_of_receiver = a->get_balance();

    //???????????? ????????? ???????????????.
    tmp_query = "INSERT INTO " + to_string(customer_no) + "_" + to_string(id) + "(type, remitter, receiver, money, result, date, cdate) VALUE('T', "+ to_string(id) + ", "+ to_string(receiver) + ", " + to_string(money) + ", " + to_string(after_balance_of_id) + ", '" + get_time() + "', UNIX_TIMESTAMP())";
    strcpy_s(query, 200, tmp_query.c_str());

    state = mysql_query(&mysql, query);

    //delete query;

    if (state != 0)
    {
        cout << "Mysql query error18 : " << mysql_error(&mysql) << endl;
        return;
    }

    //?????? ?????? ????????? ???????????????.
    tmp_query = "INSERT INTO " + to_string(a->take_customer_no()) + "_" + to_string(receiver) + "(type, remitter, receiver, money, result, date, cdate) VALUE('T', " + to_string(id) + ", " + to_string(receiver) + ", " + to_string(money) + ", " + to_string(after_balance_of_receiver) + ", '" + get_time() + "', UNIX_TIMESTAMP())";
    strcpy_s(query, 200, tmp_query.c_str());

    state = mysql_query(&mysql, query);

    delete query;

    if (state != 0)
    {
        cout << "Mysql query error19 : " << mysql_error(&mysql) << endl;
        return;
    }

    cout << "?????????????????????." << endl;
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
        cout << "??????: ";
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
            cout << "????????? ????????? ??????????????? " << endl;
            break;
        }
    }
    mysql_close(&mysql);
    return 0;
}

void account_selected()
{
    int id;
    cout << "==?????? ??????==" << endl;
    cout << "?????? ?????? : ";
    cin >> id;

    Bank  *b = new Bank(id);
    b->take();

    int choice;

    while (1)
    {
        PrintMenu();
        cout << "??????: ";
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
            cout << "????????? ????????? ??????????????? " << endl;
            break;
        }
    }
}

void PrintMenu()
{
    cout << "---MENU---" << endl;
    cout << "1. ?????? ?????? " << endl;
    cout << "2. ?????? " << endl;
    cout << "3. ?????? " << endl;
    cout << "4. ?????? ?????? " << endl;
    cout << "5. ?????? " << endl;
    cout << "6. ????????? ?????? " << endl;
}

void PrintMain()
{
    cout << "---MAIN---" << endl;
    cout << "1. ?????? ?????? " << endl;
    cout << "2. ?????? ?????? " << endl;
    cout << "3. ???????????? ?????? " << endl;
}

void MakeAccount()
{
    int id;
    int balance;
    string name;
    string mobile;

    Bank* bank;

    while (1) {
        cout << "==?????? ??????==" << endl;
        cout << "?????? : ";
        cin >> name;
        cout << "???????????? : ";
        cin >> mobile;
        cout << "?????? ?????? : ";
        cin >> id;
        cout << "????????? : ";
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
        cout << "?????? ?????? ????????? ??????????????????." << endl;
        return;
    }
    
    cout << "?????????????????????." << endl;
    
}

bool DestroyAccount(Bank *b){
    int choice;

    cout << "==?????? ??????==" << endl;
    cout << "????????? ?????????????????????????(???-1, ?????????-2)";
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
   
    cout << "==?????? ??????==" << endl;
    cout << "????????? : ";
    cin >> money;

    if (money < 0)
    {
        cout << "???????????? 0?????? ?????? ??? ????????????. ?????? ??????????????????." << endl;
        return;
    }

    b->deposit(money);
   
}

void Withdraw(Bank*& b)
{
    int money;


    cout << "==?????? ??????==" << endl;
    cout << "????????? : ";
    cin >> money;

    if (money < 0)
    {
        cout << "???????????? 0?????? ?????? ??? ????????????. ?????? ??????????????????." << endl;
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

    cout << "==?????? ??????==" << endl;
    cout << "?????? ?????? : ";
    cin >> money;
    cout << "?????? ?????? ???????????? : ";
    cin >> receiver;

    Account *a = new Account(receiver);
    a->check_id();
    if (!a->check_exist_id())
    {
        cout << "???????????? ?????? ???????????????. ?????? ??????????????????." << endl;
        return;
    }

    b->transfer(money, receiver, a);
}