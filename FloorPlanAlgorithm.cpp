#include <vector>
#include <map>
#include <string>
#include <queue>
#include "dataStruct.h"
#include "FloorPlanAlgorithm.h"
#include <iostream>
#include <stack>

using namespace std;

string op[2] = { "-","|"};

FloorPlan::FloorPlan(std::vector<Block> blocks, std::vector<Net> nets, std::map<std::string, int> blocksMap)
{
	this->blocks = blocks;
	this->nets = nets;
	this->blocksMap = blocksMap;
	srand((unsigned)time(NULL)); //设置随机数种子
}

FloorPlan::~FloorPlan()
{
	blocks.clear();
	nets.clear();
	blocksMap.clear();
	treeNodes.clear();
	portlandExpress.clear();
}

void FloorPlan::run()
{
	initTree();
	vector<string> portExp; 
	getPortlandExpress(root,&portExp);
	for (int i = 0; i < portExp.size(); i++) {
		cout << portExp[i] <<" ";
	}
	cout << endl;
	cout << judgePortlandExp(portExp) << endl;
	cout << judgePortlandExp({"2","1","|","2","1","-","-","|"}) << endl;
	findBestExpress(&portExp);

}

void FloorPlan::findBestExpress(std::vector<std::string>* exp)
{
	int area = 0;
	double len = 0;
	computeAreaLen(&area,&len,*exp);
	cout<<"Area:" << area<<endl;
	cout << "Len:" << len << endl;
	for (int i = 0; i < (*exp).size(); i++) {
		cout << (*exp)[i] << " ";
	}
	cout << endl;
	simulatedAnnealing(exp,1,0.9999,1e-6,20000,0.9);
	computeAreaLen(&area,&len,*exp);
}


void FloorPlan::initTree()
{
	std::queue<int> que;
	for (int i = 0; i < blocks.size(); i++) {
		if (blocks[i].type == HardBlock) {
			Node n;
			n.val = blocks[i].idNum;
			n.op = -1;
			n.lchild = -1;
			n.rchild = -1;
			n.parent = -1;
			treeNodes.push_back(n);
			que.push(treeNodes.size() - 1);
		}
	}
	while (!que.empty()) {
		int a = que.front();
		que.pop();
		if (que.empty()) {
			treeNodes[a].parent = -1;
			root = a;
			break;
		}
		int b = que.front();
		que.pop();
		Node n;
		n.op = 1 + rand()%2;
		n.val = -1;
		n.lchild = a;
		n.rchild = b;
		treeNodes.push_back(n);
		que.push(treeNodes.size() - 1);
		treeNodes[a].parent = treeNodes.size() - 1;
		treeNodes[b].parent = treeNodes.size() - 1;
	}
}

void FloorPlan::Monte_Carlo(std::vector<std::string>* exp,double lamda)
{
	int t = 0;
	int i, j;
	int size = (*exp).size();
	int area;
	double len,minval,val;
	computeAreaLen(&area,&len,*exp);
	std::vector<std::string> minexp = *exp;
	minval = area + lamda * len;
	while (t < 50) {
		for (int i = 0; i < size; i++) {
			j = rand() % size;
			if (i == j) {
				continue;
			}
			if (((*exp)[i] == "-" || (*exp)[i] == "|") && ((*exp)[j] == "-" || (*exp)[j] == "|")) {
				swap((*exp)[i], (*exp)[j]);
			}
			else if (((*exp)[i] != "-" && (*exp)[i] != "|") && ((*exp)[j] != "-" && (*exp)[j] != "|")) {
				swap((*exp)[i], (*exp)[j]);
				
			}
			else {
				swap((*exp)[i], (*exp)[j]);
				bool jud = judgePortlandExp(*exp);
				if (!jud) {
					swap((*exp)[i], (*exp)[j]);
				}
			}
		}
		computeAreaLen(&area, &len, *exp);
		val = area + lamda * len;
		if (val <= minval) {
			minval = val;
			minexp = *exp;
		}
		/*cout << "MC Area:" << area << endl;
		cout << "Len:" << len << endl;
		for (int i = 0; i < (*exp).size(); i++) {
			cout << (*exp)[i] << " ";
		}
		cout << endl;*/
		t++;
		cout << t << endl;
	}
	*exp = minexp;
	computeAreaLen(&area, &len, *exp);
	cout << "Area:" << area << endl;
	cout << "Len:" << len << endl;
	for (int i = 0; i < (*exp).size(); i++) {
		cout << (*exp)[i] << " ";
	}
	cout << endl;
}

