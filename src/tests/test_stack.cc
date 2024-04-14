#include <gtest/gtest.h>

#include <stack>

#include "../s21_containers.h"

TEST(StackMemberFunctions, TestDefault) {
  s21::stack<int> s21_stack;
  std::stack<int> std_stack;
  EXPECT_EQ(s21_stack.size(), std_stack.size());
  EXPECT_EQ(s21_stack.empty(), std_stack.empty());
}

TEST(StackMemberFunctions, TestInitializer) {
  s21::stack<int> s21_stack = {1, 2, 3, 4};
  EXPECT_EQ(s21_stack.top(), 4);
  EXPECT_EQ(s21_stack.size(), size_t(4));
  EXPECT_EQ(s21_stack.empty(), false);
}

TEST(StackMemberFunctions, TestConstructorCopy) {
  s21::stack<char> s21_stack_1 = {'a', 'b', 'c', 'd'};
  s21::stack<char> s21_stack_2 = s21_stack_1;

  EXPECT_EQ(s21_stack_1.size(), s21_stack_2.size());

  for (auto i = 0; i < 4; i++) {
    EXPECT_EQ(s21_stack_1.top(), s21_stack_2.top());
    EXPECT_EQ(s21_stack_1.size(), s21_stack_2.size());
    s21_stack_1.pop(), s21_stack_2.pop();
  }
}

TEST(StackMemberFunctions, TestConstructorMove) {
  s21::stack<char> s21_stack_1 = {'a', 'b', 'c', 'd'};
  s21::stack<char> s21_stack_2 = std::move(s21_stack_1);
  s21::stack<char> s21_stack_3 = {'a', 'b', 'c', 'd'};

  EXPECT_EQ(s21_stack_2.size(), s21_stack_3.size());
  EXPECT_EQ(s21_stack_1.size(), size_t(0));
  EXPECT_EQ(s21_stack_1.empty(), true);
  for (auto i = 0; i < 4; i++) {
    EXPECT_EQ(s21_stack_3.top(), s21_stack_2.top());
    EXPECT_EQ(s21_stack_3.size(), s21_stack_2.size());
    s21_stack_3.pop(), s21_stack_2.pop();
  }
}

TEST(StackElementAccess, TestPop) {
  std::initializer_list<int> il1 = {4, 5, 6, 7};

  s21::stack<int> s21_stack = {il1};

  std::stack<int> std_stack{il1};

  EXPECT_EQ(s21_stack.top(), std_stack.top());
}

TEST(StackCapacity, TestEmpty) {
  s21::stack<int> s21_stack;
  std::stack<int> std_stack;
  EXPECT_EQ(s21_stack.empty(), std_stack.empty());
  s21_stack.push(123);
  std_stack.push(123);
  EXPECT_EQ(s21_stack.empty(), std_stack.empty());
}

TEST(StackCapacity, TestSize) {
  s21::stack<int> s21_stack;
  std::stack<int> std_stack;

  ASSERT_EQ(s21_stack.size(), std_stack.size());

  s21_stack.push(1);
  std_stack.push(1);
  ASSERT_EQ(s21_stack.size(), std_stack.size());

  s21_stack.push(2);
  std_stack.push(2);
  ASSERT_EQ(s21_stack.size(), std_stack.size());

  s21_stack.pop();
  std_stack.pop();
  ASSERT_EQ(s21_stack.size(), std_stack.size());

  s21_stack.push(3);
  std_stack.push(3);
  ASSERT_EQ(s21_stack.size(), std_stack.size());

  s21_stack.pop();
  std_stack.pop();
  ASSERT_EQ(s21_stack.size(), std_stack.size());

  s21_stack.pop();
  std_stack.pop();
  ASSERT_EQ(s21_stack.size(), std_stack.size());
}

TEST(Stack_Modifiers, TestPush) {
  s21::stack<int> s21_stack;
  std::stack<int> std_stack;
  s21_stack.push(5);
  s21_stack.push(0);
  std_stack.push(5);
  std_stack.push(0);
  EXPECT_EQ(s21_stack.top(), std_stack.top());
  EXPECT_EQ(s21_stack.size(), std_stack.size());
  EXPECT_EQ(s21_stack.empty(), std_stack.empty());
}

TEST(StackModifiers, TestPop) {
  s21::stack<int> s21_stack;
  std::stack<int> std_stack;
  s21_stack.push(5);
  s21_stack.push(0);
  std_stack.push(5);
  std_stack.push(0);
  s21_stack.pop();
  s21_stack.pop();
  std_stack.pop();
  std_stack.pop();
  s21_stack.push(15);
  s21_stack.push(10);
  std_stack.push(15);
  std_stack.push(10);
  s21_stack.pop();
  std_stack.pop();
  EXPECT_EQ(s21_stack.top(), std_stack.top());
  EXPECT_EQ(s21_stack.size(), std_stack.size());
  EXPECT_EQ(s21_stack.empty(), std_stack.empty());
}

TEST(StackModifiers, TestSwap) {
  s21::stack<std::string> s21_stack_1 = {"a", "b", "c", "d"};
  s21::stack<std::string> s21_stack_2 = {"aa", "bb", "cc"};
  s21::stack<std::string> s21_stack_3 = {"a", "b", "c", "d"};
  s21::stack<std::string> s21_stack_4 = {"aa", "bb", "cc"};

  s21_stack_1.swap(s21_stack_2);

  EXPECT_EQ(s21_stack_1.size(), s21_stack_4.size());
  EXPECT_EQ(s21_stack_2.size(), s21_stack_3.size());
  for (auto i = 0; i < 4; i++) {
    EXPECT_EQ(s21_stack_3.top(), s21_stack_2.top());
    EXPECT_EQ(s21_stack_3.size(), s21_stack_2.size());
    s21_stack_3.pop(), s21_stack_2.pop();
  }
  for (auto i = 0; i < 3; i++) {
    EXPECT_EQ(s21_stack_1.top(), s21_stack_4.top());
    EXPECT_EQ(s21_stack_1.size(), s21_stack_4.size());
    s21_stack_1.pop(), s21_stack_4.pop();
  }
}
