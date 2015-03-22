#include<iostream>
#include<fstream>
#include<map>
#include<queue>
#include<vector>
#include<string>
#include<cmath>
#include<algorithm>
#include<ctime>
#include<stack>
#include<cstring>

using namespace std;
#define _CRT_SECURE_NO_WARNINGS
#define FEATURE_SIZE 3072//fixed
#define ITEM_SIZE 1000//fixed
#define THRESHOLD_VALUE 20
#define TREE_HEIGHT 3
#define LABEL_NUMBER 10//fixed
#define ITEM_NUMBER 50//5000
#define FEATURE_NUMBER 50//500
#define TREE_NUMBER 2//10
#define NODE_NUMBER (pow(2,TREE_HEIGHT+1) - 1)
#define GAIN_THRE 0.1
#define MIN_NODE_NUMBER (ITEM_NUMBER/10/3)
#define TEST_ITEM_SIZE 4000
//all thread need
map<string, vector<int> > feature_values;
map<int, vector<int> >    id_values;
string features[FEATURE_SIZE];//3072
int All_values[ITEM_SIZE][FEATURE_SIZE];//20000*3072
int Result[ITEM_SIZE];//20000
map<int, vector<double> > test_id_values;//find one id's all values
double All_test_values[TEST_ITEM_SIZE][FEATURE_SIZE];//regularized test values
int real_test_result[TEST_ITEM_SIZE];
int total_result[TREE_NUMBER][TEST_ITEM_SIZE];
int final_result[TEST_ITEM_SIZE];

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

//independed thread need
int test_result_1[TEST_ITEM_SIZE];
vector<string> chosen_features;
vector<int> left_chosen_ids;
vector<int> righ_chosen_ids;
vector<int> root_chosen_ids;
int tree_node = 0;
int delete_node = 0;

void readvalues(){
	ifstream in("train.csv");
	cin.rdbuf(in.rdbuf());
	string instring;
	cin >> instring;
	const char *con_c = instring.c_str();
	char *c = const_cast<char*>(con_c);
	int temp = -2;
	char *ptrnext = NULL;
	char *tokenptr = strtok_r(c, ",", &ptrnext);
	while (tokenptr != NULL){
		if (temp >= 0)
			features[temp] = tokenptr;
		tokenptr = strtok_r(NULL, ",", &ptrnext);
		temp++;
	}
	for (int i = 0; i<ITEM_SIZE; i++){
		cin >> instring;
		con_c = instring.c_str();
		c = const_cast<char*>(con_c);
		temp = -2;
		char *temp_ptrnext = NULL;
		char *token = strtok_r(c, ",", &temp_ptrnext);
		while (token != NULL){
			if (temp == -2)
				temp = -2;
			else if (temp == -1)
				Result[i] = atoi(token);
			else
				All_values[i][temp] = atoi(token);
			token = strtok_r(NULL, ",", &temp_ptrnext);
			temp++;
		}
	}
}

void compute_map(){
	for (int i = 0; i<FEATURE_SIZE; i++){
		vector<int> fea_vector;
		fea_vector.clear();
		for (int j = 0; j<ITEM_SIZE; j++)
			fea_vector.push_back(All_values[j][i]);
		feature_values[features[i]] = fea_vector;
	}
	for (int i = 0; i<ITEM_SIZE; i++){
		vector<int> id_vector;
		id_vector.clear();
		for (int j = 0; j<FEATURE_SIZE; j++)
			id_vector.push_back(All_values[i][j]);
		id_values[i + 1] = id_vector;
	}
}

void read_test_values(){
	ifstream in("train.csv");
	cin.rdbuf(in.rdbuf());
	string instring;
	cin >> instring;
	const char *con_c = instring.c_str();
	char *c = const_cast<char*>(con_c);
	int temp = -2;
	char *ptrnext = NULL;
	char *tokenptr = strtok_r(c, ",", &ptrnext);
	while (tokenptr != NULL){
		if (temp >= 0)
			features[temp] = tokenptr;
		tokenptr = strtok_r(NULL, ",", &ptrnext);
		temp++;
	}
	for (int i = 0; i<TEST_ITEM_SIZE; i++){
		cin >> instring;
		con_c = instring.c_str();
		c = const_cast<char*>(con_c);
		temp = -2;
		char *temp_ptrnext = NULL;
		char *token = strtok_r(c, ",", &temp_ptrnext);
		vector<double> values;
		values.clear();
		while (token != NULL){
			if (temp == -2)
				temp = -2;
			else if (temp == -1)
				real_test_result[i] = atoi(token);
			else{
				All_test_values[i][temp] = (double)atoi(token) / 255;
				values.push_back((double)atoi(token) / 255);
			}
			token = strtok_r(NULL, ",", &temp_ptrnext);
			temp++;
		}
		test_id_values[i + 1] = values;
	}

}