void FloorPlan::simulatedAnnealing(std::vector<std::string>* exps, double T0, double at, double E, int L, double lamda)
{
	int i, j;
	int size = (*exps).size();
	int area;
	double len, minval, val, preval;
	std::vector<std::string> minexp = *exps;
	Monte_Carlo(&minexp, lamda);
	*exps = minexp;
	computeAreaLen(&area, &len, *exps);
	minval = area + lamda * len;
	preval = minval;
	while (L >= 0) {
		i = rand() % size;
		j = rand() % size;
		if (i == j) {
			continue;
		}
		if (((*exps)[i] == "-" || (*exps)[i] == "|") && ((*exps)[j] == "-" || (*exps)[j] == "|")) {
			swap((*exps)[i], (*exps)[j]);
		}
		else if (((*exps)[i] != "-" && (*exps)[i] != "|") && ((*exps)[j] != "-" && (*exps)[j] != "|")) {
			swap((*exps)[i], (*exps)[j]);
		}
		else {
			swap((*exps)[i], (*exps)[j]);
			bool jud = judgePortlandExp(*exps);
			if (!jud) {
				swap((*exps)[i], (*exps)[j]);
				continue;
			}
		}
		computeAreaLen(&area, &len, *exps);
		val = area + lamda * len;
		double sj = rand() % 10000;     //sj为0~1之间的随机数
		sj /= 10000;
		if (val < minval) {
			minval = val;
			minexp = *exps;
		}
		if (val <= preval) {
			preval = val;
		}
		else if (exp((minval - val)/T0)>sj) {
			preval = val;
		}
		else {
			swap((*exps)[i], (*exps)[j]);
		}
		T0 *= at;
		if (T0 < E) {
			break;
		}
		/*cout << "SA Area:" << area << endl;
		cout << "Len:" << len << endl;
		for (int i = 0; i < (*exps).size(); i++) {
			cout << (*exps)[i] << " ";
		}
		cout << endl;*/
		L--;
	}
	(*exps) = minexp;
	computeAreaLen(&area, &len, *exps);
	cout << "Area:" << area << endl;
	cout << "Len:" << len << endl;
	for (int i = 0; i < (*exps).size(); i++) {
		cout << (*exps)[i] << " ";
	}
	cout << endl;
}

