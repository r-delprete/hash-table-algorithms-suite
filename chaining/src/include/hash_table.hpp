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

  const int h(const K key) const {
    if (fn == Hashing::divide) return key % size;

    const double A = (sqrt(5) - 1) / 2;
    const double prod = A * key;
    const double frac = prod - floor(prod);
    return floor(frac * size);
  }

public:
  HashTable(const int size, std::ifstream& input, Hashing fn = Hashing::divide) : size(size), fn(fn) {
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
      std::istringstream stream(line);

      K key;
      V value;
      stream >> key >> value;
      insert(item_factory_shared(key, value));

      line.clear();
      clear_stream(stream);
    }
  }

  void insert(const shared_item_ptr<K, V>& item) {
    const int index = h(item->get_key());
    data[index].push_back(item);
  }

  shared_item_ptr<K, V> search(const K key) {
    const int index = h(key);

    if (data[index].empty()) {
      std::cerr << "[search ERROR] List " << index << " is empty" << std::endl;
      return nullptr;
    }

    for (auto& item : data[index]) {
      if (item->get_key() == key) return item;
    }

    std::cerr << "[search ERROR] Item with key " << key << " not found" << std::endl;
    return nullptr;
  }

  void delete_item(const K key) {
    const int index = h(key);

    if (data[index].empty()) {
      std::cerr << "[delete_item ERROR] List " << index << " is empty" << std::endl;
      return;
    }

    for (auto it = data[index].begin(); it != data[index].end(); ++it) {
      std::cout << "[delete_item INFO] Deleting item ";
      (*it)->print();
      std::cout << std::endl;

      data[index].erase(it);

      std::cout << "[delete_item INFO] Item deleted successfully" << std::endl;
      return;
    }

    std::cerr << "[delete_item ERROR] Item with key " << key << " not found" << std::endl;
  }

  void print(std::ostream& out = std::cout, const std::string message = "Hash table") {
    out << message << std::endl;

    for (int i = 0; i < size; i++) {
      out << "List " << i + 1 << " => ";
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