vector<int> choose_feature_item(){
	vector<int> chosen_ids;
	srand((unsigned)time(NULL));
	int i = 1;
	while (i <= ITEM_NUMBER){
		int temp = rand() % ITEM_SIZE + 1;
		if (temp > TEST_ITEM_SIZE){
			chosen_ids.push_back(temp);
			i++;
		}
	}
	i = 0;
	int feature_exist[FEATURE_SIZE];
	memset(feature_exist, 0, sizeof(feature_exist));
	while (i<FEATURE_NUMBER){
		int temp = rand() % FEATURE_SIZE;
		if (feature_exist[temp] == 0){
			chosen_features.push_back(features[temp]);
			feature_exist[temp] = 1;
			i++;
		}
		else
			continue;
	}
	return chosen_ids;
}

double find_max(vector<double> tofind){
	double max = -999.0;
	int length = tofind.size();
	for (int i = 0; i<length; i++)
	if (tofind.at(i)>max)
		max = tofind.at(i);
	return max;
}

int find_max_position(vector<double> tofind){
	double max = -999.0;
	int max_position = 0;
	int length = tofind.size();
	for (int i = 0; i<length; i++){
		if (tofind.at(i)>max){
			max_position = i;
			max = tofind.at(i);
		}
	}
	return max_position;
}

//compute Gain to select a best feature to be node
double compute_Gain(vector<int> chosen_ids, vector<double> id_vector, vector<int> result, int threshold){
	//cout << "\ncomputing GAIN \n";
	if (chosen_ids.size() == 0)
		return 0.0;
	double thre = (double)threshold / (double)THRESHOLD_VALUE;

	vector<int> left, righ;
	left_chosen_ids.clear();
	righ_chosen_ids.clear();
	int length = id_vector.size();
	int value[LABEL_NUMBER];
	double I = 0.0;
	memset(value, 0, sizeof(value));
	for (int i = 0; i<length; i++)
	{
		value[result.at(i)]++;
		//value's range is from 0 to 9
		if (id_vector.at(i)<thre){
			left_chosen_ids.push_back(chosen_ids.at(i));//将样本的id的id号码分类到相应的孩子
			left.push_back(result.at(i));
		}
		else{
			righ_chosen_ids.push_back(chosen_ids.at(i));
			righ.push_back(result.at(i));
		}
	}
	for (int i = 0; i<LABEL_NUMBER; i++)
	if (value[i] != 0){
		double temp1 = (double)value[i] / (double)length;
		double temp2 = (double)log((double)value[i] / (double)length);
		I = I + (double)(temp1 * temp2 / (double)log(2.0));
	}
	I = -I;
	//end of computing original_node's I
	int left_len = left.size();
	int righ_len = righ.size();
	int left_value[10];
	int righ_value[10];
	memset(left_value, 0, sizeof(left_value));
	memset(righ_value, 0, sizeof(righ_value));
	for (int i = 0; i<left_len; i++)
		left_value[left.at(i)]++;
	for (int i = 0; i<righ_len; i++)
		righ_value[righ.at(i)]++;
	//compute the number of each aspect
	double I_left = 0.0, I_righ = 0.0;
	for (int i = 0; i<10; i++)
	if (left_value[i] != 0 && left_len != 0){
		double temp1 = (double)left_value[i] / (double)left_len;
		double temp2 = (double)log((double)left_value[i] / (double)left_len);
		I_left += (double)(temp1 * temp2 / (double)log(2.0));
	}
	for (int i = 0; i < 10; i++)
	if (righ_value[i] != 0 && righ_len != 0){
		double temp1 = (double)righ_value[i] / (double)righ_len;
		double temp2 = (double)log((double)righ_value[i] / (double)righ_len);
		I_righ += (double)(temp1 * temp2 / (double)log(2.0));
	}
	I_left = -I_left;
	I_righ = -I_righ;
	//cout << "I: " << I << "  I_l: " << I_left << "   I_r: " << I_righ << endl;
	//end of computing I
	//cout << "L_len: " << left_len << "  R_len: " << righ_len << "  len: " << length << endl;
	double E = (double)left_len / (double)length*I_left + (double)righ_len / (double)length*I_righ;
	//end of computing E
	double Gain = I - E;
	//cout << "Gain: " << Gain << endl;
	return Gain;
}

