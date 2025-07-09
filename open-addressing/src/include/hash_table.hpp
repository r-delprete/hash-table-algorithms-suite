#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cmath>
#include <fstream>
#include <iostream>
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
  std::vector<unique_item_ptr<K, V>> data;
  int size;
  Hashing fn;

  void format_line(std::string& line) {
    if (line.front() == '<') line = line.substr(1);
    if (line.back() == '>') line.pop_back();
    for (auto& c : line) c = c == ',' ? ' ' : c;
  }

  void clear_stream(std::istringstream& stream) {
    stream.clear();
    stream.str("");
  }

  int h(K key, int index) {
    if (fn == Hashing::linear) return (key + index) % size;
    if (fn == Hashing::quadratic) return (key + index * int(pow(index, 2))) % size;

    int hash1 = key % size;
    int hash2 = 1 + (key % (size - 1));
    return (hash1 + index * hash2) % size;
  }

  void clear() {
    for (auto& item : data) item.reset();
  }

public:
  HashTable(int size, std::ifstream& input, Hashing fn = Hashing::linear) : size(size), fn(fn) {
    data.resize(size);
    load(input);
  }

  ~HashTable() {
    clear();
    data.clear();
    size = 0;
  }

  bool empty() { return data.empty(); }

  void load(std::ifstream& input) {
    if (!empty()) clear();

    input.clear();
    input.seekg(0, std::ios::beg);

    std::string line;
    while (std::getline(input, line)) {
      format_line(line);
      std::istringstream iss(line);
      std::cout << "[load INFO] Read line => " << line << std::endl;
      K key;
      V value;
      iss >> key >> value;
      insert(unique_item_ptr<K, V>(new Item<K, V>(key, value)));

      clear_stream(iss);
    }
  }

  void insert(unique_item_ptr<K, V> item) {
    std::cout << "[insert INFO] Inserting ";
    item->print();
    std::cout << std::endl;

    for (int i = 0; i < size; i++) {
      int index = h(item->get_key(), i);

      if (!data[index]) {
        std::cout << "[insert INFO] Insert ";
        item->print();
        std::cout << " in cell " << index << std::endl;

        data[index] = std::move(item);
        return;
      }
    }

    std::cerr << "[insert ERROR] Overflow" << std::endl;
  }

  Item<K, V>* search(K key) {
    if (empty()) {
      std::cerr << "[search ERROR] Hash table is empty" << std::endl;
      return nullptr;
    }

    for (int i = 0; i < size; i++) {
      int index = h(key, i);

      if (!data[index]) {
        std::cerr << "[search ERROR] Cell " << index << " is empty" << std::endl;
        return nullptr;
      }

      if (data[index]->get_key() == key) {
        std::cout << "[search INFO] Item ";
        data[index]->print();
        std::cout << " found in cell " << index << std::endl;
        return data[index].get();
      }
    }

    std::cerr << "[search ERROR] Key " << key << " not found" << std::endl;
    return nullptr;
  }

  void delete_item(K key) {
    if (empty()) {
      std::cerr << "[delete_item ERROR] Hash table is empty" << std::endl;
      return;
    }

    for (int i = 0; i < size; i++) {
      int index = h(key, i);

      if (!data[index]) {
        std::cerr << "[delete_item ERROR] Cell " << index << " is empty" << std::endl;
        return;
      }

      if (data[index]->get_key() == key) {
        std::cout << "[delete_item INFO] Item ";
        data[index]->print();
        std::cout << " found in cell " << index << std::endl;

        std::cout << "[delete_item INFO] Item ";
        data[index]->print();
        std::cout << " deleted successfully" << std::endl;

        data[index].reset();

        return;
      }
    }

    std::cerr << "[delete_item ERROR] Key " << key << " not found" << std::endl;
    return;
  }

  void print(std::string message = "Hash table", std::ostream& out = std::cout) {
    if (empty()) {
      out << "[print INFO] Hash table is empty" << std::endl;
      return;
    }

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