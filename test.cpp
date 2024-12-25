#include "Collections.hpp"

using namespace std;
int main() {

    Collections<int, string> collection1("TestCollection1");
    Collections<int, string> collection2("TestCollection2");
    Collections<int, string> collection3("TestCollection3");

    DataHolder<int, string> dh1("DH1");
    DataHolder<int, string> dh2("DH2");
    DataHolder<int, string> dh3("DH3");

    vector<DataHolder<int, string>*> addDhVector = { &dh1, &dh2, &dh3 };
    vector<DataHolder<int, string>*> removeDhVector = { &dh1, &dh2 };


    // Adding individually
    collection1.add_to_collection(dh1);

    collection2.add_multiple_dataHolders(addDhVector);
    collection3.add_multiple_dataHolders(addDhVector);

   
    dh1.showCollectionNames();
    dh2.showCollectionNames();
    dh3.showCollectionNames();

    collection2.remove_multiple_dataHolders(removeDhVector);

    dh1.showCollectionNames();
    dh2.showCollectionNames();

    return 0;
}
