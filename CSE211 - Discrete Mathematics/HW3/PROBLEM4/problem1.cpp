#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib> //for rand()
#include <ctime>  //for srand(time(NULL))
#include <algorithm> //to use find function
using namespace std;

void deleteCommas(string & str);  //this function deletes the commas which seperates set members.
void makeReflexive(string set,vector<string> & relations); //makes reflexive (if not)
void makeAntiSymmetric(vector<string> & relations);        //makes antisymmetric (if not)
void makeTransitive(vector<string> & relations);           //makes transitive (if not)
bool isReflexive(string set,vector<string> relations);
bool isAntiSymmetric(vector<string> relations);
bool isTransitive(vector<string> relations,vector<int> & transitiveIndexes);
void printHasse(vector<string> relations,ofstream & outputFile,const vector<int>& transitiveIndexes); //prints only the relations which exists on hasse diagram.
void printer(vector<string> relations,ofstream & outputFile);  //prints the POSET

int main()
{

    ifstream inputFile;
    ofstream outputFile;
    int relationAmount;
    string set;
    string relation; // I took one relation at a time
    vector<string> relations; //Then, put it in relations vector if relation elements exists in the set.
    vector<int> transitiveIndexes;  //I keep the indexes of transitive elements, so I won't print them afterwards on hasse diagram.
    bool flag1,flag2;  //flag 1 is true if first member of relation exists in set, flag 2 is true if second member of relation exists in set.
    inputFile.open("input.txt");
    outputFile.open("output.txt");
    if(inputFile.is_open()==false || outputFile.is_open()==false){
        cerr<<"File couldn't open.";
        exit(1);
    }
    while(!inputFile.eof()){ //until the end of file.
        inputFile>>relationAmount;
        inputFile>>set;
        deleteCommas(set);  //deletes the commas between set's members. Example: a,b,c becomes abc
        if(!inputFile.eof()){  //read until the end of file.
            relations.clear();  //clear the relations array before every set input.
            transitiveIndexes.clear();  //same as relations
            for(int i=0;i<relationAmount;i++){
                flag1=false;
                flag2=false;
                inputFile>>relation; // I took one relation at a time
                deleteCommas(relation);  //deletes the commas between relation's members. Example: a,b becomes ab
                for(int j=0;j<static_cast<int>(set.size());j++){
                    if(relation[0]==set[j]) flag1=true; //if relation member exists in set.
                    if(relation[1]==set[j]) flag2=true; //if relation member exists in set.
                }
                if(flag1==true && flag2==true)
                    relations.push_back(relation);  //Then, put the relation in relations vector if relation elements exists in the set.
                else outputFile<<endl<<'('<<relation[0]<<','<<relation[1]<<')'<<" doesn't exist in set. This relation doesn't count!"<<endl;
            }
            outputFile<<endl;
            if(flag1==true&&flag2==true){
                outputFile<<endl<<'n'<<endl;
                while(!isReflexive(set,relations) || !isAntiSymmetric(relations) || !isTransitive(relations,transitiveIndexes)){ //all three conditions has to be satisfied
                    makeReflexive(set,relations);  //adds every pair of a,a b,b ... etc. if they don't exist
                    makeAntiSymmetric(relations); //if there are two relations like a,b and b,a deletes one of them.
                    makeTransitive(relations);  //if there are a,b b,c and a,c deletes a,c
                    outputFile<<"Relations have been modified. Because this is not a POSET."<<endl;
                }
                printer(relations,outputFile);
                printHasse(relations,outputFile,transitiveIndexes);
            }
        }
    }
    outputFile.close();
    inputFile.close();
    return 0;
}

void deleteCommas(string & str){ //this function deletes the commas which seperates set members.
    string newString;
    int i=0;
    for(int j=0;j<static_cast<int>(str.size());j++){
        if(str[i]!=','){
            newString.push_back(str[i]);
        }
        i++;
    }
    str = newString;
}

void makeReflexive(string set,vector<string> & relations){ //first holds the existing aa,bb pairs, then goes through set again and adds the non-existing ones.
    bool flag;
    string existingPairs;
    for(int i=0;i<set.size();i++){
        for(int j=0;j<relations.size();j++){
            if(set[i]==relations[j][0] && set[i]==relations[j][1]){
                existingPairs.push_back(set[i]);
            }
        }
    }
    for(int i=0;i<set.size();i++){
        flag = false;
        for(int j=0;j<existingPairs.size();j++){
            if(existingPairs[j]==set[i]){
                flag=true;
                j=existingPairs.size(); //break the loop
            }
        }
        if(flag==false){
            relations.resize(relations.size()+1);
            relations[relations.size()-1].push_back(set[i]);
            relations[relations.size()-1].push_back(set[i]);
        }
    }
}

