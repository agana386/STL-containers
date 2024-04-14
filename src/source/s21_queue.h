#ifndef S21_CONTAINERS_SRC_SOURCE_S21_QUEUE_H_
#define S21_CONTAINERS_SRC_SOURCE_S21_QUEUE_H_

#include <cstddef>
#include <cstdio>
#include <initializer_list>
#include <iostream>
#include <limits>

#include "s21_list.h"

namespace s21 {

template <typename T, typename queue_list = s21::list<T>>
class queue {
 public:
  using value_type = typename queue_list::value_type;
  using reference = typename queue_list::reference;
  using const_reference = typename queue_list::const_reference;
  using size_type = typename queue_list::size_type;

  // Queue Member functions
  queue() : data_() {}  // создает пустую очередь

  queue(std::initializer_list<value_type> const &items)
      : data_(items) {}  // передать значения очереди во время создания

  queue(const queue &q) : data_(q.data_) {}  // конструктор копирования

  queue(queue &&q) : data_(std::move(q.data_)) {}  // конструктор перемещения

  queue &operator=(queue &q) {
    data_ = q.data_;
    return *this;
  }  // оператор присваивания для копирования

  queue &operator=(queue &&q) {
    data_ = std::move(q.data_);
    return *this;
  }  // оператор присваивания для перемещения

  ~queue() { data_.clear(); }  // деструктор

  // Queue Element access
  const_reference front() {
    return data_.front();
  }  // доступ к первому элементу
  const_reference back() { return data_.back(); }
  // доступ к последнему элементу

  // Queue Capacity
  bool empty() { return data_.empty(); }  // проверяет, пуст ли контейнер
  size_type size() { return data_.size(); }  // возвращает количество элементов

  // Queue Modifiers
  void push(const_reference value) {
    data_.push_back(value);
  }  // вставляет элемент в конец
  void pop() { data_.pop_front(); }  // удаляет первый элемент
  void swap(queue &other) {
    data_.swap(other.data_);
  }  // меняет местами 2 очереди

 private:
  queue_list data_;
};

}  // namespace s21

#endif  // S21_CONTAINERS_SRC_SOURCE_S21_QUEUE_H_
