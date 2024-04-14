#ifndef S21_CONTAINERS_SRC_SOURCE_S21_STACK_H_
#define S21_CONTAINERS_SRC_SOURCE_S21_STACK_H_

#include <cstddef>
#include <cstdio>
#include <initializer_list>
#include <iostream>
#include <limits>

#include "s21_list.h"
#include "s21_vector.h"

namespace s21 {

template <typename T, typename stack_list = s21::vector<T>>
class stack {
 public:
  using value_type = typename stack_list::value_type;
  using reference = typename stack_list::reference;
  using const_reference = typename stack_list::const_reference;
  using size_type = typename stack_list::size_type;

  // Stack Member functions
  stack() : data_() {}  // создает пустой стек

  stack(std::initializer_list<value_type> const &items)
      : data_(items) {}  // передать значения стека во время создания

  stack(const stack &s) : data_(s.data_) {}  // конструктор копирования

  stack(stack &&s) : data_(std::move(s.data_)) {}  // конструктор перемещения

  stack &operator=(stack &s) {
    data_ = s.data_;
    return *this;
  }  // оператор присваивания для перемещения

  stack &operator=(stack &&s) {
    data_ = std::move(s.data_);
    return *this;
  }  // оператор присваивания для перемещения

  ~stack() { data_.clear(); }  // деструктор

  // Stack Element access
  const_reference top() { return data_.back(); }  // доступ к верхнему элементу

  // Stack Capacity
  bool empty() { return data_.empty(); }  // проверяет, пуст ли контейнер
  size_type size() { return data_.size(); }  // возвращает количество элементов

  // Stack Modifiers
  void push(const_reference value) {
    data_.push_back(value);
  }  // вставляет элемент вверх
  void pop() { data_.pop_back(); }  // удаляет верхний элемент
  void swap(stack &other) {
    data_.swap(other.data_);
  }  // меняет местами 2 стека

 private:
  stack_list data_;
};

}  // namespace s21

#endif  // S21_CONTAINERS_SRC_SOURCE_S21_STACK_H_
