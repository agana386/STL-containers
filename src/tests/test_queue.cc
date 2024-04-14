#include <gtest/gtest.h>

#include <queue>

#include "../s21_containers.h"

TEST(QueueMemberFunctions, TestDefault) {
  s21::queue<int> s21_queue;
  std::queue<int> std_queue;
  EXPECT_EQ(s21_queue.size(), std_queue.size());
  EXPECT_EQ(s21_queue.empty(), std_queue.empty());
}

TEST(QueueMemberFunctions, TestInitializer) {
  s21::queue<int> s21_queue = {1, 2, 3, 4};
  EXPECT_EQ(s21_queue.front(), 1);
  EXPECT_EQ(s21_queue.back(), 4);
  EXPECT_EQ(s21_queue.size(), size_t(4));
  EXPECT_EQ(s21_queue.empty(), false);
}

TEST(QueueMemberFunctions, TestConstructorCopy) {
  s21::queue<char> s21_queue_1 = {'a', 'b', 'c', 'd'};
  s21::queue<char> s21_queue_2(s21_queue_1);

  EXPECT_EQ(s21_queue_1.size(), s21_queue_2.size());

  for (auto i = 0; i < 4; i++) {
    EXPECT_EQ(s21_queue_1.front(), s21_queue_2.front());
    EXPECT_EQ(s21_queue_1.size(), s21_queue_2.size());
    s21_queue_1.pop(), s21_queue_2.pop();
  }
}

TEST(QueueMemberFunctions, TestConstructorMove) {
  s21::queue<char> s21_queue_1 = {'a', 'b', 'c', 'd'};
  s21::queue<char> s21_queue_2 = std::move(s21_queue_1);
  s21::queue<char> s21_queue_3 = {'a', 'b', 'c', 'd'};

  EXPECT_EQ(s21_queue_2.size(), s21_queue_3.size());
  EXPECT_EQ(s21_queue_1.size(), size_t(0));
  EXPECT_EQ(s21_queue_1.empty(), true);
  for (auto i = 0; i < 4; i++) {
    EXPECT_EQ(s21_queue_3.front(), s21_queue_2.front());
    EXPECT_EQ(s21_queue_3.size(), s21_queue_2.size());
    s21_queue_3.pop(), s21_queue_2.pop();
  }
}

TEST(QueueMemberFunctions, TestConstructorMove2) {
  s21::queue<int> s21_queue_1{1, 2, 3, 4};
  s21::queue<int> s21_queue_2(std::move(s21_queue_1));
  std::queue<int> s21_queue_3;
  s21_queue_3.push(1);
  s21_queue_3.push(2);
  s21_queue_3.push(3);
  s21_queue_3.push(4);
  std::queue<int> s21_queue_4(std::move(s21_queue_3));
  EXPECT_EQ(s21_queue_2.empty(), s21_queue_4.empty());
  EXPECT_EQ(s21_queue_2.size(), s21_queue_4.size());
  while (!s21_queue_2.empty()) {
    EXPECT_EQ(s21_queue_2.front(), s21_queue_4.front());
    s21_queue_2.pop();
    s21_queue_4.pop();
  }
}

TEST(QueueElementAccess, TestFront) {
  s21::queue<int> s21_queue{11, 2, 4, 6, 8};
  std::initializer_list<int> il = {11, 1, 3, 5, 7};
  std::queue<int> std_queue{il};
  EXPECT_EQ(s21_queue.front(), std_queue.front());
}

TEST(QueueElementAccess, TestBack) {
  s21::queue<int> s21_queue{1, 2, 3, 4, 10};
  std::initializer_list<int> il = {15, 14, 13, 12, 11, 10};
  std::queue<int> std_queue{il};
  EXPECT_EQ(s21_queue.back(), std_queue.back());
}

TEST(QueueCapacity, TestEmpty) {
  s21::queue<int> s21_queue;
  std::queue<int> std_queue;
  EXPECT_EQ(s21_queue.empty(), std_queue.empty());
  s21_queue.push(123);
  std_queue.push(123);
  EXPECT_EQ(s21_queue.empty(), std_queue.empty());
}