void FloorPlan::computeAreaLen(int *area, double *len, std::vector<std::string> exp)
{
	string str1, str2, str3;
	int id = blocks.size() + 1;
	int i;
	vector<Node> nodes;
	map<string, int> maps;
	for (int i = 0; i < blocks.size(); i++) {
		if (blocks[i].type == Terminals)
			continue;
		Node n;
		n.val = atoi(to_string(blocks[i].idNum).c_str());
		n.height = blocks[i].height;
		n.width = blocks[i].width;
		n.lchild = -1;
		n.rchild = -1;
		n.parent = -1;
		nodes.push_back(n);
		maps[to_string(blocks[i].idNum)] = nodes.size() - 1;
	}
	while (true) {
		for (i = 0; i < exp.size(); i++) {
			if (exp[i] == "-" || exp[i] == "|") {
				break;
			}
		}
		if (i == exp.size())
			break;
		if (i - 2 >= 0) {
			str1 = exp[i - 2];
			str2 = exp[i - 1];
			str3 = exp[i];
			Node n;
			n.val = id;
			if (str3 == "|") {
				n.height = max(nodes[maps[str1]].height, nodes[maps[str2]].height);
				n.width = nodes[maps[str1]].width + nodes[maps[str2]].width;
				n.op = 2;
			}
			else if (str3 == "-") {
				n.width = max(nodes[maps[str1]].width, nodes[maps[str2]].width);
				n.height = nodes[maps[str1]].height + nodes[maps[str2]].height;
				n.op = 1;
			}
			n.lchild = atoi(str1.c_str());
			n.rchild = atoi(str2.c_str());
			n.parent = -1;
			nodes[maps[str1]].parent = id;
			nodes[maps[str2]].parent = id;
			nodes.push_back(n);
			maps[to_string(id)] = nodes.size() - 1;
			exp[i] = to_string(id++);
			exp.erase(exp.begin() + i - 2);
			exp.erase(exp.begin() + i - 2);
		}
	}
	*area = nodes[maps[exp[0]]].width * nodes[maps[exp[0]]].height;
	int r0 = atoi(exp[0].c_str());
	nodes[maps[exp[0]]].x = 0;
	nodes[maps[exp[0]]].y = 0;
	queue<string> q;
	q.push(exp[0]);
	while (!q.empty()) {
		string a = q.front();
		q.pop();
		int l = nodes[maps[a]].lchild;
		int r = nodes[maps[a]].rchild;
		if (l == -1 && r == -1) {
			blocks[atoi(a.c_str()) - 1].x = nodes[maps[a]].x;
			blocks[atoi(a.c_str()) - 1].y = nodes[maps[a]].y;
			continue;
		}
		if (nodes[maps[a]].op == 1) {//-
			nodes[maps[to_string(l)]].y = nodes[maps[a]].y;
			nodes[maps[to_string(l)]].x = nodes[maps[a]].x;
			nodes[maps[to_string(r)]].x = nodes[maps[a]].x;
			nodes[maps[to_string(r)]].y = nodes[maps[a]].y + nodes[maps[to_string(l)]].height;
		}
		else if (nodes[maps[a]].op == 2) {// |
			nodes[maps[to_string(l)]].y = nodes[maps[a]].y;
			nodes[maps[to_string(l)]].x = nodes[maps[a]].x;
			nodes[maps[to_string(r)]].x = nodes[maps[a]].x + nodes[maps[to_string(l)]].width;
			nodes[maps[to_string(r)]].y = nodes[maps[a]].y;
		}
		q.push(to_string(l));
		q.push(to_string(r));
	}
	std::vector<Block_tmp> block_tmp;
	double x1, x2, y1, y2;
	double allen = 0;/*
	for (int i = 0; i < nodes.size(); i++) {
		cout << nodes[i].val << " " << nodes[i].x << " " << nodes[i].y<<" "<< nodes[i].lchild << " " << nodes[i].rchild << endl;
	}
	for (int i = 0; i < blocks.size(); i++) {
		cout << blocks[i].id << " " << blocks[i].x << " " << blocks[i].y << endl;
	}*/
	for (int i = 0; i < nets.size(); i++) {
		y2 = 0;
		x2 = 0;
		y1 = INT_MAX;
		x1 = INT_MAX;
		double x, y;
		int bid;
		block_tmp = nets[i].blocks;
		for (int j = 0; j < block_tmp.size(); j++) {
			bid = block_tmp[j].idNum - 1;
			if (blocks[bid].type == HardBlock) {
				x = blocks[bid].x + 0.5 * blocks[bid].width;
				y = blocks[bid].y + 0.5 * blocks[bid].height;
				x += block_tmp[j].n1 * blocks[bid].width / 100;
				y += block_tmp[j].n2 * blocks[bid].height / 100;
			}
			else if (blocks[bid].type == Terminals) {
				x = blocks[bid].x;
				y = blocks[bid].y;
			}
			x1 = (x < x1) ? x : x1;
			x2 = (x > x2) ? x : x2;
			y1 = (y < y1) ? y : y1;
			y2 = (y > y2) ? y : y2;
		}
		allen += abs(x2 - x1) + abs(y2 - y1);
	}
	block_tmp.clear();
	*len = allen;
}

bool FloorPlan::judgePortlandExp(std::vector<std::string> exp)
{
	string str1, str2, str3;
	int id = blocks.size();
	int i;
	while (true) {
		for (i = 0; i < exp.size(); i++) {
			if (exp[i] == "-" || exp[i] == "|") {
				break;
			}
		}
		if (i == exp.size())
			break;
		if (i - 2 >= 0) {
			str1 = exp[i - 2];
			str2 = exp[i - 1];
			str3 = exp[i];
			exp[i] = to_string(id++);
			exp.erase(exp.begin() + i - 2);
			exp.erase(exp.begin() + i - 2);
		}
		else {
			/*for (int j = 0; j < exp.size(); j++) {
				cout << exp[j] << " ";
			}*/
			//cout << endl;
			return false;
		}
	}
	if (exp.size() == 1) {
		return true;
	}
	/*for (int j = 0; j < exp.size(); j++) {
		cout << exp[j] << " ";
	}
	cout << endl;*/
	return false;
}

void FloorPlan::getPortlandExpress(int node, std::vector<std::string> *exp)
{
	if (node == -1) {
		return;
	}
	if (treeNodes[node].lchild == -1 && treeNodes[node].rchild == -1) {
		(*exp).push_back(to_string(treeNodes[node].val));
		return;
	}
	vector<string> e1, e2;
	getPortlandExpress(treeNodes[node].lchild, &e1);
	getPortlandExpress(treeNodes[node].rchild, &e2);
	(*exp).insert((*exp).end(), e1.begin(), e1.end());
	(*exp).insert((*exp).end(), e2.begin(), e2.end());
	e1.clear();
	e2.clear();
	(*exp).push_back(op[treeNodes[node].op - 1]);
}
