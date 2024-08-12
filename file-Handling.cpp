#include <iostream>
#include <fstream>

using namespace std;

class FileWritting
{
public:
    void write(string fileName, string data);
    void appendNewLine(string fileName, string data);
    void deleteLineByNumber(string fileName, int lineNo);
};

void FileWritting::write(string fileName, string data)
{
    ofstream out(fileName);
    if (!out)
    {
        cerr << "Error: could not open the file " << fileName << " for writing." << endl;
        return;
    }
    out << data;
    out.close();
}

void FileWritting::appendNewLine(string fileName, string data)
{
    ofstream out(fileName, ios_base::app);
    if (!out)
    {
        cerr << "Error: could not open the file " << fileName << " for appending." << endl;
        return;
    }
    out << data;
    out.close();
}

void FileWritting::deleteLineByNumber(string fileName, int lineNo)
{
    ifstream ins(fileName);
    if (!ins)
    {
        cerr << "Error: could not open the file " << fileName << " for reading." << endl;
        return;
    }

    ofstream ofs("temp.txt");
    if (!ofs)
    {
        cerr << "Error: could not open the temporary file for writing." << endl;
        return;
    }

    string line;
    int currentLine = 1;
    while (getline(ins, line))
    {
        if (currentLine != lineNo)
        {
            ofs << line << endl;
        }
        currentLine++;
    }

    ofs.close();
    ins.close();

    if (remove(fileName.c_str()) != 0)
    {
        cerr << "Error: could not remove the original file." << endl;
    }
    else if (rename("temp.txt", fileName.c_str()) != 0)
    {
        cerr << "Error: could not rename the temporary file." << endl;
    }
}
