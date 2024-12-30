#ifndef DATAHOLDER_HPP
#define DATAHOLDER_HPP

#include <unordered_map>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>
#include <tuple>
#include <filesystem>
#include <fstream>
#include <sstream>

using namespace std;
namespace fs = filesystem;

template<typename x, typename y>
class DataHolder {
private:
    //Info members
    vector <string> namesOfCollections;
    string ID = "NULL_ID";
    unordered_map<x, y> Data;
    string IDFilePath;

    //Transaction Maps
    map<int, unordered_map<x, y>> AllStatesOfData;

    //transaction manage
    bool begin = false;
    int commit_count = 1;

public:
    //Transaction functions
    //Provides a full rollback to get to the first pos in AllStatesOfData
    void rollBack_full() {
        if (AllStatesOfData.empty()) {
            cout << "No transaction history exists. Rollback not possible." << endl;
            return;
        }

        cout << "Are you sure you want a full rollback? [Y for yes]\n";
        char yn;
        cin >> yn;
        if (yn == 'y' || yn == 'Y') {
            this->Data = AllStatesOfData.begin()->second;
            AllStatesOfData.clear();
            commit_count = 0;
            cout << "Full rollback completed. All transaction history has been cleared.\n";
            cout << "You must begin a new transaction to resume tracking changes.\n";
        }
        else {
            cout << "Full rollback cancelled.\n";
        }
    }

    //single rollBack
    void rollBackTransaction() {
        rollBackTransaction(1);
    }

    //rollback n times
    void rollBackTransaction(int n) {

        if (AllStatesOfData.empty()) {
            cout << "No transaction history exists. Rollback not possible." << endl;
            return;
        }
        if (n < 0 || n >= commit_count) {
            cout << "Invalid rollback request. Rollback not possible." << endl;
            return;
        }
        n++;
        auto target = next(AllStatesOfData.begin(), commit_count - n - 1);
        this->Data = target->second;
        AllStatesOfData.erase(next(target), AllStatesOfData.end());
        commit_count -= n;
        cout << "Rollback by " << n << " steps completed successfully." << endl;
    }

    //transaction init
    void beginTransaction() {
        if (!this->begin) {
            this->begin = true;
            this->AllStatesOfData[this->commit_count] = this->Data;
            commit_count++;
            cout << "Current state of DataHolder : " << this->ID << " Recorded.\nTransaction begins\n";
        }
        else cout << "Already started recording changes for DataHolder : " << this->ID << endl;
    }

    //commit transaction
    void commitTransaction() {

        if (!this->begin) {
            cout << "No Records for DataHolder : " << this->ID << "\nThe Transaction never began." << endl;
            return;
        }

        if (AllStatesOfData.size() >= 6) {
            cout << endl << "All ready History of 10 commits." << endl;
            cout << "Do you want to still record this commit ? \nThe Initial state of Data will change to the Immediate next commit" << endl;
            cout << "[Y for yes]" << endl;
            char yn;
            cin >> yn;
            if (yn == 'y' || yn == 'Y') {
                AllStatesOfData.erase(AllStatesOfData.begin());
                cout << "Initial State deleted\nAdding the new state\n";
                this->AllStatesOfData[this->commit_count] = this->Data;
                this->commit_count++;
                cout << "All Updates Recorded for " << this->ID << endl << "Transaction Succesfully Commited" << endl;
            }
            return;
        }
        this->AllStatesOfData[this->commit_count] = this->Data;
        this->commit_count++;
        cout << "All Updates Recorded for " << this->ID << endl << "Transaction Succesfully Commited" << endl;
    }
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

    string getIDFilePath() {
        return this->IDFilePath;
    }

    bool checkBegin() {
        return this->begin;
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

    void setIDFilePath(fs::path FilePath) {
        this->IDFilePath = FilePath.string();
    }

    void renameID(string newName) {
        this->ID = newName;
        string newfilePath = newName + "_DataHolderFile.txt";
        setIDFilePath(newfilePath);
    }

    //File Functions
    void exportData() {
        try {
            fs::path current = fs::current_path() / "DataHolders";
            fs::create_directories(current);

            string fileName = this->ID + "_DataHolderFile.txt";
            fs::path exportFilePath = current / fileName;

            setIDFilePath(exportFilePath);
            writeData(exportFilePath);
        }
        catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    void importData(const string& fileName) {
        try {
            fs::path current = fs::current_path() / "DataHolders";
            fs::create_directories(current);
            fs::path importFilePath = current / fileName;

            ifstream importFile(importFilePath);
            string content((istreambuf_iterator<char>(importFile)), istreambuf_iterator<char>());

            string newContent = trimData(content);
            setKeyValueToData(newContent);
            cout << "imported details from " << fileName << " to " << this->ID << endl;
        }
        catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
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

    size_t countEntries()const {
        return this->Data.size();
    }

    void writeData(const fs::path& filePath) {

        ofstream exportFile(filePath);
        if (!exportFile.is_open()) {
            cerr << "Failed to open file: " << filePath << endl;
            return;
        }

        exportFile << "{\n";
        for (const auto& pair : this->Data) {
            exportFile << pair.first << " : " << pair.second << "\n";
        }
        exportFile << "}\n";

        cout << "Export file for " << this->ID << " created at : " << filePath << endl;
    }

    string trimData(string& input) {

        size_t start = input.find('{');
        size_t end = input.find('}');
        if (start != string::npos && end != string::npos) {
            input = input.substr(start + 1, end - start - 1);
        }
        return input;
    }

    void setKeyValueToData(string& input) {
        stringstream ss(input);
        string line;

        while (getline(ss, line)) {
            size_t separator = line.find(':');
            if (separator != string::npos) {
                string key = line.substr(0, separator);
                string value = line.substr(separator + 1);

                key.erase(0, key.find_first_not_of(" \n"));
                key.erase(key.find_last_not_of(" \n") + 1);
                value.erase(0, value.find_first_not_of(" \n"));
                value.erase(value.find_last_not_of(" \n") + 1);

                this->Data[key] = value;
            }
        }
    }

    vector <x> extractKeys(unordered_map<x, y> theMap) {
        vector<x> allKeys;
        for (const auto& pair : theMap) {
            allKeys.push_back(pair.first);
        }
        return allKeys;
    }

    void updateAddedRemoved() {
        vector<x> CurrentThisDataKeys = extractKeys(this->Data);
        vector<x> thisDataKeys;
    }


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

    //prints all keys associated with a specific value.
    vector<x> searchByValue(const y& value) {
        vector<x> allKeys;
        for (const auto& pair : this->Data) {
            if (pair.second == value) {
                allKeys.push_back(pair.first);
            }
        }
        return allKeys;
    }

};

#endif // DATAHOLDER_HPP
