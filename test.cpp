#include <iostream>
#include <vector>
#include "DataHolder.hpp"
#include "Collections.hpp"

using namespace std;
int main() {
    // Create Collections
    Collections<int, string> myCollection11("MyCollection11");
    Collections<int, string> myCollection22("MyCollection22");


    // Create DataHolder objects
    DataHolder<int, string> dh1("DH1");
    DataHolder<int, string> dh2("DH2");
    DataHolder<int, string> dh3("DH3");

    // Add DataHolder objects to a vector
    vector<DataHolder<int, string>> dataHolders = { dh1, dh2, dh3 };

    // Add multiple DataHolders
    myCollection11.add_to_collection(dh2);
    myCollection11.add_to_collection(dh1);

    myCollection22.add_to_collection(dh2);


    // Verify updates
    dh2.showCollectionNames();



    return 0;
}
