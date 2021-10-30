/*
 * @Descripttion:
 * @version:
 * @Author: Fang
 * @email: 2192294938@qq.com
 * @Date: 2021-10-30 01:05:32
 * @LastEditors: Fang
 * @LastEditTime: 2021-10-31 01:32:51
 */
#include "rbtree.hpp"
#include <iostream>
#include <string>
typedef struct Node
{
    int nums = 10;
    std::string name = "sssss";
    Node(){};
    Node(int n, std::string s)
        : nums(n),
          name(s) {}
} Node;

int mycmp(Node &cm1, Node &cm2)
{
    if (cm1.nums > cm2.nums)
        return 1;
    else if (cm1.nums == cm2.nums)
        return 0;
    else if (cm1.nums < cm2.nums)
        return -1;
}

int main()
{
    RbTree<Node> rb(&mycmp);
    static const int N = 9;
    Node node[9];
    const int nums[] = {16, 3, 7, 11, 9, 26, 18, 14, 15};
    for (int i = 0; i < 9; i++)
    {
        node[i] = Node(nums[i], std::string("node_") + std::to_string(i));
        rb.RB_INSERT(node[i]);
    }

    std::cout << "The tree have " << rb.getNumber() << " Nodes!" << std::endl;

    const Node node0 = rb.getData(rb.getRoot());
    std::cout << node0.nums << std::endl
              << node0.name << std::endl;

    const Node node1 = rb.getData(rb.RB_NFIND(node[5]));
    std::cout << node1.nums << std::endl
              << node1.name << std::endl;
    const Node node2 = rb.getData(rb.RB_FIND(node[7]));
    std::cout << node2.nums << std::endl
              << node2.name << std::endl;

    const Node node3 = rb.getData(rb.RB_MAXMIN());
    std::cout << node3.nums << std::endl
              << node3.name << std::endl;
    const Node node4 = rb.getData(rb.RB_MAXMIN(1));
    std::cout << node4.nums << std::endl
              << node4.name << std::endl;

    std::vector<const Node *> all;
    RbTree<Node>::RB_SOLTPRINT(rb.getRoot(), all);
    for (int i = 0; i < all.size(); i++)
        std::cout << all[i]->nums << " ";
    std::cout << std::endl;

    for (int i = 0; i < 4; i++)
        rb.RB_REMOVE(node[i]);
    std::cout << "The tree have " << rb.getNumber() << " Nodes!" << std::endl;
    std::cout << rb.RB_EMPTY() << std::endl;
    return 0;
}