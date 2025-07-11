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
  std::vector<std::list<shared_item_ptr<K, V>>> data;
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

  int h(K key) {
    if (fn == Hashing::divide) return key % size;

    double A = (sqrt(5) - 1) / 2;
    double prod = A * key;
    double frac = prod - floor(prod);
    return floor(frac * size);
  }

public:
  HashTable(int size, std::ifstream& input, Hashing fn = Hashing::divide) : size(size), fn(fn) {
    data.resize(size);
    load(input);
  }

  void reset() {
    for (auto& list : data) {
      for (auto& item : list) item.reset();
    }
  }

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
      insert(make_shared_item(key, value));

      clear_stream(iss);
      line.clear();
    }
  }

  void insert(shared_item_ptr<K, V> item) {
    int index = h(item->get_key());
    data[index].push_back(item);
  }

  shared_item_ptr<K, V> search(K key) {
    int index = h(key);

    if (data[index].empty()) {
      std::cerr << "[search ERROR] List " << index << " is empty" << std::endl;
      return nullptr;
    }

    for (auto& item : data[index]) {
      if (item->get_key() == key) {
        std::cout << "[search INFO] Item ";
        item->print();
        std::cout << " found in list " << index << std::endl;

        return item;
      }
    }

    std::cerr << "[search ERROR] Item " << key << " not found" << std::endl;
    return nullptr;
  }

  void delete_item(K key) {
    int index = h(key);

    if (data[index].empty()) {
      std::cerr << "[delete_item ERROR] List " << index << " is empty" << std::endl;
      return;
    }

    for (auto it = data[index].begin(); it != data[index].end(); ++it) {
      if ((*it)->get_key() == key) {
        data[index].erase(it);
        std::cout << "[delete_item INFO] Item with key " << key << " deleted successfully" << std::endl;

        return;
      }
    }

    std::cerr << "[delete_item ERROR] Item " << key << " not found" << std::endl;
  }

  void print(std::ostream& out = std::cout, std::string message = "Hash table") {
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
  }
};

#endif