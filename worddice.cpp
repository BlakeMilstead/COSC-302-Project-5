/*
Blake Milstead & Randy Lin
Project 5
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <map>
#include <queue>
#include <algorithm>
#include <limits.h>

using namespace std;

bool bfs(vector<vector<int>> &rGraph, int &source, int &sink, vector<int> &parent ){
    vector<bool> visited(rGraph.size(), false);
    queue<int> que;
    que.push(source);
    visited[source] = true;
    parent[source] = -1;

    while(!que.empty()){
        int u = que.front();
        que.pop();

        for(int i = 0; i < rGraph.size(); i++){
            if(visited[i] == false && rGraph[u][i] > 0){
                if(i == sink){
                    parent[i] = u;
                    return true;
                }
                que.push(i);
                parent[i] = u;
                visited[i] = true;
            }
        }
    }

    return false;
}

int karp(vector<vector<int>> &graph, int source, int sink, vector<int> &diceTrace){
    int u, v;

    vector<vector<int>> rGraph(graph.size(), vector<int> (graph.size(), 0));
    for (int i = 0; i < graph.size(); i++)
    {
        for (int j = 0; j < graph[i].size(); j++)
        {
            rGraph[i][j] = graph[i][j];
        }
    }

    vector<int> parent (graph.size());

    int maxFlow = 0;

    while(bfs(rGraph, source, sink, parent)){
        int pathFlow = INT_MAX;
        for(v = sink; v != source; v = parent[v]){
            u = parent[v];
            pathFlow = min(pathFlow, rGraph[u][v]);
        }

        for (v = sink; v != source; v = parent[v]){
            u = parent[v];
            rGraph[u][v] -= pathFlow;
            rGraph[v][u] += pathFlow;
        }

        maxFlow += pathFlow;
    }

        int wordPlace = rGraph.size()-diceTrace.size()-1;
        for(int i = 0; i < wordPlace; i++){
            for(int j = 0; j < rGraph.size(); j++){
                if (rGraph[i+wordPlace][j] != 0){
                    diceTrace[i] = (j - 1);
                }
            }
        }
        
        return maxFlow;
        
}

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

    vector<vector<int>> adjMat;
    map<int, string> diceMap;
    map<int, string>:: iterator mit;

    int counter = 0;

    vector<int> tempVector;
    list<int>:: iterator lit;

    tempVector.push_back(0);

    while(fnF >> temp){ //Getting Dice
        tempVector.push_back(1);
        counter++;
        diceMap.insert({counter, temp});
    }
    
    adjMat.push_back(tempVector); //Creating Source Row of AdjMatrix

    char tempLet;
    while (fnS >> temp){ //Main while loop
        int adjMatSize = counter+2+temp.size();

        adjMat[0].resize(adjMatSize, 0);
        
        tempVector.clear();
        tempVector.resize(adjMatSize, 0);

        for (int i = 0; i < counter; i++){ // Creating Dice Rows of AdjMatrix
            adjMat.push_back(tempVector);
        }

        tempVector[0] = 0;
        tempVector[adjMatSize-1] = 1;

        for (int i = 0; i < temp.size(); i++){ // Creating Word Rows of AdjMatrix
            adjMat.push_back(tempVector);
        }

        tempVector[adjMatSize-1] = 0;
        adjMat.push_back(tempVector);
    
        for(int i = 0; i < temp.length(); i++){
            tempLet = temp[i];
            for(mit = diceMap.begin(); mit != diceMap.end(); mit++){
                for(int j = 0; j < mit->second.length(); j++){
                    if(mit->second[j] == tempLet){
                        adjMat[mit->first][counter+i+1] = 1;
                    }
                }
            }
        }

        /*
        
        for(int i = 0; i < adjMat.size(); i++){
            cout << "Node " << i << ": ";
            for(int j = 0; j < adjMat.size(); j++){
                if(adjMat[i][j] == 1){
                    cout << j << ", ";
                }
            }
            cout << endl;
        }cout << endl  << endl;

        */

        vector<int> diceTrace(temp.size(), -1);

        

        if(karp(adjMat, 0, adjMat.size()-1, diceTrace) != temp.size()){
            cout << "CANNOT SPELL " << temp << endl;
        } else {
            for(int i = 0; i < diceTrace.size(); i++){
                cout << diceTrace[i];
                if(i != diceTrace.size()-1){
                    cout << ",";
                } else {
                    cout << ": ";
                }
            }
            cout << temp << endl;
        }

    
        adjMat.resize(1);
    }

    fnF.close();
    fnS.close();
}