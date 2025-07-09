#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cmath>
#include <fstream>
#include <list>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "item.hpp"

template <typename K, typename V>
using unique_item_ptr = std::unique_ptr<Item<K, V>>;

enum Hashing { multiply, divide };

template <typename K, typename V>
class HashTable {
  std::vector<std::list<unique_item_ptr<K, V>>> data;
  int size;
  Hashing fn;

  int h(K key) {
    if (fn == Hashing::divide) return key % size;

    double A = (sqrt(5) - 1) / 2;
    double prod = key * A;
    double frac = prod - floor(prod);

    return int(frac * size);
  }

  void clear() {
    for (auto& list : data) {
      for (auto& item : list) item.reset();
    }
  }

  void format_line(std::string& line) {
    if (line.front() == '<') line = line.substr(1);
    if (line.back() == '>') line.pop_back();
    for (auto& c : line) c = c == ',' ? ' ' : c;
  }

  void clear_stream(std::istringstream& stream) {
    stream.clear();
    stream.str("");
  }

public:
  HashTable(int size, std::ifstream& input, Hashing fn = Hashing::divide) : size(size), fn(fn) {
    data.resize(size);
    load(input);
  }

  ~HashTable() {
    clear();
    data.clear();
  }

  void load(std::ifstream& input) {
    clear();
    input.clear();
    input.seekg(0, std::ios::beg);

    std::string line;
    while (std::getline(input, line)) {
      format_line(line);
      std::istringstream iss(line);
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

    int index = h(item->get_key());

    std::cout << "[insert INFO] Index = " << index << std::endl;
    data[index].push_back(std::move(item));

    std::cout << "[insert INFO] Inserted successfully" << std::endl;
  }

  Item<K, V>* search(K key) {
    int index = h(key);

    if (data[index].empty()) {
      std::cerr << "[search ERROR] List " << index << " empty" << std::endl;
      return nullptr;
    }

    for (auto& item : data[index]) {
      if (item->get_key() == key) {
        std::cout << "[search INFO] Item ";
        item->print();
        std::cout << " found in list " << index << std::endl;

        return item.get();
      }
    }

    std::cerr << "[search ERROR] Item " << key << " not found" << std::endl;
    return nullptr;
  }

  void delete_item(K key) {
    int index = h(key);

    if (data[index].empty()) {
      std::cerr << "[delete_item ERROR] List " << index << " empty" << std::endl;
      return;
    }

    for (auto it = data[index].begin(); it != data[index].end(); ++it) {
      if ((*it)->get_key() == key) {
        std::cout << "[delete_item INFO] Item ";
        (*it)->print();
        std::cout << " found in list " << index << std::endl;

        data[index].erase(it);

        std::cout << "[delete_item INFO] Deleted successfully item " << key << " from list " << index << std::endl;
        return;
      }
    }

    std::cerr << "[delete_item ERROR] Item " << key << " not found" << std::endl;
  }

  void print(std::string message = "Hash table", std::ostream& out = std::cout) {
    out << message << std::endl;

    for (int i = 0; i < size; i++) {
      out << "List " << i << " => ";
      if (data[i].empty())
        out << "empty";
      else {
        bool first = true;

        for (auto& item : data[i]) {
          if (first) {
            item->print(out);
            first = false;
          } else {
            out << " -> ";
            item->print(out);
          }
        }
      }
      out << std::endl;
    }

    out << std::endl;
  }
};

#endif