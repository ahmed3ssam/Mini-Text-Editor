#pragma once
#include <string>
class LevelList
{
private:
	class Node
	{
	public:
		struct Block {
			int holds;
			LevelList::Node* next;
		};
		std::string data;
		Block* forward;
		Node(std::string, int);
		~Node();
	};
	LevelList::Node::Block* head;
	LevelList::Node** tail;
	int MAXLVL;
	int level;
	float P;
	int length;
	bool modified;
	LevelList::Node::Block* current;
	int currentIndex;
	LevelList::Node* createNode(std::string, int);
	int randomLevel();
	std::string& find(int);
public:
	LevelList(int = 40, float = 0.5);
	~LevelList();
	void insert(int, std::string);
	void display();
	void delete_all();
	void remove(int);
	void debug();
	int len();
	void push(std::string);
	std::string& operator [](int);
};

