#ifndef THEDB_HPP
#define THEDB_HPP

#include "Collections.hpp"

template <typename x, typename y>
class DB {
private:
    unordered_map<string, shared_ptr<Collections<x, y>>> allCollections;

    //private Helpers
    void merge_multiple(vector<string> collectionNames, string newCollectionName) {
        for (const auto& collectionName : collectionNames) {
            if (!check_collection_in_DB(collectionName)) {
                cout << "Collection: " << collectionName << " does not exist in DB" << endl;
                return;
            }
        }

        if (check_collection_in_DB(newCollectionName)) {
            cout << "Collection: " << newCollectionName << " already exists in DB" << endl;
            return;
        }

        const auto& newCollection = make_shared<Collections<x, y>>();
        newCollection->setCollectionName(newCollectionName);

        shared_ptr<Collections<x, y>> collection1 = get_collection_from_DB(collectionNames[0]);
        unordered_map<string, DataHolder<x, y>> mergedData = collection1->getCollectionMap();

        for (size_t i = 1; i < collectionNames.size(); ++i) {
            shared_ptr<Collections<x, y>> collection = get_collection_from_DB(collectionNames[i]);
            const unordered_map<string, DataHolder<x, y>>& toBeMergedData = collection->getCollectionMap();

            for (const auto& item : toBeMergedData) {
                mergedData[item.first] = item.second;
            }
        }

        newCollection->setCollectionMap(mergedData);
        this->allCollections[newCollectionName] = newCollection;

        cout << "Merged Collection: " << newCollectionName << " has been created and added to the DB" << endl;
        cout << "Merged collections: ";
        for (const auto& collectionName : collectionNames) {
            cout << collectionName << " ";
        }
        cout << "to new collection: " << newCollectionName << endl;

        char yn;
        cout << "Do you want all these collections to be removed from DB? [Y for Yes]" << endl;
        cin >> yn;

        if (yn == 'Y' || yn == 'y') {
            for (const auto& collectionName : collectionNames) {
                shared_ptr<Collections<x, y>> collectionToRemove = get_collection_from_DB(collectionName);
                Collections<x, y> collection___ = *collectionToRemove;
                remove_from_DB(collection___);
            }
        }
    }

    void merge_(string collectionName1, string collectionName2, string newCollectionName) {
        if (!check_collection_in_DB(collectionName1)) {
            cout << "Collection : " << collectionName1 << " does not exist in DB" << endl;
            return;
        }

        if (!check_collection_in_DB(collectionName2)) {
            cout << "Collection : " << collectionName2 << " does not exist in DB" << endl;
            return;
        }

        shared_ptr<Collections<x, y>> collection1 = get_collection_from_DB(collectionName1);
        shared_ptr<Collections<x, y>> collection2 = get_collection_from_DB(collectionName2);

        if (check_collection_in_DB(newCollectionName)) {
            cout << "Collection: " << newCollectionName << " already exists in DB" << endl;
            return;
        }

        const auto& newCollection = make_shared<Collections<x, y>>();
        newCollection->setCollectionName(newCollectionName);

        unordered_map<string, DataHolder<x, y>> mergedData = collection1->getCollectionMap();
        const unordered_map<string, DataHolder<x, y>>& toBeMerged = collection2->getCollectionMap();

        for (const auto& item : toBeMerged) {
            mergedData[item.first] = item.second;
        }

        newCollection->setCollectionMap(mergedData);
        this->allCollections[newCollectionName] = newCollection;

        cout << "Merged Collection: " << newCollectionName << " has been created and added to the DB" << endl;

        char yn;
        cout << "Do you want Collection: " << collectionName1 << " and Collection: " << collectionName2
            << " to be removed from DB? [Y for Yes]" << endl;
        cin >> yn;
        if (yn == 'Y' || yn == 'y') {
            Collections<x, y> collection1___ = *collection1;
            Collections<x, y> collection2___ = *collection2;

            remove_from_DB(collection1___);
            remove_from_DB(collection2___);
        }
    }

public:

    //Helper Functions
    bool check_collection_in_DB(string theCollectionName) {
        return this->allCollections.find(theCollectionName) != this->allCollections.end();
    }

