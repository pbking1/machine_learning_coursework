#include <iostream>
#include <fstream>
#include <sstream>
#include "decision_tree.h"

using namespace std;

decision_tree				*root;

vector<TupleData>	train,			
					test;			

vector<int>			attributes;	
// ifstream			fin;			
int trainAllNum=0;														
int testAllNum=0;								
int MaxAttr;														
int *ArrtNum;
const int leafattrnum=-1;
int TP=0,
	FN=0,
	FP=0,
	TN=0,
	TestP=0,
	TestN=0;

void init(char * trainname, char * testname)
{
	trainAllNum=readData(train, trainname);
	testAllNum=readData(test, testname);
	calculate_attributes();
	calculate_ArrtNum();
	
}

int readData(vector<TupleData> &data, const char* fileName)
{
	ifstream fin;
	fin.open(fileName);
	string line;
	
	int datanum=0;
	
	while(getline(fin,line))
	{
		TupleData d;
        istringstream stream(line);
        string str;
		while(stream>>str)
		{
			if(str.find('+')==0)
			{
				d.label='+';
			}
			else if(str.find('-')==0)
			{
				 d.label='-';
			}
			else
			{
				int j=stringtoint(str);
				d.A.push_back(j);
			}
		}
		
		data.push_back(d);	
		datanum++;
	}
	
	fin.close();
	return datanum;
}

void calculate_attributes()
{
	TupleData d=train.at(0);
	MaxAttr=d.A.size();
	for (int i = 0; i < MaxAttr; i++)
	{
		attributes.push_back(i);
	}
}
	

int stringtoint(string s)
{
	int sum=0;
	for(int i=0; s[i]!='\0';i++)
	{
		int j=int(s[i])-48;
		sum=sum*10+j;
	}
	return sum;
}

