/*
 * @Descripttion:
 * @version:
 * @Author: Fang
 * @email: 2192294938@qq.com
 * @Date: 2021-10-30 00:27:29
 * @LastEditors: Fang
 * @LastEditTime: 2021-10-31 01:32:18
 */
#include "rbtree.h"
#include <iostream>

#define RB_ROOT() (this->m_root)
#define RB_PARENT(node) ((node)->m_parent)
#define RB_LCHILD(node) ((node)->m_lchild)
#define RB_RCHILD(node) ((node)->m_rchild)
#define RB_COLOR(node) ((node)->m_color)

template <typename T>
void RbTree<T>::RB_SOLTPRINT(RbTreeNode<T> *root, std::vector<const T *> &ver)
{
    if (root == nullptr)
        return;
    RB_SOLTPRINT(RB_LCHILD(root), ver);
    ver.push_back(&root->m_data);
    RB_SOLTPRINT(RB_RCHILD(root), ver);
}

template <typename T>
void RbTree<T>::RB_SWITCH_COLOR(RbTreeNode<T> *bnode, RbTreeNode<T> *rnode)
{
    RB_COLOR(bnode) = RbTreeNode<T>::RB_BLACK;
    RB_COLOR(rnode) = RbTreeNode<T>::RB_RED;
}

template <typename T>
void RbTree<T>::RB_ROTATE_LEFT(RbTreeNode<T> *node)
{
    RbTreeNode<T> *rtmp = RB_RCHILD(node);
    if ((RB_RCHILD(node) = RB_LCHILD(rtmp)) != nullptr)
        RB_PARENT(RB_LCHILD(rtmp)) = node;
    if ((RB_PARENT(rtmp) = RB_PARENT(node)) != nullptr)
        if (node == RB_LCHILD(RB_PARENT(node)))
            RB_LCHILD(RB_PARENT(node)) = rtmp;
        else
            RB_RCHILD(RB_PARENT(node)) = rtmp;
    else
        RB_ROOT() = rtmp;
    RB_LCHILD(rtmp) = node;
    RB_PARENT(node) = rtmp;
}

template <typename T>
void RbTree<T>::RB_ROTATE_RIGHT(RbTreeNode<T> *node)
{
    RbTreeNode<T> *ltmp = RB_LCHILD(node);
    if ((RB_LCHILD(node) = RB_RCHILD(ltmp)) != nullptr)
        RB_PARENT(RB_RCHILD(ltmp)) = node;
    if ((RB_PARENT(ltmp) = RB_PARENT(node)) != nullptr)
        if (node == RB_LCHILD(RB_PARENT(node)))
            RB_LCHILD(RB_PARENT(node)) = ltmp;
        else
            RB_RCHILD(RB_PARENT(node)) = ltmp;
    else
        RB_ROOT() = ltmp;
    RB_RCHILD(ltmp) = node;
    RB_PARENT(node) = ltmp;
}

template <typename T>
void RbTree<T>::RB_INSERT(T data)
{
    RbTreeNode<T> *tmp = RB_ROOT(); //当前结点为根结点
    RbTreeNode<T> *parent = nullptr;
    int ret = 0;
    while (tmp != nullptr)
    {
        parent = tmp;
        ret = (m_cb)(data, parent->m_data);
        if (ret < 0) //如果插入结点key小于当前结点key，当前结点迭代为左子结点
            tmp = RB_LCHILD(tmp);
        else if (ret > 0) //如果插入结点key大于当前结点key，当前结点迭代为右子结点
            tmp = RB_RCHILD(tmp);
        else //如歌插入结点key与当前结点key相等，更新当前结点的数据，结束插入操作
        {
            RbTreeNode<T> *node = new RbTreeNode<T>(data);
            RB_COLOR(node) = RB_COLOR(tmp);
            if ((RB_PARENT(node) = RB_PARENT(tmp)) != nullptr)
                if (tmp == RB_LCHILD(RB_PARENT(tmp)))
                    RB_LCHILD(RB_PARENT(tmp)) = node;
                else
                    RB_RCHILD(RB_PARENT(tmp)) = node;
            else
                RB_ROOT() = node;
            if ((RB_LCHILD(node) = RB_LCHILD(tmp)) != nullptr)
                RB_PARENT(RB_LCHILD(tmp)) = node;
            if ((RB_RCHILD(node) = RB_RCHILD(tmp)) != nullptr)
                RB_PARENT(RB_RCHILD(tmp)) = node;
            free(tmp);
            return;
        }
    }
    RbTreeNode<T> *node = new RbTreeNode<T>(data); //为插入数据创建一个树结点
    RB_PARENT(node) = parent;
    if (parent != nullptr)
        if (ret < 0)
            RB_LCHILD(parent) = node;
        else
            RB_RCHILD(parent) = node;
    else //如果插入位置是很结点
        RB_ROOT() = node;

    /*
        以上代码已经处理了三种插入树的情景
        1、红黑树为空树--->把插入结点作为根结点，并设置结点颜色为黑色
        2、插入位置的父结点为黑色结点--->直接插入
        3、插入数据的key已经存在--->更新key值结点的数据，设置插入结点颜色为当前结点的颜色
    */
    RB_INSERT_ADJUST(node); //插入结点后进行树的调整
    m_count++;
}

