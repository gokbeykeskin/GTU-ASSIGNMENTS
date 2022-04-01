#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

string deleteCommas(string& set);  //this function deletes the commas which seperates set members.
void isReflexive(string set, vector<string> relations, ofstream& outputFile);
void isSymmetric(vector<string> relations, ofstream& outputFile);
void isAntiSymmetric(vector<string> relations, ofstream& outputFile);
void isTransitive(vector<string> relations, ofstream& outputFile);
int main()
{

	ifstream inputFile;
	ofstream outputFile;
	int relationAmount;
	string set;
	string relation; // I took one relation at a time
	vector<string> relations; //Then, put it in relations vector if relation elements exists in the set.
	bool flag1, flag2;  //flag 1 is true if first member of relation exists in set, flag 2 is true if second member of relation exists in set.
	inputFile.open("input.txt");
	outputFile.open("output.txt");
	if (inputFile.is_open() == false || outputFile.is_open() == false) {
		cerr << "File couldn't open.";
		exit(1);
	}
	while (!inputFile.eof()) { //until the end of file. (don't put spaces on the bottom of the page.)
		inputFile >> relationAmount;
		inputFile >> set;
		set = deleteCommas(set);  //deletes the commas between set's members. Example: a,b,c becomes abc
		outputFile << endl << 'n' << endl;
		relations.clear();  //clear the relations array before every set input.
		for (int i = 0; i < relationAmount; i++) {
			flag1 = false;
			flag2 = false;
			inputFile >> relation; // I took one relation at a time
			outputFile << '(' << relation << ')';  //Ex: (a,b)
			relation = deleteCommas(relation);  //deletes the commas between relation's members. Example: a,b becomes ab
			for (int j = 0; j < static_cast<int>(set.size()); j++) {
				if (relation[0] == set[j]) flag1 = true; //if relation member exists in set.
				if (relation[1] == set[j]) flag2 = true; //if relation member exists in set.
			}
			if (flag1 == true && flag2 == true)
				relations.push_back(relation);  //Then, put the relation in relations vector if relation elements exists in the set.
			else outputFile << endl << '(' << relation[0] << ',' << relation[1] << ')' << " doesn't exist in set. This relation doesn't count!" << endl;
		}
		outputFile << endl;
		if (flag1 == true && flag2 == true) {
			isReflexive(set, relations, outputFile);
			isSymmetric(relations, outputFile);
			isAntiSymmetric(relations, outputFile);
			isTransitive(relations, outputFile);
		}
	}
	outputFile.close();
	inputFile.close();
	return 0;
}


string deleteCommas(string& set) { //this function deletes the commas which seperates set members.
	string returnString;
	int i = 0;
	for (int j = 0; j < static_cast<int>(set.size()); j++) {
		if (set[i] != ',') {
			returnString.push_back(set[i]);
		}
		i++;
	}
	return returnString;
}

void isReflexive(string set, vector<string> relations, ofstream& outputFile) {  //counts the equal relation members (a,a) (b,b) etc. Then compares it with amount of set members.
	int amount = 0;															//if they are equal, then it is reflexive.
	char x;
	for (int i = 0; i < static_cast<int>(relations.size()); i++) {
		for (int j = 0; j < static_cast<int>(set.size()); j++) {
			if (relations[i][0] == set[j] && relations[i][1] == set[j]) {
				amount++;
			}
			else x = set[j];
		}
	}
	//the following two lines prints the result and the reason.
	if (amount == static_cast<int>(set.size())) outputFile << "Reflexive: Yes, because all elements are present. " << endl;
	else outputFile << "Reflexive: No, because " << '(' << x << ',' << x << ')' << " is not present." << endl;
}

