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

#include <cstdint>
#include <functional>
#include <iostream>
#include <string>

enum class Color { RED = 0, BLACK };

template <class Key, class Value> class Map {

    struct Node {
        Key key;
        Value value;
        Node* parent;
        Node* left;
        Node* right;
        Color color;
        int id;
    };

    using NodePtr = Node*;
    using ConstNodePtr = const NodePtr;
    using ConstColor = const Color;

    struct SearchResult {
        NodePtr node = nullptr;
        NodePtr parent = nullptr;
    };

public:
    Map();
    Map(const Map& other);
    Map& operator=(const Map& other);
    Map(Map&& other);
    Map& operator=(Map&& other);
    ~Map();

    Value At(const Key& key);
    void Insert(const Key& key, const Value& value);
    void Remove(const Key& key);
    std::size_t Size() const;
    std::size_t MaxDepth(NodePtr root = nullptr, const bool first_node = true);
    void SaveTree(const std::string& filename) const;

private:
    NodePtr CreateNode(ConstNodePtr parent, const Key& key, const Value& value,
                       ConstColor color = Color::RED);
    SearchResult Search(const Key& key, ConstNodePtr root, ConstNodePtr parent);
    void LeftRotate(ConstNodePtr x);
    void RightRotate(ConstNodePtr x);
    void Recolor(NodePtr new_node, NodePtr uncle_node);
    inline bool LeafNode(ConstNodePtr node);
    inline bool HasOnlyLeftChild(ConstNodePtr node);
    inline bool HasOnlyRightChild(ConstNodePtr node);
    inline bool HasTwoChildren(ConstNodePtr node);
    inline static bool LeftChild(ConstNodePtr node);
    inline static bool RightChild(ConstNodePtr node);
    inline void Transplant(NodePtr x, NodePtr y);
    inline static NodePtr Sibling(ConstNodePtr node);
    void DeleteTree(NodePtr node);
    void CopyNode(NodePtr node, NodePtr sentinel);

private:
    std::less<Key> m_comparator;
    NodePtr m_root;
    NodePtr m_sentinel;
    std::size_t m_size;
};

std::ostream& operator<<(std::ostream& os, const Color color)
{
    if (color == Color::RED)
        os << "red";
    else
        os << "black";

    return os;
}
