#ifndef S21_CONTAINERS_SRC_SOURCE_TREE_H_
#define S21_CONTAINERS_SRC_SOURCE_TREE_H_

#include <iostream>
#include <iterator>

namespace s21 {
/**
 * @name Left-leaning red–black tree
 */
template <typename T, typename Comparator = std::less<T>>
class LLRBTree {
 public:
  // Types
  using key_type = T;
  using size_type = std::size_t;

  typedef struct TNode {
    key_type data;
    TNode *parent;
    TNode *left;
    TNode *right;
    int color;  // 0 = black, 1 = red
  } TreeNode;

  class iterator {
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = key_type;
    using pointer = key_type *;
    using reference = key_type &;

   public:
    // Default constructor
    iterator() noexcept : tree_ptr(nullptr), node_ptr(nullptr) {}

    // Constructor with paramaters
    iterator(LLRBTree *tree, TreeNode *node) noexcept {
      node_ptr = node;
      tree_ptr = tree;
    }

    // Copy constructor
    iterator(const iterator &other) noexcept {
      tree_ptr = other.tree_ptr;
      node_ptr = other.node_ptr;
    }

    // Move constructor
    iterator(iterator &&other) noexcept {
      tree_ptr = other.tree_ptr;
      node_ptr = other.node_ptr;
    }

    // Assignment operators
    iterator &operator=(const iterator &other) noexcept {
      if (this != &other) {
        tree_ptr = other.tree_ptr;
        node_ptr = other.node_ptr;
      }
      return *this;
    }

    iterator &operator=(iterator &&other) noexcept {
      tree_ptr = other.tree_ptr;
      node_ptr = other.node_ptr;
      return *this;
    }

    // Data access operators
    reference operator*() { return node_ptr->data; }

    pointer operator->() { return &node_ptr->data; }

    // Prefix increment
    iterator &operator++() {
      node_ptr = tree_ptr->GetNextNode(node_ptr);
      return *this;
    }

    // Prefix decrement
    iterator &operator--() {
      node_ptr = tree_ptr->GetPrevNode(node_ptr);
      return *this;
    }

    // Postfix increment
    iterator operator++(int) {
      iterator tmp_iter = *this;
      node_ptr = tree_ptr->GetNextNode(node_ptr);
      return tmp_iter;
    }

    // Postfix decrement
    iterator operator--(int) {
      iterator tmp_iter = *this;
      node_ptr = tree_ptr->GetPrevNode(node_ptr);
      return tmp_iter;
    }

    // Comparision operators
    friend bool operator==(const iterator &a, const iterator &b) noexcept {
      return a.node_ptr == b.node_ptr;
    }

    friend bool operator!=(const iterator &a, const iterator &b) noexcept {
      return a.node_ptr != b.node_ptr;
    }

    LLRBTree *tree_ptr;
    TreeNode *node_ptr;
  };

  class const_iterator {
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = key_type;
    using pointer = const key_type *;
    using reference = const key_type &;

   public:
    // Default constructor
    const_iterator() noexcept : tree_ptr(nullptr), node_ptr(nullptr) {}

    // Constructor with paramaters
    const_iterator(const LLRBTree *tree, const TreeNode *node) noexcept
        : node_ptr(node), tree_ptr(tree) {}

    // Data access operators
    reference operator*() const noexcept { return node_ptr->data; }

    pointer operator->() const noexcept { return &node_ptr->data; }

    // Prefix increment
    const_iterator &operator++() {
      node_ptr = tree_ptr->GetNextNode(const_cast<TreeNode *>(node_ptr));
      return *this;
    }

    // Prefix decrement
    const_iterator &operator--() {
      node_ptr = tree_ptr->GetPrevNode(const_cast<TreeNode *>(node_ptr));
      return *this;
    }

    // Postfix increment
    const_iterator operator++(int) {
      const_iterator tmp_iter{tree_ptr, node_ptr};
      node_ptr = tree_ptr->GetNextNode(const_cast<TreeNode *>(node_ptr));
      return tmp_iter;
    }