template <typename T>
void RbTree<T>::RB_INSERT_ADJUST(RbTreeNode<T> *node)
{
    RbTreeNode<T> *parent = nullptr;
    RbTreeNode<T> *gparent = nullptr;
    RbTreeNode<T> *uncle = nullptr;
    //插入位置不是根结点，父结点为红色结点
    while ((parent = RB_PARENT(node)) != nullptr && RB_COLOR(parent) == RbTreeNode<T>::RB_RED)
    {
        gparent = RB_PARENT(parent);      //获取祖父结点
        if (parent == RB_LCHILD(gparent)) //如果父结点为左结点
        {
            uncle = RB_RCHILD(gparent);                                       //获取叔叔结点
            if (uncle != nullptr && RB_COLOR(uncle) == RbTreeNode<T>::RB_RED) //如果叔叔结点存在，那么叔叔结点肯定和父亲结点一样同为红色
            {
                //叔叔结点与父亲结点变为黑色，祖父结点变为红色，把祖父结点看成插入结点继续进入循环
                RB_COLOR(uncle) = RbTreeNode<T>::RB_BLACK;
                RB_SWITCH_COLOR(parent, gparent);
                node = gparent;
                continue;
            }
            //叔叔结点不存在
            if (node == RB_RCHILD(parent)) //插入结点是父亲结点的右孩子
            {
                //如果父结点属于左结点，插入结点属于右孩子(左右),则以父结点进行左旋转
                RB_ROTATE_LEFT(parent);
                uncle = parent;
                parent = node;
                node = uncle;
            }
            //左右情况进行左旋转后变成了左左情况
            //交换父亲结点与祖父结点的颜色
            RB_SWITCH_COLOR(parent, gparent);
            //以祖父结点右旋转
            RB_ROTATE_RIGHT(gparent);
        }
        else
        {
            uncle = RB_LCHILD(gparent);
            if (uncle != nullptr && RB_COLOR(uncle) == RbTreeNode<T>::RB_RED)
            {
                RB_COLOR(uncle) = RbTreeNode<T>::RB_BLACK;
                RB_SWITCH_COLOR(parent, gparent);
                node = gparent;
                continue;
            }
            if (node == RB_LCHILD(parent))
            {
                RB_ROTATE_RIGHT(parent);
                uncle = parent;
                parent = node;
                node = uncle;
            }
            RB_SWITCH_COLOR(parent, gparent);
            RB_ROTATE_LEFT(gparent);
        }
    }
    RB_COLOR(RB_ROOT()) = RbTreeNode<T>::RB_BLACK; //设置根结点为黑色
}