int ThesameLabel(vector<int> chosen_ids){
	int length = chosen_ids.size();
	if (length == 0)
		return -1;
	int thelabel = Result[chosen_ids.at(0) - 1];
	for (int i = 1; i<length; i++)
	if (Result[chosen_ids.at(i) - 1] != thelabel)
		return -1;
	return thelabel;
}

int MostLabel(vector<int> chosen_ids){
	int length = chosen_ids.size();
	if (length == 0)
		return -1;
	int templabel[LABEL_NUMBER];
	memset(templabel, 0, sizeof(templabel));
	for (int i = 0; i < length; i++){
		templabel[Result[chosen_ids.at(i) - 1]]++;
	}
	int max = templabel[0];
	int returnlabel = 0;
	for (int j = 1; j < 10; j++)
	if (templabel[j]>max)
		max = templabel[j], returnlabel = j;
	return returnlabel;
}

//node->threshold=-1 stands for a leaf
Node* create_node(vector<int> chosen_ids){
	tree_node++;
	vector<double> feature_entropy;
	vector<int> feature_threshold;
	int all_features_size = chosen_features.size();
	int all_items_size = chosen_ids.size();
	//chosen feature is all used, the node is a leaf and its label is set.
	//the item in a node is too small, the node is a leaf and its label is set.
	if (tree_node > NODE_NUMBER || all_items_size < MIN_NODE_NUMBER){
		Node *newnode = new Node();
		newnode->node_id = tree_node;
		newnode->left = NULL;
		newnode->feature = "leaf";
		newnode->right = NULL;
		newnode->threshold = -1;
		newnode->LeftChosenIds = chosen_ids;
		int templabel = ThesameLabel(chosen_ids);
		if (templabel != -1)
			newnode->label = templabel;
		else
			newnode->label = MostLabel(chosen_ids);
		return newnode;
	}
	if (ThesameLabel(chosen_ids) != -1){
		Node *newnode = new Node();
		newnode->node_id = tree_node;
		newnode->left = NULL;
		newnode->right = NULL;
		newnode->feature = "leaf";
		newnode->threshold = -1;
		newnode->LeftChosenIds = chosen_ids;
		newnode->label = ThesameLabel(chosen_ids);
		return newnode;
	}
	vector<int> best_left, best_righ;
	//the node is not a leaf.
	for (int i = 0; i < all_features_size; i++){
		vector<int> feature_vector = feature_values[chosen_features.at(i)];
		//某一列所有的样本
		vector<double> id_vector;
		vector<int> result_vector;
		best_left.clear();
		best_righ.clear();
		id_vector.clear();
		result_vector.clear();
		for (int j = 0; j < all_items_size; j++){
			result_vector.push_back(Result[chosen_ids.at(j) - 1]);
			id_vector.push_back((double)feature_vector.at(chosen_ids.at(j) - 1));
		}
		double max_item = find_max(id_vector);
		for (unsigned int t = 0; t < id_vector.size(); t++)
			id_vector.at(t) /= max_item;
		//regularize
		double max_entropy = -999.0;
		int best_thre = 0;
		double temp_entropy;
		for (int z = 1; z<THRESHOLD_VALUE; z++){
			temp_entropy = compute_Gain(chosen_ids, id_vector, result_vector, z);
			if (temp_entropy>max_entropy){
				max_entropy = temp_entropy;
				best_thre = z;
				best_left = left_chosen_ids;
				best_righ = righ_chosen_ids;
			}
		}
		//choose threshold of a feature
		feature_entropy.push_back(max_entropy);
		feature_threshold.push_back(best_thre);
	}
	int best_position = find_max_position(feature_entropy);
	string best_feature = chosen_features.at(best_position);
	int best_threshold = feature_threshold.at(best_position);
	//found best feature and its threshold
	vector<string>::iterator it = chosen_features.begin() + best_position;
	chosen_features.erase(it);
	//delete used feature
	Node *newnode = new Node();
	newnode->node_id = tree_node;
	//pruning when gain is too small
	if (best_threshold < GAIN_THRE){
		cout << "GAIN too small" << endl;
		newnode->left = NULL;
		newnode->right = NULL;
		newnode->threshold = -1;
		newnode->feature = "leaf";
		newnode->LeftChosenIds = chosen_ids;
		int templabel = ThesameLabel(chosen_ids);
		if (templabel != -1)
			newnode->label = templabel;
		else
			newnode->label = MostLabel(chosen_ids);
		return newnode;
	}
	newnode->feature = best_feature;
	newnode->threshold = best_threshold;
	newnode->LeftChosenIds = best_left;
	newnode->RightChosenIds = best_righ;
	return newnode;
}

