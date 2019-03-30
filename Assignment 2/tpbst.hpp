#ifndef TWOPHASEBST_H
#define TWOPHASEBST_H

#include <iostream>
#include <string>
#include <stack>
#include <list>
// do not add any other library.
// modify parts as directed by assignment text and comments here.

template <class T>
class TwoPhaseBST {
private: //do not change
    struct SecondaryNode {
        std::string key;
        T data;
        SecondaryNode *left;
        SecondaryNode *right;

        SecondaryNode(const std::string &k, const T &d, SecondaryNode *l, SecondaryNode *r);
    };

    struct PrimaryNode {
        std::string key;
        PrimaryNode *left;
        PrimaryNode *right;
        SecondaryNode *rootSecondaryNode;

        PrimaryNode(const std::string &k, PrimaryNode *l, PrimaryNode *r, SecondaryNode *rsn);
    };

public: // do not change.
    TwoPhaseBST();
    ~TwoPhaseBST();

    TwoPhaseBST &insert(const std::string &primaryKey, const std::string &secondaryKey, const T &data);
    TwoPhaseBST &remove(const std::string &primaryKey, const std::string &secondaryKey);
    TwoPhaseBST &print(const std::string &primaryKey = "", const std::string &secondaryKey = "");
    T *find(const std::string &primaryKey, const std::string &secondaryKey);

private: // you may add your own utility member functions here.
    void destructNode(PrimaryNode * &root);
    void destructNode(SecondaryNode * &root);

    PrimaryNode* findNode(PrimaryNode *root, const std::string &key) {
        // Binary search on the given binary search tree.
        if (!root) return NULL;
        if (key == root->key)
            return root;
        if (key > root->key)
            return findNode(root->right, key);
        else
            return findNode(root->left, key);
    }

    SecondaryNode* findNode(SecondaryNode *root, const std::string &key) {
        // Binary search on the given binary search tree.
        if (!root) return NULL;
        if (key == root->key)
            return root;
        if (key > root->key)
            return findNode(root->right, key);
        else
            return findNode(root->left, key);
    }

    SecondaryNode* findParent(SecondaryNode *root, const std::string &key) {
        // Function returns the pointer to the parent of the node with given key.
        if (!root) return NULL;
        if (key == root->key) return NULL;
        if ((root->right && key == root->right->key) || (root->left && key == root->left->key))
            return root;
        else if (key < root->key)
            return findParent(root->left, key);
        else
            return findParent(root->right, key);
    }

    // Recursive overloaded function to insert a node to the primary tree.
    void insertNode(PrimaryNode *root, PrimaryNode *node);
    void insertNode(SecondaryNode *root, SecondaryNode *node);

    // Helper functions for print()
    void printPrimaryTree(const PrimaryNode *root);
    void printSecondaryTree(const SecondaryNode *root);


private: // do not change.
    PrimaryNode *root; //designated root.

    // do not provide an implementation. TwoPhaseBST's are not copiable.
    TwoPhaseBST(const TwoPhaseBST &);
    const TwoPhaseBST &operator=(const TwoPhaseBST &);
};

template <class T>
TwoPhaseBST<T>::SecondaryNode::SecondaryNode(const std::string &k, const T &d, SecondaryNode *l, SecondaryNode *r)
        : key(k), data(d), left(l), right(r) {}

template <class T>
TwoPhaseBST<T>::PrimaryNode::PrimaryNode(const std::string &k, PrimaryNode *l, PrimaryNode *r, SecondaryNode *rsn)
        : key(k), left(l), right(r), rootSecondaryNode(rsn) {}

template <class T>
TwoPhaseBST<T>::TwoPhaseBST() : root(NULL) {}

template <class T>
TwoPhaseBST<T>::~TwoPhaseBST() {
    destructNode(root);
}

template <class T>
TwoPhaseBST<T> &
TwoPhaseBST<T>::insert(const std::string &primaryKey, const std::string &secondaryKey, const T &data) {
    /* IMPLEMENT THIS */
    SecondaryNode *newSecondaryNode;
    newSecondaryNode = new SecondaryNode(secondaryKey, data, NULL, NULL);
    if (!root) {
        root = new PrimaryNode(primaryKey, NULL, NULL, newSecondaryNode);
    }
    else { // Search for the primary node
        PrimaryNode* primaryNode = findNode(root, primaryKey);
        if (primaryNode) { // Primary node exists with the given primaryKey.
            insertNode(primaryNode->rootSecondaryNode, newSecondaryNode);
        }
        else { // Primary node does not exist, create one.
            PrimaryNode *newPrimaryNode;
            newPrimaryNode = new PrimaryNode(primaryKey, NULL, NULL, newSecondaryNode);
            insertNode(root, newPrimaryNode);
        }
    }
    return *this;
}