template <typename T>
void RbTree<T>::RB_REMOVE(T data)
{
    RbTreeNode<T> *tmp = RB_ROOT();
    RbTreeNode<T> *node = nullptr;
    int ret = 0;
    while (tmp != nullptr) //查找删除结点key的位置
    {
        ret = (m_cb)(data, tmp->m_data);
        if (ret < 0)
            tmp = RB_LCHILD(tmp);
        else if (ret > 0)
            tmp = RB_RCHILD(tmp);
        else
        {
            node = tmp; //找到删除结点key，记录在node中并跳出循环
            break;
        }
    }
    if (node == nullptr) //如果为空表示没有目标删除结点key
    {
        std::cout << "Node data not found!" << std::endl;
        return;
    }
    RbTreeNode<T> *child = nullptr;
    RbTreeNode<T> *parent = nullptr;
    RbTreeNode<T> *old = node;
    int color = -1;
    if (RB_LCHILD(node) == nullptr)
        child = RB_RCHILD(node);
    else if (RB_RCHILD(node) == nullptr)
        child = RB_LCHILD(node);
    else //如果删除的结点有两个孩子
    {
        /* 找到删除结点右子树中最左边的结点与之调换 */
        RbTreeNode<T> *tmp = nullptr;
        node = RB_RCHILD(node);
        while ((tmp = RB_LCHILD(node)) != nullptr)
            node = tmp;
        child = RB_RCHILD(node);
        parent = RB_PARENT(node);
        color = RB_COLOR(node);
        if (child != nullptr)
            RB_PARENT(child) = parent;
        if (parent != nullptr)
            if (node == RB_LCHILD(parent))
                RB_LCHILD(parent) = child;
            else
                RB_RCHILD(parent) = child;
        else
            RB_ROOT() = child;
        if (old == RB_PARENT(node))
            parent = node;
        RB_PARENT(node) = RB_PARENT(old);
        RB_LCHILD(node) = RB_LCHILD(old);
        RB_RCHILD(node) = RB_RCHILD(old);
        RB_COLOR(node) = RB_COLOR(old);
        if (RB_PARENT(old) != nullptr)
            if (old == RB_LCHILD(RB_PARENT(old)))
                RB_LCHILD(RB_PARENT(old)) = node;
            else
                RB_RCHILD(RB_PARENT(old)) = node;
        else
            RB_ROOT() = node;
        RB_PARENT(RB_LCHILD(old)) = node;
        if (RB_RCHILD(old) != nullptr)
            RB_PARENT(RB_RCHILD(old)) = node;
        goto color;
    }
    parent = RB_PARENT(node);
    color = RB_COLOR(node);
    if (child != nullptr)
        RB_PARENT(child) = parent;
    if (parent != nullptr)
        if (node == RB_LCHILD(parent))
            RB_LCHILD(parent) = child;
        else
            RB_RCHILD(parent) = child;
    else
        RB_ROOT() = child;
color:
    if (color == RbTreeNode<T>::RB_BLACK)
        RB_REMOVE_ADJUST(parent, child);
    m_count--;
    free(old);
}

