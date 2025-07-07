#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "item.hpp"

enum Hashing { linear, double_hash, quadratic };

template <typename K, typename V>
class HashTable {
  std::vector<Item<K, V>*> data;
  int size;
  Hashing hashing;

  void clear() {
    for (auto& item : data) {
      delete item;
      item = nullptr;
    }
  }

  int h(K key, int index) {
    switch (hashing) {
      case Hashing::quadratic:
        return (key + index * int(std::pow(index, 2))) % size;

      case Hashing::double_hash: {
        int hash1 = key % size;
        int hash2 = 1 + (key % (size - 1));

        return (hash1 + index * hash2) % size;
      }

      default:
        return (key + index) % size;
    }
  }

public:
  HashTable(int size, std::ifstream& input) : size(size) {
    data.resize(size, nullptr);
    load(input);
  }

  ~HashTable() { clear(); }

  void load(std::ifstream& input) {
    clear();
    input.clear();
    input.seekg(0, std::ios::beg);

    std::string line;
    while (std::getline(input, line)) {
      line = line.front() == '<' ? line.substr(1) : line;
      if (line.back() == '>') line.pop_back();
      for (auto& ch : line) ch = ch == ',' ? ' ' : ch;

      std::istringstream iss(line);
      K key;
      V value;
      iss >> key >> value;
      Item<K, V>* element = new Item<K, V>(key, value);
      insert(element);

      iss.clear();
      iss.str("");
    }
  }

  void insert(Item<K, V>* element) {
    for (int i = 0; i < size; i++) {
      int index = h(element->get_key(), i);

      if (!data[index]) {
        data[index] = element;
        return;
      }
    }

    std::cerr << "[insert ERROR] Overflow" << std::endl;
  }

  void print(std::string message = "Hash table", std::ostream& out = std::cout) {
    out << message << std::endl;

    for (int i = 0; i < size; i++) {
      out << "Index " << i << " => ";
      if (!data[i])
        out << "empty";
      else
        data[i]->print(out);
      out << std::endl;
    }

    out << std::endl;
  }

  Item<K, V>* search(K key) {
    std::ostringstream oss;
    for (int i = 0; i < size; i++) {
      int index = h(key, i);

      if (!data[index]) {
        std::cerr << "[search ERROR] Cell " << index << " empty" << std::endl;
        return nullptr;
      }

      if (data[index]->get_key() == key) {
        std::cout << "[search INFO] Item ";
        data[index]->print();
        std::cout << " found in cell " << index << std::endl;
        return data[index];
      }
    }

    std::cerr << "[search ERROR] Item " << key << " not found" << std::endl;
    return nullptr;
  }

  void delete_element(K key) {
    std::ostringstream oss;
    for (int i = 0; i < size; i++) {
      int index = h(key, i);

      if (!data[index]) {
        std::cerr << "[delete_element ERROR] Cell " << index << " empty" << std::endl;
        return;
      }

      if (data[index]->get_key() == key) {
        std::cout << "[delete_element INFO] Item ";
        data[index]->print();
        std::cout << " found in cell " << index << std::endl;

        delete data[index];
        data[index] = nullptr;

        std::cout << "[delete_element INFO] Item " << key << " deleted successfully" << std::endl;
        return;
      }
    }

    std::cerr << "[delete_element ERROR] Item " << key << " not found" << std::endl;
  }
};

#endif