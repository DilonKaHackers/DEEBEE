#ifndef COLLECTIONS_HPP
#define COLLECTIONS_HPP

#include "DataHolder.hpp"

template<typename x, typename y>
class Collections {
private:
    string collection_name;
    unordered_map<string, DataHolder<x, y>> collectionMap;
    string collection_nameFilePath;

    //Transaction Maps
    map<int, unordered_map<string, DataHolder<x, y>>> AllStatesOfCollection;

    //Transaction manage
    bool begin_c = false;
    int commit_count_c = 1;

public:
    //Transaction functions
    //Provides a full rollback to get to the first pos in AllStatesOfCollection
    void rollBack_full() {
        if (AllStatesOfCollection.empty()) {
            cout << "No transaction history exists for Collection : " << this->collection_name << ".Rollback not possible." << endl;
            return;
        }

        cout << "Are you sure you want a full rollback? [Y for yes]\n";
        char yn;
        cin >> yn;
        if (yn == 'y' || yn == 'Y') {
            this->collectionMap = AllStatesOfCollection.begin()->second;
            AllStatesOfCollection.clear();
            commit_count_c = 0;
            cout << "Full rollback completed. All transaction history has been cleared for this collection.\n";
            cout << "You must begin a new transaction to resume tracking changes.\n";
        }
        else {
            cout << "Full rollback cancelled.\n";
        }
    }

    //rollback n times
    void rollBackTransaction(int n) {

        if (AllStatesOfCollection.empty()) {
            cout << "No transaction history exists. Rollback not possible." << endl;
            return;
        }
        if (n < 0 || n >= commit_count_c) {
            cout << "Invalid rollback request. Rollback not possible." << endl;
            return;
        }
        auto target = next(AllStatesOfCollection.begin(), commit_count_c - n - 1);
        this->collectionMap = target->second;
        AllStatesOfCollection.erase(next(target), AllStatesOfCollection.end());
        commit_count_c -= n;
        cout << "Rollback by " << n << " steps completed successfully for this Collection." << endl;
    }

    //single rollBack
    void rollBackTransaction() {
        rollBackTransaction(1);
    }

    //transaction init
    void beginTransaction() {
        if (!this->begin_c) {
            this->begin_c = true;
            this->AllStatesOfCollection[this->commit_count_c] = this->collectionMap;
            commit_count_c++;
            cout << "Current state of Collection : " << this->get_collection_name() << " Recorded.\nTransaction begins\n";
        }
        else cout << "Already started recording changes for Collection : " << this->get_collection_name() << endl;
    }

    //commit transaction
    void commitTransaction() {

        if (!this->begin_c) {
            cout << "No Records for Collection : " << this->get_collection_name() << "\nThe Transaction never began." << endl;
            return;
        }

        if (AllStatesOfCollection.size() >= 3) {
            cout << endl << "All ready History of 10 commits." << endl;
            cout << "Do you want to still record this commit ? \nThe Initial state of Data will change to the Immediate next commit" << endl;
            cout << "[Y for yes]" << endl;
            char yn;
            cin >> yn;
            if (yn == 'y' || yn == 'Y') {
                AllStatesOfCollection.erase(AllStatesOfCollection.begin());
                cout << "Initial State deleted\nAdding the new state\n";
                this->AllStatesOfCollection[this->commit_count_c] = this->collectionMap;
                this->commit_count_c++;
                cout << "All Updates Recorded for Collection : " << this->get_collection_name() << endl << "Transaction Succesfully Commited" << endl;
            }
            return;
        }
        this->AllStatesOfCollection[this->commit_count_c] = this->collectionMap;
        this->commit_count_c++;
        cout << "All Updates Recorded for " << this->get_collection_name() << endl << "Transaction Succesfully Commited" << endl;
    }

    // Constructors
    Collections(const string& collectionName) : collection_name(collectionName) {}
    Collections() = default;

    //getter
    string get_collection_name() {
        return this->collection_name;
    }

    unordered_map<string, DataHolder<x, y>>& get_collection_map() {
        return this->collectionMap; 
    }

    DataHolder<x, y>& get_dataholder(string nameOfDataHolder) {
        if (collectionMap.find(nameOfDataHolder) != collectionMap.end()) {
            return collectionMap[nameOfDataHolder];
        }
    }

    string getcollection_nameFilePath() {
        return this->collection_nameFilePath;
    }

    //setter
    void set_collection_map(unordered_map<string, DataHolder<x, y>> newCollectionMap) {
        this->collectionMap = newCollectionMap;
    }
    void set_collection_name(string name) {
        this->collection_name = name;
    }

