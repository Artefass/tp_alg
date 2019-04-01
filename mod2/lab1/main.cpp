#include <iostream>
#include <string>
#include <vector>
#include <cassert>

#define BASE_SIZE      4
#define RESIZE_FACTOR  2
#define FILL_FACTOR    (3.0 / 4.0)

using namespace std;

size_t Hash1(string str, size_t size) {
    size_t hash = 0;
    size_t a    = 3469;

    for (size_t i = 0; i < str.size(); i++) {
        hash = (hash * a + (size_t)str[i]) % size;
    }

    return hash;
}

size_t Hash2(string str, size_t size) {
    size_t hash = 0;
    size_t a    = 1987;

    for (size_t i = 0; i < str.size(); i++) {
        hash = (hash * a + (size_t)str[i]) % size;
    }

    if (hash % 2 == 0)
        hash =(hash + 1) % size;

    return hash;
}

size_t Hash(string key, size_t size, size_t i) {
    size_t hash1 = Hash1(key, size);
    size_t hash2 = Hash2(key, size);
    size_t hash  = (hash1 + i * hash2) % size;

    return hash;
}

class HashTable {
    vector<string> table;
    size_t         fillTable;

    void grow() {
        assert( fillTable / (double)table.capacity() > FILL_FACTOR );

        size_t         tableSize    = table.capacity();
        size_t         newTableSize = RESIZE_FACTOR * tableSize;
        vector<string> newTable(newTableSize);

        for (size_t i = 0; i < tableSize; i++) {
            if ( !table[i].empty() && table[i] != "DEL") {

                for (size_t j = 0; j < newTableSize; j++) {
                    size_t hash = Hash(table[i], newTableSize, j);

                    if ( newTable[hash].empty() ) {
                        newTable[hash] = table[i];
                        break;
                    }
                }
            }
        }

        table = move(newTable);
    }

    int getIndex( string key ) {
        assert( key != "" && key != "DEL" );

        size_t tableSize = table.capacity();
        size_t hash = 0;
        size_t i;
        for (i = 0; i < tableSize; i++) {
            hash = Hash(key, tableSize, i);

            if (table[hash].empty())
                return -1;
            else if (table[hash] == key)
                return hash;
        }

        return -1;
    }

public:
    HashTable(): table(BASE_SIZE), fillTable(0) {}

    bool Has(string key) {
        int index = getIndex(key);

        return ( index != -1 );
    }

    bool Insert(string key) {
        assert( key != "" && key != "DEL" );

        if ( Has(key) )
            return false;

        size_t tableSize = table.capacity();

        if ( fillTable / (double)tableSize > FILL_FACTOR )
            grow();

        size_t i;
        size_t hash = 0;
        for (i = 0; i < tableSize; i++) {
            hash = Hash(key, tableSize, i);

            if ( table[hash].empty() || table[hash] == "DEL" ) {
                table[hash] = key;
                fillTable++;
                return true;
            }
        }

        assert( i == tableSize );
        return false;
    }

    bool Delete(string key) {
        assert( key != "" && key != "DEL" );

        int index = getIndex(key);
        if ( index == -1 )
            return false;

        table[index] = "DEL";
        fillTable--;
        return true;
    }
};



int main() {
    HashTable hashTable;
    char   op;
    string key;
    while ( cin >> op >> key ) {
        switch (op) {
            case '+':
                cout << ((hashTable.Insert(key)) ? "OK" : "FAIL") << endl;
                break;
            case '-':
                cout << ((hashTable.Delete(key)) ? "OK" : "FAIL") << endl;
                break;
            case '?':
                cout << ((hashTable.Has(key)) ? "OK" : "FAIL") << endl;
                break;
            default:
                assert(false);
        }
    }

    return 0;
}