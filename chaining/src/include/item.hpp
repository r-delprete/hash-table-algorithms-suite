#ifndef ITEM_HPP
#define ITEM_HPP

#include <iostream>

template <typename K, typename V>
class Item {
private:
  K key;
  V value;

public:
  Item(K key, V value) : key(key), value(value) {}

  const K get_key() const { return key; }
  const V get_value() const { return value; }

  void set_key(K key) { this->key = key; }
  void set_value(V value) { this->value = value; }

  void print(std::ostream& out = std::cout) { out << "[" << key << " - " << value << "]"; }
};

#endif  // ITEM_HPP