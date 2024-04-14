#ifndef S21_CONTAINERS_SRC_SOURCE_S21_LIST_H_
#define S21_CONTAINERS_SRC_SOURCE_S21_LIST_H_

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <limits>
#include <utility>

namespace s21 {

template <typename T>
struct Node {
  T value_ = T();
  Node* next_ = nullptr;
  Node* prev_ = nullptr;
  explicit Node(T value = T(), Node* prev = nullptr, Node* next = nullptr)
      : value_(value), next_(next), prev_(prev) {}
};

template <typename T, bool Const = false>
class ListIterator;

template <typename T>
class list {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using iterator = ListIterator<T, false>;
  using const_iterator = ListIterator<T, true>;
  using size_type = size_t;

  list() { end_ = new Node(value_type()); }
  list(size_type n) : list() {
    for (size_type i = 0; i < n; i += 1) {
      push_back(value_type());
    }
  }

  list(std::initializer_list<value_type> const& items) : list() {
    for (const auto& i : items) {
      push_back(i);
    }
  }

  list(const list& l) : list() { operator=(l); }

  list(list&& l) : list() {
    if (this != &l) {
      clear();
      size_ = l.size_;
      begin_ = l.begin_;
      end_->prev_ = l.end_->prev_;
      if (end_->prev_ != nullptr) {
        end_->prev_->next_ = end_;
      }
      l.size_ = 0;
      l.end_->prev_ = nullptr;
    }
  }

  list& operator=(const list& l) {
    if (this != &l) {
      clear();
      Node<value_type>* buf = l.begin_;
      while (buf != l.end_) {
        push_back(buf->value_);
        buf = buf->next_;
      }
    }
    return *this;
  }

  list& operator=(list&& l) {
    if (this != &l) {
      clear();
      size_ = l.size_;
      begin_ = l.begin_;
      end_->prev_ = l.end_->prev_;
      if (end_->prev_ != nullptr) {
        end_->prev_->next_ = end_;
      }
      l.size_ = 0;
      l.end_->prev_ = nullptr;
    }
    return *this;
  }

  ~list() {
    clear();
    delete end_;
  }

  const_reference front() {
    if (begin_ == nullptr) {
      throw "Collection is empty";
    }
    return (*const_iterator(begin_));
  }

  const_reference back() {
    if (begin_ == nullptr) {
      throw "Collection is empty";
    }
    return (*const_iterator(end_->prev_));
  }

  iterator begin() {
    if (empty()) {
      return iterator(end_);
    } else {
      return iterator(begin_);
    }
  }

  iterator end() { return iterator(end_); }

  const_iterator cbegin() {
    if (empty()) {
      return const_iterator(end_);
    } else {
      return const_iterator(begin_);
    }
  }

  const_iterator cend() const { return const_iterator(end_); }
  bool empty() const noexcept { return size_ == 0; }
  size_type size() const { return size_; }
  size_type max_size() const noexcept {
    return (std::numeric_limits<size_type>::max() / sizeof(Node<T>) / 2);
  }

  void clear() {
    while (!empty()) {
      pop_back();
    }
    end_->prev_ = nullptr;
  }

  iterator insert(iterator pos, const_reference value) {
    if (size_ + 1 > max_size()) throw "Maximum of container";
    iterator res;
    if (pos == end()) {
      if (end_->prev_ == nullptr) {
        begin_ = new Node<value_type>(value, nullptr, end_);
        end_->prev_ = begin_;
        res = iterator(begin_);
      } else {
        Node<value_type>* buf = end_->prev_;
        end_->prev_ = new Node<value_type>(value, end_->prev_, end_);
        buf->next_ = end_->prev_;
        res = iterator(end_->prev_);
      }
    } else if (pos == begin()) {
      begin_ = new Node<value_type>(value, nullptr, pos.node_);
      pos.node_->prev_ = begin_;
      res = begin_;
    } else {
      pos.node_->prev_->next_ =
          new Node<value_type>(value, pos.node_->prev_, pos.node_);
      pos.node_->prev_ = pos.node_->prev_->next_;
      res = iterator(pos.node_->prev_);
    }
    size_ += 1;
    return res;
  }

  void erase(iterator pos) {
    if (size_ != 0 && pos.node_ != nullptr && pos != end_) {
      if (pos == begin()) {
        begin_ = begin_->next_;
        delete begin_->prev_;
        begin_->prev_ = nullptr;
      } else {
        pos.node_->prev_->next_ = pos.node_->next_;
        pos.node_->next_->prev_ = pos.node_->prev_;
        delete pos.node_;
      }
      size_ -= 1;
    }
  }

