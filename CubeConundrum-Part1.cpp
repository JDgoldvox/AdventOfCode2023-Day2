#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>

using namespace std;

int FindGameNumber(const string& line);
bool PlayRound(const string& line, const int& start, const int& end);
int ConvertASCII(const int& num);
int SumAndReturn(const vector<int>& vec);
int FindColourNumber(const string& colourInput, const string& line, const int& start, const int& end);

const int redMax = 12, greenMax = 13, blueMax = 14;

int main() {
	fstream myFile;

	string line;
	vector<int> playableIDs;
	int currentPosition = 0;
	int currentEndPosition = 0;

	myFile.open("../../../cool.txt", ios::in);
	
	if (myFile.is_open()) {
		while (getline(myFile, line)) {
			//find match number
			bool matchPlayable = true;
			int currentID = FindGameNumber(line);

			//Loop through each round
			while (currentPosition < (line.size() - 1)) {

				//Find next semicolon or end of line
				//if no, it returns the last item (this is end of line)
				currentEndPosition = line.find(";", currentPosition);

				//if we don't find the end of the round with a ; , set to end of the line instead
				if (currentEndPosition == -1) {
					currentEndPosition = line.size() - 1;
				}

				//play the round, if false, round is unplayable, quit this match
				if (!PlayRound(line, currentPosition, currentEndPosition)) {
					matchPlayable = false;
					//cout << currentID << " Match unplayable!" << endl;
					break;
				}

				//if match is playable
				currentPosition = currentEndPosition + 1;
			}
			
			if (matchPlayable) {
				playableIDs.push_back(currentID);
			}

			//reset vars
			currentPosition = 0;
		}
	}
	else {
		cout << "file failed to open\n";
	}

	cout << "Sum: " << SumAndReturn(playableIDs) << endl;
}

int FindGameNumber(const string& line) {
	//find the space!
	int currentPosition = line.find(" ", 0);
	currentPosition += 1;
	string matchNumberString = "";
	int matchID = 0;

	//keep seraching unless we get to a colon
	while (line[currentPosition] != ':') {
		matchNumberString += line[currentPosition];
		currentPosition++;
	}

	//convert string to int
	matchID = stoi(matchNumberString);

	//cout << "game number:" << matchID << endl;
	return matchID;
}

bool PlayRound(const string& line, const int& start, const int& end) {

	//find the colour red, green and blue
	//if they exist, find the number associated with them
	//if the number is larger than the maximum, return false, so the match ends and this match ID
	//is not counted

	int current = 0;
	int red = 0, green = 0, blue = 0;

	//find red
	red = FindColourNumber("red", line, start, end);

	//find green
	green = FindColourNumber("green", line, start, end);

	//find blue
	blue = FindColourNumber("blue", line, start, end);

	//cout << "------------------------------------------------ round end" << endl;

	//check if any colours exceed limit
	return (red <= redMax && green <= greenMax && blue <= blueMax);
}

int ConvertASCII(const int& num) {
	int convertedNumber = num;
	convertedNumber -= 48;
	return convertedNumber;
}

int SumAndReturn(const vector<int>& vec) {
	int total = 0;
	for (int element : vec) {
		total += element;
	}
	return total;
}

int FindColourNumber(const string& colourInput, const string& line, const int& start, const int& end) {
	int current = line.find(colourInput, start);
	
	if (current == -1) {
		current = end;
	}

	if (current < end) {
		stack<char> numberAsStringStack;
		string numberAsString = "";
		current = current - 2;

		//keep going back until we find the space
		while (line[current] != ' ') {
			numberAsStringStack.push(line[current]);
			current--;
		}

		//reverse string
		while (!numberAsStringStack.empty()) {
			numberAsString += numberAsStringStack.top();
			numberAsStringStack.pop();
		}

		//convert into int
		return stoi(numberAsString);
	}

	return 0;
}