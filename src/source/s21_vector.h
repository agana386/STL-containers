#ifndef S21_CONTAINERS_SRC_SOURCE_S21_VECTOR_H_
#define S21_CONTAINERS_SRC_SOURCE_S21_VECTOR_H_

#include <initializer_list>
#include <iostream>
#include <limits>

namespace s21 {
template <typename T>
class vector {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using iterator = T *;
  using const_iterator = const T *;
  using size_type = size_t;

  // Vector Member functions
 public:
  vector() : data_(nullptr), size_(0), capacity_(0) {}  // создает пустой вектор

  explicit vector(size_type n)
      : data_(new value_type[n]), size_(n), capacity_(n) {
    std::fill_n(data_, n, value_type());
  }  // создает вектор размера n

  vector(std::initializer_list<value_type> const &items)
      : data_(new value_type[items.size()]),
        size_(items.size()),
        capacity_(items.size()) {
    std::copy(items.begin(), items.end(), data_);
  }  // передать несколько значений вектору во время создания

  vector(const vector &v)
      : data_(new value_type[v.capacity_]),
        size_(v.size_),
        capacity_(v.capacity_) {
    std::copy(v.data_, v.data_ + v.size_, data_);
  }  // конструктор копирования

  vector(vector &&v) noexcept {
    size_ = v.size_;
    v.size_ = 0;
    capacity_ = v.capacity_;
    v.capacity_ = 0;
    data_ = v.data_;
    v.data_ = nullptr;
  }  // конструктор перемещения

  vector<T> &operator=(vector<T> &v) {
    data_ = new value_type[v.capacity_];
    size_ = v.size_;
    capacity_ = v.capacity_;
    std::copy(v.data_, v.data_ + v.size_, data_);
    return *this;
  }  // оператор присваивания для копирования

  vector<T> &operator=(vector<T> &&v) noexcept {
    if (this != &v) {
      swap(v);
    }
    return *this;
  }  // оператор присваивания для перемещения

  ~vector() {
    delete[] data_;
    size_ = 0;
    capacity_ = 0;
    data_ = nullptr;
  }  // деструктор

  // Vector Element access
  reference at(size_type pos) {
    if (pos >= size_) {
      throw std::out_of_range("AtError: Index out of range");
    }
    return data_[pos];
  }  // Доступ к указанному элементу с проверкой границ

  reference operator[](size_type pos) {
    if (pos >= size_) {
      throw std::out_of_range("IndexError: Index out of range");
    }
    return data_[pos];
  }  // Доступ к указанному элементу

  const_reference front() const {
    if (!size_) {
      throw std::out_of_range("FrontError: vector is empty");
    }
    return data_[0];
  }  // Доступ к первому элементу

  const_reference back() const {
    if (!size_) {
      throw std::out_of_range("BackError: vector is empty");
    }
    return data_[size_ - 1];
  }  // Доступ к последнему элементу

  iterator data() noexcept {
    return data_;
  }  // Прямой доступ к базовому массиву

  // Vector Iterators
  iterator begin() { return iterator(data_); }  // возвращает итератор в начало

  iterator end() {
    return iterator(data_ + size_);
  }  // возвращает итератор в конец

  // Vector Capacity
  bool empty() const { return size_ == 0; }  // проверяет, пуст ли контейнер

  size_type size() const { return size_; }  // возвращает количество элементов

  size_type max_size() const {
    return std::numeric_limits<std::size_t>::max() / sizeof(value_type);
  }  // возвращает максимально возможное количество элементов

  void reserve(size_type new_capacity) {
    if (new_capacity <= capacity_) {
      return;
    }
    if (new_capacity > max_size()) {
      throw std::out_of_range(
          "ReserveError: Too large size for a new capacity");
    }
    iterator new_data = new value_type[new_capacity];
    std::copy(data_, data_ + size_, new_data);
    delete[] data_;
    data_ = new_data;
    capacity_ = new_capacity;
  }  // копирует текущие элементы массива в новый выделенный массив

  size_type capacity() const {
    return capacity_;
  }  // количество элементов, которые могут храниться в выделенном в данный
     // момент хранилище

  void shrink_to_fit() {
    if (size_ < capacity_) {
      iterator new_data = new value_type[size_];
      std::copy(data_, data_ + size_, new_data);
      delete[] data_;
      data_ = new_data;
      capacity_ = size_;
    }
  }  // освобождение неиспользуемой памяти

  // Vector Modifiers
  void clear() {
    if (data_) {
      delete[] data_;
      data_ = nullptr;
    }
    size_ = 0;
  }  // очистка

  iterator insert(iterator pos, const_reference value) {
    size_type tmp = pos - begin();
    if (tmp > size_) {
      throw std::out_of_range(
          "InsertError: The insertion position is out of range of the vector "
          "memory");
    }
    if (size_ == capacity_) {
      reserve(capacity_ ? capacity_ * 2 : 1);
    }
    iterator new_pos = begin() + tmp;
    for (iterator it = end(); it != new_pos; --it) {
      *it = *(it - 1);
    }
    *new_pos = value;
    ++size_;
    return new_pos;
  }  // Вставляет элементы в конкретный POS и возвращает итератор, указывающий
     // на новый элемент

  void erase(iterator pos) {
    size_type position = pos - data_;
    if (position > size_) {
      throw std::out_of_range("EraseError: Index out of range");
    }
    for (iterator it = pos; it != end() - 1; ++it) {
      *it = *(it + 1);
    }
    --size_;
  }  // стирает элемент в pos

  void push_back(value_type value) {
    if (size_ >= capacity_) {
      if (capacity_ == 0) {
        reserve(1);
      } else {
        reserve(capacity_ * 2);
      }
    }
    data_[size_] = value;
    size_ += 1;
  }  // добавляет элемент в конец

  void pop_back() {
    if (size_ > 0) {
      --size_;
    }
  }  // удаляет последний элемент

  void swap(vector<T> &other) {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }  // меняет содержимое 2х векторов местами

 private:
  iterator data_;  //= nullptr;
  size_type size_;
  size_type capacity_;
};
}  // namespace s21

#endif  // S21_CONTAINERS_SOURCE_S21_VECTOR_H_