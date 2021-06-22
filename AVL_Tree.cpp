#include <iostream>
#include <cmath>
#include <algorithm>
#include <exception>
#include <new>

#include "AVL_Tree.h"

using namespace std;


//  Constructor
AVL_Tree::AVL_Tree()
{
	root = NULL; // Set root to NULL
	int numNodes = 0; // Set number of nodes to NULL
}

// Copy Constructor
AVL_Tree::AVL_Tree(const AVL_Tree &source)
	: root (treeCopier(source.root)) // Copy tree
{
	this->numNodes = source.numNodes; // Copy number of nodes
}

// Assignment Operator
AVL_Tree& AVL_Tree::operator=(const AVL_Tree& source)
{
	if (this == &source) // Check for self assignment
		return *this;

	deleteTree(root); // Reallocate 
	treeCopier(source.root); // Deep copy
	this->numNodes = source.numNodes; // Copy number of nodes
	return *this;
}

// Destructor
AVL_Tree::~AVL_Tree()
{
	deleteTree(root); // Delete tree
}

// Insert function (private)
AVL_Tree::Node* AVL_Tree::insert(int value, Node* node)
{
	try
	{
		if (node == NULL) // If pointer is empty, then insert node at the leaf
		{
			node = new Node;
			node->data = value;
			node->left = NULL;
			node->right = NULL;
			node->height = 0;

			if (root == NULL)
			{
				root = node; // If root doesn't exist, then node becomes the root
			}

			numNodes++; // Increase number of nodes

			return node;
		}
		else if (value < node->data) // If data is less than current nodes data
		{
			node->left = insert(value, node->left); // Then move onto the left subtree of current node and insert
		}
		else if (value > node->data) // If data is greater than current nodes data
		{
			node->right = insert(value, node->right); // Then move onto the right subtree of current node and insert
		}
		else
		{
			throw 1; // If value already exists in the tree, print error
		}

		// Update the height of node
		node->height = 1 + max(height(node->left),height(node->right));

		// Check AVL properties
		int balance = getBalance(node); // Find the balance factor of node

		if (balance == 2)
		{
			if (node->left != NULL)
				if (node->left->left == NULL) // Is it a left right case?
					node = doubleLeftRightRotate(node);
				else						  // Is it a left left case?
					node = singleRightRotate(node);
			else if (node->right != NULL)
				if (node->right->right == NULL) // Is it a right left case?
					node = doubleRightLeftRotate(node);
				else
					node = singleLeftRotate(node);
		}

		return node;
	}
	catch (int)
	{
		cout << "Value already exists in the tree. Choose a different value!" << endl;
		return node;
	}
}

// Remove function (private)
AVL_Tree::Node* AVL_Tree::remove(int value, Node* node)
{
	try
	{
		Node* temp;

		if (node == NULL) // If tree is empty, throw exception
			throw 'a';
		else if (node->data > value) // Search left subtree if value is smaller than nodes data
			node->left = remove(value, node->left); 
		else if (node->data < value) // Search right subtree if value is greater than nodes data
			node->right = remove(value, node->right);
		else
		{
			if (node->left != NULL && node->right != NULL) // Node with two children
			{
				temp = findMin(node->right); // Find smallest successor
				node->data = temp->data; // Replace nodes data with smallest successors data
				node->right = remove(node->data, node->right); // Go into right subtree to find copied node for deletion
			}
			else // Node with one or no child
			{
				temp = node; 
				if (isExternal(temp)) // If node is leaf - No child
					node = NULL;
				else // If node has a child
					*node = *temp; // Copy data from temp onto node.

				delete temp;
				numNodes--; // Decrease number of nodes
			}
		}

		if (node == NULL) // If node is NULL then return and go back up the tree
			return node;

		node->height = max(height(node->left), height(node->right))+1; // Adjust heights as you go through

		int balance = getBalance(node); // Find balance factor
		
		// Left Left Case  
		if (balance > 1 && value < node->left->data)
			return singleRightRotate(node);

		// Right Right Case  
		if (balance < -1 && value > node->right->data)
			return singleLeftRotate(node);

		// Left Right Case  
		if (balance > 1 && value > node->left->data)
		{
			node->left = singleLeftRotate(node->left);
			return singleRightRotate(node);
		}

		// Right Left Case  
		if (balance < -1 && value < node->right->data)
		{
			node->right = singleRightRotate(node->right);
			return singleLeftRotate(node);
		}

		return node;		
	}
	catch (char)
	{
		cout << "Tree is empty!" << endl;
		return NULL;
	}
}

