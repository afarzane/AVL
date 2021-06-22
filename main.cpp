#include <iostream>
#include <ctime>

#include "AVL_Tree.h"

using namespace std;

int main()
{
	AVL_Tree t;

	t.insert(20);
	t.insert(21);
	t.insert(22);
	t.insert(23);
	t.insert(24);
	t.insert(25);

	t.display();

	system("pause");

	return 0;

}