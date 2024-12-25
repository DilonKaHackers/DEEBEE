#ifndef DATAHOLDER_H
#define DATAHOLDER_H

#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

template<typename x, typename y>
class DataHolder {
private:
    vector <string> namesOfCollections;
    string ID;
    unordered_map<x, y> Data;

public:
    //DataHolder Constructors
    DataHolder(const string& name) : ID(name) {}

    DataHolder() {
        cout << "Enter Name for the DataHolder : ";
        cin >> this->ID;
    }

    //DataHolder Functions
    // Getters
    string getID() const { return this->ID; }
    unordered_map<x, y> getData() const { return this->Data; }
    void showCollectionNames() {

        cout << "Collections for " << this->ID << " : ";
        for (int i = 0;i < namesOfCollections.size();i++) {
            cout << namesOfCollections[i] << " ";
        }
        cout << endl;
    }

    //Setters
    //updates vector to know what collections the dataHolder is in
    void addInCollectionVector(string& collectionName) {
        namesOfCollections.push_back(collectionName);
    }

    void removeFromCollectionVector(string& collectionName) {
        namesOfCollections.erase(find(namesOfCollections.begin(), namesOfCollections.end(), collectionName));
    }

    //functions
    // Add a key with a default value
    void add_Key(const x& key) {
        if (Data.find(key) != Data.end()) {
            cout << "Key: " << key << " already exists in " << this->ID << "." << endl;
        }
        else {
            Data[key] = y();
            cout << "Key : " << key << " added successfully to " << this->ID << "." << endl;
        }
    }

    // Add a key-value pair
    void add_KeyValue_pair(const x& key, const y& val) {
        if (Data.find(key) != Data.end()) {
            cout << "Key: " << key << " already exists in " << this->ID << "." << endl;
        }
        else {
            Data[key] = val;
            cout << "Key : " << key << ", Value : " << val << " added successfully to " << this->ID << "." << endl;
        }
    }

    // Add multiple key-value pairs
    void add_multiple_KeyValue_pairs(const vector<x>& keys, const vector<y>& vals) {
        size_t lesser = min(keys.size(), vals.size());
        for (size_t i = 0; i < lesser; i++) {
            add_KeyValue_pair(keys[i], vals[i]);
        }

        if (keys.size() > vals.size()) {
            for (size_t j = lesser; j < keys.size(); j++) {
                add_Key(keys[j]);
            }
        }
    }

    // Add multiple keys with default values
    void add_multiple_Keys(const vector<x>& keys) {
        for (const auto& key : keys) {
            add_Key(key);
        }
    }

    // Remove a key
    void remove_Key(const x& key) {
        if (Data.find(key) != Data.end()) {
            Data.erase(key);
            cout << "Key: " << key << " removed successfully from " << this->ID << "." << endl;
        }
        else {
            cout << "Key: " << key << " does not exist in " << this->ID << "." << endl;
        }
    }

    // Remove multiple keys
    void remove_multiple_Keys(const vector<x>& keys) {
        for (const auto& key : keys) {
            remove_Key(key);
        }
    }

    void printData() const {
        cout << endl << "DataHolder : " << this->ID;
        cout << endl << "{" << endl;
        for (const auto& pair : Data) {
            cout << pair.first << " : " << pair.second << endl;
        }
        cout << "}" << endl;

    }
};

#endif // DATAHOLDER_H