template <typename T>
void RbTree<T>::RB_REMOVE_ADJUST(RbTreeNode<T> *parent, RbTreeNode<T> *child)
{
    RbTreeNode<T> *brother = nullptr;
    while ((child == nullptr || RB_COLOR(child) == RbTreeNode<T>::RB_BLACK) && child != RB_ROOT()) //替换结点是黑色
    {
        if (child == RB_LCHILD(parent)) //替换结点是其父亲结点的左结点
        {
            brother = RB_RCHILD(parent);                    //获取兄弟结点
            if (RB_COLOR(brother) == RbTreeNode<T>::RB_RED) //兄弟结点是红色
            {
                RB_SWITCH_COLOR(brother, parent); //兄弟结点设置为黑色，父亲结点设置为红色
                RB_ROTATE_LEFT(parent);           //以父亲结点进行左旋
                brother = RB_RCHILD(parent);
            }
            //兄弟结点的左右子结点都是黑色结点
            if ((RB_LCHILD(brother) == nullptr || RB_COLOR(RB_LCHILD(brother)) == RbTreeNode<T>::RB_BLACK) && (RB_RCHILD(brother) == nullptr || RB_COLOR(RB_RCHILD(brother)) == RbTreeNode<T>::RB_BLACK))
            {
                RB_COLOR(brother) = RbTreeNode<T>::RB_RED; //兄弟结点转化为红色
                child = parent;                            //把父亲结点结点当成替换结点
                parent = RB_PARENT(child);
            }
            else
            {
                if (RB_RCHILD(brother) == nullptr || RB_COLOR(RB_RCHILD(brother)) == RbTreeNode<T>::RB_BLACK) //兄弟结点的右子结点存在且为黑色
                {
                    if (RB_LCHILD(brother) != nullptr)
                        RB_COLOR(RB_LCHILD(brother)) = RbTreeNode<T>::RB_BLACK; //兄弟结点的左结点存在设置为黑色
                    RB_COLOR(brother) = RbTreeNode<T>::RB_RED;                  //兄弟结点设置为红色
                    RB_ROTATE_RIGHT(brother);                                   //以兄弟结点进行右旋
                    child = RB_RCHILD(parent);
                }
                //兄弟结点的右子结点是红色，左子结点为任意颜色
                RB_COLOR(brother) = RB_COLOR(parent);       //兄弟结点设置为父亲结点的颜色
                RB_COLOR(parent) = RbTreeNode<T>::RB_BLACK; //父亲结点设置为黑色
                if (RB_RCHILD(brother) != nullptr)
                    RB_COLOR(RB_RCHILD(brother)) = RbTreeNode<T>::RB_BLACK; //如果兄弟结点存在右子结点，设置为黑色
                RB_ROTATE_LEFT(parent);                                     //以父亲结点进行左旋
                child = RB_ROOT();
                break;
            }
        }
        else
        {
            brother = RB_LCHILD(parent);
            if (RB_COLOR(brother) == RbTreeNode<T>::RB_RED)
            {
                RB_SWITCH_COLOR(brother, parent);
                RB_ROTATE_RIGHT(parent);
                brother = RB_LCHILD(parent);
            }
            if ((RB_LCHILD(brother) == nullptr || RB_COLOR(RB_LCHILD(brother)) == RbTreeNode<T>::RB_BLACK) && (RB_RCHILD(brother) == nullptr || RB_COLOR(RB_RCHILD(brother)) == RbTreeNode<T>::RB_BLACK))
            {
                RB_COLOR(brother) = RbTreeNode<T>::RB_RED;
                child = parent;
                parent = RB_PARENT(child);
            }
            else
            {
                if (RB_LCHILD(brother) == nullptr || RB_COLOR(RB_LCHILD(brother)) == RbTreeNode<T>::RB_BLACK)
                {
                    if (RB_RCHILD(brother) != nullptr)
                        RB_COLOR(RB_RCHILD(brother)) = RbTreeNode<T>::RB_BLACK;
                    RB_COLOR(brother) = RbTreeNode<T>::RB_RED;
                    RB_ROTATE_LEFT(brother);
                    brother = RB_LCHILD(parent);
                }
                RB_COLOR(brother) = RB_COLOR(parent);
                RB_COLOR(parent) = RbTreeNode<T>::RB_BLACK;
                if (RB_LCHILD(brother) != nullptr)
                    RB_COLOR(RB_LCHILD(brother)) = RbTreeNode<T>::RB_BLACK;
                RB_ROTATE_RIGHT(parent);
                child = RB_ROOT();
                break;
            }
        }
    }
    if (child != nullptr)
        RB_COLOR(child) = RbTreeNode<T>::RB_BLACK;
}

template <typename T>
RbTreeNode<T> *RbTree<T>::RB_FIND(T data)
{
    RbTreeNode<T> *tmp = RB_ROOT();
    int ret = 0;
    while (tmp != nullptr)
    {
        ret = (m_cb)(data, tmp->m_data);
        if (ret < 0)
            tmp = RB_LCHILD(tmp);
        else if (ret > 0)
            tmp = RB_RCHILD(tmp);
        else
            return tmp;
    }
    return nullptr;
}

template <typename T>
RbTreeNode<T> *RbTree<T>::RB_NFIND(T data)
{
    RbTreeNode<T> *tmp = RB_ROOT();
    RbTreeNode<T> *res = nullptr;
    int ret = 0;
    while (tmp != nullptr)
    {
        ret = (m_cb)(data, tmp->m_data);
        if (ret < 0)
        {
            res = tmp;
            tmp = RB_LCHILD(tmp);
        }
        else if (ret > 0)
            tmp = RB_RCHILD(tmp);
        else
            return tmp;
    }
    return res;
}

template <typename T>
RbTreeNode<T> *RbTree<T>::RB_MAXMIN(int val)
{
    RbTreeNode<T> *tmp = RB_ROOT();
    RbTreeNode<T> *res = nullptr;
    while (tmp != nullptr)
    {
        res = tmp;
        if (val == 0)
            tmp = RB_LCHILD(tmp);
        else
            tmp = RB_RCHILD(tmp);
    }
    return res;
}

#undef RB_ROOT
#undef RB_PARENT
#undef RB_LCHILD
#undef RB_RCHILD
#undef RB_COLOR