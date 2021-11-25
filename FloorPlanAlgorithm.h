#pragma once

#include <vector>
#include <map>
#include <string>
#include "dataStruct.h"

class FloorPlan {
public:
	std::vector<Block> blocks;
	std::vector<Net> nets;
	std::map<std::string, int> blocksMap;
	std::vector<Node> treeNodes;
	int root;
	std::vector<std::string> portlandExpress;

	FloorPlan(std::vector<Block> blocks,
			std::vector<Net> nets,
			std::map<std::string, int> blocksMap);
	~FloorPlan();
	void run();
	//找最优逆波兰表达式
	void findBestExpress(std::vector<std::string>* exp);
	//初始化
	void initTree();
	//检查逆波兰表达式是否合法
	bool judgePortlandExp(std::vector<std::string> exp);
	//计算逆波兰表达式对应结果
	void computeAreaLen(int *area,double *len, std::vector<std::string> exp);
	//初始化逆波兰表达式
	void getPortlandExpress(int node, std::vector<std::string>* exp);

	//模拟退火算法实现
	void Monte_Carlo(std::vector<std::string>* exp,double lamda);
	void simulatedAnnealing(std::vector<std::string>* exp, double T0, double at, double E, int L, double lamda);
};
