#include <iostream>
#include <fstream>
#include <conio.h>
#include <vector>
#include <ctime>
#include "admin.cpp"

using namespace std;

FileWritting writter;

bool is_empt(std::ifstream &pFile)
{
    return pFile.peek() == std::ifstream::traits_type::eof();
}

string searchVoter(string id, string &pin)
{
    ifstream inVoter("DATA/VoterDetails.txt");
    string ID, name, dob;
    while (inVoter >> ID >> name >> dob >> pin)
    {
        if (id == ID)
        {
            inVoter.close();
            return "Voter Id: " + id + ", Name: " + name + ", DOB: " + dob;
        }
    }
    inVoter.close();
    return "";
}

bool isAlredyVoted(string id)
{
    ifstream av("DATA/AlredyVoted.txt");
    string ID;
    getline(av, ID);
    while (!av.eof())
    {
        if (ID.compare(id) == 0)
        {
            return true;
        }
        getline(av, ID);
    }
    return false;
}

bool isValidAge(string dob)
{
    int day, month, year;
    sscanf(dob.c_str(), "%d/%d/%d", &day, &month, &year);

    time_t t = time(0);
    tm* now = localtime(&t);
    int age = (now->tm_year + 1900) - year;
    if ((now->tm_mon + 1) < month || ((now->tm_mon + 1) == month && now->tm_mday < day))
        age--;

    return age >= 18;
}

bool verifyPIN(string inputPin, string actualPin)
{
    return inputPin == actualPin;
}

void voting()
{
    ifstream res("DATA/Result.txt");
    if (!res)
    {
        cout << "Result file is not in DATA folder" << endl;
        return;
    }
    if (is_empt(res))
    {
        cout << "Setup first" << endl;
        return;
    }

    int ch = 10;
    vector<int> vote;
    vector<string> candidate;
    int voteCount;
    string line;

    res >> voteCount;
    getline(res, line);
    while (!res.eof())
    {
        vote.push_back(voteCount);
        candidate.push_back(line);
        res >> voteCount;
        getline(res, line);
    }

    while (ch != 0)
    {
        system("cls");
        cout << "                   WELCOME TO THE VOTING PAGE " << endl;
        cout << "---------------------------------------------------------------" << endl;
        cout << endl;
        cout << "         1. Give VOTE                        0. End Voting" << endl;
        cin >> ch;
        system("cls");

        if (ch == 0)
        {
            writter.write("DATA/Result.txt", "");
            for (int i = 0; i < candidate.size(); i++)
            {
                string s = to_string(vote[i]) + " " + candidate[i] + "\n";
                writter.appendNewLine("DATA/Result.txt", s);
            }
        }
        else if (ch == 1)
        {
            cout << endl << "       Enter your Voter-ID: ";
            string id;
            cin >> id;

            string pin;
            string voterDetails = searchVoter(id, pin);
            if (voterDetails == "")
            {
                cout << "\n\n       Voter Not Found \n      Enter valid voter-ID" << endl;
                continue;
            }

            cout << endl;
            cout << "       " << voterDetails;
            size_t pos = voterDetails.find_last_of(" ");
            string dob = voterDetails.substr(pos + 1);

            if (!isValidAge(dob))
            {
                cout << "\n\n       You are not eligible to vote (must be 18 years old)." << endl;
                cout << endl << "       Press any key to continue..";
                getch();
                continue;
            }

            if (isAlredyVoted(id))
            {
                cout << endl << "\n       You have already voted." << endl;
                cout << endl << "       Press any key to continue..";
                getch();
                continue;
            }

            cout << "       Enter your PIN: ";
            string inputPin;
            cin >> inputPin;

            if (!verifyPIN(inputPin, pin))
            {
                cout << "\n\n       Incorrect PIN. Please try again." << endl;
                cout << endl << "       Press any key to continue..";
                getch();
                continue;
            }

            int v;
            cout << "\n\n";
            for (int i = 0; i < candidate.size(); i++)
            {
                cout << "\n\n" << "       " << i + 1 << "." << candidate[i];
            }

            cout << "\n------------------------------------------------------------------" << endl;
            cout << endl << "\n       Choose Your Vote: ";
            cin >> v;
            vote[v - 1]++;
            writter.appendNewLine("DATA/AlredyVoted.txt", id + "\n");
            cout << "------------------------------------------------------------------\n" << endl;
        }
        else
        {
            cout << "Invalid Input" << endl;
        }
    }
    res.close();
}