    // Postfix decrement
    const_iterator operator--(int) {
      const_iterator tmp_iter{tree_ptr, node_ptr};
      node_ptr = tree_ptr->GetPrevNode(const_cast<TreeNode *>(node_ptr));
      return tmp_iter;
    }

    // Comparision operators
    friend bool operator==(const const_iterator &a,
                           const const_iterator &b) noexcept {
      return a.node_ptr == b.node_ptr;
    }

    friend bool operator!=(const const_iterator &a,
                           const const_iterator &b) noexcept {
      return a.node_ptr != b.node_ptr;
    }

    const TreeNode *node_ptr;
    const LLRBTree *tree_ptr;
  };

  // Constructors and assignment operators

  /**
   * @brief Default constructor
   */
  LLRBTree() {
    null_node_ = new TreeNode;
    null_node_->left = nullptr;
    null_node_->right = nullptr;
    null_node_->color = 0;  // black
    root_ = null_node_;
    tree_size_ = 0;
  }

  /**
   * @brief Copy constructor. It copies LLRBTree object
   * @param other LLRBTree object to be copied
   */
  LLRBTree(const LLRBTree &other) {
    if (this != &other) {
      Clear();
      const_iterator iter(&other, other.GetMinNodeSubtree(other.root_));
      for (; iter != other.cend(); ++iter) {
        InsertNode(iter.node_ptr->data);
      }
    }
  }

  /**
   * @brief Move constructor
   * It creates a new LLRBTree object and destroys an old
   * @param other LLRBTree object to be moved
   */
  LLRBTree(LLRBTree &&other) noexcept
      : root_{other.root_},
        tree_size_{other.tree_size_},
        null_node_{other.null_node_} {
    other.root_ = nullptr;
    other.tree_size_ = 0;
    other.null_node_ = nullptr;
  }

  /**
   * @brief Copy assignment operator overloading
   * @details It's used for l-values
   * @param other LLRBTree object to be copied
   * @return Copied LLRBTree
   */
  LLRBTree &operator=(const LLRBTree &other) {
    if (this != &other) {
      Clear();
      const_iterator iter(&other, other.GetMinNodeSubtree(other.root_));
      for (; iter != other.cend(); ++iter) {
        InsertNode(iter.node_ptr->data);
      }
    }
    return *this;
  }

  /**
   * @brief Move assignment operator overloading
   * @details It's used for r-values
   * @param other LLRBTree object to be moved
   * @return Moved LLRBTree
   */
  LLRBTree &operator=(LLRBTree &&other) noexcept {
    if (this != &other) {
      std::swap(root_, other.root_);
      std::swap(tree_size_, other.tree_size_);
      std::swap(null_node_, other.null_node_);
    }
    return *this;
  }

  // Destructor
  ~LLRBTree() {
    Clear();
    delete null_node_;
    null_node_ = nullptr;
  }

  // Iterators functions

  iterator begin() noexcept {
    return iterator(this, GetMinNodeSubtree(this->root_));
  }

  iterator end() noexcept { return iterator(this, null_node_); }

  const_iterator cbegin() const noexcept {
    return const_iterator(this, GetMinNodeSubtree(this->root_));
  }

  const_iterator cend() const noexcept {
    return const_iterator(this, null_node_);
  }

  // Modifiers

  /**
   * @brief Insert node into the tree
   * @param key Data to be inserted into tree
   */
  void InsertNode(key_type key) {
    TreeNode *new_node = InitializeNode(key);
    TreeNode *new_parent = nullptr;
    TreeNode *comparing_node = this->root_;
    // Find parent node
    while (comparing_node != null_node_) {
      new_parent = comparing_node;
      if (new_node->data < comparing_node->data) {
        comparing_node = comparing_node->left;
      } else {
        comparing_node = comparing_node->right;
      }
    }
    new_node->parent = new_parent;
    // Determine where to place new node
    if (new_parent == nullptr) {
      this->root_ = new_node;
    } else {
      if (new_node->data < new_parent->data) {
        new_parent->left = new_node;
      } else {
        new_parent->right = new_node;
      }
    }
    // Determine a color of the new node
    if (new_node->parent == nullptr) {
      new_node->color = 0;  // Change color of the root to black
    } else {
      FixTree(new_node);
    }
    ++tree_size_;
  }

