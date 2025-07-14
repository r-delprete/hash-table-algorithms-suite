#ifndef ITEM_HPP
#define ITEM_HPP

#include <iostream>
#include <memory>

template <typename K, typename V>
class Item {
  K key;
  V value;

public:
  Item(const K key, const V value) : key(key), value(value) {}

  int get_key() const { return key; }
  int get_value() const { return value; }

  void set_key(const K key) { this->key = key; }
  void set_value(const V value) { this->value = value; }

  void print(std::ostream& out = std::cout) { out << "[" << key << " - " << value << "]"; }
};

template <typename K, typename V>
using shared_item = std::shared_ptr<Item<K, V>>;

template <typename K, typename V>
shared_item<K, V> create_item(const K key, const V value) {
  return std::make_shared<Item<K, V>>(key, value);
}

#endif  // ITEM_HPP