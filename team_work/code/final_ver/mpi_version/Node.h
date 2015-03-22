#include<string>
#include<iostream>
#include<vector>
using namespace std;

struct Node{
	int node_id;
	string feature;
	int threshold;
	int label;
	Node* left;
	Node* right;
	vector<int> LeftChosenIds;
	vector<int> RightChosenIds;
	Node(){
		node_id = -1;
		label = -1;
		left = NULL;
		right = NULL;
		feature = "";
		threshold = -1;
		LeftChosenIds.clear();
		RightChosenIds.clear();
	}
};