    void setcollection_nameFilePath(fs::path FilePath) {
        this->collection_nameFilePath = FilePath.string();
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

    void rename_collection(const string& newCollectionName) {
        set_collection_name(newCollectionName);
        string newfilePath = newCollectionName + "_CollectionFile.txt";
        setcollection_nameFilePath(newfilePath);
    }

    void clear_collection() {
        this->collectionMap.clear();
    }

    size_t count_total_entries() {
        if (collectionMap.empty()) {
            cout << "No DataHolders in the Collection : " << this->collection_name;
            return;
        }

        size_t count = 0;
        for (const auto& pair : this->collectionMap) {
            const auto& dataholder = pair.second;
            count += dataholder.getData().size();
        }

        return count;
    }

    vector<pair<string, size_t>> entries_per_dataholder() {
        vector<pair<string, size_t>> result;

        if (collectionMap.empty()) {
            cout << "No DataHolders in the Collection: " << this->collection_name << endl << "Returning and empty Vector"<<endl;
            return result; 
        }

        for (const auto& pair : this->collectionMap) {
            const auto& dataHolder = pair.second;
            size_t count = dataHolder.getData().size(); 
            result.emplace_back(pair.first, count); 
        }

        return result; 
    }
    
    void writeData(const fs::path& FilePath) {
        ofstream exportFile(FilePath);
        if (!exportFile.is_open()) {
            cerr << "Failed to open file: " << FilePath << endl;
            return;
        }

        exportFile << endl << this->collection_name<< endl;
        for (const auto& pair : this->collectionMap) {
            DataHolder<x, y> dataholder = pair.second;
            exportFile << "DataHolder ID : " << pair.first<<endl;
            exportFile << "{\n";
            for (const auto &pairInner : dataholder.getData()) {
                exportFile << pairInner.first << " : " << pairInner.second << "\n";
            }
            exportFile << "}\n\n";
        }
        cout << endl << "Export file for Collection : " << this->collection_name << " created at : " << FilePath << endl;
    }

   
    void print_collection() const {
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

    //import, export
    void export_collection() {
        for (const auto& pair : this->collectionMap) {
            DataHolder<x, y> dataholder = pair.second;
            dataholder.exportData();
        }

        try {
            fs::path current = fs::current_path() / "Collections";
            fs::create_directories(current);

            string fileName = this->collection_name + "_CollectionFile.txt";
            fs::path exportFilePath = current / fileName;

            setcollection_nameFilePath(exportFilePath);
            writeData(exportFilePath);
        }
        catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    void import_collection(const string& filePath) {
        ifstream inputFile(filePath);
        if (!inputFile.is_open()) {
            cerr << "Error opening file: " << filePath << endl;
            return;
        }

        string line;
        bool isFirstLine = true;
        string currentKey;

        while (getline(inputFile, line)) {
            line.erase(0, line.find_first_not_of(" \t"));
            line.erase(line.find_last_not_of(" \t") + 1);

            if (isFirstLine) {
                set_collection_name(line);
                isFirstLine = false;
                continue;
            }

            if (line.empty()) {
                continue;
            }

            if (line.find("DataHolder ID :") != string::npos) {
                currentKey = line.substr(line.find(":") + 2);
                continue;
            }

            if (line == "{") {
                unordered_map<x, y> innerMap;

                while (getline(inputFile, line) && line != "}") {
                    line.erase(0, line.find_first_not_of(" \t"));
                    line.erase(line.find_last_not_of(" \t") + 1);

                    auto colonPos = line.find(':');
                    if (colonPos != string::npos) {
                        string key = line.substr(0, colonPos);
                        string value = line.substr(colonPos + 2);
                        innerMap[key] = value;
                    }
                }

                DataHolder<x, y> tempDataHolder;
                tempDataHolder.setData(innerMap);
                tempDataHolder.setID(currentKey);

                collectionMap[currentKey] = tempDataHolder;
            }
        }
        fs::path path(filePath);
        string newCollectionName = path.filename().string() + " (Imported)";
        set_collection_name(newCollectionName);
        inputFile.close();
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

    //returns all the collections which have the specific value from collections
    vector<DataHolder<x, y>> search_by_value_in_collection(const y& value) {
        vector<DataHolder<x, y>> selectedDataHolders;
        for (const auto& pair : this->collectionMap) {
            DataHolder <x, y> dataholder = get_dataholder(pair.first);
            unordered_map<x, y> theMap = dataholder.getData();
            for (const auto& innerPair : theMap) {
                if (innerPair.second == value) {
                    selectedDataHolders.push_back(dataholder);
                }
            }
        }
        return selectedDataHolders;
    }

    //returns a subset collection
    Collections<x, y> subset_collection(const vector<string>& dataHolderNames) {
        Collections<x, y> theSubset;
        unordered_map<string, DataHolder<x, y>> subsetCollectionMap;
        for (const auto &dataholderElement : dataHolderNames) {
            if (exists(dataholderElement)) {
                DataHolder<x, y> tempDataHolder = get_dataholder(dataholderElement);
                subsetCollectionMap[dataholderElement] = tempDataHolder;
            }
            else cout << "Skipping " << dataholderElement << " because it does not exist in Collection : "<< this->collection_name << endl;
        }

        theSubset.set_collection_map(subsetCollectionMap);
        string subsetCollectionName = "subset_" + this->collection_name;
        theSubset.set_collection_name(subsetCollectionName);
        return theSubset;
    }
};

#endif // COLLECTIONS_HPP
