#ifndef COLLECTIONS_HPP
#define COLLECTIONS_HPP

#include "DataHolder.hpp"
#include <unordered_map>
#include <iostream>

template<typename x, typename y>
class Collections {
private:
    string collection_name;
    unordered_map<string, DataHolder<x, y>> collectionMap;

public:
    // Constructors
    Collections(const string& collectionName) : collection_name(collectionName) {}
    Collections() {
        cout << "Enter the Collection name: ";
        cin >> collection_name;
    }

    // Add a DataHolder by ID
    void add_to_collection(DataHolder<x, y>& dataHolder) {

        string CN = this->collection_name;
        string id = dataHolder.getID();

        if (collectionMap.find(id) != collectionMap.end()) {
            cout << "DataHolder: " << id << " already exists in the collection: " << collection_name << endl;
        }
        else {
            dataHolder.addInCollectionVector(CN); 
            collectionMap.emplace(id, dataHolder);  
            cout << "DataHolder: " << id << " added to the collection: " << collection_name << endl;
        }
    }


    // Add multiple DataHolders
    void add_multiple_dataHolders(vector<DataHolder<x, y>*>& dataHolders) {

        for (auto* dataHolder : dataHolders) { 
            if (dataHolder != nullptr) {       
                add_to_collection(*dataHolder); 
            }
            else {
                cout << "Null DataHolder encountered, skipping." << endl;
            }
        }
    }


    // Remove a DataHolder by ID
    void remove_from_collection(DataHolder<x, y>& dataHolder) {

        string CN = this->collection_name;
        string id = dataHolder.getID();

        if (collectionMap.find(id) != collectionMap.end()) {
            dataHolder.removeFromCollectionVector(CN); 
            collectionMap.erase(id); 
            cout << "DataHolder: " << id << " removed from the collection: " << collection_name << endl;
        }
        else {
            cout << "DataHolder: " << id << " not found the collection: " << collection_name << endl;
        }
    }

    // Remove multiple DataHolders by IDs
    void remove_multiple_dataHolders(vector<DataHolder<x, y>*>& dataHolders) {

        for (auto* dataHolder : dataHolders) {
            if (dataHolder != nullptr) {
                remove_from_collection(*dataHolder);
            }
            else {
                cout << "Null DataHolder encountered, skipping." << endl;
            }
        }
    }

    // Print the collection
    void printCollection() const {
        cout << endl << "Collection name: " << collection_name << endl;
        for (const auto& pair : collectionMap) {
            cout << "ID: " << pair.first << endl;
            pair.second.printData();
        }
    }
};

#endif // COLLECTIONS_HPP
