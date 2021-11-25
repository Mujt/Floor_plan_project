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
	//�������沨�����ʽ
	void findBestExpress(std::vector<std::string>* exp);
	//��ʼ��
	void initTree();
	//����沨�����ʽ�Ƿ�Ϸ�
	bool judgePortlandExp(std::vector<std::string> exp);
	//�����沨�����ʽ��Ӧ���
	void computeAreaLen(int *area,double *len, std::vector<std::string> exp);
	//��ʼ���沨�����ʽ
	void getPortlandExpress(int node, std::vector<std::string>* exp);

	//ģ���˻��㷨ʵ��
	void Monte_Carlo(std::vector<std::string>* exp,double lamda);
	void simulatedAnnealing(std::vector<std::string>* exp, double T0, double at, double E, int L, double lamda);
};