  /**
   * @brief The function deletes node from the tree by key.
   * @param key The key of the to-be-deleted node.
   * @details The function works only for set-like cases, where the key_type is
   * a simple data type.
   */
  /*   void RemoveNodeKey(key_type key) {
    TreeNode *del_node = FindNode(key);
    del_node = BalanceTreeBeforeRemoving(del_node);
    RemoveLeaf(del_node);
  } */

  /**
   * @brief The function to remove node from the tree by iterator
   * @param pos Iterator of the to-be-deleted node.
   */
  void Erase(iterator pos) {
    TreeNode *del_node = pos.node_ptr;
    del_node = BalanceTreeBeforeRemoving(del_node);
    RemoveLeaf(del_node);
  }

  /**
   * @brief The funcion to remove all elements from a tree.
   */
  void Clear() {
    if (root_ != null_node_) {
      ClearSubTree(root_);
    }
    null_node_ = new TreeNode;
    null_node_->left = nullptr;
    null_node_->right = nullptr;
    null_node_->color = 0;  // black
    root_ = null_node_;
    tree_size_ = 0;
  }

  /**
   * @brief The function to insert element into a set-like tree
   * @param data The data of the inserted node
   * @return 1. Iterator to the inserted node 2. 'True' if element was
   * inserted and 'false' if not.
   */
  std::pair<iterator, bool> Insert(key_type data) {
    bool status = false;
    if (FindNode(data) == null_node_) {  // check there's no this element yet
      InsertNode(data);
      status = true;
    }
    return std::make_pair(iterator(this, FindNode(data)), status);
  }

  /**
   * @brief The function to insert element into a map-like tree
   * @param data The data of the inserted node
   * @return 1. Iterator to the inserted node 2. 'True' if element was
   * inserted and 'false' if not.
   */
  std::pair<iterator, bool> InsertMap(key_type data) {
    bool status = false;
    if (FindNodeMap(data) == null_node_) {  // check there's no this element yet
      InsertNode(data);
      status = true;
    }
    return std::make_pair(iterator(this, FindNodeMap(data)), status);
  }

  /**
   * @brief Exchanges the contents of the trees with those of other.
   * @param other The tree to exchange content with.
   */
  void Swap(LLRBTree &other) noexcept {
    std::swap(root_, other.root_);
    std::swap(null_node_, other.null_node_);
    std::swap(tree_size_, other.tree_size_);
  }

  /**
   * @brief The function to merge 2 trees.
   * @details Elements are merged from the other tree to this tree.
   * In case this tree already has certain element, the element stays in
   * the other tree.
   * @attention Actually inserts and removes nodes.
   * @param other The tree to merge content with.
   */
  void Merge(LLRBTree &other) {
    if (this != &other) {
      iterator other_current = other.begin();
      iterator other_end = other.end();
      while (other_current != other_end) {
        if (FindNode(other_current.node_ptr->data) == null_node_) {
          TreeNode *node_to_move = other_current.node_ptr;
          ++other_current;
          InsertNode(node_to_move->data);
          other.BalanceTreeBeforeRemoving(node_to_move);
          other.RemoveLeaf(node_to_move);
        } else {
          ++other_current;
        }
      }
    }
  }

  // Other functions

  /**
   * @brief Function to find an element in a set-like tree.
   * @param key The element of the tree for searching.
   * @return Iterator to the element.
   * In case there's no such element in the tree the return is
   * iterator(this, null_node_), which is ecual to a method iterator (end).
   */
  iterator Find(const key_type &key) {
    TreeNode *node = FindNode(key);
    return iterator(this, node);
  }

  /**
   * @brief Function to find an element in a map-like tree.
   * @param search_pair The element of the tree for searching. The element
   * consists of 2 variables.
   * @return Iterator to the element.
   * In case there's no such element in a tree the return is
   * iterator(this, null_node_), which is ecual to a method iterator (end)
   */
  iterator FindMap(const key_type &search_pair) {
    TreeNode *node = FindNodeMap(search_pair);
    return iterator(this, node);
  }

