#include <iostream>
#include <vector>
#include <list>
#include <stdexcept>
#include <utility>

const size_t DEFAULT_SIZE = 100;

template<class KeyType, class ValueType, class Hash = std::hash<KeyType> >
class HashMap {
public:
public:
    typedef typename std::list<std::pair<const KeyType, ValueType>>::iterator iterator;
    typedef typename std::list<std::pair<const KeyType, ValueType>>::const_iterator const_iterator;
private:
    std::vector<std::list<iterator>> table;
    std::list<std::pair<const KeyType, ValueType>> list;
    Hash hash;
    size_t entriesCount;
    size_t tableSize;

public:

    explicit HashMap(Hash h = Hash())
            : hash(h), entriesCount(0), tableSize(DEFAULT_SIZE) {
        table.resize(tableSize);
    }

    template<class Iterator>
    HashMap(Iterator begin, Iterator end, Hash h = Hash())
            : HashMap(h) {
        table.resize(tableSize);
        while (begin != end) {
            insert(*begin);
            begin++;
        }
    }

    HashMap(std::initializer_list<std::pair<const KeyType, ValueType>> list, Hash h = Hash())
            : HashMap(list.begin(), list.end(), h) {
    }

    iterator begin() {
        return list.begin();
    }

    iterator end() {
        return list.end();
    }

    const_iterator begin() const {
        return list.cbegin();
    }

    const_iterator end() const {
        return list.cend();
    }

    size_t size() const {
        return entriesCount;
    }

    bool empty() const {
        return entriesCount == 0;
    }

    Hash hash_function() const {
        return hash;
    }

    size_t findIndex(KeyType key, size_t size) const {
        return hash(key) % size;
    }

    double load_factor() const {
        return static_cast<double>(entriesCount) / tableSize;
    }

    size_t capacity() const {
        return tableSize;
    }

    bool rebuild() {
        if (load_factor() < 2) {
            return false;
        }
        size_t newSize = tableSize * 2;
        std::vector<std::list<iterator>> newTable;
        newTable.resize(newSize);
        std::list<std::pair<const KeyType, ValueType>> newList;
        for (const auto &i: *this) {
            size_t index = findIndex(i.first, newSize);
            newList.push_front(i);
            newTable[index].push_front(newList.begin());
        }
        tableSize = newSize;
        table = move(newTable);
        list = move(newList);
        return true;
    }

    std::pair<const KeyType, ValueType> &insert(std::pair<const KeyType, ValueType> entry) {
        iterator iter;
        iter = find(entry.first);
        if (iter != end()) {
            return *iter;
        }
        size_t index = findIndex(entry.first, tableSize);
        list.push_front(entry);
        table[index].push_front(list.begin());
        entriesCount++;
        if (rebuild()) {
            iter = find(entry.first);
            if (iter != end()) {
                return *iter;
            }
        }
        return *table[index].front();
    }

    void erase(const KeyType entry) {
        size_t index = findIndex(entry, tableSize);
        for (auto i = table[index].begin(); i != table[index].end(); ++i) {
            if ((*i)->first == entry) {
                list.erase(*i);
                table[index].erase(i);
                entriesCount--;
                rebuild();
                return;
            }
        }
    }

    ValueType &operator[](const KeyType &key) {
        iterator iter;
        iter = find(key);
        if (iter != end()) {
            return iter->second;
        }
        return insert({key, ValueType{}}).second;
    }

    const ValueType &at(const KeyType &key) const {
        const_iterator iter;
        iter = find(key);
        if (iter != end()) {
            return iter->second;
        }
        throw std::out_of_range("");
    }

    HashMap &operator=(const HashMap &rvalue) {
        if (this == &rvalue) {
            return *this;
        }
        clear();
        tableSize = rvalue.capacity();
        table.resize(tableSize);
        list.clear();
        for (const auto &i: rvalue) {
            size_t index = findIndex(i.first, tableSize);
            list.push_front(i);
            table[index].push_front(list.begin());
            entriesCount++;
        }
        return *this;
    }

    void clear() {
        table.clear();
        entriesCount = 0;
        list.clear();
        tableSize = DEFAULT_SIZE;
        table.resize(tableSize);
    }

    iterator innerFind(const KeyType &key) const {
        size_t index = findIndex(key, tableSize);
        for (auto i = table[index].begin(); i != table[index].end(); ++i) {
            if ((*i)->first == key) {
                return *i;
            }
        }
        return iterator{};
    }

    iterator find(const KeyType &key) {
        iterator position = innerFind(key);
        if (position == iterator{}) {
            return end();
        }
        return position;
    }

    const_iterator find(const KeyType &key) const {
        iterator position = innerFind(key);
        if (position == iterator{}) {
            return end();
        }
        return static_cast<const_iterator>(position);
    }
};
