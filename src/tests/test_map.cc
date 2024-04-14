#include <gtest/gtest.h>

#include <map>

#include "../s21_containers.h"

TEST(map, ConstructorDefaultMap) {
  s21::map<int, char> s21_empty_map;
  std::map<int, char> std_empty_map;
  EXPECT_EQ(s21_empty_map.empty(), std_empty_map.empty());
}

TEST(map, ConstructorInitializerMap) {
  s21::map<int, char> s21_map = {{1, 'x'}, {2, 'b'}, {3, 'z'}, {4, 'y'}};
  std::map<int, char> std_map = {{1, 'x'}, {2, 'b'}, {3, 'z'}, {4, 'y'}};
  EXPECT_EQ(s21_map.size(), std_map.size());
  auto s21_it = s21_map.begin();
  auto std_it = std_map.begin();
  for (; s21_it != s21_map.end(); ++s21_it, ++std_it) {
    EXPECT_TRUE((*s21_it).first == (*std_it).first);
    EXPECT_TRUE((*s21_it).second == (*std_it).second);
  }
}

TEST(map, ConstructorInitializer2Map) {
  s21::map<int, char> s21_map = {};
  std::map<int, char> std_map = {};
  EXPECT_EQ(s21_map.size(), std_map.size());
  auto s21_it = s21_map.begin();
  auto std_it = std_map.begin();
  for (; s21_it != s21_map.end(); ++s21_it, ++std_it) {
    EXPECT_TRUE((*s21_it).first == (*std_it).first);
    EXPECT_TRUE((*s21_it).second == (*std_it).second);
  }
}

TEST(map, ConstructorCopyMap) {
  s21::map<int, int> s21_map = {{1, 2}, {3, 4}, {5, 6}};
  std::map<int, int> std_map = {{1, 2}, {3, 4}, {5, 6}};
  s21::map<int, int> s21_map_copy = s21_map;
  std::map<int, int> std_map_copy = std_map;
  EXPECT_EQ(s21_map_copy.size(), std_map_copy.size());
  auto s21_it = s21_map_copy.begin();
  auto std_it = std_map_copy.begin();
  for (; s21_it != s21_map_copy.end(); ++s21_it, ++std_it) {
    EXPECT_TRUE((*s21_it).first == (*std_it).first);
    EXPECT_TRUE((*s21_it).second == (*std_it).second);
  }
}

TEST(map, ConstructorMoveMap) {
  s21::map<int, int> s21_map = {{1, 2}, {3, 4}, {5, 6}};
  std::map<int, int> std_map = {{1, 2}, {3, 4}, {5, 6}};
  s21::map<int, int> s21_map_copy = std::move(s21_map);
  std::map<int, int> std_map_copy = std::move(std_map);
  EXPECT_EQ(s21_map_copy.size(), std_map_copy.size());
  auto s21_it = s21_map_copy.begin();
  auto std_it = std_map_copy.begin();
  for (; s21_it != s21_map_copy.end(); ++s21_it, ++std_it) {
    EXPECT_TRUE((*s21_it).first == (*std_it).first);
    EXPECT_TRUE((*s21_it).second == (*std_it).second);
  }
}

TEST(map, MapOperator) {
  s21::map<char, std::string> s21_map = {
      {'a', "Alina"}, {'b', "Boris"}, {'c', "Chuck"}};
  std::map<char, std::string> std_map = {
      {'a', "Alina"}, {'b', "Boris"}, {'c', "Chuck"}};
  s21_map['a'] = "Alisa";
  std_map['a'] = "Alisa";
  std_map['b'] = "Ben";
  EXPECT_TRUE(s21_map['a'] == std_map['a']);
  EXPECT_FALSE(s21_map['b'] == std_map['b']);
  EXPECT_TRUE(s21_map['c'] == std_map['c']);
}

TEST(map, MapAtOperatorException) {
  s21::map<char, std::string> s21_map = {
      {'a', "Alina"}, {'b', "Boris"}, {'c', "Chuck"}};
  EXPECT_THROW(s21_map.at('g') = "Alisa", std::out_of_range);
}