  /**
   * @brief Function to check if the tree contains element with specific key
   * @details Works only for set-like cases, where the key_type is
   * a simple data type.
   * @param key The element of the tree for searching.
   * @return 'True' if element is in the tree 'false' if not.
   */
  bool Contains(const key_type &key) const noexcept {
    bool ret = false;
    if (FindNode(key) != null_node_) {
      ret = true;
    }
    return ret;
  }

  /**
   * @brief Returns number of elements in the tree.
   */
  size_type Size() const noexcept { return tree_size_; }

  /**
   * @brief Returns max size of the tree.
   */
  size_type MaxSize() const noexcept {
    return std::numeric_limits<size_type>::max() / sizeof(TreeNode) / 2;
  }

  /**
   * @brief Returns 'true' if there's no elements in a tree and 'false'
   * otherwise
   */
  bool Empty() const noexcept {
    bool ret = false;
    if (tree_size_ == 0) {
      ret = true;
    }
    return ret;
  }

  TreeNode *root_;
  TreeNode *null_node_;
  size_type tree_size_;

 private:
  // Tree modification functions

  /**
   * @name Left rotation
   * @param node The node of sub-tree that are closer to the root before
   * rotation
   * @return The node of modified sub-tree that are closer to the root after
   * rotation
   */
  TreeNode *RotateLeft(TreeNode *node) {
    TreeNode *tmp_node = node->right;
    node->right = tmp_node->left;
    tmp_node->left = node;
    // change pointers to and from parent
    if (node->parent) {
      if (node->parent->left == node) {
        node->parent->left = tmp_node;
      } else {
        node->parent->right = tmp_node;
      }
    } else {
      this->root_ = tmp_node;
    }
    tmp_node->parent = node->parent;
    node->parent = tmp_node;
    // set colors
    tmp_node->color = node->color;
    node->color = 1;  // red
    if (node->right != null_node_) {
      node->right->parent = node;
    }
    return tmp_node;
  }

  /**
   * @name Right rotation
   * @param node The node of sub-tree that are closer to the root before
   * rotation
   * @return The node of modified sub-tree that are closer to the root after
   * rotation
   */
  TreeNode *RotateRight(TreeNode *node) {
    TreeNode *tmp_node = node->left;
    node->left = tmp_node->right;
    tmp_node->right = node;
    // change pointers to and from parent
    if (node->parent) {
      if (node->parent->left == node) {
        node->parent->left = tmp_node;
      } else {
        node->parent->right = tmp_node;
      }
    } else {
      this->root_ = tmp_node;
    }
    tmp_node->parent = node->parent;
    node->parent = tmp_node;
    if (node->left != null_node_) {
      node->left->parent = node;
    }
    // set colors
    tmp_node->color = node->color;
    node->color = 1;  // red
    return tmp_node;
  }

  /**
   * @brief Flip colors between parent and children
   * @details Make children black
   * @param node The parent node
   */
  void FlipColors(TreeNode *node) {
    if (node->parent == nullptr) {
      node->color = 0;
    } else {
      node->color = 1;
    }
    node->left->color = 0;
    node->right->color = 0;
  };

  /**
   * @brief The function goes from inserted leaf towards the root. It checks
   * defects in the tree structure and fixes them. The function is also used
   * in one case of pre-deletion rebalancing of the tree.
   * @details Possible defects for fixing are: right red leaf and two
   * consecutive red nodes (parent and child)
   * @param new_node The new node after insertion or old node in case of
   * pre-deletion balancing.
   */
  void FixTree(TreeNode *new_node) {
    bool is_balanced = false;
    TreeNode *node = new_node->parent;
    while (node != nullptr && is_balanced == false) {
      if (node->left->color == 1 && node->right->color == 1) {
        FlipColors(node);
        node = node->parent;
      } else if (node->right->color == 1) {
        node = RotateLeft(node);
      } else if (node->color == 1 && node->left->color == 1) {
        node = RotateRight(node->parent);
      } else {
        is_balanced = true;
      }
    }
  };

