#pragma once
// the "//" beside each function was for me to check off finished functions!
class AVL_Tree
{
private:
    struct Node {
        int data;
        Node* left;
        Node* right;
        int height;
    };
    Node* root;

    Node* insert(int, Node*); //
    Node* singleRightRotate(Node*&); //
    Node* singleLeftRotate(Node*&); //
    Node* doubleLeftRightRotate(Node*&); //
    Node* doubleRightLeftRotate(Node*&); //
    Node* findMin(Node*); //
    Node* findMax(Node*); //
    Node* remove(int, Node*); //
    Node* getRoot(); //
    Node* parent(Node*, int); //

    int height(Node*); //
    int getBalance(Node*); //
    int childCount(Node*); //
    int size(); //

    bool isRoot(Node*); //
    bool isInternal(Node*); //
    bool isExternal(Node*); //
    bool search(Node*, int); //

    void inOrder(Node*); //
    void postOrder(Node*); //
    void preOrder(Node*); //

    // Helper Functions and Variables
    int numNodes; //
    bool isBalanced(Node*); //
    bool checkFull(Node*); //
    bool checkComplete(Node*, int); //
    void deleteTree(Node*); //
    Node* treeCopier(const Node*); //

public:
    AVL_Tree(); //
    AVL_Tree(const AVL_Tree&); //
    AVL_Tree& operator=(const AVL_Tree&); // 
    ~AVL_Tree(); //

    bool isFull(); //
    bool isComplete(); //
    bool isAVL(); //
     
    void insert(int); //
    void remove(int); //

    // Helper Functions
    void display(); //
    void printSize(); //
    void findValue(int); //
    void printRoot(); //
};