  void push_back(const_reference value) { insert(iterator(end_), value); }
  void pop_back() {
    if (!empty()) {
      erase(iterator(end_->prev_));
    }
  }
  void push_front(const_reference value) {
    if (empty()) {
      push_back(value);
    } else {
      insert(begin(), value);
    }
  }

  void pop_front() { erase(iterator(begin_)); }

  void swap(list& other) {
    std::swap(size_, other.size_);
    std::swap(begin_, other.begin_);
    std::swap(end_, other.end_);
  }

  void merge(list& other) {
    if (size_ + other.size_ > max_size()) throw "Maximum of container";
    auto merged = list(size_ + other.size_);
    auto firstIter = begin();
    auto secondIter = other.begin();
    for (auto iter = merged.begin(); iter != merged.end(); ++iter) {
      if (firstIter == end()) {
        *iter = *secondIter;
        ++secondIter;
      } else {
        if (secondIter == other.end()) {
          *iter = *firstIter;
          ++firstIter;
        } else {
          if (*firstIter < *secondIter) {
            *iter = *firstIter;
            ++firstIter;
          } else {
            *iter = *secondIter;
            ++secondIter;
          }
        }
      }
    }
    clear();
    other.clear();
    *this = std::move(merged);
  }

  void splice(const_iterator pos, list& other) {
    if (size_ + other.size_ > max_size()) throw "Maximum of container";
    for (auto i = (other.begin()); i != other.end(); ++i) {
      insert(iterator(pos.node_), *i);
    }
    other.clear();
  }

  void reverse() {
    auto leftIter = begin();
    auto rigthIter = iterator(end_->prev_);
    for (size_type i = 0; i < size_ / 2; i += 1) {
      auto buf = *leftIter;
      *leftIter = *rigthIter;
      *rigthIter = buf;
      ++leftIter;
      --rigthIter;
    }
  }

  void unique() {
    iterator i = begin();
    auto check = *i;
    ++i;
    while (i != end()) {
      if (*i == check) {
        erase(i++);
      } else {
        check = *i;
        ++i;
      }
    }
  }

  void sort() {
    iterator middle = GetMiddleList();
    list<value_type> temp;
    for (auto it = temp.begin(); middle != end();) {
      iterator temp_it = middle;
      temp.insert(it, *middle);
      ++middle;
      erase(temp_it);
      ++it;
    }
    if (size() != 1) {
      sort();
    }
    if (temp.size() != 1) {
      temp.sort();
    }
    merge(temp);
  }

 private:
  iterator GetMiddleList() {
    auto it_fast = begin();
    auto it_slow = begin();
    auto it_fake = end();
    for (; it_fast != it_fake && ++it_fast != it_fake;) {
      ++it_slow;
      ++it_fast;
    }
    return it_slow;
  }

  Node<value_type>*begin_ = nullptr, *end_ = nullptr;
  size_type size_ = 0;
};

template <typename T, bool Constness>
class ListIterator {
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = T;
  using reference = typename std::conditional_t<Constness, T const&, T&>;

 public:
  friend class list<value_type>;

  ListIterator() {}

  //  ListIterator(list<T> list) { node_ = list.begin_; }

  ListIterator(Node<value_type>* node) { node_ = node; }

  ListIterator& operator++() {
    if (node_->next_ != nullptr) {
      node_ = node_->next_;
    }
    return *this;
  }

  ListIterator operator++(int) {
    ListIterator tmp_iter = *this;
    if (node_->next_ != nullptr) {
      node_ = node_->next_;
    }
    return tmp_iter;
  }

  ListIterator& operator--() {
    if (node_->prev_ != nullptr) {
      node_ = node_->prev_;
    }
    return *this;
  }

  template <bool Const = Constness>
  std::enable_if_t<Const, reference> operator*() const {
    return node_->value_;
  }

  template <bool Const = Constness>
  std::enable_if_t<!Const, reference> operator*() {
    return node_->value_;
  }

  bool operator==(const ListIterator& other) const {
    bool res = (this->node_ == other.node_) ? true : false;
    return res;
  }

  bool operator!=(const ListIterator& other) const {
    bool res = (this->node_ != other.node_) ? true : false;
    return res;
  }

 private:
  Node<value_type>* node_;
};

}  // namespace s21

#endif  // S21_CONTAINERS_SRC_SOURCE_S21_LIST_H_
