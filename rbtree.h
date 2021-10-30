/*
 * @Descripttion:
 * @version:
 * @Author: Fang
 * @email: 2192294938@qq.com
 * @Date: 2021-10-30 00:16:58
 * @LastEditors: Fang
 * @LastEditTime: 2021-10-31 01:30:44
 */
#ifndef __RBTREE_H__
#define __RBTREE_H__
#include <vector>
template <typename T>
class RbTree;

template <typename T>
class RbTreeNode
{
public:
    friend RbTree<T>;
    enum
    {
        RB_BLACK = 0,
        RB_RED = 1
    };

public:
    RbTreeNode(T);
    // ~RbTreeNode() = default;
    // RbTreeNode(const RbTreeNode &) = delete;
    // const RbTreeNode &operator=(const RbTreeNode &) = delete;
    const T &getData() const;

private:
    T m_data;
    int m_color;
    RbTreeNode<T> *m_parent;
    RbTreeNode<T> *m_lchild;
    RbTreeNode<T> *m_rchild;
};

template <typename T>
using CMP = int (*)(T &, T &);

template <typename T>
class RbTree
{
public:
    RbTree(CMP<T> cb);
    // ~RbTree() = default;
    // RbTree(const RbTree &) = delete;
    // const RbTree &operator=(const RbTree &) = delete;

public:
    bool RB_EMPTY();
    int getNumber() const;
    const T &getData(const RbTreeNode<T> *) const;
    void RB_INSERT(T);
    void RB_REMOVE(T);
    RbTreeNode<T> *getRoot() const;
    RbTreeNode<T> *RB_FIND(T);
    RbTreeNode<T> *RB_NFIND(T);
    RbTreeNode<T> *RB_MAXMIN(int val = 0);
    static void RB_SOLTPRINT(RbTreeNode<T> *, std::vector<const T *> &);

private:
    void RB_SWITCH_COLOR(RbTreeNode<T> *, RbTreeNode<T> *);
    void RB_ROTATE_LEFT(RbTreeNode<T> *);
    void RB_ROTATE_RIGHT(RbTreeNode<T> *);
    void RB_INSERT_ADJUST(RbTreeNode<T> *);
    void RB_REMOVE_ADJUST(RbTreeNode<T> *, RbTreeNode<T> *);

private:
    int m_count;
    RbTreeNode<T> *m_root;
    CMP<T> m_cb;
};

template <typename T>
inline RbTreeNode<T>::RbTreeNode(T data)
    : m_data(data),
      m_color(RbTreeNode::RB_RED),
      m_parent(nullptr),
      m_lchild(nullptr),
      m_rchild(nullptr) {}

template <typename T>
inline RbTree<T>::RbTree(CMP<T> cb)
    : m_count(0),
      m_cb(cb),
      m_root(nullptr) {}

template <typename T>
inline int RbTree<T>::getNumber() const { return this->m_count; }

template <typename T>
inline bool RbTree<T>::RB_EMPTY() { return this->m_root == nullptr; }

template <typename T>
inline const T &RbTree<T>::getData(const RbTreeNode<T> *node) const { return node->m_data; }

template <typename T>
inline RbTreeNode<T> *RbTree<T>::getRoot() const { return this->m_root; }

#endif