#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <set>


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

    bool bigCave() const
    {
        return bigCave_;
    }

    bool canVisit() const
    {
        if (name_ == "start")
            return false;

        return bigCave_ || visitCount_ < allowedVisitCount_;
    }

    void setAllowedVisitCount(int count)
    {
        allowedVisitCount_ = count;
    }

private:
    const std::string name_;
    bool bigCave_{false};
    int visitCount_{0};
    int allowedVisitCount_{1};
    std::vector<Node *> linked_;
};


std::set<std::string> foundPaths;


int traverseNodes(Node *node, const std::string &path = "")
{
    int pathCount = 0;
    std::string name = path + (path.empty() ? "" : ",") + node->name();
    if (node->name() == "end")
    {
        if (foundPaths.find(name) == foundPaths.end())
        {
            foundPaths.insert(name);
            std::cout << name << "\n";
            return 1;
        }
        return 0;
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

    // Traverse the nodes to find unique paths, and set each small cave to be
    // allowed more than once for each set of traversals
    int pathCount{0};
    for (auto &node : nodes)
    {
        if (!node.second->bigCave() && node.second->name() != "start" && node.second->name() != "end")
        {
            node.second->setAllowedVisitCount(2);
            pathCount += traverseNodes(nodes.find("start")->second);
            node.second->setAllowedVisitCount(1);
        }
    }
    std::cout << "Path count = " << pathCount << "\n";

    return 0;
}
