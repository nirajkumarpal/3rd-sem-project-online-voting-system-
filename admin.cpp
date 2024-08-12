#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "file-Handling.cpp"

using namespace std;

class adminC
{
private:
    FileWritting writter;

public:
    string password = "password";
    void reset();
    void deleteAllVoters();
    void deleteVoter(string id);
    void startSetup();
    void addVoters();
    void seeResult();
};

void adminC::seeResult()
{
    cout << endl;
    ifstream in("DATA/Result.txt");
    if (!in)
    {
        cout << "Result file is not available or empty." << endl;
        return;
    }

    vector<string> candidates;
    vector<int> votes;
    int voteCount;
    string line;

    while (in >> voteCount)
    {
        getline(in, line); // Read the rest of the line which includes candidate info
        candidates.push_back(line);
        votes.push_back(voteCount);
    }

    int maxVotes = 0;
    int secondMaxVotes = 0;
    vector<string> winners;
    vector<string> winnerParties;

    for (size_t i = 0; i < candidates.size(); ++i)
    {
        cout << "      " << candidates[i] << " received " << votes[i] << " votes." << endl;
        if (votes[i] > maxVotes)
        {
            secondMaxVotes = maxVotes;
            maxVotes = votes[i];
            winners.clear();
            winners.push_back(candidates[i]);
            size_t pos = candidates[i].find(" of party ");
            if (pos != string::npos)
                winnerParties.push_back(candidates[i].substr(pos + 10));
        }
        else if (votes[i] == maxVotes)
        {
            winners.push_back(candidates[i]);
            size_t pos = candidates[i].find(" of party ");
            if (pos != string::npos)
                winnerParties.push_back(candidates[i].substr(pos + 10));
        }
        else if (votes[i] > secondMaxVotes)
        {
            secondMaxVotes = votes[i];
        }
    }

    if (maxVotes > 0)
    {
        int winningMargin = maxVotes - secondMaxVotes;
        if (winners.size() == 1)
        {
            cout << endl
                 << "       The winner is " << winners[0] << " with a winning margin of " << winningMargin << " votes."
                  << endl;
        }
        else
        {
            cout << endl
                 << "       There is a tie between the following candidates with " << maxVotes << " votes each:" << endl;
            for (size_t i = 0; i < winners.size(); ++i)
            {
                cout << "       " << winners[i] << " (Party: " << winnerParties[i] << ")" << endl;
            }
        }
    }
    else
    {
        cout << "No votes casted yet." << endl;
    }
}

void adminC::addVoters()
{
    cout << "       Now we are going to add new voters to the voter list  " << endl
         << endl;

    cout << "       Enter how many voters you want to add: ";
    int n;
    cin >> n;
    cout << endl
         << endl;
    string voterName, ID, dob;
    srand(static_cast<unsigned int>(time(0))); // Seed for random number generation

    for (int i = 0; i < n; i++)
    {
        cin.ignore();
        cout << "       Enter the name of the voter " << i + 1 << ": ";
        getline(cin, voterName);
        cout << "       Enter the voter ID of " << voterName << ": ";
        getline(cin, ID);
        cout << "       Enter the date of birth (DD/MM/YYYY) of " << voterName << ": ";
        getline(cin, dob);

        int pin = rand() % 9000 + 1000; // Generate a 4-digit PIN
        writter.appendNewLine("DATA/VoterDetails.txt", ID + " " + voterName + " " + dob + " " + to_string(pin) + "\n");

        cout << "       The PIN for voter " << voterName << " (ID: " << ID << ") is: " << pin << endl;
        cout << "-------------------------------------------------------------------------------\n"
             << endl;
    }
}

void adminC::reset()
{
    writter.write("DATA/Result.txt", "NA");
    writter.write("DATA/AlredyVoted.txt", "");
}

void adminC::startSetup()
{
    reset();
    writter.write("DATA/Result.txt", "");

    cout << endl;
    cout << "       Enter how many candidates: ";
    int n;
    cin >> n;
    cin.ignore();
    string candidateName, party, symbol;
    for (int i = 0; i < n; i++)
    {
        cout << "       Enter the name of the candidate " << i + 1 << ": ";
        getline(cin, candidateName);
        cout << "       Enter the party name of the candidate " << candidateName << ": ";
        getline(cin, party);
        cout << "       Enter the symbol of the party " << party << ": ";
        getline(cin, symbol);

        writter.appendNewLine("DATA/Result.txt", "0 " + candidateName + " of party " + party + " (" + symbol + ")\n");
        cout << endl;
    }
}

void adminC::deleteAllVoters()
{
    writter.write("DATA/VoterDetails.txt", "");
}

// delete voter from voter list by ID
void adminC::deleteVoter(string ID)
{
    ifstream inVoter("DATA/VoterDetails.txt");
    if (!inVoter)
    {
        cout << "Voter list file not found." << endl;
        return;
    }

    string id, name, dob, pin;
    int n = 1;
    bool found = false;
    ofstream temp("temp.txt");

    while (inVoter >> id >> name >> dob >> pin)
    {
        if (id == ID)
        {
            cout << "Voter Id: " << id << ", Name: " << name << ", DOB: " << dob << endl;
            cout << "Deleted from the voter list." << endl;
            found = true;
        }
        else
        {
            temp << id << " " << name << " " << dob << " " << pin << "\n";
        }
        n++;
    }

    inVoter.close();
    temp.close();

    if (found)
    {
        remove("DATA/VoterDetails.txt");
        rename("temp.txt", "DATA/VoterDetails.txt");
    }
    else
    {
        cout << "Voter Not Found" << endl;
    }
}
