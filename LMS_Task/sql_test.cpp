#include <iostream>
#include <string>
#include <mysqlx/xdevapi.h>

//#pragma commont (lib, "libmysqlcppconn8.2.8.0.27.dylib")
#pragma commont (lib, "libmysqlcppconn8.dylib")


//using namespace ::mysqlx;
using namespace std;

// Scope controls life-time of objects such as session or schema

void sql_init(){
  Session sess("localhost", 33060, "root", "root");
  Schema db= sess.getSchema("test");
  // or Schema db(sess, "test");

  Collection myColl = db.getCollection("my_collection");
  // or Collection myColl(db, "my_collection");

  DocResult myDocs = myColl.find("name like :param")
                           .limit(1)
                           .bind("param","L%").execute();

  cout << myDocs.fetchOne();
}

int main(){
  sql_init();
  return 0;
}