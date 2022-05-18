#include <iostream>
#include <string>
#include <assert.h>
#include "levellist.h"
using std::cout ;
using std::endl;
using std::string;
LevelList::Node::Node( string data, int level)
{
	this->data = data;
	this->forward = new Block[level + 1];

	for (int i = 0; i <= level; i++)
	{
		this->forward[i].next = NULL;
		this->forward[i].holds = 1;
	}
}

LevelList::Node::~Node()
{
	delete[] forward;
}

LevelList::LevelList(int MAXLVL, float P)
{
	this->MAXLVL = MAXLVL;
	this->P = P;
	level = 0;
	length = 0;
	head = new LevelList::Node::Block[MAXLVL];
	current = NULL;
	currentIndex = 0;
	modified = true;
	for (int i{}; i < MAXLVL; i++)
	{
		head[i].holds = 1;
		head[i].next = NULL;
	}
	tail = new LevelList::Node * [MAXLVL];
	for (int i{}; i < MAXLVL; i++)
	{
		tail[i] = 0;
		tail[i] = NULL;
	}
}

LevelList::~LevelList()
{
}

int LevelList::len()
{
	return this->length;
}

LevelList::Node* LevelList::createNode( string key, int level)
{
	LevelList::Node* n = new Node(key, level);
	return n;
}

int LevelList::randomLevel()
{
	int lvl = 0;
	while ((float)rand() / (float)RAND_MAX < P && lvl < MAXLVL - 1)
		lvl++;
	return lvl;
}

string& LevelList::find(int index)
{
	this->current = head;
	this->currentIndex = index;

	for (int i = level; i >= 0; i--)
	{
		while (current[i].holds <= index)
		{
			index -= this->current[i].holds;
			this->current = current[i].next->forward;
		}
	}
	return this->current->next->data;
}

string& LevelList::operator [](int index)
{
	assert(index >= 0);
	assert(this->length >= index);

	if (modified)
	{
		this->modified = false;
		return find(index);
	}
	else
	{
		if (index == currentIndex + 1)
		{
			currentIndex += 1;
			this->current = current->next->forward;
			return this->current->next->data;
		}
		else if (index == currentIndex)
		{
			return this->current->next->data;
		}
		else
		{
			return find(index);
		}
	}
}

void LevelList::push(string line)
{
	int rlevel = randomLevel();
	LevelList::Node* n = new Node(line, rlevel);

	if (rlevel > level)
	{
		for (int i = level + 1; i < rlevel + 1; i++)
		{
			head[i].holds = length + 1;
			head[i].next = n;
		}
		level = rlevel;
	}
	if (level > rlevel)
	{
		for (int i = level; i > rlevel; i--)
		{
			tail[i]->forward[i].holds += 1;
		}
	}

	for (int i = rlevel; i >= 0; i--)
	{
		if (tail[i] == NULL)
		{
			head[i].next = n;
			tail[i] = n;
		}
		else
		{
			tail[i]->forward[i].next = n;
			tail[i] = n;
		}
	}
	length += 1;
}

void LevelList::insert(int index, string line)
{
	LevelList::Node::Block* current = head;
	int* lvlIndex = new int[MAXLVL];
	LevelList::Node::Block** update = new Node::Block * [MAXLVL];
	int startIndex = index;

	assert(index >= 0);
	assert(this->length >= index);

	for (int i = level; i >= 0; i--)
	{
		while (current[i].holds <= index)
		{
			index -= current[i].holds;
			current = current[i].next->forward;
		}
		update[i] = current;
		lvlIndex[i] = index;
	}

	int rlevel = randomLevel();
	LevelList::Node* n = new Node(line, rlevel);

	if (rlevel > level)
	{
		for (int i = level + 1; i < rlevel + 1; i++)
		{
			head[i].holds = length + 1;
			update[i] = head;
			lvlIndex[i] = startIndex;
		}
		level = rlevel;
	}
	if (level > rlevel)
	{
		for (int i = level; i > rlevel; i--)
		{
			update[i][i].holds += 1;
		}
	}

	for (int i = rlevel; i >= 0; i--)
	{
		n->forward[i].holds = update[i][i].holds - lvlIndex[i];
		update[i][i].holds = lvlIndex[i] + 1;
		n->forward[i].next = update[i][i].next;
		update[i][i].next = n;
		if (n->forward[i].next == NULL)
			tail[i] = n;
	}

	delete[] lvlIndex;
	delete[] update;
	length += 1;
	modified = true;
}

void LevelList::remove(int index)
{
	LevelList::Node::Block* current = head;
	LevelList::Node::Block** update = new Node::Block * [MAXLVL];
	LevelList::Node** updateTail = new Node * [MAXLVL];
	LevelList::Node* currentNode = NULL;

	for (int i = level; i >= 0; i--)
	{
		updateTail[i] = head[i].next;
	}

	assert(index >= 0);
	assert(this->length >= index);

	for (int i = level; i >= 0; i--)
	{
		while (current[i].holds <= index)
		{
			index -= current[i].holds;
			currentNode = current[i].next;
			current = current[i].next->forward;
		}
		updateTail[i] = currentNode;
		update[i] = current;
	}

	LevelList::Node* delNode = current->next;

	for (int i = level; i >= 0; i--)
	{
		if (update[i][i].next == delNode)
		{
			update[i][i].holds += update[i][i].next->forward[i].holds - 1;
			update[i][i].next = update[i][i].next->forward[i].next;
		}
		else
		{
			update[i][i].holds -= 1;
		}
	}

	for (int i = level; i >= 0; i--)
	{
		if (head[i].next == NULL)
		{
			level -= 1;
			tail[i] = NULL;
		}
	}

	for (int i = level; i >= 0; i--)
		if (updateTail[i] != NULL)
			if (updateTail[i]->forward[i].next == NULL)
				tail[i] = updateTail[i];

	delete delNode;
	delete[] update;
	delete[] updateTail;

	length -= 1;
	modified = true;
}

void LevelList::debug()
{
	cout << "\n*****Level List*****" << "\n";
	for (int i = 0; i <= level; i++)
	{
		LevelList::Node* node = head[i].next;
		cout << "Level " << i << ": ";
		while (node != NULL)
		{
			cout << node->data << " ";
			node = node->forward[i].next;
		}
		cout << "\n";
	}
	cout << "\ntail\n";
	for (int i = 0; i <= level; i++)
	{
		LevelList::Node* node = tail[i];
		cout << "Level " << i << ": ";
		cout.flush();
		if (node != NULL)
		{
			cout << node->data << " ";
			cout.flush();
		}
		cout << "\n";
	}
}
void LevelList::display()
{
	int i = 0;
	int i_length = 1;
	int space = std::to_string(length).size();                 
	LevelList::Node* node = head[0].next;
	while (node != NULL)
	{
		i++;
		i_length = std::to_string(i).size();
		for (int k = space; k > i_length; k--)
		{
			cout << " ";
		}
		cout << i << " " << node->data;
		node = node->forward[0].next;
		cout << "\n";
	}
}
void LevelList::delete_all()
{
	for (int i = 0; i < length; i++)
	{
		remove(i);
	}
}