TEST(map, MapAtOperator) {
  s21::map<char, std::string> s21_map = {
      {'a', "Alina"}, {'b', "Boris"}, {'c', "Chuck"}};
  std::map<char, std::string> std_map = {
      {'a', "Alina"}, {'b', "Boris"}, {'c', "Chuck"}};
  s21_map.at('a') = "Alisa";
  std_map.at('a') = "Alisa";
  std_map.at('b') = "Ben";
  EXPECT_TRUE(s21_map['a'] == std_map['a']);
  EXPECT_FALSE(s21_map['b'] == std_map['b']);
  EXPECT_TRUE(s21_map['c'] == std_map['c']);
}

TEST(map, empty1) {
  s21::map<int, char> map1 = {{1, 'a'}, {2, 'b'}, {3, 'c'}};
  std::map<int, char> map2 = {{1, 'a'}, {2, 'b'}, {3, 'c'}};
  map1.clear();
  map2.clear();
  EXPECT_EQ(map1.empty(), map2.empty());
  s21::map<int, char> a;
  std::map<int, char> b;
  EXPECT_EQ(map1.empty(), map2.empty());
}

TEST(map, empty2) {
  s21::map<int, char> map1 = {{1, 'a'}, {2, 'b'}, {3, 'c'}};
  std::map<int, char> map2 = {{1, 'a'}, {2, 'b'}, {3, 'c'}};
  EXPECT_EQ(map1.empty(), map2.empty());
}

TEST(map, MapClear) {
  s21::map<int, int> s21_map;
  std::map<int, int> std_map;
  s21_map.clear();
  std_map.clear();
  EXPECT_EQ(s21_map.empty(), std_map.empty());
  s21_map.insert(std::make_pair(1, 1));
  std_map.insert(std::make_pair(1, 1));
  EXPECT_EQ(s21_map.empty(), std_map.empty());
  s21_map.clear();
  std_map.clear();
  EXPECT_EQ(s21_map.empty(), std_map.empty());
}

TEST(map, MapInsert1) {
  s21::map<int, char> s21_map;
  std::map<int, char> std_map;
  s21_map.insert(std::make_pair(1, 'a'));
  s21_map.insert(std::make_pair(2, 'a'));
  s21_map.insert(std::make_pair(3, 'a'));
  std_map.insert(std::make_pair(1, 'a'));
  std_map.insert(std::make_pair(2, 'a'));
  std_map.insert(std::make_pair(3, 'a'));

  auto s21_it = s21_map.begin();
  auto std_it = std_map.begin();
  for (; s21_it != s21_map.end(); ++s21_it, ++std_it) {
    EXPECT_TRUE((*s21_it).first == (*std_it).first);
    EXPECT_TRUE((*s21_it).second == (*std_it).second);
  }

  auto pr1 = s21_map.insert(std::make_pair(1, 'a'));
  auto pr2 = std_map.insert(std::make_pair(1, 'a'));
  EXPECT_TRUE(pr1.second == pr2.second);
}

TEST(map, MapInsert2) {
  s21::map<int, char> s21_map;
  std::map<int, char> std_map;
  s21_map.insert(1, 'a');
  s21_map.insert(2, 'a');
  s21_map.insert(3, 'a');
  std_map.insert(std::make_pair(1, 'a'));
  std_map.insert(std::make_pair(2, 'a'));
  std_map.insert(std::make_pair(3, 'a'));

  auto s21_it = s21_map.begin();
  auto std_it = std_map.begin();
  for (; s21_it != s21_map.end(); ++s21_it, ++std_it) {
    EXPECT_TRUE((*s21_it).first == (*std_it).first);
    EXPECT_TRUE((*s21_it).second == (*std_it).second);
  }

  auto pr1 = s21_map.insert(1, 'a');
  auto pr2 = std_map.insert(std::make_pair(1, 'a'));
  EXPECT_TRUE(pr1.second == pr2.second);
}