//////////////////////////////////////////////////////////////////////////
// function: calculate_ArrtNum
// brief: 找出每一个特征中最大的特征向量的值+1，存在arrtNum[i]中，共i个特征
// parameter:
// return:
//////////////////////////////////////////////////////////////////////////
void calculate_ArrtNum()
{
	ArrtNum=new int[MaxAttr];
	for(int i=0; i<MaxAttr;i++) ArrtNum[i]=0;
	for (vector<TupleData>::const_iterator it = train.begin(); it != train.end(); it++)	
	{
		int i=0;
		for (vector<int>::const_iterator intt=(*it).A.begin(); intt!=(*it).A.end();intt++)
		{
			int valuemax=(*intt)+1;   //(*it).A.at(i)???
			if(valuemax>ArrtNum[i]) ArrtNum[i]=valuemax;
			i++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// function: Entropy
// brief: 返回样本集的类别熵
// parameter: p 正样本数， s 负样本数
// return:
//////////////////////////////////////////////////////////////////////////
double Entropy(double p, double s)
{
	double n = s - p;
	double result = 0;
	if (n != 0)
		result += - double(n) / s * log(double(n) / s) / log(2.0);
	if (p != 0)
		result += double(-p) / s * log(double(p) / s) / log(2.0);
	return result;
}

int creat_classifier(decision_tree *&p, const vector<TupleData> &samples, vector<int> &attributes)
{
	// 创建节点N
	if (p == NULL)
		p = new decision_tree();
	// 如果训练集中的所有记录都属于同一个类别，则以该类别标记节点N
	if (Allthesame(samples, '+'))
	{
		p->node.label = '+';
		p->node.attrNum = leafattrnum;
		p->childs.clear();
		return 1;
	}
	if (Allthesame(samples, '-'))
	{
		p->node.label = '-';
		p->node.attrNum = leafattrnum;
		p->childs.clear();
		return 1;
	}
	// 如果训练集为空，在返回节点N标记为Failure
	if (attributes.size() == 0)
	{
		p->node.label = Majorityclass(samples);
		p->node.attrNum = leafattrnum;
		p->childs.clear();
		return 1;
	}
	// 如果候选属性为空，则返回n作为叶节点，标记为训练集中最普通的类
	p->node.label = ' ';
// 	5for each 候选属性 attribute_list
// 	6if 候选属性是联系的then
// 	7对该属性进行离散化
// 	8选择候选属性attribute_list中具有最高信息增益的属性D
// 	9标记节点N为属性D
	// 其中6、7步在这里是不需要的
	p->node.attrNum = BestGainArrt(samples, attributes);
// 	10for each 属性D的一致值d
// 	11由节点N长出一个条件为D=d的分支
// 	12设s是训练集中D=d的训练样本的集合
// 	13if s为空
// 	14加上一个树叶，标记为训练集中最普通的类
// 	15else加上一个有C4.5（R - {D},C，s）返回的点

	// 把已经使用的这个特征去掉，重新生成新的attributes
	vector<int> newAttributes;
	for (vector<int>::iterator it = attributes.begin(); it != attributes.end(); it++)
		if ((*it) != p->node.attrNum)
			newAttributes.push_back((*it));

	int maxvalue=ArrtNum[p->node.attrNum];
	vector<TupleData>* subSamples = new vector<TupleData>[maxvalue];
	for (int i = 0; i < maxvalue; i++)
		subSamples[i].clear();

	for (vector<TupleData>::const_iterator it = samples.begin(); it != samples.end(); it++)
	{
		subSamples[(*it).A.at(p->node.attrNum)].push_back((*it));
	}

	decision_tree *child;
	for (int i = 0; i < maxvalue; i++)
	{
		child = new decision_tree;
		child->node.attr = i;
		if (subSamples[i].size() == 0)
			child->node.label = Majorityclass(samples);
		else
			creat_classifier(child, subSamples[i], newAttributes);
		p->childs.push_back(child);
	}
	delete[] subSamples;
	return 0;
}

int BestGainArrt(const vector<TupleData> &samples, vector<int> &attributes)
{
	int attr, 
		bestAttr = 0,
		p = 0,
		s = (int)samples.size();
		
	for (vector<TupleData>::const_iterator it = samples.begin(); it != samples.end(); it++)
	{
		if ((*it).label == '+')
			p++;
	}
	
	double infoD;
	double bestResult = 0;
	infoD=Entropy(p, s);
	
	for (vector<int>::iterator it = attributes.begin(); it != attributes.end(); it++)
	{
		attr = (*it);
		double result = infoD;
		
		int maxvalue=ArrtNum[attr];
		int* subN = new int[maxvalue];
		int* subP = new int[maxvalue];
		int* sub = new int[maxvalue];
		for (int i = 0; i < maxvalue; i++)
		{
			subN[i] = 0;
			subP[i] = 0;
			sub[i]=0;
		}
		for (vector<TupleData>::const_iterator jt = samples.begin(); jt != samples.end(); jt++)
		{
			if ((*jt).label == '+')
				subP[(*jt).A.at(attr)] ++;
			else
				subN[(*jt).A.at(attr)] ++;
			sub[(*jt).A.at(attr)]++;
		}// 对于第it个特征，subP为正样本对应该特征的分布，subN为负样本对应该特征的分布，sub为样本的分布
		
		double SplitInfo=0;
		for(int i=0; i<maxvalue; i++)
		{
			double partsplitinfo;
			partsplitinfo=-double(sub[i])/s*log(double(sub[i])/s)/log(2.0);
			SplitInfo=SplitInfo+partsplitinfo;
		}
		
		double infoattr=0;
		for (int i = 0; i < maxvalue; i++)
		{
			double partentropy;
			partentropy=Entropy(subP[i], subP[i] + subN[i]);
			infoattr=infoattr+((double)(subP[i] + subN[i])/(double)(s))*partentropy;
		}
		result=result-infoattr;
		result=result/SplitInfo;
		
		if (result > bestResult)
		{
			bestResult = result;
			bestAttr = attr;
		}
		delete[] subN;
		delete[] subP;
		delete[] sub;
	}

	if (bestResult == 0)
	{
		bestAttr=attributes.at(0);
	}
	return bestAttr;
}

//////////////////////////////////////////////////////////////////////////
// function: Allthesame
// brief: 判断样本集内所有样本是否同类
// parameter:
// return:
//////////////////////////////////////////////////////////////////////////
bool Allthesame(const vector<TupleData> &samples, char ch)
{
	for (vector<TupleData>::const_iterator it = samples.begin(); it != samples.end(); it++)
		if ((*it).label != ch)
			return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// function: Majorityclass
// brief: 根据样本集的样本分布返回样本集的分类（少数服从多数^_^）
// parameter:
// return:
//////////////////////////////////////////////////////////////////////////
char Majorityclass(const vector<TupleData> &samples)
{
	int p = 0, n = 0;
	for (vector<TupleData>::const_iterator it = samples.begin(); it != samples.end(); it++)
		if ((*it).label == '+')
			p++;
		else
			n++;
	if (p >= n)
		return '+';
	else
		return '-';
}

char testClassifier(decision_tree *p, TupleData d)
{
	if (p->node.label != ' ')
		return p->node.label;
	int attrNum = p->node.attrNum;
	if (d.A.at(attrNum) < 0)
		return ' ';
	return testClassifier(p->childs.at(d.A.at(attrNum)), d);
}

void testData()
{
	for (vector<TupleData>::iterator it = test.begin(); it != test.end(); it++)
	{
		if((*it).label=='+') TestP++;
		else TestN++;
		if(testClassifier(root, (*it))=='+')
		{
			if((*it).label=='+') TP++;
			else FP++;
		}
		else
		{
			if((*it).label=='+') FN++;
			else TN++;
		}
	}
}

void freeClassifier(decision_tree *p)
{
	if (p == NULL)
		return;
	for (vector<decision_tree*>::iterator it = p->childs.begin(); it != p->childs.end(); it++)
	{
		freeClassifier(*it);
	}
	delete p;
}

void freeArrtNum()
{
	delete[] ArrtNum;
}

void showResult()
{
	cout<<"Train size:	"<< trainAllNum<<endl;
	cout<<"Test size:	"<<testAllNum<<endl;
	cout<<"Test positive:	"<<TestP<<endl;
	cout<<"Test negative:	"<<TestN<<endl;														
	cout << "True positive:	" << TP << endl;
	cout << "False negative:	"<< FN<<endl;
	cout << "False positive:	"<<FP<<endl;
	cout << "True negative:	"<<TN<<endl;
	
// 	cout<<TP<< endl;
// 	cout<<FN<<endl;
// 	cout<<FP<<endl;
// 	cout<<TN<<endl;
}

int main(int argc, char **argv)
{
	char * trainfile=argv[1];
	char * testfile=argv[2];
	
	init(trainfile, testfile);
	creat_classifier(root, train, attributes);
	testData();
	freeClassifier(root);
	freeArrtNum();
	showResult();
	return 0;
}
