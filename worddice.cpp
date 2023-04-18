/*
Blake Milstead & Randy Lin
Project 5
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <map>
#include <algorithm>

using namespace std;

int main(int argc, char *argv[]){
    ifstream fnF, fnS;

    if(argc != 3){
        cerr << "Usage Error: worddice Dice.txt Words.txt";
        return -1;
    }

    fnF.open(argv[1]);
    fnS.open(argv[2]);
    if(!fnF.is_open())
    {
        cerr << "Dice File Failed to Open";
        return -1;
    }
    if (!fnS.is_open())
    {
        cerr << "Words File Failed to Open";
        return -1;
    }

    string temp;

    vector<list<int>> adjList;
    map<int, string> diceMap;
    map<int, string>:: iterator mit;

    int counter = 0;

    list<int> tempList;
    list<int>:: iterator lit;

    while(fnF >> temp){
        tempList.push_back(++counter);
        diceMap.insert({counter, temp});
    }
    
    adjList.push_back(tempList);
    tempList.clear();

    for(int i = 0; i < counter; i++){
        adjList.push_back(tempList);
    }

    counter++;
    char tempLet;
    while (fnS >> temp){ //Main while loop
        tempList.push_back(adjList.size()+temp.length());
        for(int i = 0; i < temp.length(); i++){
            tempLet = temp[i];
            for(mit = diceMap.begin(); mit != diceMap.end(); mit++){
                for(int j = 0; j < mit->second.length(); j++){
                    if(mit->second[j] == tempLet){
                        if (find(adjList[mit->first].begin(), adjList[mit->first].end(), counter+i) == adjList[mit->first].end()){
                            adjList[mit->first].push_back(counter + i);
                        }
                    }
                }
            }
            adjList.push_back(tempList);
        }
        tempList.clear();
        adjList.push_back(tempList);
        
        for(int i = 0; i < adjList.size(); i++){
            cout << "Node " << i << ": ";
            for(lit = adjList[i].begin(); lit != adjList[i].end(); lit++){
                cout <<  *lit << ", ";  
            }
            cout << endl;
        }
        

       adjList.resize(diceMap.size()+1);
       for(int i = 1; i < adjList.size(); i++){
            adjList[i].clear();
       }
       cout << endl;
    }

    fnF.close();
    fnS.close();
}