TEST(map, MapInsert3) {
  s21::map<int, char> s21_map;
  std::map<int, char> std_map;
  s21_map.insert(1, 'a');
  s21_map.insert(2, 'a');
  s21_map.insert(3, 'a');
  std_map.insert(std::make_pair(1, 'a'));
  std_map.insert(std::make_pair(2, 'a'));
  std_map.insert(std::make_pair(3, 'a'));

  auto s21_it = s21_map.begin();
  auto std_it = std_map.begin();
  for (; s21_it != s21_map.end(); ++s21_it, ++std_it) {
    EXPECT_TRUE((*s21_it).first == (*std_it).first);
    EXPECT_TRUE((*s21_it).second == (*std_it).second);
  }

  auto pr1 = s21_map.insert_or_assign(1, 'b');
  auto i = std_map.begin();
  EXPECT_TRUE((*pr1.first).first == (*i).first);
  EXPECT_FALSE((*pr1.first).second == (*i).second);
}

TEST(map, MapErase) {
  s21::map<int, char> s21_map = {{1, 'a'}, {2, 'b'}, {3, 'c'}, {4, 'd'}};
  std::map<int, char> std_map = {{1, 'a'}, {2, 'b'}, {3, 'c'}, {4, 'd'}};
  EXPECT_EQ(s21_map.size(), std_map.size());
  s21_map.erase(s21_map.begin());
  std_map.erase(std_map.begin());
  EXPECT_EQ(s21_map.size(), std_map.size());
  auto s21_it = s21_map.begin();
  auto std_it = std_map.begin();
  for (; s21_it != s21_map.end(); ++s21_it, ++std_it) {
    EXPECT_TRUE((*s21_it).first == (*std_it).first);
    EXPECT_TRUE((*s21_it).second == (*std_it).second);
  }
}

TEST(map, swap1) {
  s21::map<int, char> map1 = {{1, 'a'}, {2, 'b'}, {3, 'c'}};
  std::map<int, char> map2 = {{1, 'a'}, {2, 'b'}, {3, 'c'}};
  s21::map<int, char> a;
  std::map<int, char> b;
  map1.swap(a);
  map2.swap(b);
  EXPECT_EQ(map1.size(), map2.size());
  EXPECT_EQ(a.size(), b.size());
  auto it = a.begin();
  for (auto it2 = b.begin(); it2 != b.end(); it++, it2++) {
    EXPECT_EQ((*it).second, (*it2).second);
  }
}

TEST(map, swap2) {
  s21::map<int, char> map1 = {{1, 'a'}, {2, 'b'}, {3, 'c'}};
  std::map<int, char> map2 = {{1, 'a'}, {2, 'b'}, {3, 'c'}};
  s21::map<int, char> a = {{1, 'a'}, {2, 'b'}, {3, 'c'}, {4, 'd'}, {5, 'e'}};
  std::map<int, char> b = {{1, 'a'}, {2, 'b'}, {3, 'c'}, {4, 'd'}, {5, 'e'}};
  map1.swap(a);
  map2.swap(b);
  EXPECT_EQ(map1.size(), map2.size());
  EXPECT_EQ(a.size(), b.size());
  auto it = a.begin();
  for (auto it2 = b.begin(); it2 != b.end(); it++, it2++) {
    EXPECT_EQ((*it).second, (*it2).second);
  }
  s21::map<int, char> map11;
  std::map<int, char> map22;
  s21::map<int, char> a1;
  std::map<int, char> b1;
  map11.swap(a1);
  map22.swap(b1);
  EXPECT_EQ(map11.size(), map22.size());
  EXPECT_EQ(a1.size(), b1.size());
}

TEST(map, contains1) {
  s21::map<int, int> map1 = {{1, 1}, {2, 2}, {100, 3}, {4, 4}, {5, 5}};
  EXPECT_EQ(map1.contains(1), true);
}

TEST(map, contains2) {
  s21::map<int, char> map1 = {{1, 'a'}, {2, 'b'}, {3, 'c'}};
  EXPECT_EQ(map1.contains(2), true);
}