template <class T>
TwoPhaseBST<T> &
TwoPhaseBST<T>::remove(const std::string &primaryKey, const std::string &secondaryKey) {
    /* IMPLEMENT THIS */
    PrimaryNode *primaryNode = findNode(root, primaryKey);
    if (!primaryNode) return *this;
    SecondaryNode *nodeToDelete = findNode(primaryNode->rootSecondaryNode, secondaryKey);
    if (!nodeToDelete) return *this;

    // Deletion case where the node has 2 children
    if (nodeToDelete->left && nodeToDelete->right) {
        // Find the in-order successor of the node
        SecondaryNode *inOrderSuccessor = nodeToDelete->right;
        while (inOrderSuccessor->left) inOrderSuccessor = inOrderSuccessor->left;

        // Find inOrderSuccessor's parent and set the related child to null.
        SecondaryNode *parent = findParent(primaryNode->rootSecondaryNode, inOrderSuccessor->key);
        if (parent->left == inOrderSuccessor)
            parent->left = NULL;
        else
            parent->right = NULL;

        // Set inOrderSuccessor's child pointers to nodeToDelete's children.
        // If inOrderSuccessor is nodeToDelete's child, do nothing.
        inOrderSuccessor->left = nodeToDelete->left;
        if (nodeToDelete->right != inOrderSuccessor)
            inOrderSuccessor->right = nodeToDelete->right;

        // If nodeToDelete is root, set inOrderSuccessor as the new root.
        // Otherwise set inOrderSuccessor as a child of nodeToDelete's parent
        parent = findParent(primaryNode->rootSecondaryNode, nodeToDelete->key);
        if (!parent) // nodeToDelete is the root node.
            primaryNode->rootSecondaryNode = inOrderSuccessor;
        else { // nodeToDelete is not root, set the pointers of its parent
            if (parent->left == nodeToDelete)
                parent->left = inOrderSuccessor;
            else
                parent->right = inOrderSuccessor;
        }

        // Set nodeToDelete's children to null. This is necessary since destruction is
        // done recursively.
        nodeToDelete->right = NULL;
        nodeToDelete->left = NULL;

        // Destruct nodeToDelete
        destructNode(nodeToDelete);
    }

    // Deletion case where the node has 1 child
    else if (nodeToDelete->left || nodeToDelete->right) {
        SecondaryNode *parent = findParent(primaryNode->rootSecondaryNode, secondaryKey);
        if (parent) { // A non-root node will be deleted.
            // Set parent's pointer
            if (parent->left == nodeToDelete) {
                if (nodeToDelete->left) {
                    parent->left = nodeToDelete->left;
                    nodeToDelete->left = NULL;
                    // This assignment is required since destruction is done recursively.
                }
                else {
                    parent->left = nodeToDelete->right;
                    nodeToDelete->right = NULL;
                }
            }
            else {
                if(nodeToDelete->right) {
                    parent->right = nodeToDelete->right;
                    nodeToDelete->right = NULL;
                }
                else {
                    parent->right = nodeToDelete->left;
                    nodeToDelete->left = NULL;
                }
            }
        }
        else { // Root will be deleted.
            // Set the new root
            if (nodeToDelete->left) { // Root (nodeToDelete) has only left child
                primaryNode->rootSecondaryNode = nodeToDelete->left;
                nodeToDelete->left = NULL;
            }
            else {
                primaryNode->rootSecondaryNode = nodeToDelete->right;
                nodeToDelete->right = NULL;
            }
        }

        destructNode(nodeToDelete);
    }

    // Deletion case where the node is a leaf
    else {
        SecondaryNode *parent = findParent(primaryNode->rootSecondaryNode, secondaryKey);
        if (parent) { // If the node is not the only node, set parent's pointer to NULL
            if (parent->left == nodeToDelete)
                parent->left = NULL;
            else
                parent->right = NULL;
        }
        else {
            primaryNode->rootSecondaryNode = NULL;
        }
        destructNode(nodeToDelete);
    }
    return *this;
}