// Remove function (public)
void AVL_Tree::remove(int value)
{
	root = remove(value, root);
}

// Find height of node
int AVL_Tree::height(Node* node)
{
	if (node == NULL)
	{
		return -1; // If node doesnt exist, return -1
	}

	return (max(height(node->left), height(node->right))+1); // Return height of node
}

// Find balance factor of node
int AVL_Tree::getBalance(Node* node)
{
	if (node == NULL)
		return 0; // If node doesnt exist, return 0

	int BF = height(node->left) - height(node->right); // Balance Factor formula
	return BF; // Return absolute value of BF because the insert function will try to see if BF is equal to 2 (BF could be -2)
}

// Single left rotation function
AVL_Tree::Node* AVL_Tree::singleLeftRotate(Node* &node)
{
	Node* rchild = node->right; // Create a new pointer for the right child of current node

	if (node == root)
	{
		root = rchild; // If current node is the root, we need to assign the root-to-be after rotation, which is the right child
	}

	node->right = rchild->left; // Left child of rchild becomes current nodes new right child
	rchild->left = node; // Rotate left

	rchild->height = max(height(rchild->left), height(rchild->right)) + 1; // Assign new heights for rchild
	node->height = max(height(node->left), height(node->right)) + 1; // Assign new heights for current node
	
	return rchild; // Return rotated subtree
}

// Single left rotation function
AVL_Tree::Node* AVL_Tree::singleRightRotate(Node* &node)
{
	Node* lchild = node->left; // Create a new pointer for the left child of current node

	if (node == root)
	{
		root = lchild; // If current node is the root, then we need to assign the root-to-be after rotation, which is the left child
	}

	node->left = lchild->right; // Right child of lchild becomes current nodes left child
	lchild->right = node; // Rotate right

	lchild->height = max(height(lchild->left), height(lchild->right)) + 1; // Assign new heights for lchild
	node->height = max(height(node->left), height(node->right)) + 1; // Assign new heights for current node

	return lchild; // Return rotated subtree
}

// Left Right rotation function
AVL_Tree::Node* AVL_Tree::doubleLeftRightRotate(Node*& node)
{
	node->left = singleLeftRotate(node->left); // Left child's subtree is rotated left

	node = singleRightRotate(node); // Node is rotated right

	return node; // Return rotated subtree
}

// Right Left rotation function
AVL_Tree::Node* AVL_Tree::doubleRightLeftRotate(Node*& node)
{
	node->right = singleRightRotate(node->right); // Right child's subtree is rotated right

	node = singleLeftRotate(node); // Node is rotated left
	
	return node; // Return rotated subtree
}

// Insert function (public)
void AVL_Tree::insert(int value)
{
	root = insert(value, root); // Start at root and insert value
}

// Display function - Displays all traversal orders of the tree
void AVL_Tree::display()
{
	try
	{
		if (root == NULL)
			throw 4; // If tree is empty, then throw exception

		cout << "preOrder Traversal: "; preOrder(root); cout << "." << endl;

		cout << "inOrder Traversal: "; inOrder(root); cout << "." << endl;

		cout << "postOrder Traversal: "; postOrder(root); cout << "." << endl;

	}
	catch (int)
	{
		cout << "Tree is empty! Insert values first." << endl;
	}
}

// Preorder Traversal function
void AVL_Tree::preOrder(Node* node)
{
	if (node != NULL)
	{
		cout << node->data << " ";
		preOrder(node->left);
		preOrder(node->right);
	}
}