void makeAntiSymmetric(vector<string> & relations){  //randomly deletes one of the a,b b,a pairs.
    srand(time(NULL));
    int x= rand()%2; // 0 or 1
    for (auto it = relations.begin(); it != relations.end(); ++it){
        if((*it)[0]!=(*it)[1]){
            for (auto it2 = relations.begin(); it2 != relations.end(); ++it2){
                if((*it)[0] == (*it2)[1] && (*it)[1] == (*it2)[0]){
                    if(x==0)
                        relations.erase(it);
                    else if(x==1)
                        relations.erase(it2);
                }
            }
        }
    }
}

void makeTransitive(vector<string>& relations){ 
    bool flag;

    for(auto it=relations.begin();it != relations.end(); ++it){
        for(auto it2=relations.begin();it2 != relations.end(); ++it2){
            if((*it)[1] == (*it2)[0] && (*it)[0] != (*it)[1] && (*it2)[0]!=(*it2)[1]){
                string newRelation;  //if there is a (a,b) (b,c) holds (a,c)
                newRelation.push_back((*it)[0]);
                newRelation.push_back((*it2)[1]);
                flag=false;
                for(auto it3=relations.begin();it3 != relations.end(); ++it3){
                    if(*it3==newRelation){
                        flag=true;
                        break;
                    }
                }
                if(flag==false)
                    relations.push_back(newRelation);
                break;
            }
        }
    }
}


bool isReflexive(string set,vector<string> relations){
    int amount=0;
    for(int i=0;i<static_cast<int>(relations.size());i++){
        for(int j=0;j<static_cast<int>(set.size());j++){
            if(relations[i][0]==set[j]&& relations[i][1]==set[j]){
                amount++;
            }
        }
    }
    if(amount==static_cast<int>(set.size())) return true; //if they are equal, then it is reflexive.
    else return false;
}
bool isAntiSymmetric(vector<string> relations){
    bool flag=true;
    for(int i=0;i<static_cast<int>(relations.size());i++){
        if(relations[i][0]!=relations[i][1]&& flag==true){
            for(int j=0;j<static_cast<int>(relations.size());j++){
                if(relations[j][0] == relations[i][1] && relations[j][1] == relations[i][0]){
                    flag=false;
                    break;
                }
            }
        }
    }
    if(flag==true) return true;
    else return false;
}
bool isTransitive(vector<string> relations,vector<int> & transitiveIndexes){
    bool flag=true;
    for(int i=0;i<static_cast<int>(relations.size());i++){
        for(int j=0;j<static_cast<int>(relations.size());j++){
            if(relations[i][1] == relations[j][0] && relations[i][0] != relations[i][1] && relations[j][0]!=relations[j][1]){
                string newRelation;  //if there is a (a,b) (b,c) holds (a,c)
                newRelation.push_back(relations[i][0]);
                newRelation.push_back(relations[j][1]);
                flag=false;
                for(int k=0;k<static_cast<int>(relations.size());k++){
                    if(relations[k]==newRelation){
                        flag=true;
                        transitiveIndexes.push_back(k);
                        break;
                    }
                }
            }
        }
    }
    if(flag==true) return true;
    else return false;
}

void printHasse(vector<string> relations,ofstream & outputFile,const vector<int>& transitiveIndexes){
    outputFile<<"Hasse: "<<endl;
    for(int i=0;i<relations.size();i++){
        if(relations[i][0]!=relations[i][1] && find(transitiveIndexes.begin(),transitiveIndexes.end(),i)==transitiveIndexes.end()){
            outputFile<<'('<<relations[i][0]<<','<<relations[i][1]<<')'<<',';
        }
    }
    outputFile<<endl;
}
void printer(vector<string> relations,ofstream & outputFile){
    outputFile<<"POSET: ";
    for(int i=0;i<relations.size();i++){
        outputFile<<'('<<relations[i][0]<<','<<relations[i][1]<<')'<<',';
    }
    outputFile<<endl;
}