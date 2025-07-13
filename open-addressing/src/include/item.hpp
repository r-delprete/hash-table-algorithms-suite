#ifndef ITEM_HPP
#define ITEM_HPP

#include <iostream>
#include <memory>

template <typename K, typename V>
class Item {
private:
  K key;
  V value;

public:
  Item(const K key, const V value) : key(key), value(value) {}

  const K get_key() const { return key; }
  const V get_value() const { return value; }

  void set_key(const K key) { this->key = key; }
  void set_value(const V value) { this->value = value; }

  void print(std::ostream& out = std::cout) { out << "[" << key << " - " << value << "]"; }
};

template <typename K, typename V>
using shared_item_ptr = std::shared_ptr<Item<K, V>>;

template <typename K, typename V>
shared_item_ptr<K, V> item_factory_shared(const K key, const V value) {
  return std::make_shared<Item<K, V>>(key, value);
}

#endif  // ITEM_HPP