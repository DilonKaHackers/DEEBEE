#ifndef DATAHOLDER_HPP
#define DATAHOLDER_HPP

#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>
#include <tuple>

using namespace std;

template<typename x, typename y>
class DataHolder {
private:
    vector <string> namesOfCollections;
    string ID = "NULL_ID";
    unordered_map<x, y> Data;

public:
    //DataHolder Constructors
    DataHolder(const string& name) : ID(name) {}
    DataHolder() = default;
    
    //Functions
 
    // Getters
    string getID() const {
        return this->ID; 
    }

    unordered_map<x, y> getData() const { 
        return this->Data; 
    }

    void showCollectionNames() {

        cout << "Collections for " << this->ID << " : ";
        for (int i = 0;i < namesOfCollections.size();i++) {
            cout << namesOfCollections[i] << " ";
        }
        cout << endl;
    }

    //Setters
    void addInCollectionVector(string& collectionName) {
        namesOfCollections.push_back(collectionName);
    }

    void setData(unordered_map<x, y> MAP) {
        this->Data = MAP;
    }

    void setID(string name) {
        this->ID = name;
    }

    void removeFromCollectionVector(string& collectionName) {
        namesOfCollections.erase(find(namesOfCollections.begin(), namesOfCollections.end(), collectionName));
    }

    //Helper Functions
    bool checkKey(x& key) {

        if (this->Data.find(key) != this->Data.end()) {
            return true;
        }
        return false;
    }

    y getValue(x& key) {
        return this->Data[key];
    }

    void clearData() {
        this->Data.clear();
    }

    bool isEmpty() {
        return this->Data.empty();
    }

    //print Data
    void printData() const {
        if (this->Data.empty()) {
            cout << "DataHolder has no data";
            return;
        }
        cout << "DataHolder : " << this->ID;
        cout << endl << "{" << endl;
        for (const auto& pair : Data) {
            cout << pair.first << " : " << pair.second << endl;
        }
        cout << "}" << endl;
    }

    //Major 
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

    //update key value
    void update_key(const x& key, const y& val) {
        if (Data.find(key) != Data.end()) {
            Data[key] = val;
            return;
        }
        else {
            char yn;
            cout << "Key : " << key << " does not exist in Collection.\nDo you want to add this key to collection? [Y/N] " << endl;
            cin >> yn;
            if (yn == 'y' || yn == 'Y') add_KeyValue_pair(key, val);
            else return;
        }
    }

    //update multiple keys and values
    void update_multiple_Keys(const vector<x>& keys, const vector<y>& vals) {

        if (keys.size() != vals.size()) {
            cout << "Not equal size of keys and values" << endl;
            return;
        }

        for (int i = 0;i < keys.size();i++) {
            update_key(keys[i], vals[i]);
        }
    }

};

#endif // DATAHOLDER_HPP
