#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cmath>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "item.hpp"

template <typename K, typename V>
using unique_item_ptr = std::unique_ptr<Item<K, V>>;

enum Hashing { linear, quadratic, double_hash };

template <typename K, typename V>
class HashTable {
  std::vector<shared_item_ptr<K, V>> data;
  int size;
  Hashing fn;

  const int h(const K key, const int index) const {
    if (fn == Hashing::quadratic) return (key + index * int(pow(index, 2))) % size;
    if (fn == Hashing::linear) return (key + index) % size;

    const int hash1 = key % size;
    const int hash2 = 1 + (key % (size - 1));
    return (hash1 + index * hash2) % size;
  }

  void format_line(std::string& line) {
    if (line.empty()) return;
    if (line.front() == '<') line = line.substr(1);
    if (line.back() == '>') line.pop_back();
    for (auto& ch : line) {
      if (ch == ',') ch = ' ';
    }
  }

  void clear_stream(std::istringstream& stream) {
    stream.clear();
    stream.str("");
  }

public:
  HashTable(const int size, std::ifstream& input) : size(size) {
    data.resize(size);
    load(input);
  }

  void reset() {
    data.clear();
    size = 0;
  }

  void load(std::ifstream& input) {
    std::string line;
    while (std::getline(input, line)) {
      format_line(line);
      std::istringstream iss(line);

      K key;
      V value;
      iss >> key >> value;
      insert(item_factory_shared(key, value));
    }
  }

  void insert(const shared_item_ptr<K, V>& item) {
    for (int i = 0; i < size; i++) {
      int index = h(item->get_key(), i);

      if (!data[index]) {
        data[index] = item;
        return;
      }
    }

    std::cerr << "[insert ERROR] Overflow" << std::endl;
  }

  shared_item_ptr<K, V> search(const K key) {
    if (data.empty()) {
      std::cerr << "[search ERROR] Hash table is empty" << std::endl;
      return nullptr;
    }

    for (int i = 0; i < size; i++) {
      int index = h(key, i);

      if (!data[index]) {
        std::cerr << "[search ERROR] Item with key " << key << " not found in cell " << index << std::endl;
        return nullptr;
      }

      if (data[index]->get_key() == key) return data[index];
    }

    std::cerr << "[search ERROR] Item with key " << key << " not found" << std::endl;
    return nullptr;
  }

  void delete_item(const K key) {
    if (data.empty()) {
      std::cerr << "[delete_item ERROR] Hash table is empty" << std::endl;
      return;
    }

    for (int i = 0; i < size; i++) {
      int index = h(key, i);

      if (!data[index]) {
        std::cerr << "[delete_item ERROR] Item with key " << key << " not found in cell " << index << std::endl;
        return;
      }

      if (data[index]->get_key() == key) {
        data[index] = nullptr;
        std::cout << "[delete_item INFO] Item with key " << key << " deleted successfully" << std::endl;
        return;
      }
    }

    std::cerr << "[delete_item ERROR] Item with key " << key << " not found" << std::endl;
  }

  void print(std::ostream& out = std::cout, const std::string message = "Hash table") const {
    out << message << std::endl;

    for (int i = 0; i < size; i++) {
      out << "Cell " << i << " => ";
      if (!data[i])
        out << "empty";
      else
        data[i]->print(out);
      out << std::endl;
    }
    out << std::endl;
  }
};

#endif