  /**
   * @brief The function to choose a way to balance a tree depending on the
   * removing node features.
   * @details The function redirects to other functions where actual
   * rebalancing is carried out.
   * @param node Node to be deleted from the tree.
   * @return Node to be deleted from the tree.
   */
  TreeNode *BalanceTreeBeforeRemoving(TreeNode *node) {
    TreeNode *del_node = node;
    if (node != null_node_) {
      // case 1: node with 2 children
      if (node->left != null_node_ && node->right != null_node_) {
        del_node = BalanceAnyWith2Children(node);
        // case 2: black with 1 red child
      } else if (node->left->color == 1) {
        del_node = BalanceBlackWith1Child(node);
        // case 3: black with no child
      } else if (node->color == 0 && node->left == null_node_ &&
                 node->right == null_node_) {
        BalanceChildlessBeforeRemoving(node);
      }
    }
    return del_node;
  }

  /**
   * @brief The function to balance a tree before removing a black node without
   * children.
   * @details The function is recursive in some cases. It balances the tree
   * from a leaf to the root.
   * @param node The node to balance the tree around it. It may be a leaf at
   * the start of the function execution or a node in the middle of the tree
   * during the processing of recursion.
   */
  void BalanceChildlessBeforeRemoving(TreeNode *node) {
    TreeNode *tmp_node;
    if (node->parent) {
      // Case 1: black right sibling
      if (node->parent->right != node) {  // node == left child
        if (node->parent->right != null_node_ &&
            node->parent->right->left->color ==
                1) {  // sibling's left child is red
          node->parent->right = RotateRight(node->parent->right);
          RotateLeft(node->parent);
          node->parent->color = 0;
          node->parent->parent->right->color = 0;
        } else {  // sibling has no red children
          node->parent->right->color = 1;
          if (node->parent->color == 1) {
            node->parent->color = 0;
            tmp_node =
                RotateLeft(node->parent);  // To rebalance right red child.
          } else {
            tmp_node =
                RotateLeft(node->parent);  // To rebalance right red child.
            BalanceChildlessBeforeRemoving(tmp_node);
          }
        }
      }
      // Case 2: black left sibling
      else if (node->parent->left != node &&
               node->parent->left->color ==
                   0) {  // node == right child, sibling is black
        if (node->parent->left != null_node_ &&
            node->parent->left->left->color ==
                1) {  // sibling's left child is red
          RotateRight(node->parent);
          node->parent->color = 0;
          node->parent->parent->left->color = 0;
        } else {  // sibling has no red children
          node->parent->left->color = 1;
          if (node->parent->color == 1) {
            node->parent->color = 0;
          } else {
            tmp_node = node->parent;
            BalanceChildlessBeforeRemoving(tmp_node);
          }
        }
      }
      // Case 3: red left sibling
      else {
        RotateRight(node->parent);
        node->parent->color = 0;
        node->parent->left->color = 1;
        if (node->parent->left->left->color == 1) {
          FixTree(node->parent->left->left);
        }
      }
    }
  }

  /**
   * @brief The function to balance a tree before removing a node with 2
   * children.
   * @attention There's no actual replacement of nodes, just data exchange
   * through data pointer redirection. It's expected that del_node will be
   * removed right after execution of this function.
   * @param node Node to be deleted from the tree.
   * @return Node to be deleted from the tree.
   */
  TreeNode *BalanceAnyWith2Children(TreeNode *node) {
    TreeNode *left_neighbor = FindSmallerNeighborBelow(node);
    TreeNode *right_neighbor = FindGreaterNeighborBelow(node);
    TreeNode *exchanged_node;
    if (right_neighbor->color == 1) {
      exchanged_node = right_neighbor;
    } else {
      exchanged_node = left_neighbor;
    }
    // exchange of data between del_node and exchanged_node
    key_type tmp_data = node->data;
    node->data = exchanged_node->data;
    exchanged_node->data = tmp_data;
    TreeNode *del_node = exchanged_node;
    // balancing in case exchanged_node is black
    if (exchanged_node->color == 0) {
      // black with 1 red child
      if (exchanged_node->left->color == 1) {
        del_node = BalanceBlackWith1Child(exchanged_node);
        // black with no children
      } else {
        BalanceChildlessBeforeRemoving(exchanged_node);
      }
    }
    return del_node;
  }