    bool check_collection_in_DB(Collections<x, y>& theCollection) {
        return this->allCollections.find(theCollection.getCollectionName()) != this->allCollections.end();
    }

    shared_ptr<Collections<x, y>> get_collection_from_DB(const string& collectionName) {

        const auto it = this->allCollections.find(collectionName);
        if (it != this->allCollections.end()) {
            return it->second;
        }
        cout << "Collection: " << collectionName << " not found in DB." << endl;
        return nullptr;
    }

    void list_DB() {
        cout << "All Collections in the DB ->" << endl << endl;
        for (const auto& collections : this->allCollections) {
            cout << collections.first << endl;
        }
        cout << endl;
    }

    //Major
    //adds collections to DB
    void add_to_DB(Collections<x, y>& theCollection) {

        const auto it = this->allCollections.find(theCollection.getCollectionName());
        if (it != this->allCollections.end()) {
            cout << "Collection : " << theCollection.getCollectionName() << "already exists in DB" << endl;
            return;
        }

        this->allCollections[theCollection.getCollectionName()] = make_shared<Collections<x, y>>(theCollection);
        cout << "Collection : " << theCollection.getCollectionName() << " added to DB" << endl;
    }

    //removes collections from DB
    void remove_from_DB(Collections<x, y>& theCollection) {

        const auto it = this->allCollections.find(theCollection.getCollectionName());
        if (it != this->allCollections.end()) {
            this->allCollections.erase(it);
            cout << "Collection : " << theCollection.getCollectionName() << " removed from DB" << endl;
            return;
        }

        cout << "Collection : " << theCollection.getCollectionName() << " does not exist in DB" << endl;
    }

    //merge collections using 2 collections
    void merge_collections(Collections<x, y> c1, Collections<x, y> c2, string nc) {
        string collectionName1 = c1.getCollectionName();
        string collectionName2 = c2.getCollectionName();
        merge_(collectionName1, collectionName2, nc);
    }

    //merge collections using 2 collection names
    void merge_collections(string collectionName1, string collectionName2, string newCollectionName) {
        merge_(collectionName1, collectionName2, newCollectionName);
    }

    //merge multiple collections using collections vector
    void merge_multiple_collections(vector<Collections<x, y>> collections, string newCollectionName) {
        vector<string> collectionNames;
        for (const auto& collection : collections) {
            collectionNames.push_back(collection.getCollectionName());
        }
        merge_multiple(collectionNames, newCollectionName);
    }

    //merge multiple collections using collection names
    void merge_multiple_collections(vector<string> collectionNames, string newCollectionName) {
        merge_multiple(collectionNames, newCollectionName);
    }

    //fetches details on a Dataholder from all collections in db, using id
    void fetchDataHolderInfo(const string& dataHolderName) {
        vector<tuple<x, y, string>> dataList;

        for (const auto& collectionPair : this->allCollections) {
            const auto& collection = collectionPair.second;
            const unordered_map<string, DataHolder<x, y>>& tempCollectionMap = collection->getCollectionMap();

            auto it = tempCollectionMap.find(dataHolderName);
            if (it != tempCollectionMap.end()) {
                const DataHolder<x, y>& tempDataHolder = it->second;
                const unordered_map<x, y>& tempData = tempDataHolder.getData();

                for (const auto& item : tempData) {
                    dataList.emplace_back(item.first, item.second, collection->getCollectionName());
                }
            }
        }

        if (dataList.empty()) {
            cout << "DataHolder : " << dataHolderName << " not found in DB." << endl;
            return;
        }

        cout << "DataHolder ID: " << dataHolderName << endl;

        unordered_map<x, int> keyCount;
        for (const auto& entry : dataList) {
            keyCount[get<0>(entry)]++;
        }

        for (const auto& entry : dataList) {
            const x& key = get<0>(entry);
            const y& value = get<1>(entry);
            const string& collectionName = get<2>(entry);

            if (keyCount[key] > 1) {
                cout << key << " (" << collectionName << ") : " << value << endl;
            }
            else {
                cout << key << " : " << value << endl;
            }
        }
    }

};

#endif // !THEDB_HPP