Node* create_DT(vector<int> chosen_ids){ //& chosen_features
	Node *root = new Node();
	root = create_node(chosen_ids);
	queue<Node*> nodes;
	while (!nodes.empty())
		nodes.pop();
	nodes.push(root);
	Node * first_node;
	while (chosen_features.size()>0 && !nodes.empty()){
		first_node = nodes.front();
		nodes.pop();
		if (first_node->threshold != -1 && chosen_features.size() > 0){
			if (first_node->LeftChosenIds.size() > 0){
				Node *left_node = create_node(first_node->LeftChosenIds);
				nodes.push(left_node);
				first_node->left = left_node;
			}
			else
				first_node->left = NULL;
		}
		if (first_node->threshold != -1 && chosen_features.size() > 0){
			if (first_node->RightChosenIds.size() > 0){
				Node *right_node = create_node(first_node->RightChosenIds);
				nodes.push(right_node);
				first_node->right = right_node;
			}
			else
				first_node->right = NULL;
		}
	}
	if (chosen_features.size() == 0){
		while (!nodes.empty()){
			first_node = nodes.front();
			nodes.pop();
			if (first_node->left == NULL){
				if (first_node->LeftChosenIds.size() == 0)
					first_node->left = NULL;
				else
					first_node->left = create_node(first_node->LeftChosenIds);
			}
			if (first_node->right == NULL){
				if (first_node->RightChosenIds.size() == 0)
					first_node->right = NULL;
				else
					first_node->right = create_node(first_node->RightChosenIds);
			}
		}
	}
	return root;
}

double compute_eTt(Node* anode){
	if (anode == NULL)
		return 0.0;
	double returnval = 0.0;
	stack<Node*> Stack;
	Node *node = anode;
	while (node != NULL || !Stack.empty()){
		while (node != NULL){
			if (node->threshold == -1){
				int n_size = node->LeftChosenIds.size();
				double n_error = 0.0;
				for (int i = 0; i < n_size; i++)
				if (Result[node->LeftChosenIds.at(i) - 1] != node->label)
					n_error += 1.0;
				n_error += 0.5;
				returnval += (double)n_error;
			}
			Stack.push(node);
			node = node->left;
		}
		node = Stack.top();
		Stack.pop();
		node = node->right;
	}
	return returnval;
}

void delete_subtree(Node* anode){
	if (anode == NULL)
		return;
	delete_subtree(anode->left);
	delete_subtree(anode->right);
	delete anode;
}

Node* PEP_pruning(Node * anode){
	vector<int> anode_ids = anode->LeftChosenIds;
	//还原该节点的所有样本
	anode_ids.insert(anode_ids.begin(), anode->RightChosenIds.begin(), anode->RightChosenIds.end());
	//节点的总样本数:n_t
	int n_t = anode_ids.size();
	//e'(t):e_t
	int mostlabel = MostLabel(anode_ids);
	double e_t = 0.0;
	for (int i = 0; i < n_t; i++)
	if (Result[anode_ids.at(i) - 1] != mostlabel)
		e_t += 1.0;
	e_t += 0.5;
	//e'(Tt):e_Tt
	double e_Tt = compute_eTt(anode);
	//SE
	double SE = sqrt((double)e_Tt * ((double)n_t - (double)e_Tt) / (double)n_t);
	//validate
	if (e_t <= (e_Tt + SE)){//pruning
		delete_node++;
		anode->threshold = -1;
		anode->label = mostlabel;
		anode->feature = "";
		Node* temp_l_child = anode->left;
		Node* temp_r_child = anode->right;
		delete_subtree(temp_l_child);
		delete_subtree(temp_r_child);
		anode->left = NULL;
		anode->right = NULL;
		anode->LeftChosenIds.clear();
		anode->RightChosenIds.clear();
		return anode;
	}
	return anode;
}

