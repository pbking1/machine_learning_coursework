#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include<set>
#include<cmath>
using namespace std;
const int DataRow = 14;
const int DataColumn = 6;
struct Node{
	double value; //the probability of yes
	int attrid;
	Node *parentNode;
	vector<Node *> childNode;
};

string DataTable[DataRow][DataColumn];
map<string, int> str2int;
set<int> S;
set<int> Attributes;
string attrname[DataColumn] = { "Day", "Outlook", "Temperature", "Humidity", "wind", "PlayTennis"};
string attrValue[DataColumn][DataRow] = 
{
	{},
	{"Sunny", "Overcast", "Rain"},
	{"Hot", "Mild", "Cool"},
	{"High", "Normal"},
	{"Weak", "Strong"},
	{"No", "Yes"}
};

int attrCount[DataColumn] = {14,3,3,2,2,2};
double lg2(double n){
	return log(n)/log(2);
}

void init(){
	ifstream fin("data.txt");
	for(int i = 0; i < 14; i++){
		for(int j = 0; j < 6; j++){
			fin>>DataTable[i][j];
		}
	}
	fin.close();
	for(int i = 1; i <= 5; i++){
		str2int[attrname[i]] = i;
		for(int j = 0; j < attrCount[i]; j++)
			str2int[attrValue[i][j]] = j;
	}
	for(int i = 0; i < DataRow; i++)
		S.insert(i);
	for(int i = 1; i <= 4; i++)
		Attributes.insert(i);
}

double Entropy(const set<int> &s){
	double yes = 0, no = 0, sum = s.size(), ans = 0;
	for(set<int>::iterator it=s.begin(); it != s.end(); it++){
		string s = DataTable[*it][str2int["PlayTennis"]];
		if(s == "Yes")
			yes++;
		else
			no++;
	}
	if(no == 0 || yes == 0)
		return ans = 0;
	ans = -yes/sum * lg2(yes/sum)-no/sum*lg2(no/sum);
	return ans;
}

double Gain(const set<int> &example, int attrid){
	int attrcount = attrCount[attrid];
	double ans = Entropy(example);
	double sum = example.size();
	set<int> *pset = new set<int>[attrcount];
	for(set<int>::iterator it=example.begin(); it != example.end(); it++){
		pset[str2int[DataTable[*it][attrid]]].insert(*it);
	}
	for(int i = 0; i < attrcount; i++){
		ans -= pset[i].size()/sum*Entropy(pset[i]);
	}
	return ans;
}

int FindBestAttribute(const set<int>&example, const set<int> &attr){
	double mx = 0;
	int k = -1;
	for(set<int>::iterator i = attr.begin(); i != attr.end(); i++){
		double ret = Gain(example, *i);
		if(ret > mx){
			mx = ret;
			k = *i;
		}
	}
	if(k == -1)
		cout<<"Find Best Attribute error!"<<endl;
	return k;
}

Node *ID3(set<int> example, set<int> &attributes, Node *parent){
	Node *now = new Node;//create tree node
	now -> parentNode = parent;
	if(attributes.empty())
		return now;//if the attribute list is empty, then reurn empty
	//count the example,if they are all positive or all negative then they reach the bottom
	//the leaf is known when the childnode is null
	int yes = 0, no = 0, sum = example.size();
	for(set<int>::iterator it = example.begin(); it != example.end(); it++){
		string s = DataTable[*it][str2int["PlayTennis"]];
		if(s == "Yes")
			yes++;
		else
			no++;
	}
	if(yes == sum || yes == 0){
		now -> value = yes/sum;
		return now;
	}

	//in order to find the attribute with highest gain and remove it from the set
	int bestattrid = FindBestAttribute(example, attributes);
	now -> attrid = bestattrid;
	attributes.erase(attributes.find(bestattrid));

	//split the example attribute into different split using the best attribute, and each split is a tree
	vector< set<int> > child = vector< set<int> >(attrCount[bestattrid]);
	for(set<int>::iterator i = example.begin(); i != example.end(); i++){
		int id = str2int[DataTable[*i][bestattrid]];
		child[id].insert(*i);
	}
	for(int i = 0; i < child.size(); i++){
		Node *ret = ID3(child[i], attributes, now);
		now -> childNode.push_back(ret);
	}
	return now;
}

int main(){
	init();
	Node *Root = ID3(S, Attributes, NULL);
	return 0;
}






















