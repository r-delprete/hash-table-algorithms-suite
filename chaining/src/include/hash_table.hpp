#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cmath>
#include <fstream>
#include <list>
#include <sstream>
#include <string>
#include <vector>

#include "item.hpp"

enum Hashing { multiply, divide };

template <typename K, typename V>
class HashTable {
  std::vector<std::list<Item<K, V>*>> data;
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
    switch (fn) {
      case Hashing::multiply: {
        double A = (sqrt(5) - 1) / 2;
        double frac = fmod(key * A, 1.0);

        return frac * size;
      }

      default:
        return key % size;
    }
  }

public:
  HashTable(int size, std::ifstream& input, Hashing fn = Hashing::divide) : size(size), fn(fn) {
    data.resize(size);
    load(input);
  }

  void clear() {
    for (auto& list : data) {
      for (auto& item : list) {
        delete item;
        item = nullptr;
      }

      list.clear();
    }
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
      insert(new Item<K, V>(key, value));

      clear_stream(iss);
    }
  }

  void insert(Item<K, V>* item) {
    int index = h(item->get_key());
    data[index].push_back(item);
  }

  Item<K, V>* search(K key) {
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

    std::cerr << "[search ERROR] Item " << key << " not found";
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
        std::cout << "[delete_item INFO] Item ";
        (*it)->print();
        std::cout << " found in list " << index << std::endl;

        delete *it;
        data[index].erase(it);

        std::cout << "[delete_item INFO] Item " << key << " deleted successfully" << std::endl;
        return;
      }
    }

    std::cerr << "[delete_item ERROR] Item " << key << " not found";
  }

  void print(std::string message = "Hash table", std::ostream& out = std::cout) {
    out << message << std::endl;

    for (int i = 0; i < data.size(); i++) {
      out << "List " << i;
      if (data[i].empty())
        out << " => empty";
      else {
        bool first = true;
        for (auto& item : data[i]) {
          if (first) {
            out << " => ";
            first = false;
          } else {
            out << " -> ";
          }
          item->print(out);
        }
      }
      out << std::endl;
    }

    out << std::endl;
  }
};

#endif