#include <iostream>
#include "ReadWrite.h"
#include "LevelList.h"
#include <string>
#include <fstream>
#include <exception>
#include <list>
#include <regex>
#include <sstream>
using namespace std;

ReadWrite::ReadWrite()
{
}

void ReadWrite::load(string file, LevelList& lines)
{
    
    string line;
    ifstream input_file(file);
    if (!input_file.is_open()) {
       throw "Could't Open File";
    }

    while (getline(input_file, line)) {
        lines.push(line);
    }

    input_file.close();
    return ;

}

void ReadWrite::save_text_file(string file_name, LevelList& lines)
{
	fstream file;
	file.open(file_name + ".txt", ios_base::out);

	if (!file.is_open())
	{
		cout << "Unable to open the file.\n";
		throw "Could't Open File";
	}
	for (int i = 0; i < lines.len(); i++)
	{
	file << lines[i];
	file << '\n';
	}

	file.close();
}

void ReadWrite::save_pdf_file(string file_name, LevelList& lines)
{
	string pdf = toPdf(lines);
	fstream file;
	file.open(file_name+".pdf", ios_base::out);

	if (!file.is_open())
	{
		cout << "Unable to open the file.\n";
		throw "Could't Open File";
	}

	file << pdf;

	file.close();
}
struct pdfLine
{
	bool startOfLine;
	string line;
};

string ReadWrite::toPdf(LevelList& lines)
{
	std::list<pdfLine> pdfLineList;

	for (int i{}; i < lines.len(); ++i)
	{
		bool newLine = true;
		string line = lines[i];
		istringstream ss(line);
		bool begin = true;
		string buffer;
		string del;
		list<string> words;
		while (getline(ss, del, ' ')) {
			words.push_back(del);
		}
		while (!words.empty())
		{
			if (words.front().size() >= 70)
			{
				if (begin)
				{
					pdfLineList.push_back(pdfLine{ newLine, buffer });
					newLine = false;
					buffer.clear();
					begin = false;
				}
				del = words.front();
				words.pop_front();
				words.push_front(del.substr(70, del.size() - 70));
				pdfLineList.push_back(pdfLine{ newLine, del.substr(0, 70) });
				newLine = false;
			}
			else
			{
				begin = true;
				if (buffer.size() + words.front().size() + 1 <= 70)
				{
					buffer += words.front() + " ";
					words.pop_front();
				}
				else
				{
					pdfLineList.push_back(pdfLine{ newLine, buffer });
					newLine = false;
					buffer.clear();
				}
			}
		}
		pdfLineList.push_back(pdfLine{ newLine, buffer });
	}

	std::list<string> contentStreams;

	int lineNumber = 0;
	while (true)
	{
		string stream1 = "BT\n/F1 12 Tf\n0 0 0 rg\n";
		string stream2 = "BT\n/F1 12 Tf\n0 0 .7 rg\n";
		for (int i{}; i < 63; i++)
		{
			if (pdfLineList.empty())
			{
				stream1 += "ET\n";
				stream2 += "ET\n";
				contentStreams.push_back(stream1 + stream2);
				goto OUT_OF_LOOP;
			}
			else
			{
				if (pdfLineList.front().startOfLine)
				{
					lineNumber += 1;
					stream2 += "1 0 0 1 4 " + to_string(792 - i * 12) + " Tm\n";
					stream2 += "[(" + padLeft(to_string(lineNumber), 7) + ")270(.)] TJ\n";
				}
				string line = pdfLineList.front().line;
				sanitize(line);
				stream1 += "1 0 0 1 58 " + to_string(792 - i * 12) + " Tm\n";
				stream1 += "(" + line + ") Tj\n";
				pdfLineList.pop_front();
			}
		}
		stream2 += "ET\n";
		stream1 += "ET\n";
		contentStreams.push_back(stream1 + stream2);
	}

OUT_OF_LOOP:
	list<int> xref;
	int size = contentStreams.size();
	string pdf = "%PDF-1.1\n";
	xref.push_back(pdf.size());
	pdf += "1 0 obj <</Type/Catalog/Pages 2 0 R>> endobj\n";
	xref.push_back(pdf.size());
	pdf += "2 0 obj <</Type/Pages/Kids["
		+ kids(size)
		+ "]/Count "
		+ to_string(size)
		+ "/MediaBox[0 0 595 842]/Resources 3 0 R>> endobj\n";
	xref.push_back(pdf.size());
	pdf += "3 0 obj <</Font<</F1<</Type/Font/Subtype/Type1/BaseFont/Courier>>>>>> endobj\n";
	xref.push_back(pdf.size());
	for (int i{}; i < size; i++)
	{
		pdf += to_string(i + 4)
			+ " 0 obj <</Type/Page/Parent 2 0 R/Contents "
			+ to_string(i + 4 + size)
			+ " 0 R>> endobj\n";
		xref.push_back(pdf.size());
	}
	for (int i{}; i < size; i++)
	{
		pdf += to_string(i + 4 + size)
			+ " 0 obj <</Length "
			+ to_string(contentStreams.front().size())
			+ ">>\nstream\n"
			+ contentStreams.front()
			+ "\nendstream\nendobj\n";
		xref.push_back(pdf.size());
		contentStreams.pop_front();
	}
	int xsize = xref.size();
	pdf += "\nxref\n0 "
		+ to_string(xsize)
		+ "\n0000000000 65536 f \n";
	for (int i{}; i < xsize - 1; i++)
	{
		pdf += padLeft(to_string(xref.front()), 10, '0')
			+ " 00000 n \n";
		xref.pop_front();
	}
	pdf += "\ntrailer\n<</Size "
		+ to_string(xsize)
		+ "/Root 1 0 R>>\nstartxref\n"
		+ to_string(xref.front())
		+ "\n%%EOF";

	return pdf;
}

string ReadWrite::kids(int size)
{
	string kidsArray = "4 0 R";
	for (int i{}; i < size - 1; i++)
	{
		kidsArray += " " + to_string(i + 5) + " 0 R";
	}
	return kidsArray;
}

string ReadWrite::padLeft(string const& str, int s, char del)
{
	if (str.size() < s)
		return string(s - str.size(), del) + str;
	else
		return str;
}

void ReadWrite::sanitize(string& line)
{
	regex backslash_brackets("\\\\|\\(|\\)", regex_constants::awk);
	line = regex_replace(line, backslash_brackets, "\\$&");
}
ReadWrite::~ReadWrite()
{
}