void admin()
{
    adminC ad;

    cout << "       Enter the Admin password: ";
    string p;
    char ch;
    ch = _getch();
    while (ch != 13) // 13 is the ASCII value for Enter key
    {
        p.push_back(ch);
        cout << '*';
        ch = _getch();
    }
    cout << endl;

    if (p != ad.password)
    {
        cout << "Incorrect password!" << endl;
        return;
    }

    int ch_menu = 100;
    while (ch_menu != 0)
    {
        system("cls");
        cout << "***************************************************************************************************" << endl;
        cout << "                                   THIS IS AN ADMIN PAGE\n" << endl;
        cout << "       1. See Result                    2. Enrollment of Candidates       3. delete voter" << endl;
        cout << "       4. delete all voters             5. RESET()                        6. Add new voters\n" << endl;
        cout << "                                        0: Back" << endl;
        cout << "---------------------------------------------------------------------------------------------------" << endl;
        cin >> ch_menu;
        switch (ch_menu)
        {
        case 1:
        {
            system("cls");
            cout << "*********************************************************" << endl;
            cout << "       This is the final result of the election-- " << endl;
            ad.seeResult();
            cout << endl << "\n       Press any key to continue..";
            getch();
            break;
        }
        case 2:
        {
            system("cls");
            cout << "\n                   THIS IS AN ENROLLMENT PAGE" << endl;
            cout << "------------------------------------------------------------------\n" << endl;
            cout << "           This is a Enrollment page for elected candidate " << endl;
            cout << "       All candidates who want to stand in election fill up the Enrollment form  \n" << endl;
            ad.startSetup();
            cout << "       CONGRATULATIONS.......   " << endl << endl;
            cout << "       CANDIDATE ENROLLMENT COMPLETE SUCCESSFULLY" << endl;
            cout << endl << "       Press any key to continue..";
            getch();
            break;
        }
        case 3:
        {
            cout << "       Enter Voter ID-";
            string id;
            cin >> id;
            ad.deleteVoter(id);
            cout << "       Press any key to continue..";
            getch();
            break;
        }
        case 4:
        {
            system("cls");
            ad.deleteAllVoters();
            cout << "       All voters details are deleted from the voter list " << endl;
            cout << endl << "       Press any key to continue..";
            getch();
            break;
        }
        case 5:
        {
            ad.reset();
            system("cls");
            cout << "\n\n       System has been Reset() Successfully." << endl << endl;
            cout << endl << "       Press any key to continue..";
            getch();
            break;
        }
        case 6:
        {
            system("cls");
            cout << "  *****************************************************************" << endl;
            ad.addVoters();
            cout << "\n        New voters have been added into the voter list" << endl;
            cout << endl << "       Press any key to continue..";
            getch();
            break;
        }
        case 0:
            cout << "Exiting";
            break;
        default:
            cout << "Invalid Input";
            break;
        }
    }
}

void askForPin()
{
    // Implementation for asking admin PIN or similar functionality
}

int main()
{
    askForPin();
    int ch = 10;
    while (ch != 0)
    {
        cout << "*********************************************************************" << endl;
        cout << "                      WELCOME TO ONLINE VOTING\n " << endl;
        cout << "       1.ADMIN PAGE      2.START VOTING        0.EXIT THE PAGE" << endl;
        cout << "----------------------------------------------------------------------\n";
        cin >> ch;
        switch (ch)
        {
        case 1:
            system("cls");
            admin();
            break;
        case 2:
            system("cls");
            voting();
            break;
        case 0:
            cout << "Exiting..";
            break;
        default:
            break;
        }
        system("cls");
    }
}
