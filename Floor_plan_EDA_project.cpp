#include <regex>
#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <map>
#include <sstream>
#include <stdlib.h>

#include "dataStruct.h"
using namespace std;

string typeStr[] = { "softrectilinear","hardrectilinear","terminal" };
vector<Block> blocks;
vector<Net> nets;

void readFromFile() {
    ifstream input1, input2;
    input1.open("ami33.blocks", ios::out | ios::in);
    input2.open("ami33.nets", ios::out | ios::in);

    string data1, data2;
    regex reg1("(.+) (softrectilinear|hardrectilinear|terminal)( \\d+){0,1}( \((\\d+)\, (\\d+)\))*");
    regex reg2("(\\()(\\d+)(\\, )(\\d+)(\\))");
    regex reg3("\\d+");
    smatch m;
    int id = 1;
    map<string, int> blocksMap;
    if (input1) {
        while (getline(input1, data1)) {
            //cout << data1 << endl;
            bool found = regex_search(data1, m, reg1);
            if (found) {
                Block b;
                b.idNum = id++;
                b.id = m.str(1);
                blocksMap[b.id] = b.idNum;
                for (int i = 0; i < 3; i++) {
                    if (typeStr[i].compare(m.str(2)) == 0) {
                        b.type = blockType(i);
                        if (b.type == HardBlock)
                            b.coordNum = 4;
                    }
                }
                if (b.type == Terminals) {
                    blocks.push_back(b);
                    continue;
                }
                string str1(m.suffix().str() + "");
                string::const_iterator its = str1.begin();
                string::const_iterator ite = str1.end();
                int j = 0;
                while (regex_search(its, ite, m, reg2)) {
                    b.coords[j].x = atoi(m.str(2).c_str());
                    b.coords[j].y = atoi(m.str(4).c_str());
                    j++;
                    its = m[0].second;
                }
                blocks.push_back(b);

            }
        }
    }
    else {
        cout << "ami33.blocks not exists!" << endl;
    }
    /*for (int i = 0; i < blocks.size(); i++) {
        cout << blocks[i].id << " " << blocks[i].idNum<<" "<<blocks[i].type<<" "
            << blocks[i].coordNum;
        for (int j = 0; j < 4; j++) {
            cout << " (" << blocks[i].coords[j].x << ",  " << blocks[i].coords[j].y<<")";
        }
        cout << endl;
    }*/

    regex reg4("(NetDegree : )(\\d+)(\n)(#(.*)\n)?");
    regex reg5("(.*)( B)(\t: %)?((-)?\\d+(.)?\\d+)?( %)?((-)?\\d+(.)?\\d+)?");
    id = 1;
    if (input2) {
        //input2 >> data2;
        ostringstream buf;
        char ch;
        while (buf && input2.get(ch)) {
            buf.put(ch);
        }
        data2 = buf.str();
        //cout << data2<<endl;
        string::const_iterator its = data2.begin();
        string::const_iterator ite = data2.end();
        while (regex_search(its, ite, m, reg4)) {
            cout << m.str() << " " << m.str(2) << endl;
            its = m[0].second;
            int j = atoi(m.str(2).c_str());
            Net net;
            net.id = id++;
            net.netDegree = j;
            while (regex_search(its, ite, m, reg5) && j > 0) {
                /*cout << m.str() <<" " << m.str(1) <<" 4:"
                    <<m.str(4)<<" 8:"
                    << m.str(8)<<" "
                    << endl;*/
                int pos = blocksMap[m.str(1)];
                Block_tmp bt;
                bt.idNum = pos + 1;
                if (strlen(m.str(4).c_str()) > 0) {
                    bt.n1 = stod(m.str(4).c_str());
                    bt.n2 = stod(m.str(8).c_str());
                }
                net.blocks.push_back(bt);
                its = m[0].second;
                j--;
            }
            nets.push_back(net);
        }
    }
    else {
        cout << "ami33.nets not exists!" << endl;
    }
    /*for (int i = 0; i < nets.size(); i++) {
        cout << nets[i].id << " " << nets[i].netDegree;
        for (int j = 0; j < nets[i].blocks.size(); j++) {
            cout << " ("<<nets[i].blocks[j].idNum << " " << nets[i].blocks[j].n1 << " " << nets[i].blocks[j].n2 << ") ";
        }
        cout << endl;
    }*/
    input1.close();
    input2.close();
}

int main()
{
    readFromFile();
}