  /**
   * @brief The function to balance a tree before removing a black node with 1
   * (red) child.
   * @attention There's no actual replacement of nodes, just data exchange
   * through data pointer redirection. It's expected that returned (child) node
   * will be removed right after execution of this function.
   * @param node Node to be deleted from the tree.
   * @return Node to be deleted from the tree.
   */
  TreeNode *BalanceBlackWith1Child(TreeNode *node) {
    key_type tmp_data = node->data;
    node->data = node->left->data;
    node->left->data = tmp_data;
    return node->left;
  }

  /**
   * @brief The function to actually nullify info about a leaf
   * @param node Node to be deleted from tree
   * @attention The function works right only if node argument has no children
   */
  void RemoveLeaf(TreeNode *node) {
    if (node != null_node_) {
      if (node->parent) {
        // std::cout << node->parent->left->color << '\n';
        if (node->parent->left == node) {
          node->parent->left = null_node_;
        } else {
          node->parent->right = null_node_;
        }
      } else {
        this->root_ = null_node_;
      }
      node->parent = nullptr;
      node->left = nullptr;
      node->right = nullptr;
      --tree_size_;
    }
  }

  /**
   * @brief The funcion to remove all elements from a subtree.
   * @param sub_root A root of the subtree.
   */
  void ClearSubTree(TreeNode *sub_root) {
    if (sub_root->left != null_node_) {
      ClearSubTree(sub_root->left);
    }
    if (sub_root->right != null_node_) {
      ClearSubTree(sub_root->right);
    }
    RemoveLeaf(sub_root);
  }

  // Functions to iterate through the tree

  /**
   * @brief Find largest node in a subtree.
   * @param node Root of the subtree.
   * @return Largest node of the subtree.
   */
  TreeNode *GetMaxNodeSubtree(TreeNode *node) const {
    if (node != null_node_) {
      while (node->right != null_node_) {
        node = node->right;
      }
    }
    return node;
  }

  /**
   * @brief Find smallest node in a subtree.
   * @param node Root of the subtree.
   * @return Smallest node of the subtree.
   */
  TreeNode *GetMinNodeSubtree(TreeNode *node) const {
    if (node != null_node_) {
      while (node->left != null_node_) {
        node = node->left;
      }
    }
    return node;
  }

  /**
   * @brief The function to find previous node.
   * @param node The node of the tree.
   * @return A node that is previous to the given one. In case given node is
   * the smallest in the tree, the return is null_node_.
   */
  TreeNode *GetPrevNode(TreeNode *node) const {
    TreeNode *tmp_node;
    // case 0. There's no nodes in the tree or it's the end() of iterator
    if (node == null_node_) {
      tmp_node = GetMaxNodeSubtree(this->root_);
      ;
    } else {
      // case 1. The node has left child
      if (node->left != null_node_) {
        tmp_node = GetMaxNodeSubtree(node->left);
        // case 2. The node is a right child itself
      } else if (node->parent && node == node->parent->right) {
        tmp_node = node->parent;
        // cases 3 and 4
      } else {
        tmp_node = node;
        while (tmp_node->parent && tmp_node == tmp_node->parent->left) {
          tmp_node = tmp_node->parent;
        }
        // case 3. The node is a left child, but appears in a right subtree
        // somewhere
        if (tmp_node->parent) {
          tmp_node = tmp_node->parent;
          // case 4. The node is the most left node. It has no previous node
        } else {
          tmp_node = null_node_;
        }
      }
    }
    return tmp_node;
  }

