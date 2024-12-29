#ifndef COLLECTIONS_HPP
#define COLLECTIONS_HPP

#include "DataHolder.hpp"

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
    string getCollectionName() {
        return this->collection_name;
    }

    unordered_map<string, DataHolder<x, y>>& getCollectionMap() {
        return this->collectionMap;
    }

    DataHolder<x, y>& getDataHolder(string nameOfDataHolder) {
        if (collectionMap.find(nameOfDataHolder) != collectionMap.end()) {
            return collectionMap[nameOfDataHolder];
        }
    }

    //setter
    void setCollectionMap(unordered_map<string, DataHolder<x, y>> newCollectionMap) {
        this->collectionMap = newCollectionMap;
    }
    void setCollectionName(string name) {
        this->collection_name = name;
    }

    //Helper functions
    bool exists(string nameOfDataHolder) {
        if (collectionMap.find(nameOfDataHolder) != collectionMap.end()) {
            return true;
        }
        return false;
    }

    bool exists(DataHolder<x, y> theDataHolder) {
        if (collectionMap.find(theDataHolder.getID()) != collectionMap.end()) {
            return true;
        }
        return false;
    }

    void renameCollection(const string& newCollectionName) {
        setCollectionName(newCollectionName);
    }

    void clearCollection() {
        this->collectionMap.clear();
    }

    // Print the collection
    void printCollection() const {
        if (this->collectionMap.empty()) {
            cout << "Collection has no data";
            return;
        }
        cout << "Collection name: " << collection_name << endl << endl;
        for (const auto& pair : collectionMap) {
            pair.second.printData();
            cout << endl;
        }
    }

    //Major
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

    // Remove a DataHolder by reference
    void remove_from_collection(DataHolder<x, y>& dataHolder) {
        string id = dataHolder.getID();

        auto it = collectionMap.find(id);
        if (it != collectionMap.end()) {
            collectionMap.erase(it);
            cout << "DataHolder \"" << id << "\" removed from collection \"" << collection_name << "\"." << endl;
        }
        else {
            cout << "DataHolder \"" << id << "\" not found in collection \"" << collection_name << "\"." << endl;
        }
    }

    // Remove a DataHolder by ID
    void remove_from_collection(const string& dataHolderName) {
        auto it = collectionMap.find(dataHolderName);
        if (it != collectionMap.end()) {
            collectionMap.erase(it);
            cout << "DataHolder \"" << dataHolderName << "\" removed from collection \"" << collection_name << "\"." << endl;
        }
        else {
            cout << "DataHolder \"" << dataHolderName << "\" not found in collection \"" << collection_name << "\"." << endl;
        }
    }

    // Remove multiple DataHolders by ID
    void remove_multiple_dataHolders(const vector<string>& dataHolderNames) {
        for (const auto& name : dataHolderNames) {
            remove_from_collection(name);
        }
    }

    // Remove multiple DataHolders by reference
    void remove_multiple_dataHolders(const vector<DataHolder<x, y>>& dataHolders) {
        for (const auto& dataHolder : dataHolders) {
            remove_from_collection(dataHolder);
        }
    }

};

#endif // COLLECTIONS_HPP