void isSymmetric(vector<string> relations, ofstream& outputFile) {  //if there is no (a,b) which a!=b flag stays true, else if there is a (a,b) which a!=b, checks if there is a (b,a) and if there is, makes the flag true.
	bool flag = true;
	char x[2];
	string reason;  //holds one of the relations which proves this is a Symmetric relation.
	for (int i = 0; i < static_cast<int>(relations.size()); i++) {
		if (relations[i][0] != relations[i][1] && flag == true) {
			flag = false;
			for (int j = 0; j < static_cast<int>(relations.size()); j++) {
				if (relations[j][0] == relations[i][1] && relations[j][1] == relations[i][0]) {
					flag = true;
					reason.resize(2);
					reason[0] = relations[i][0];
					reason[1] = relations[i][1];
				}
				else {
					x[0] = relations[i][0];
					x[1] = relations[i][1];
				}
			}
		}
	}
	//the following three lines prints the result and the reason.
	if (flag == true && reason.size() == 2) outputFile << "Symmetric: Yes, because " << '(' << reason[0] << ',' << reason[1] << ')' << " and " << '(' << reason[1] << ',' << reason[0] << ')' << " are present." << endl;
	else if (reason.size() != 2 && flag == true) outputFile << " Symmetric: Yes, because there is no different elements. " << endl;
	else outputFile << "Symmetric: No, because" << '(' << x[1] << ',' << x[0] << ')' << " is not present. " << endl;
}



void isAntiSymmetric(vector<string> relations, ofstream& outputFile) { ////if there is no (a,b) which a!=b flag stays true, else if there is a (a,b) which a!=b, checks if there is a (b,a) and if there is, makes the flag false.
	bool flag = true;
	char x[2];
	for (int i = 0; i < static_cast<int>(relations.size()); i++) {
		if (relations[i][0] != relations[i][1] && flag == true) {
			for (int j = 0; j < static_cast<int>(relations.size()); j++) {
				if (relations[j][0] == relations[i][1] && relations[j][1] == relations[i][0]) {
					flag = false;
				}
				else {
					x[0] = relations[i][0];
					x[1] = relations[i][1];
				}
			}
		}
	}
	//the following three lines prints the result and the reason.
	if (flag == true && x[1] != '\0') outputFile << "Anti-Symmetric: Yes, because " << '(' << x[0] << ',' << x[1] << ')' << " is found, but " << '(' << x[1] << ',' << x[0] << ')' << " is not found." << endl;
	else if (flag == true && x[1] == '\0') outputFile << "Anti-Symmetric: Yes, because there is no different elements." << endl;
	else outputFile << "Anti-Symmetric: No, because " << '(' << x[1] << ',' << x[0] << ')' << " is present. " << endl;
}
void isTransitive(vector<string> relations, ofstream& outputFile) { //if there is no a,b b,c which a!=c flag stays true. else if there is a a,b b,c which a!=c, it checks if there is a (a,c) which a!=c. If there is, makes the flag true.
	bool flag = true;
	char x[2];
	string reason; //holds one of the relations which proves this is a Transitive relation.
	for (int i = 0; i < static_cast<int>(relations.size()); i++) {
		for (int j = 0; j < static_cast<int>(relations.size()); j++) {
			if (relations[i][1] == relations[j][0] && relations[i][0] != relations[i][1] && relations[j][0] != relations[j][1]) {
				string newRelation;
				newRelation.push_back(relations[i][0]);
				newRelation.push_back(relations[j][1]);
				flag = false;
				for (int k = 0; k < static_cast<int>(relations.size()); k++) {
					if (relations[k] == newRelation) {
						flag = true;
						reason.resize(6);
						reason[0] = newRelation[0];
						reason[1] = newRelation[1];
						reason[2] = relations[i][0];
						reason[3] = relations[i][1];
						reason[4] = relations[j][0];
						reason[5] = relations[j][1];
					}
					else {
						x[0] = newRelation[0];
						x[1] = newRelation[1];
					}
				}
			}
		}
	}
	//the following three lines prints the result and the reason.
	if (flag == true && reason.size() == 6) outputFile << "Transitive: Yes,because " << '(' << reason[0] << ',' << reason[1] << ')' << " is found since " << '(' << reason[2] << ',' << reason[3] << ')' << " and " << '(' << reason[4] << ',' << reason[5] << ')' << " are found." << endl;
	else if (reason.size() != 6 && flag == true) outputFile << "Transitive: Yes, because there is no {a,b} , {b,c} which a!=c" << endl;
	else outputFile << "Transitive: No, because " << '(' << x[0] << ',' << x[1] << ')' << " doesn't exist." << endl;
}