TEST(QueueCapacity, TestSize) {
  s21::queue<int> s21_queue{1, 2, 3, 4, 5, 6, 7, 8};
  std::initializer_list<int> il = {1, 2, 3, 4, 5, 6, 7, 8};
  std::queue<int> std_queue{il};
  EXPECT_EQ(s21_queue.size(), std_queue.size());
}

TEST(QueueModifier, TestPush) {
  s21::queue<int> s21_queue;
  std::queue<int> std_queue;
  s21_queue.push(5);
  s21_queue.push(0);
  std_queue.push(5);
  std_queue.push(0);
  EXPECT_EQ(s21_queue.front(), std_queue.front());
  EXPECT_EQ(s21_queue.back(), std_queue.back());
  EXPECT_EQ(s21_queue.size(), std_queue.size());
  EXPECT_EQ(s21_queue.empty(), std_queue.empty());
}

TEST(QueueModifiers, TestPop) {
  s21::queue<int> s21_queue;
  std::queue<int> std_queue;
  s21_queue.push(5);
  s21_queue.push(0);
  std_queue.push(5);
  std_queue.push(0);
  s21_queue.pop();
  s21_queue.pop();
  std_queue.pop();
  std_queue.pop();
  s21_queue.push(15);
  s21_queue.push(10);
  std_queue.push(15);
  std_queue.push(10);
  s21_queue.pop();
  std_queue.pop();
  EXPECT_EQ(s21_queue.front(), std_queue.front());
  EXPECT_EQ(s21_queue.back(), std_queue.back());
  EXPECT_EQ(s21_queue.size(), std_queue.size());
  EXPECT_EQ(s21_queue.empty(), std_queue.empty());
}

TEST(QueueModifiers, TestSwap) {
  s21::queue<std::string> s21_queue_1 = {"a", "b", "c", "d"};
  s21::queue<std::string> s21_queue_2 = {"aa", "bb", "cc"};
  s21::queue<std::string> s21_queue_3 = {"a", "b", "c", "d"};
  s21::queue<std::string> s21_queue_4 = {"aa", "bb", "cc"};

  s21_queue_1.swap(s21_queue_2);

  EXPECT_EQ(s21_queue_1.size(), s21_queue_4.size());
  EXPECT_EQ(s21_queue_2.size(), s21_queue_3.size());
  for (auto i = 0; i < 4; i++) {
    EXPECT_EQ(s21_queue_3.front(), s21_queue_2.front());
    EXPECT_EQ(s21_queue_3.size(), s21_queue_2.size());
    s21_queue_3.pop(), s21_queue_2.pop();
  }
  for (auto i = 0; i < 3; i++) {
    EXPECT_EQ(s21_queue_1.front(), s21_queue_4.front());
    EXPECT_EQ(s21_queue_1.size(), s21_queue_4.size());
    s21_queue_1.pop(), s21_queue_4.pop();
  }
}

TEST(QueueModifiers, TestSwap2) {
  s21::queue<int> s21_queue_1{1, 2, 3};
  s21::queue<int> s21_queue_2{4, 5, 6};
  s21::queue<int> s21_queue_1_copy{s21_queue_1};
  s21::queue<int> s21_queue_2_copy{s21_queue_2};

  s21_queue_1.swap(s21_queue_2);

  EXPECT_EQ(s21_queue_1.size(), s21_queue_2_copy.size());
  EXPECT_EQ(s21_queue_2.size(), s21_queue_1_copy.size());
  EXPECT_EQ(s21_queue_1.front(), 4);
  EXPECT_EQ(s21_queue_1.back(), 6);
  EXPECT_EQ(s21_queue_2.front(), 1);
  EXPECT_EQ(s21_queue_2.back(), 3);

  s21_queue_2.swap(s21_queue_1);

  EXPECT_EQ(s21_queue_2.size(), s21_queue_1_copy.size());
  EXPECT_EQ(s21_queue_1.size(), s21_queue_2_copy.size());
  EXPECT_EQ(s21_queue_2.front(), 4);
  EXPECT_EQ(s21_queue_2.back(), 6);
  EXPECT_EQ(s21_queue_1.front(), 1);
  EXPECT_EQ(s21_queue_1.back(), 3);
}
