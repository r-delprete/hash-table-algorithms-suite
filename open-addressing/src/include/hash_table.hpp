#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "item.hpp"

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

enum Hashing { linear, quadratic, double_hash };

template <typename K, typename V>
class HashTable {
  std::vector<shared_item<K, V>> data;
  int size;
  Hashing fn;

  int h(const K key, const int index) const {
    if (fn == Hashing::linear) return (key + index) % size;
    if (fn == Hashing::quadratic) return (key + index * int(pow(index, 2))) % size;

    int hash1 = key % size;
    int hash2 = 1 + (key % (size - 1));
    return (hash1 + index * hash2) % size;
  }

public:
  HashTable(int size, std::ifstream& input, const Hashing fn = Hashing::linear) : size(size), fn(fn) {
    data.resize(size);
    load(input);
  }

  int get_size() const { return size; }

  void resize(const int size) {
    this->size = size;
    data.resize(size);
  }

  void reset() {
    data.clear();
    data.resize(size);
  }

  bool empty() const { return data.empty(); }

  void load(std::ifstream& input) {
    reset();
    input.clear();
    input.seekg(0, std::ios::beg);

    std::string line;
    while (std::getline(input, line)) {
      format_line(line);
      std::istringstream iss(line);

      K key;
      V value;
      iss >> key >> value;
      insert(create_item(key, value));

      clear_stream(iss);
      line.clear();
    }
  }

  void insert(const shared_item<K, V>& item) {
    for (int i = 0; i < size; i++) {
      int index = h(item->get_key(), i);

      if (!data[index]) {
        data[index] = item;
        return;
      }
    }

    std::cerr << "[insert ERROR] Overflow" << std::endl;
  }

  shared_item<K, V> search(const K key) const {
    if (empty()) {
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
    if (empty()) {
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
        std::cout << "[delete_item INFO] Deleting item ";
        data[index]->print();
        std::cout << std::endl;

        data[index].reset();

        std::cout << "[delete_item INFO] Item deleted successfully" << std::endl;
        return;
      }
    }

    std::cerr << "[delete_item ERROR] Item with key " << key << " not found" << std::endl;
  }

  void print(std::ostream& out = std::cout, const std::string message = "Hash table") {
    out << message << std::endl;

    for (int i = 0; i < size; i++) {
      out << "Cell #" << i + 1 << " => ";
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