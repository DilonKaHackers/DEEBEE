#include "theDB.hpp"

using namespace std;
int main() {

    DataHolder<string, string> JOE("Joe");
    DataHolder<string, string> JOHN("John");
    DataHolder<string, string> JOHN2("John");

    Collections<string, string> CLERKS("Clerks");
    Collections<string, string> MANGERS("managers");

    vector<DataHolder<string, string>*> addPeopleVector = { &JOHN, &JOE };

    JOE.add_KeyValue_pair("name", "Joe");
    JOE.add_KeyValue_pair("age", "28");
    JOE.add_KeyValue_pair("salary", "20K");

    JOHN.add_KeyValue_pair("name", "John");
    JOHN.add_KeyValue_pair("age", "35");
    JOHN.add_KeyValue_pair("salary", "90K");

    JOHN2.add_KeyValue_pair("addr", "potheri");
    JOHN2.add_KeyValue_pair("phn num", "246583215513");
    JOHN2.add_KeyValue_pair("salary", "105k");

    CLERKS.add_multiple_dataHolders(addPeopleVector);
    CLERKS.printCollection();

    CLERKS.remove_from_collection("John");
    CLERKS.printCollection();

    return 0;
}