// Inorder Traversal function
void AVL_Tree::inOrder(Node* node)
{
	if (node != NULL)
	{
		if (isInternal(node))
		{
			inOrder(node->left);
		}
		cout << node->data << " ";
		if (isInternal(node))
		{
			inOrder(node->right);
		}
	}
}

// Postorder Traversal function
void AVL_Tree::postOrder(Node* node)
{
	if (node != NULL)
	{
		postOrder(node->left);
		postOrder(node->right);
		cout << node->data << " ";
	}
}

// Find out if node is an external node
bool AVL_Tree::isExternal(Node* node)
{
	try
	{
		if (root == NULL)
			throw 5; // If tree is empty...

		if (childCount(node) == 0) // If node is a leaf...
			return true; // Then it is an external node
		else
			return false; // Else, it must be internal
	}
	catch (int)
	{
		cout << "Tree is empty! Insert values first." << endl;
		return false;
	}
}

// Find out if node is an internal node
bool AVL_Tree::isInternal(Node* node)
{
	try
	{
		if (root == NULL)
			throw 6; // If tree is empty...

		if (childCount(node) >= 1) // If node has more than one child...
			return true; // Then it is an internal node
		else
			return false; // Else, it must be external

	}
	catch (int)
	{
		cout << "Tree is empty! Insert values first." << endl;
		return false;
	}
}

// Find size of AVL tree
int AVL_Tree::size()
{
	try
	{
		if (root == NULL)
			throw 7; // If tree is empty...
		else
			return numNodes;
	}
	catch(int)
	{
		cout << "Tree is empty!" << endl;
		return 0;
	}
}

// Prints size of AVL tree
void AVL_Tree::printSize()
{
	cout << "There are " << size() << " nodes in the AVL tree" << endl;
}

// Find highest value in the tree
AVL_Tree::Node* AVL_Tree::findMax(Node* node)
{
	try
	{
		if (root == NULL)
			throw 2;
		else if (node->right == NULL)
			return node; // If node doesnt have a value higher than it, then it must be the highest value in the tree
		else
			return findMax(node->right); // Else, it go to the right child
	}
	catch (int)
	{
		cout << "Tree is empty!" << endl;
		return NULL;
	}
}

// Find lowest value in the tree
AVL_Tree::Node* AVL_Tree::findMin(Node* node)
{
	try
	{
		if (root == NULL)
			throw 3;
		else if (node->left == NULL)
			return node; // If node doesnt have a left child, then it must be the lowest value of tree
		else 
			return findMin(node->left); // Else, go to its left child
	}
	catch (int)
	{
		cout << "Tree is empty!" << endl;
		return NULL;
	}
}

// Finds the number of children of node
int AVL_Tree::childCount(Node* node)
{
	if (node->left != NULL && node->right != NULL) // If node has a left and right child...
		return 2;
	else if (node->right == NULL && node->left == NULL) // If node is a leaf...
		return 0;
	else // If node has only one child...
		return 1;
}

// Gets root
AVL_Tree::Node* AVL_Tree::getRoot()
{
	return root;
}

// Find value in tree
void AVL_Tree::findValue(int value)
{
	try
	{
		if (root == NULL)
			throw 8;

		if (search(root, value)) // If value exists in the tree...
			cout << value << " exists in the tree." << endl;
		else // Else its not in the tree
			cout << value << " does not exist in the tree." << endl;

		if (root->data == value) // Check if value is stored in the root. If so, then it doesnt have a parent
			cout << value << "'s node has no parent." << endl;
		
			
	}
	catch (int)
	{
		cout << "Tree is empty!" << endl;
	}
}

// Searches through the tree for inputted value
bool AVL_Tree::search(Node* node, int value)
{
	if (node == NULL) // Value doesnt exist
		return false;

	if (parent(node,value) != NULL) // If the parent function found the parent of node with value
		cout << value << "'s parent contains the value of " << node->data << endl; // then print value of parent

	if (node->data == value) // If value was found in tree
	{
		if (isRoot(node)) // Check if it is stored in the root
			cout << value << " is stored in the root!" << endl;

		return true; // Return true if value exists in tree
	}
	else if (node->data > value)
		return search(node->left, value); // Check left subtree if value is less than current nodes data
	else if (node->data < value)
		return search(node->right, value); // Check right subtree if value is greater than current nodes data

	return false; // Return false if none of the above conditions are satisfied

}