void PreOrderTraverse(Node *root)//非递归先序遍历
{
	Node *newnode = root;
	stack<Node*> Stack;
	if (newnode == NULL)
	{
		return;
	}
	while ((newnode != NULL && newnode->threshold != -1) || !Stack.empty())
	{
		while (newnode != NULL && newnode->threshold != -1)
		{
			Stack.push(newnode);
			newnode = PEP_pruning(newnode);//process node
			newnode = newnode->left;
		}
		newnode = Stack.top();
		Stack.pop();
		newnode = newnode->right;
	}
}

void compute_test(Node *root){
	memset(test_result_1, -1, sizeof(test_result_1));
	Node * Root = root;
	for (int item = 1; item <= TEST_ITEM_SIZE; item++){
		while (Root->label == -1){
			string node_feature = Root->feature;
			const char* temp_c = node_feature.c_str();
			int temp_c_len = strlen(temp_c);
			char c_c[5];
			int i = 5;
			for (i = 5; i < temp_c_len; i++)
				c_c[i - 5] = temp_c[i];
			c_c[temp_c_len - 5] = '\0';
			int position = atoi(c_c) - 1;
			vector<double> item_value = test_id_values[item];
			double thre = (double)Root->threshold / (double)THRESHOLD_VALUE;
			if (item_value.at(position) < thre){
				Root = Root->left;
				if (Root->label != -1 && test_result_1[item - 1] == -1)
					test_result_1[item - 1] = Root->label;
			}
			else{
				Root = Root->right;
				if (Root->label != -1 && test_result_1[item - 1] == -1)
					test_result_1[item - 1] = Root->label;
			}
		}
		Root = root;
	}
}

void root_rank(){
	//-------------------------------------------read data--------------------------------------
	cout << "Thread Main: Start reading data" << endl;
	readvalues();
	compute_map();

	//-------------------------------------------read test data---------------------------------
	cout << "Thread Main: Start reading test data" << endl;
	read_test_values();
}

void thread_rank(int rank){
	ofstream fout;
	fout.open("result_1.txt");
	//-----------------------------------------choose items-------------------------------------
	cout << "Thread "<<rank<<": Start randomly choosing ids and features" << endl;
	root_chosen_ids = choose_feature_item();
	//-------------------------------------------create DT---------------------------------------
	cout << "Thread " << rank << ": Start creating DT" << endl;
	Node *Root = create_DT(root_chosen_ids);
	//--------------------------------------------pruning----------------------------------------
	cout << "Thread " << rank << ": Start pruning" << endl;
	PreOrderTraverse(Root);
	//-------------------------------------------compute test data-------------------------------
	cout << "Thread " << rank << ": Start computing test data" << endl;
	compute_test(Root);
	for (int i = 0; i < TEST_ITEM_SIZE; i++)
		total_result[rank][i] = test_result_1[i]; 
	delete_subtree(Root);
	root_chosen_ids.clear();
	left_chosen_ids.clear();
	righ_chosen_ids.clear();
	tree_node = 0;
	delete_node = 0;
	chosen_features.clear();
	cout << "Thread " << rank << ": complete !" << endl;
}

int max_label(int a[]){
	int returnval = -1;
	int max = -1;
	for (int i = 0; i < LABEL_NUMBER; i++){
		if (a[i]>max){
			max = a[i];
			returnval = i;
		}
	}
	return returnval;
}

void compute_final_result(){
	ofstream fout;
	fout.open("result_1.txt");
	for (int j = 0; j < TEST_ITEM_SIZE; j++){
		int temp[LABEL_NUMBER];
		memset(temp, 0, sizeof(temp));
		for (int i = 0; i < TREE_NUMBER; i++){
			cout << i << "  " << total_result[i][j] << endl;
			temp[total_result[i][j]]++;
		}
		final_result[j] = max_label(temp);
	}
	for (int i = 0; i < TEST_ITEM_SIZE; i++){
		fout << i + 1 << "   " << final_result[i] << "-----" << real_test_result[i] << endl;
	}
	fout << flush;
	fout.close();
}

int main(){
	memset(total_result, 0, sizeof(total_result));
	root_rank();
	for (int i = 0; i < TREE_NUMBER;i++){
		thread_rank(i);
		cout<<"The "<<i<<"th tree done"<<endl;
	}
	//这里的total_result二维数组就是最终的结果，每一行数一棵树，每一列是一个测试数据
	cout << "Begin computing final labels" << endl;
	compute_final_result();
	system("pause");
}
