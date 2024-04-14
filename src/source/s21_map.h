#ifndef S21_CONTAINERS_SRC_SOURCE_S21_MAP_H_
#define S21_CONTAINERS_SRC_SOURCE_S21_MAP_H_

#include "s21_tree.h"

namespace s21 {
template <typename Key, typename Type>
class map {
 public:
  using key_type = Key;                        // type of element - key
  using mapped_type = Type;                    // value of element
  using value_type = std::pair<Key, Type>;     // pair: key-value
  using reference = value_type &;              // reference to element
  using const_reference = const value_type &;  // const reference to element
  using size_type = std::size_t;               // type for container's size

  // Inner classes
  using tree_type = LLRBTree<value_type, MapComparator<value_type>>;
  using iterator = typename tree_type::iterator;
  using const_iterator = typename tree_type::const_iterator;

  // Default constructor
  map() : tree_ptr_(new tree_type()) {}

  // Initializer list constructor
  map(std::initializer_list<value_type> const &items) : map() {
    for (auto item : items) {
      insert(item);
    }
  }

  // Copy constructor
  map(const map &m) : tree_ptr_(new tree_type(*m.tree_ptr_)) {}

  // Move constructor
  map(map &&m) noexcept { tree_ptr_ = std::move(m.tree_ptr_); }

  // Copy assignment operator
  map *operator=(const map &m) {
    *tree_ptr_ = *m.tree_ptr_;
    return *this;
  }

  // Move assignment operator
  map &operator=(map &&other) noexcept {
    tree_ptr_ = std::move(other.tree_ptr_);
    return *this;
  };

  // Destructor
  ~map() {
    tree_ptr_->Clear();
    delete tree_ptr_->null_node_;
    tree_ptr_->null_node_ = nullptr;
  }

  // Access specified element with bounds checking (public member function)
  mapped_type &at(const key_type &key) {
    value_type search_pair(key, mapped_type{});
    iterator search_iter = tree_ptr_->FindMap(search_pair);
    if (search_iter == end()) {
      throw std::out_of_range("No elements with key");
    }
    return (*search_iter).second;
  }  // Returns a reference to the value with the key. If there is no such
     // element, an exception is thrown

  // Const at()
  const mapped_type &at(const key_type *key) const {
    return const_cast<map<Key, Type> *>(this)->at(key);
  }

  // Access or insert specified element (public member function)
  mapped_type &operator[](const key_type &key) {
    value_type search_pair(key, mapped_type{});
    iterator search_iter = tree_ptr_->FindMap(search_pair);

    if (search_iter == end()) {
      std::pair<iterator, bool> res = tree_ptr_->Insert(search_pair);
      return (*res.first).second;
    } else
      return (*search_iter).second;
  }  // Returns a reference to the value with the key key. If there is no such
     // element, then insertion is performed

  // Returns an iterator to the beginning
  iterator begin() noexcept { return tree_ptr_->begin(); }

  // Returns an iterator to the beginning for const
  const_iterator cbegin() const noexcept { return tree_ptr_->cbegin(); }

  // Returns iterator to the end
  iterator end() noexcept { return tree_ptr_->end(); }

  // Returns iterator to the end for const
  const_iterator cend() const noexcept { return tree_ptr_->cend(); }

  // Returns the number of elements
  size_type size() const noexcept { return tree_ptr_->tree_size_; }

  // Returns true if the container is empty, returns false if not empty
  bool empty() const noexcept { return tree_ptr_->Empty(); }

  // Returns the maximum number of elements that can be
  size_type max_size() const noexcept { return tree_ptr_->MaxSize(); }

  void clear() noexcept { tree_ptr_->Clear(); }

  // Removes element at position
  void erase(iterator pos) noexcept { tree_ptr_->Erase(pos); }

  // Exchanges contents with "other"
  void swap(map &other) noexcept { tree_ptr_->Swap(*other.tree_ptr_); }

  // Take from "other" and insert into container. If the element exists, insert
  // operation doesn't work
  void merge(map &other) noexcept { tree_ptr_->Merge(*other.tree_ptr_); }

  // Check for element with key
  bool contains(const key_type &key) {
    bool ret = false;
    value_type search_pair(key, mapped_type{});
    iterator search_iter = tree_ptr_->FindMap(search_pair);
    if (search_iter != end()) {
      ret = true;
    }
    return ret;
  }

  // Insert element with value into container if the key doesn't exist
  std::pair<iterator, bool> insert(const value_type &value) {
    return tree_ptr_->InsertMap(value);
  }

  // Insert element with value key и obj into container if the key doesn't exist
  std::pair<iterator, bool> insert(const key_type &key,
                                   const mapped_type &obj) {
    return tree_ptr_->InsertMap(value_type{key, obj});
  }

  // Searches for an element by key and inserts it, if not found, inserts a new
  // value
  std::pair<iterator, bool> insert_or_assign(const key_type &key,
                                             const mapped_type &obj) {
    iterator res = tree_ptr_->FindMap(value_type{key, obj});
    if (res == end()) return tree_ptr_->InsertMap(value_type{key, obj});
    (*res).second = obj;

    return {res, false};
  }

 private:
  tree_type *tree_ptr_;
};

}  // namespace s21

#endif  // S21_CONTAINERS_SRC_SOURCE_S21_MAP_H_