// Finds the nodes parent
AVL_Tree::Node* AVL_Tree::parent(Node* node, int value)
{
	if(node->left != NULL) 
		if (node->left->data == value)
			return node; // Return parent of node with value
	
	if (node->right != NULL)
		if (node->right->data == value)
			return node; // Return parent of node with value

	return NULL; // If parent's children dont contain value, then return null
}

// Finds out if node is the root
bool AVL_Tree::isRoot(Node* node)
{
	if (node == root)
		return true;
	else
		return false;
}

// Prints the root of tree
void AVL_Tree::printRoot()
{
	cout << "The root contains the value of: " << getRoot()->data << endl;
}

// Checks if tree is an AVL tree (public)
bool AVL_Tree::isAVL()
{
	try
	{
		Node* rootNode = getRoot();

		if (rootNode == NULL)
			throw 11;

		return isBalanced(rootNode); // Check if nodes are balanced

	}
	catch (int)
	{
		cout << "Tree is Empty!" << endl;
		return true; // Tree is AVL
	}
}

// Checks if tree is a full tree (public)
bool AVL_Tree::isFull()
{
	try
	{
		Node* rootNode = getRoot();

		if (rootNode == NULL)
			throw 12;

		return checkFull(rootNode); // Check if subtrees are full

	}
	catch (int)
	{
		cout << "Tree is Empty!" << endl;
		return true; // Tree is full
	}

}

// Checks if tree is a complete tree
bool AVL_Tree::isComplete()
{
	try
	{
		Node* rootNode = getRoot();

		if (rootNode == NULL)
			throw 13;

		return checkComplete(rootNode, 1); // Check if subtrees are complete

	}
	catch (int)
	{
		cout << "Tree is Empty!" << endl;
		return true; // Tree is complete
	}
}


// Checks if node is AVL balanced
bool AVL_Tree::isBalanced(Node* node)
{
	int BF; // Balance Factor
	if (node != NULL)
	{
		BF = getBalance(node); // Find balance factor

		if (BF == 2 && !isBalanced(node->left) && !isBalanced(node->right)) // If balance factor of node and its subtrees are equal to 2...
			return false; // ...Then tree is not AVL
	}

	return true; // Else it is
}

// Checks if subtree is full
bool AVL_Tree::checkFull(Node* node)
{
	if (isExternal(node)) // If subtree is a leaf then it is full
		return true;
	else if (node->left != NULL && node->right != NULL) // If tree has two children, check if the subtree has 1 child
		if (!checkFull(node->left) || !checkFull(node->right))
			return false; // If they have one child, then tree is not full
		else
			return true;

	return false; // If all conditions are passed, then tree must be empty
}

// Checks if subtree is complete
bool AVL_Tree::checkComplete(Node* node, int index)
{
	if (node == NULL)
		return true;

	if (index > numNodes) // If index is bigger than the number of nodes in tree, then tree is not complete
		return false;

	if (!checkComplete(node->left, index * 2) || !checkComplete(node->right, index * 2 + 1)) 
		return false; // Check left and right subtrees to see if index is larger than number of nodes in tree

	return true; // If all conditions are passed, then tree must be complete
}

// Deletes entire tree
void AVL_Tree::deleteTree(Node* node)
{
	if (node != NULL)
	{
		deleteTree(node->left); // Recurse into left subtree...
		deleteTree(node->right); // ...Then right subtree...
		delete node; // ...and delete subtrees
	}
}

// Copies entire tree
AVL_Tree::Node* AVL_Tree::treeCopier(const Node* source)
{
	if (source == NULL) // If node is empty then return NULL
		return NULL;

	Node* temp = new Node; // Create new node for new tree (Deep copy)
	temp->data = source->data; // Save original node's data into copy tree's node
	temp->left = treeCopier(source->left); // Recurse for left...
	temp->right = treeCopier(source->right); // ... and right subtrees
	return temp; // Return final subtree/tree
}