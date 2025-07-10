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

  void format_line(std::string& line) {
    if (line.front() == '<') line = line.substr(1);
    if (line.back() == '>') line.pop_back();
    for (auto& c : line) {
      if (c == ',') c = ' ';
    }
  }

  void clear_stream(std::istringstream& stream) {
    stream.clear();
    stream.str("");
  }

  int h(K key, int index) {
    if (fn == Hashing::linear) return (key + index) % size;
    if (fn == Hashing::quadratic) return (key + index + int(pow(index, 2))) % size;

    int hash1 = key % size;
    int hash2 = 1 + (key % (size - 1));
    return (hash1 + index * hash2) % size;
  }

public:
  HashTable(int size, std::ifstream& input, Hashing fn = linear) : size(size), fn(fn) {
    data.resize(size);
    load(input);
  }

  void reset() {
    for (auto& item : data) item = nullptr;
  }

  void load(std::ifstream& input) {
    std::string line;
    while (std::getline(input, line)) {
      format_line(line);
      std::istringstream iss(line);
      K key;
      V value;
      iss >> key >> value;
      insert(shared_item_ptr<K, V>(new Item<K, V>(key, value)));

      clear_stream(iss);
      line.clear();
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

  shared_item_ptr<K, V> search(K key) {
    for (int i = 0; i < size; i++) {
      int index = h(key, i);

      if (!data[index]) {
        std::cerr << "[search ERROR] Cell " << index << " empty" << std::endl;
        return nullptr;
      }

      if (data[index]->get_key() == key) {
        std::cout << "[search INFO] Item ";
        data[index]->print();
        std::cout << " found" << std::endl;

        return data[index];
      }
    }

    std::cerr << "[search ERROR] Item " << key << " not found" << std::endl;
    return nullptr;
  }

  void delete_item(K key) {
    for (int i = 0; i < size; i++) {
      int index = h(key, i);

      if (!data[index]) {
        std::cerr << "[delete_item ERROR] Cell " << index << " empty" << std::endl;
        return;
      }

      if (data[index]->get_key() == key) {
        data[index] = nullptr;

        std::cout << "[delete_item INFO] Item " << key << " deleted successfully" << std::endl;
        return;
      }
    }

    std::cerr << "[delete_item ERROR] Item " << key << " not found" << std::endl;
  }

  void print(std::ostream& out = std::cout, std::string message = "Hash table") {
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