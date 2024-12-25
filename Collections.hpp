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
    Collections() = default;

    //getter
    string getCollectionName(){
        return this->collection_name;
    }
    unordered_map<string, DataHolder<x, y>> getCollectionMap(){
        return this->collectionMap;
    }
    DataHolder<x, y> getDataHolder(string nameOfDataHolder) {
        if (collectionMap.find(nameOfDataHolder) != collectionMap.end()) {
            return collectionMap[nameOfDataHolder];
        }
    }

    //exists 
    //Checks if dataholder exists with dataholder id
    bool exists(string nameOfDataHolder) {
        bool ans = false;
        
        if (collectionMap.find(nameOfDataHolder) != collectionMap.end()) {
            ans = true;          
        }
        return ans;
    }
    //Checks if dataholder exists with dataholder parameter
    bool exists(DataHolder<x,y> theDataHolder) {
        bool ans = false;

        if (collectionMap.find(theDataHolder.getID()) != collectionMap.end()) {
            ans = true;
        }
        return ans;
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
        if (this->collectionMap.empty()) {
            cout << "Collection has no data";
            return;
        }
        cout << endl << "Collection name: " << collection_name << endl;
        for (const auto& pair : collectionMap) {
            cout << "ID: " << pair.first << endl;
            pair.second.printData();
        }
    }
    // rename Collection
    void renameCollection(const string& newCollectionName) {
        this->collection_name = newCollectionName;
    }

    //clear collectionMap for a collection
    void clearCollection() {
        this->collectionMap.clear();
    }

};

#endif // COLLECTIONS_HPP







