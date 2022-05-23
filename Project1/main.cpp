#include <iostream>
#include <string>
#include <list>
#include <regex>
#include <sstream>
#include "LevelList.h"
#include "ReadWrite.h"
using namespace std;
void interface();
struct found
{
	int line_number;
	list<int> index;
};
struct my_vect
{
	list<found> found_arr;
};
my_vect my_find(LevelList& lvlst, string word)
{
	bool flag = false;
	my_vect ret;
	for (int i = 0; i < lvlst.len(); i++)
	{
		found match;
		string line = lvlst[i];
		int it = line.size() - word.size() + 1;
		for (int j = 0; j < it; j++)
		{
			if (word[0] == line[j])
			{
				for (int k = 0; k < word.size(); k++)
				{

					if (word[k] != line[j + k])
					{
						flag = false;
						break;
					}
					flag = true;

				}
				if (flag)
				{
					match.index.push_back(j);
					match.line_number = i;
				}

			}
		}
		if (!match.index.empty())
		{
			ret.found_arr.push_back(match);
			match.index.clear();
		}
	}
	return ret;
}
void replace(LevelList& lvlst, string word, string with)
{
	int deff = with.size() - word.size();
	my_vect all = my_find(lvlst, word);
	while (!all.found_arr.empty())
	{
		int i = 0;
		found current = all.found_arr.front();
		while (!current.index.empty())
		{

			string line = lvlst[current.line_number];
			line.replace(current.index.front() + deff * i, word.size(), with);
			lvlst[current.line_number] = line;
			current.index.pop_front();
			i++;
		}
		all.found_arr.pop_front();
	}
}
void dis_find(LevelList& lvlst, string word)
{
	my_vect all = my_find(lvlst, word);
	while (!all.found_arr.empty())
	{
		int i = 0;
		found current = all.found_arr.front();
		while (!current.index.empty())
		{
			current.index.pop_front();
			i++;
		}
		cout << "\x1b[33mFound " << word << " " << i << " Times " << "At Line Number " << current.line_number + 1 << endl;
		cout << "\x1b[0m" << endl;
		cout << endl;
		all.found_arr.pop_front();
	}
}
int main()
{
	unsigned int seed = (unsigned)time(0);
	srand(seed);
	LevelList lvlst;
	interface();
	return 0;
}
void interface() {
	ReadWrite Load_save;
	cout <<"<----*************** Welcome Mini Text Editor ***************---->" << endl;
	bool Exitfile = false;
	LevelList lvlst;
	string input;
	string replace_word;
	string file_path = "";
	bool check = false;
	bool flag = false;
	while (!Exitfile) {
		int choice;

		cout << "1 - Exit" << endl;
		cout << endl;
		if (flag)
		{
			cout << "2 - Write Text" << endl;
			cout << endl;
			cout << "3 - Display Line" << endl;
			cout << endl;
			cout << "4 - Remove Line" << endl;
			cout << endl;
			cout << "5 - Re-Write Line" << endl;
			cout << endl;
			cout << "6 - Find Word" << endl;
			cout << endl;
			cout << "7 - Find && Replace" << endl;
			cout << endl;
			cout << "8 - Display Text " << endl;
			cout << endl;
		}
		cout << "9 - Load & Edit Text File" << endl;
		cout << endl;
		if (check)
		{
			cout << "10 - Convert File To Text " << endl;
			cout << endl;
			cout << "11 - Convert To PDF" << endl;
			cout << endl;
		}
		cin >> choice;
		cin.ignore();
		switch (choice) {
		case 1: {
			Exitfile = true;
			break;
		}
		case 2: {
			int idx;
			cout << "Enter Line number " << endl;
			cin >> idx;
			cin.ignore();
			cout << "Enter text " << endl;
			getline(cin, input);
			if (idx > lvlst.len())
			{
				int until = idx - lvlst.len() - 1;
				for (int i = 0; i < until; i++)
				{
					lvlst.push("");

				}
				lvlst.push(input);
			}
			else
			{
				lvlst.insert(idx - 1, input);
			}
			cout << endl;
			break;
		}

		case 3: {
			cout << "Enter line number" << endl;
			int idx;
			cin >> idx;
			cin.ignore();
			while (idx > lvlst.len())
			{
				cout << "Line Does Not Exist" << endl;
				cout << "Enter line number Again" << endl;
				cin >> idx;
				cin.ignore();
			}
			cout << lvlst[idx - 1] << endl;
			cout << endl;
			break;
		}
		case 4: {
			cout << "Enter line number" << endl;
			int idx;

			cin >> idx;
			cin.ignore();
			while (idx > lvlst.len())
			{
				cout << "Line Does Not Exist";
				cout << "Enter line number " << endl;
				cin >> idx;
				cin.ignore();
			}
			lvlst.remove(idx - 1);
			cout << endl;
			break;
		}
		case 5: {
			int idx;
			cout << "Enter line number" << endl;
			cin >> idx;
			cin.ignore();
			while (idx > lvlst.len())
			{
				cout << "Line Does Not Exist";
				cout << "Enter line number " << endl;
				cin >> idx;
				cin.ignore();
			}
			cout << "Enter text " << endl;
			getline(cin, input);
			lvlst[idx - 1] = input;
			cout << endl;
			break;

		}
		case 6: {

			cout << "Enter string" << endl;
			getline(cin, input);
			dis_find(lvlst, input);
			cout << endl;
			break;
		}
		case 7: {

			cout << "Enter Word You Want To Replace" << endl;
			getline(cin, input);
			cout << "Enter String You Want To Replace IT With" << endl;
			getline(cin, replace_word);
			replace(lvlst, input, replace_word);
			cout << endl;
			break;

		}
		case 8: {

			cout << "OutPut : " << endl;
			cout << endl;
			lvlst.display();
			cout << endl;
			break;
		}
		case 9: {
			flag = true;
			check = true;
			lvlst.delete_all();
			cout << "Enter Text File Path" << endl;
			getline(cin, input);

			while (true)
			{
				try
				{
					Load_save.load(input, lvlst);
					break;
				}
				catch (string error)
				{
					cout << "Error, Please Enter A Correct Path";
					getline(cin, input);
				}
			}
			cout << "Loaded Succesfully" << endl;
			cout << endl;
			break;
		}
		case 10: {
			if (!check)
			{
				break;
			}
			if (file_path.empty())
			{
				cout << "Enter Correct Text File Path" << endl;
				getline(cin, input);

				while (true)
				{
					try
					{
						Load_save.save_text_file(input, lvlst);
						break;
					}
					catch (string error)
					{
						cout << "Error, Please Enter A Correct Path";
						cout << endl;
						getline(cin, input);
					}
				}
				cout << "Saved as TEXT Succesfully" << endl;
			}
			else
			{
				Load_save.save_text_file(file_path, lvlst);
			}
			cout << endl;
			break;
		}
		case 11: {
			if (!check)
			{
				break;
			}
			if (file_path.empty())
			{
				cout << "Enter Correct Text File Path" << endl;
				getline(cin, input);

				while (true)
				{
					try
					{
						Load_save.save_pdf_file(input, lvlst);
						break;
					}
					catch (string error)
					{
						cout << "Error, Please Enter A Correct Path";
						cout << endl;
						getline(cin, input);
					}
				}
				cout << "Saved as PDF Succesfully" << endl;
			}
			else
			{
				Load_save.save_pdf_file(file_path, lvlst);
			}
			cout << endl;
			break;
		}
		}
	}
}