template <class T>
TwoPhaseBST<T> &TwoPhaseBST<T>::print(const std::string &primaryKey, const std::string &secondaryKey) {
    /* IMPLEMENT THIS */
    if (primaryKey.empty() && secondaryKey.empty()) { // print everything
        std::cout << "{";
        printPrimaryTree(root);
        std::cout << "}" << std::endl;
    }
    else if (!secondaryKey.empty() && !primaryKey.empty()) { // print a specific item
        std::cout << "{";
        PrimaryNode *primaryNode = findNode(root, primaryKey);
        if (primaryNode) {
            SecondaryNode *secondaryNode = findNode(primaryNode->rootSecondaryNode, secondaryKey);
            if (secondaryNode) {
                std::cout << "\"" << primaryNode->key << "\" : {";
                std::cout << "\"" << secondaryNode->key << "\" : \"" << secondaryNode->data << "\"}";
            }
        }
        std::cout << "}" << std::endl;
    }
    else if (secondaryKey.empty()) { // Print information about the primary key
        std::cout << "{";
        PrimaryNode *primaryNode = findNode(root, primaryKey);
        if (primaryNode) {
            std::cout << "\"" << primaryNode->key << "\"" << " : {";
            printSecondaryTree(primaryNode->rootSecondaryNode);
            std::cout << "}";
        }
        std::cout << "}" << std::endl;
    }
    // If secondary key is given and primary key is not given, do nothing.

    return *this;
}

template <class T>
T *TwoPhaseBST<T>::find(const std::string &primaryKey, const std::string &secondaryKey) {
    /* IMPLEMENT THIS */
    PrimaryNode *primaryNode = findNode(root, primaryKey);
    if (!primaryNode) return NULL;
    SecondaryNode *secondaryNode = findNode(primaryNode->rootSecondaryNode, secondaryKey);
    if (!secondaryNode) return NULL;
    return &secondaryNode->data;
}

template <class T>
void TwoPhaseBST<T>::destructNode(TwoPhaseBST::PrimaryNode * &root)
{
    if (root == NULL)
        return;

    destructNode(root->left);
    destructNode(root->right);

    destructNode(root->rootSecondaryNode);

    delete root;

    root = NULL;
}

template <class T>
void TwoPhaseBST<T>::destructNode(TwoPhaseBST::SecondaryNode * &root)
{
    if (root == NULL)
        return;

    destructNode(root->left);
    destructNode(root->right);

    delete root;

    root = NULL;
}


template<class T>
void TwoPhaseBST<T>::insertNode(TwoPhaseBST::PrimaryNode *root,
                                TwoPhaseBST::PrimaryNode *node) {
    // Function inserts the given primary node to the given tree
    if (!root) return;
    if (node->key == root->key){} // Normally, this case should not happen.
    if (node->key > root->key) {
        if (root->right)
            insertNode(root->right, node);
        else // Place to insert
            root->right = node;
    }
    else {
        if (root->left)
            insertNode(root->left, node);
        else // Place to insert
            root->left = node;
    }
}

template<class T>
void TwoPhaseBST<T>::insertNode(TwoPhaseBST::SecondaryNode *root,
                                TwoPhaseBST::SecondaryNode *node) {
    // Function inserts the given secondary node to the given tree.
    if (!root) return;
    if (node->key == root->key){} // Normally, this case should not happen.
    if (node->key > root->key) {
        if (root->right)
            insertNode(root->right, node);
        else // Found the place to insert
            root->right = node;
    }
    else {
        if (root->left)
            insertNode(root->left, node);
        else // Found the place to insert
            root->left = node;
    }
}

template<class T>
void TwoPhaseBST<T>::printPrimaryTree(const TwoPhaseBST::PrimaryNode *root) {
    // Function prints the primary tree of the given root.
    if (!root) return;
    if (root->left) {
        printPrimaryTree(root->left);
        std::cout << ", ";
    }
    std::cout << "\"" << root->key << "\"" << " : " << "{";
    printSecondaryTree(root->rootSecondaryNode);
    std::cout << "}";
    if (root->right) {
        std::cout << ", ";
        printPrimaryTree(root->right);
    }
}

template<class T>
void
TwoPhaseBST<T>::printSecondaryTree(const TwoPhaseBST::SecondaryNode *root) {
    // Function prints the secondary tree of the given root.
    if (!root) return;
    if (root->left) {
        printSecondaryTree(root->left);
        std::cout << ", ";
    }
    std::cout << "\"" << root->key << "\"" << " : " << "\"" << root->data << "\"";
    if (root->right) {
        std::cout << ", ";
        printSecondaryTree(root->right);
    }
}


#endif //TWOPHASEBST_H
