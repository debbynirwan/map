/*
 * Copyright 2023 Debby Nirwan
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include "map.h"
#include <fstream>
#include <queue>
#include <stdexcept>
#include <string>

template <class Key, class Value>
Map<Key, Value>::Map()
    : m_comparator()
    , m_root(nullptr)
    , m_sentinel(nullptr)
    , m_size(0)
{
    m_sentinel = new Node { Key(), Value(), nullptr, nullptr, nullptr, Color::BLACK };
    m_root = m_sentinel;
}

template <class Key, class Value>
Map<Key, Value>::Map(const Map& other)
    : m_comparator()
    , m_root(nullptr)
    , m_sentinel(nullptr)
    , m_size(0)
{
    m_sentinel = new Node { Key(), Value(), nullptr, nullptr, nullptr, Color::BLACK };
    m_root = m_sentinel;

    CopyNode(other.m_root, other.m_sentinel);
}

template <class Key, class Value> Map<Key, Value>& Map<Key, Value>::operator=(const Map& other)
{
    if (this != &other) {
        DeleteTree(m_root);
        m_root = m_sentinel;

        CopyNode(other.m_root, other.m_sentinel);
    }

    return *this;
}

template <class Key, class Value>
Map<Key, Value>::Map(Map&& other)
    : m_comparator()
    , m_root(other.m_root)
    , m_sentinel(other.m_sentinel)
    , m_size(other.m_size)
{
    other.m_root = nullptr;
    other.m_sentinel = nullptr;
    other.m_size = 0;
}

template <class Key, class Value> Map<Key, Value>& Map<Key, Value>::operator=(Map&& other)
{
    if (this != &other) {
        DeleteTree(m_root);
        delete m_sentinel;

        m_root = other.m_root;
        m_sentinel = other.m_sentinel;
        m_size = other.m_size;

        other.m_root = nullptr;
        other.m_sentinel = nullptr;
        other.m_size = 0;
    }

    return *this;
}

template <class Key, class Value> Map<Key, Value>::~Map()
{
    DeleteTree(m_root);
    m_root = nullptr;
    delete m_sentinel;
}

template <class Key, class Value> Value Map<Key, Value>::At(const Key& key)
{
    SearchResult result = Search(key, m_root, nullptr);

    if (result.node == m_sentinel)
        throw std::out_of_range("invalid key: " + std::to_string(key));

    return result.node->value;
}

template <class Key, class Value> void Map<Key, Value>::Insert(const Key& key, const Value& value)
{
    if (m_root == m_sentinel) {
        m_root = CreateNode(nullptr, key, value, Color::BLACK);
        m_size++;
        return;
    }

    SearchResult result = Search(key, m_root, nullptr);
    if (result.node != m_sentinel) {
        result.node->value = value;
        return;
    }

    NodePtr new_node = CreateNode(nullptr, key, value);
    new_node->parent = result.parent;
    if (m_comparator(key, new_node->parent->key)) {
        new_node->parent->left = new_node;
    } else {
        new_node->parent->right = new_node;
    }
    m_size++;

    if (new_node->parent == nullptr || new_node->parent->parent == nullptr)
        return;

    NodePtr uncle_node;
    while (new_node->parent != nullptr && new_node->parent->color == Color::RED) {
        if (RightChild(new_node->parent)) {
            uncle_node = new_node->parent->parent->left;
            if (uncle_node->color == Color::BLACK) {
                if (LeftChild(new_node)) {
                    new_node = new_node->parent;
                    RightRotate(new_node);
                }

                new_node->parent->color = Color::BLACK;
                new_node->parent->parent->color = Color::RED;
                LeftRotate(new_node->parent->parent);
            } else {
                Recolor(new_node, uncle_node);
                new_node = new_node->parent->parent;
            }
        } else {
            uncle_node = new_node->parent->parent->right;
            if (uncle_node->color == Color::BLACK) {
                if (RightChild(new_node)) {
                    new_node = new_node->parent;
                    LeftRotate(new_node);
                }

                new_node->parent->color = Color::BLACK;
                new_node->parent->parent->color = Color::RED;
                RightRotate(new_node->parent->parent);
            } else {
                Recolor(new_node, uncle_node);
                new_node = new_node->parent->parent;
            }
        }
    }

    m_root->color = Color::BLACK;
}

template <class Key, class Value> void Map<Key, Value>::Remove(const Key& key)
{
    SearchResult result = Search(key, m_root, nullptr);
    if (result.node == m_sentinel)
        return;

    NodePtr node_to_be_fixed = result.node;
    Color original_color = result.node->color;

    if (LeafNode(result.node) || HasOnlyRightChild(result.node)) {
        node_to_be_fixed = result.node->right;
        Transplant(result.node, result.node->right);
    } else if (HasOnlyLeftChild(result.node)) {
        node_to_be_fixed = result.node->left;
        Transplant(result.node, result.node->left);
    } else {
        NodePtr right_min = result.node->right;
        while (right_min->left != m_sentinel) {
            right_min = right_min->left;
        }

        original_color = right_min->color;
        node_to_be_fixed = right_min->right;

        if (right_min->parent == result.node) {
            node_to_be_fixed->parent = right_min;
        } else {
            Transplant(right_min, right_min->right);
            right_min->right = result.node->right;
            right_min->right->parent = right_min;
        }

        Transplant(result.node, right_min);
        right_min->left = result.node->left;
        right_min->left->parent = right_min;
        right_min->color = result.node->color;
    }

    if (original_color == Color::BLACK) {
        if (node_to_be_fixed) {
            NodePtr x = node_to_be_fixed;
            NodePtr s;
            while (x != m_root && x->color == Color::BLACK) {
                if (LeftChild(x)) {
                    s = x->parent->right;

                    if (s->color == Color::RED) {
                        s->color = Color::BLACK;
                        x->parent->color = Color::RED;
                        LeftRotate(x->parent);
                        s = x->parent->right;
                    }

                    if (s->left && s->left->color == Color::BLACK && s->right
                        && s->right->color == Color::BLACK) {
                        s->color = Color::RED;
                        x = x->parent;
                    } else {
                        if (s->right && s->right->color == Color::BLACK) {
                            if (s->left)
                                s->left->color = Color::BLACK;
                            s->color = Color::RED;
                            RightRotate(s);
                            s = x->parent->right;
                        }

                        s->color = x->parent->color;
                        x->parent->color = Color::BLACK;
                        if (s->right)
                            s->right->color = Color::BLACK;
                        LeftRotate(x->parent);
                        x = m_root;
                    }
                } else {
                    s = x->parent->left;
                    if (s->color == Color::RED) {
                        s->color = Color::BLACK;
                        x->parent->color = Color::RED;
                        RightRotate(x->parent);
                        s = x->parent->left;
                    }

                    if (s->left && s->left->color == Color::BLACK && s->right
                        && s->right->color == Color::BLACK) {
                        s->color = Color::RED;
                        x = x->parent;
                    } else {
                        if (s->left && s->left->color == Color::BLACK) {
                            if (s->right)
                                s->right->color = Color::BLACK;
                            s->color = Color::RED;
                            LeftRotate(s);
                            s = x->parent->left;
                        }

                        s->color = x->parent->color;
                        x->parent->color = Color::BLACK;
                        if (s->left)
                            s->left->color = Color::BLACK;
                        RightRotate(x->parent);
                        x = m_root;
                    }
                }
            }
            m_sentinel->parent = nullptr;
            x->color = Color::BLACK;
        }
    }

    delete result.node;
    m_size--;
}

template <class Key, class Value> std::size_t Map<Key, Value>::Size() const { return m_size; }

template <class Key, class Value>
std::size_t Map<Key, Value>::MaxDepth(NodePtr root, const bool first_node)
{
    if (first_node)
        root = m_root;

    if (root == m_sentinel || LeafNode(root))
        return 0;
    else {
        auto left_depth = MaxDepth(root->left, false);
        auto right_depth = MaxDepth(root->right, false);

        if (left_depth > right_depth)
            return (left_depth + 1);
        else
            return (right_depth + 1);
    }
}

template <class Key, class Value>
typename Map<Key, Value>::NodePtr Map<Key, Value>::CreateNode(ConstNodePtr parent, const Key& key,
                                                              const Value& value, ConstColor color)
{
    NodePtr node = new Node { key, value, parent, m_sentinel, m_sentinel, color };

    return node;
}

template <class Key, class Value>
typename Map<Key, Value>::SearchResult Map<Key, Value>::Search(const Key& key, ConstNodePtr root,
                                                               ConstNodePtr parent)
{
    if (root == m_sentinel || (key == root->key))
        return { root, parent };

    if (m_comparator(key, root->key))
        return Search(key, root->left, root);
    else
        return Search(key, root->right, root);
}

template <class Key, class Value> void Map<Key, Value>::LeftRotate(ConstNodePtr x)
{
    ConstNodePtr y = x->right;

    x->right = y->left;
    if (y->left != m_sentinel) {
        y->left->parent = x;
    }

    y->parent = x->parent;
    if (x->parent == nullptr) {
        m_root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

template <class Key, class Value> void Map<Key, Value>::RightRotate(ConstNodePtr x)
{
    ConstNodePtr y = x->left;

    x->left = y->right;
    if (y->right != m_sentinel) {
        y->right->parent = x;
    }

    y->parent = x->parent;
    if (x->parent == nullptr) {
        m_root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }

    y->right = x;
    x->parent = y;
}

template <class Key, class Value>
void Map<Key, Value>::Recolor(NodePtr new_node, NodePtr uncle_node)
{
    if (new_node == nullptr || uncle_node == nullptr)
        return;

    NodePtr parent_node = new_node->parent;
    if (parent_node == nullptr)
        return;

    NodePtr grandparent_node = parent_node->parent;
    uncle_node->color = Color::BLACK;
    parent_node->color = Color::BLACK;
    if (grandparent_node != nullptr && grandparent_node != m_root)
        grandparent_node->color = Color::RED;
}

template <class Key, class Value> inline bool Map<Key, Value>::LeafNode(ConstNodePtr node)
{
    return (node->left == m_sentinel && node->right == m_sentinel);
}

template <class Key, class Value> inline bool Map<Key, Value>::HasOnlyLeftChild(ConstNodePtr node)
{
    return (node->left != m_sentinel && node->right == m_sentinel);
}

template <class Key, class Value> inline bool Map<Key, Value>::HasOnlyRightChild(ConstNodePtr node)
{
    return (node->left == m_sentinel && node->right != m_sentinel);
}

template <class Key, class Value> inline bool Map<Key, Value>::HasTwoChildren(ConstNodePtr node)
{
    return (node->left != m_sentinel && node->right != m_sentinel);
}

template <class Key, class Value> inline bool Map<Key, Value>::LeftChild(ConstNodePtr node)
{
    return (node->parent != nullptr && node->parent->left == node);
}

template <class Key, class Value> inline bool Map<Key, Value>::RightChild(ConstNodePtr node)
{
    return (node->parent != nullptr && node->parent->right == node);
}

template <class Key, class Value> inline void Map<Key, Value>::Transplant(NodePtr x, NodePtr y)
{
    if (x == nullptr)
        return;

    if (x->parent == nullptr) {
        m_root = y;
    } else if (LeftChild(x)) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    if (y != nullptr)
        y->parent = x->parent;
}

template <class Key, class Value>
inline typename Map<Key, Value>::NodePtr Map<Key, Value>::Sibling(ConstNodePtr node)
{
    if (node->parent) {
        if (LeftChild(node)) {
            return node->parent->right;
        } else {
            return node->parent->left;
        }
    } else {
        return nullptr;
    }
}

template <class Key, class Value> void Map<Key, Value>::DeleteTree(NodePtr node)
{
    if (node == m_sentinel || node == nullptr)
        return;

    DeleteTree(node->left);
    DeleteTree(node->right);

    delete node;
}

template <class Key, class Value> void Map<Key, Value>::CopyNode(NodePtr node, NodePtr sentinel)
{
    if (node == sentinel)
        return;

    CopyNode(node->left, sentinel);
    CopyNode(node->right, sentinel);

    Insert(node->key, node->value);
}

template <class Key, class Value> void Map<Key, Value>::SaveTree(const std::string& filename) const
{
    if (m_root == m_sentinel)
        return;

    int id = 1;
    std::ofstream fout(filename);

    fout << "digraph g{\n";
    fout << "node [shape = record,height = .1];\n";

    std::queue<Node*> q;
    q.push(m_root);

    fout << " node0"
         << "[label = \"<f0> |<f1>" << m_root->key << "|<f2>\", color=" << m_root->color << "];\n";

    while (!q.empty()) {
        Node* node = q.front();
        q.pop();

        if (node->left) {
            node->left->id = id;
            id++;
            q.push(node->left);
            if (node->left == m_sentinel) {
                fout << " node" << node->left->id << "[label = \"<f0> |<f1>"
                     << "NULL"
                     << "|<f2>\", color=" << node->left->color << "];\n";
            } else {
                fout << " node" << node->left->id << "[label = \"<f0> |<f1>" << node->left->key
                     << "|<f2>\", color=" << node->left->color << "];\n";
            }
            fout << "\"node" << node->id << "\":f0->\"node" << node->left->id << "\":f1;\n";
        }

        if (node->right) {
            node->right->id = id;
            id++;
            q.push(node->right);
            if (node->right == m_sentinel) {
                fout << " node" << node->right->id << "[label = \"<f0> |<f1>"
                     << "NULL"
                     << "|<f2>\", color=" << node->right->color << "];\n";
            } else {
                fout << " node" << node->right->id << "[label = \"<f0> |<f1>" << node->right->key
                     << "|<f2>\", color=" << node->right->color << "];\n";
            }
            fout << "\"node" << node->id << "\":f2->\"node" << node->right->id << "\":f1;\n";
        }
    }

    fout << "}\n";
    fout.close();
}
