#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>



struct Node
{
    Node *parent{nullptr};
    int value{0};
    Node * leftNode{nullptr};
    Node * rightNode{nullptr};

    ~Node()
    {
        if (leftNode) delete leftNode;
        if (rightNode) delete rightNode;
    }

    bool isValueNode() const
    {
        return (!leftNode && !rightNode);
    }

    friend std::ostream &operator<<(std::ostream &os, const Node &node)
    {
        if (node.leftNode || node.rightNode)
        {
            os << "[";
            if (node.leftNode)
                os << *node.leftNode;
            os << ",";
            if (node.rightNode)
                os << *node.rightNode;
            os << "]";
        }
        else
            os << node.value;
        return os;
    }
};


class Stream
{
public:
    Stream(const std::string &input) : input_(input) { }

    char get() { return input_[pos_++]; }
    char peek() { return input_[pos_]; }

    int pos() const { return pos_; }
    const std::string &data() const { return input_; }

private:
    const std::string &input_;
    int pos_{0};
};


Node *readNode(Stream &input)
{
    Node *currentNode = new Node;

    try
    {
        // First char of a node should always be either '[' or a digit. If a digit,
        // this is a simple value node.
        char c = input.get();
        if (c >= '0' && c <= '9')
        {
            currentNode->value = int(c - '0');
            while (input.peek() >= '0' && input.peek() <= '9')
            {
                currentNode->value = 10*currentNode->value + int(input.get() - '0');
            }
        }
        else
        {
            if (c != '[')
            {
                std::ostringstream oss;
                oss << "Expecting '[' - got '" << c << "'";
                throw std::runtime_error(oss.str());
            }

            // Read left node
            Node *leftNode = readNode(input);
            leftNode->parent = currentNode;
            currentNode->leftNode = leftNode;

            // Read comma
            c = input.get();
            if (c != ',')
            {
                std::ostringstream oss;
                oss << "Expecting ',' - got '" << c << "'";
                throw std::runtime_error(oss.str());
            }

            // Read right node
            Node *rightNode = readNode(input);
            rightNode->parent = currentNode;
            currentNode->rightNode = rightNode;

            // Read closing ']'
            c = input.get();
            if (c != ']')
            {
                std::ostringstream oss;
                oss << "Expecting ']' - got '" << c << "'";
                throw std::runtime_error(oss.str());
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "Failed to parse:" << input.data() << " at " << input.pos() << "\n";
        std::cout << "Exception " << e.what() << "\n";
        delete currentNode;
        currentNode = nullptr;
    }

    return currentNode;
}


Node *findLeftNeighbour(Node *search)
{
    auto *child = search;
    auto *parent = search->parent;
    while (parent && parent->leftNode == child)
    {
        child = parent;
        parent = parent->parent;
    }

    if (!parent)
        return parent;

    auto *res = parent->leftNode;
    while (res && (res->leftNode || res->rightNode))
        res = res->rightNode;
    return res;
}


Node *findRightNeighbour(Node *search)
{
    auto *child = search;
    auto *parent = search->parent;
    while (parent && parent->rightNode == child)
    {
        child = parent;
        parent = parent->parent;
    }

    if (!parent)
        return parent;

    auto *res = parent->rightNode;
    while (res && (res->leftNode || res->rightNode))
        res = res->leftNode;
    return res;
}


Node *findPairAtDepth(Node *search, int depth)
{
    if (depth <= 0 &&
        search->leftNode && search->leftNode->isValueNode() &&
        search->rightNode && search->rightNode->isValueNode())
    {
        return search;
    }

    if (search->leftNode)
    {
        Node *left = findPairAtDepth(search->leftNode, depth-1);
        if (left)
            return left;
    }

    if (search->rightNode)
    {
        Node *right = findPairAtDepth(search->rightNode, depth-1);
        if (right)
            return right;
    }

    return nullptr;
}


Node *findNumberGreaterThan9(Node *search)
{
    if (search->isValueNode())
    {
        if (search->value > 9)
            return search;
    }

    else
    {
        if (search->leftNode)
        {
            Node *left = findNumberGreaterThan9(search->leftNode);
            if (left)
                return left;
        }

        if (search->rightNode)
        {
            Node *right = findNumberGreaterThan9(search->rightNode);
            if (right)
                return right;
        }
    }

    return nullptr;
}


Node *reduce(Node *node)
{
    // To reduce a snailfish number
    // - If any pair is nested inside four pairs, the leftmost pair explodes
    // - If a regular number is 10 or greater, leftmost regular number splits

    // Loop until nothing left to reduce
    while (true)
    {
        // Look for a pair which is four (or more) levels deep. If we find one,
        // we need to explode it
        if (Node *pairNode = findPairAtDepth(node, 4))
        {
            // Explode the node. Search left for the node with a number.
            Node *left = findLeftNeighbour(pairNode);
            Node *right = findRightNeighbour(pairNode);

            // Do the explode
            if (left)
                left->value += pairNode->leftNode->value;

            if (right)
                right->value += pairNode->rightNode->value;

            delete pairNode->leftNode;
            pairNode->leftNode = nullptr;
            delete pairNode->rightNode;
            pairNode->rightNode = nullptr;
            pairNode->value = 0;
        }

        // Look for a number which is greater than 9. If we find one, we need
        // to split it
        else if (Node *largeNode = findNumberGreaterThan9(node))
        {
            largeNode->leftNode = new Node;
            largeNode->leftNode->value = largeNode->value / 2;
            largeNode->leftNode->parent = largeNode;
            largeNode->rightNode = new Node;
            largeNode->rightNode->value = (largeNode->value + 1) / 2;
            largeNode->rightNode->parent = largeNode;
            largeNode->value = 0;
        }

        else
        {
            break;
        }
    }

    return node;
}


int calculateMagnitude(Node *node)
{
    if (node->isValueNode())
        return node->value;

    int leftValue{0};
    if (node->leftNode)
        leftValue = calculateMagnitude(node->leftNode);

    int rightValue{0};
    if (node->rightNode)
        rightValue = calculateMagnitude(node->rightNode);

    return leftValue*3 + rightValue*2;
}


int main(int, char **)
{
    Node *currentNode = nullptr;

    std::string line;
    while (std::cin >> line)
    {
        // Read a line from the file to create the snailfish node
        Stream stream(line);
        Node *node = readNode(stream);

        // If we have a current node, we need to add this new snailfish node
        // to it. This basically means create a parent with the old node as
        // the left child, and the new node as the right child
        if (currentNode)
        {
            Node *newNode = new Node;
            newNode->leftNode = currentNode;
            newNode->leftNode->parent = newNode;
            newNode->rightNode = node;
            newNode->rightNode->parent = newNode;
            currentNode = newNode;
        }

        // If this is the first node, just set as the parent node
        else
        {
            currentNode = node;
        }

        // Now we need to reduce the snailfish number
        currentNode = reduce(currentNode);
    }

    // Calculate magnitude
    std::cout << "final " << *currentNode << "\n";
    std::cout << "Magnitude " << calculateMagnitude(currentNode) << "\n";
    delete currentNode;

    return 0;
}

