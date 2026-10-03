// Unit test: Model / TreeNode structure handling (append, insert, remove, paths).
#include <cstdio>
#include <string>
#include <vector>

#include "master.h"
#include "model.h"

/** Prints the message and returns 1 when the condition is false. */
static int expect(bool condition, char const * message)
{
	if (condition) return 0;
	std::fprintf(stderr, "%s\n", message);
	return 1;
}

/** Appends a one-string row to the model. */
static void add_row(xgui::Model * model, char const * text)
{
	std::vector<std::string> row;
	row.push_back(text);
	model->appendChild(row);
}

int main()
{
	int failures = 0;
	xgui::Model * root = xgui::Master::CreateModel();
	xgui::Model * extra = xgui::Master::CreateModel();
	extra->addString("inserted");

	add_row(root, "a");
	add_row(root, "b");
	add_row(root, "c");
	failures += expect(root->size() == 3, "size after three appends");
	failures += expect(root->getChild(1)->getString(0) == "b", "getChild(1) text");
	failures += expect(root->getChild(3) == 0 && root->getChild(-1) == 0, "out of range getChild returns NULL");
	failures += expect(root->getChild(0)->getParent() == root, "child parent pointer");

	xgui::Model * second = root->getChild(1);
	failures += expect(root->findChildPos(second) == 1, "findChildPos");
	failures += expect(root->findByPath(second->path()) == second, "findByPath(path()) returns the node");
	failures += expect(second->path().find('/') != std::string::npos, "child path contains the parent id");

	root->insertChild(extra, 1);
	failures += expect(root->size() == 4 && root->getChild(1) == extra, "insertChild in the middle");
	failures += expect(root->getChild(2) == second, "insertChild shifts the following children");

	root->removeChild(1);
	failures += expect(root->size() == 3 && root->getChild(1) == second, "removeChild keeps the order");

	root->removeAllChildren();
	failures += expect(root->size() == 0, "removeAllChildren");

	return failures ? 1 : 0;
}