  /**
   * @brief The function to find next node.
   * @param node The node of the tree.
   * @return A node that is next to the given one. In case given node is
   * the largest in the tree, the return is null_node_.
   */
  TreeNode *GetNextNode(TreeNode *node) const {
    TreeNode *tmp_node;
    // case 1. The node has right child
    if (node->right != null_node_) {
      tmp_node = GetMinNodeSubtree(node->right);
      // case 2. The node is a left child itself
    } else if (node->parent && node == node->parent->left) {
      tmp_node = node->parent;
      // cases 3 and 4
    } else {
      tmp_node = node;
      while (tmp_node->parent && tmp_node == tmp_node->parent->right) {
        tmp_node = tmp_node->parent;
      }
      // case 3. The node is a right child, but appears in a left subtree
      // somewhere
      if (tmp_node->parent) {
        tmp_node = tmp_node->parent;
        // case 4. The node is the most right node. It has no next node
      } else {
        tmp_node = null_node_;
      }
    }
    return tmp_node;
  }

  /**
   * @brief The function to find previous node but only in the subtree below
   * the given node.
   * @param node A node of the tree.
   * @attention Argument 'node' must not be a null_node_.
   * @return The most right node in the left subtree. In case root has no right
   * subtree it returns null_node_.
   */
  TreeNode *FindSmallerNeighborBelow(TreeNode *node) const {
    TreeNode *tmp_node = node->left;
    if (tmp_node != null_node_) {
      while (tmp_node->right != null_node_) {
        tmp_node = tmp_node->right;
      }
    }
    return tmp_node;
  }

  /**
   * @brief The function to find next node but only in the subtree below
   * the given node.
   * @param node A node of the tree.
   * @attention Argument 'node' must not be a null_node_
   * @return The most left node in the right subtree. In case root has no left
   * subtree it returns null_node_;
   */
  TreeNode *FindGreaterNeighborBelow(TreeNode *node) const {
    TreeNode *tmp_node = node->right;
    if (tmp_node != null_node_) {
      while (tmp_node->left != null_node_) {
        tmp_node = tmp_node->left;
      }
    }
    return tmp_node;
  }

  /**
   * @brief The function finds a pointer to the node where data is equal to key
   * @param key The key of the to-be-find node.
   * @details The function works only for set-like cases, where the key_type is
   * a simple data type.
   * @return Pointer to the node. If there is no data equal to key, it returns
   * null_node_
   */
  TreeNode *FindNode(const key_type key) const noexcept {
    TreeNode *tmp_node = root_;
    while (tmp_node != null_node_ && tmp_node->data != key) {
      if (tmp_node->data > key) {
        tmp_node = tmp_node->left;
      } else {
        tmp_node = tmp_node->right;
      }
    }
    return tmp_node;
  }

  /**
   * @brief The function finds a pointer to the node where data is equal to key
   * @param search_pair The data of the to-be-find node.
   * @details The function works only for map-like cases, where the key_type is
   * a pair of variables. It compares only first variable in the search_pair.
   * @return Pointer to the node. If there is no data equal to
   * search_pair.first, it returns null_node_.
   */
  TreeNode *FindNodeMap(const key_type search_pair) const noexcept {
    TreeNode *tmp_node = root_;
    while (tmp_node != null_node_ &&
           tmp_node->data.first != search_pair.first) {
      if (tmp_node->data.first > search_pair.first) {
        tmp_node = tmp_node->left;
      } else {
        tmp_node = tmp_node->right;
      }
    }
    return tmp_node;
  }

  // Other functions

  /**
   * @brief Initialization of the new node.
   * @param key The data of the node.
   * @return Pointer to the new node.
   */
  TreeNode *InitializeNode(key_type key) {
    TreeNode *node = new TreeNode;
    node->parent = nullptr;
    node->data = key;
    node->left = null_node_;
    node->right = null_node_;
    node->color = 1;  // red
    return (node);
  };
};

// Comparator for map
template <typename T>
struct MapComparator {
  bool operator()(const T &comp1, const T &comp2) const noexcept {
    return comp1.first < comp2.first;
  }
};  // elements are equal if values are equal

}  // namespace s21

#endif  // S21_CONTAINERS_SRC_SOURCE_TREE_H_