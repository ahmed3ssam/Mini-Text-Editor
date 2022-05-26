#pragma once
#include <iostream>
#include "ReadWrite.h"
#include "LevelList.h"
#include <string>
#include <fstream>
#include <exception>
using namespace std;
class ReadWrite
{
public:
	ReadWrite();
	int load(string, LevelList&);
	int save_text_file(string file_name, LevelList&);
	int save_pdf_file(string file_name,LevelList&);
	
	~ReadWrite();

private:
	string toPdf(LevelList&);
	string padLeft(string const&, int, char = ' ');
	void sanitize(string&);
	string kids(int);

};
//find & replace
//insert line at
//add line
//remove line
//replace line
