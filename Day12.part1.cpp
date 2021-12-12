#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>


class Node
{
public:
    Node(const std::string &name)
    : name_(name)
    {
        if (std::accumulate(name.begin(), name.end(), 0, [](int a, char c) { return a + (c >= 'A' && c <= 'Z'); } ) > 0)
            bigCave_ = true;
    }

    void link(Node *other)
    {
        linked_.push_back(other);
        other->linked_.push_back(this);
    }

    int linkCount() const
    {
        return linked_.size();
    }

    Node *linked(int idx)
    {
        return linked_[idx];
    }

    const std::string &name() const
    {
        return name_;
    }

    bool visited() const
    {
        return visitCount_ > 0;
    }

    void updateVisited(int v)
    {
        visitCount_ += v;
    }

    bool canVisit() const
    {
        return bigCave_ || visitCount_ == 0;
    }

private:
    const std::string name_;
    bool bigCave_{false};
    int visitCount_{0};
    std::vector<Node *> linked_;
};


int traverseNodes(Node *node, const std::string &path = "")
{
    int pathCount = 0;
    std::string name = path + (path.empty() ? "" : ",") + node->name();
    if (node->name() == "end")
    {
        std::cout << name << "\n";
        return 1;
    }

    node->updateVisited(1);
    for (int i = 0; i < node->linkCount(); ++i)
    {
        if (node->linked(i)->canVisit())
        {
            pathCount += traverseNodes(node->linked(i), name);
        }
    }
    node->updateVisited(-1);

    return pathCount;
}


int main(int, char **)
{
    // Read each line from the file and setup the nodes and links
    std::map<std::string, Node *> nodes;
    std::string path;
    while (std::cin >> path)
    {
        std::string name1 = path.substr(0, path.find("-"));
        std::string name2 = path.substr(name1.size()+1);
        auto node1 = nodes.find(name1);
        if (node1 == nodes.end())
        {
            auto res = nodes.insert({name1, new Node(name1)});
            node1 = res.first;
        }
        auto node2 = nodes.find(name2);
        if (node2 == nodes.end())
        {
            auto res = nodes.insert({name2, new Node(name2)});
            node2 = res.first;
        }
        node1->second->link(node2->second);
    }

    // Traverse all nodes and count the paths
    int pathCount = traverseNodes(nodes.find("start")->second);
    std::cout << "Path count = " << pathCount << "\n";

    return 0;
}
