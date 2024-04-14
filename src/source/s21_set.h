#ifndef S21_CONTAINERS_SRC_SOURCE_S21_SET_H_
#define S21_CONTAINERS_SRC_SOURCE_S21_SET_H_

#include "s21_tree.h"

namespace s21 {

template <typename Key>
class set {
 public:
  using key_type = Key;
  using value_type = Key;
  using reference = key_type &;
  using const_reference = const key_type &;
  using tree_type = LLRBTree<key_type, std::less<key_type>>;
  using iterator = typename tree_type::iterator;
  using const_iterator = typename tree_type::const_iterator;
  using size_type = std::size_t;

  // Default constructor
  set() : tree_ptr_(new tree_type()) {}

  // Initializer list constructor
  set(std::initializer_list<value_type> const &items) : set() {
    for (auto item : items) {
      insert(item);
    }
  }

  // Copy constructor
  set(const set &s) : tree_ptr_(new tree_type(*s.tree_ptr_)) {}

  // Move constructor
  set(set &&s) { tree_ptr_ = std::move(s.tree_ptr_); }

  // Move assignment operator
  set &operator=(set &&s) noexcept {
    tree_ptr_ = std::move(s.tree_ptr_);
    return *this;
  }

  // Copy assignment operator
  set &operator=(const set &s) {
    *tree_ptr_ = *s.tree_ptr_;
    return *this;
  }

  // Destructor
  ~set(){};

  // Returns an iterator to the beginning
  iterator begin() noexcept { return tree_ptr_->begin(); }

  // Returns an iterator to the beginning
  const_iterator cbegin() const noexcept { return tree_ptr_->cbegin(); }

  // Returns an iterator to the end
  iterator end() noexcept { return tree_ptr_->end(); }

  // Returns an iterator to the end
  const_iterator cend() const noexcept { return tree_ptr_->cend(); }

  // Finds an element with a specific key
  iterator find(const key_type &key) noexcept {
    return tree_ptr_->Find(key);
  }  // finds an element with a key equivalent to key

  // Checks if the container contains an element with a specific key
  bool contains(const key_type &key) const noexcept {
    return tree_ptr_->Contains(key);
  }

  // Returns the number of elements
  size_type size() const noexcept {
    return tree_ptr_->Size();
  }  // returns the number of elements

  // Checks whether the container is empty
  bool empty() const noexcept {
    return tree_ptr_->Empty();
  }  // returns true if the container is empty, returns false if not empty

  // Returns the maximum possible number of elements
  size_type max_size() const noexcept {
    return tree_ptr_->MaxSize();
  }  // returns the maximum number of elements that can be

  // Clears the contents
  void clear() noexcept { tree_ptr_->Clear(); }

  // Inserts a node and returns an iterator to where the element is in the
  // container and bool denoting whether the insertion took place
  std::pair<iterator, bool> insert(const value_type &value) {
    return tree_ptr_->Insert(value);
  }

  // Erases an element at pos
  void erase(iterator pos) noexcept { tree_ptr_->Erase(pos); }

  // Swaps the contents
  void swap(set &other) noexcept { tree_ptr_->Swap(*other.tree_ptr_); }

  // Splices nodes from another container
  void merge(set &other) noexcept { tree_ptr_->Merge(*other.tree_ptr_); }

 private:
  tree_type *tree_ptr_;
};
}  // namespace s21

#endif  // S21_CONTAINERS_SRC_SOURCE_S21_SET_H_