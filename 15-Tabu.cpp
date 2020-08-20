#include <iostream>
#include <io.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <cmath>
#include <cstdlib> // 产生随机数 
using namespace std;

#define MAXN 250
int n;
int v;
int weights[MAXN];
int bins[MAXN];
int tabuTime = 15;			// 禁忌时长 
int neighbourhoodNum = 10;	// 搜索邻居个数 

struct TabuNode {
	int x;
	int y;
	int leftTime;
	
	TabuNode(){}
	TabuNode(int posx, int posy, int temp) {
		x = posx;
		y = posy;
		leftTime = temp;
	}
};

vector<TabuNode> tabuList;

void generateNeighbor(int *temp) {
	int posx, posy;
	while(1) {
		posx = rand() % n;
		posy = rand() % n;
		int i = 0;
		for(; i < tabuList.size(); ++i) {
			if(posx == tabuList[i].x && posy == tabuList[i].y) {
				break;
			}
		}
		if(i == tabuList.size())
			break;
	}
	swap(temp[posx], temp[posy]);
	
	// 更新之前的禁忌表
	for(vector<TabuNode>::iterator it = tabuList.begin(); it != tabuList.end(); ++it) {
		(*it).leftTime--;
		if((*it).leftTime == 0)
			tabuList.erase(it);
	}
	 
	// 添加到禁忌表
	TabuNode tempNode(posx, posy, tabuTime);
	tabuList.push_back(tempNode); 
}

int first_fit(int *temp) {
	int ans = 0;
	for(int i = 0; i < n; ++i) {
		int j;
		for(j = 0; j < ans; ++j) {
			if(bins[j] >= temp[i]) {
				bins[j] -= temp[i];
				break;
			}
		}
		if(j == ans) {
			bins[ans] = v - temp[i];
			ans++;
		}
	}
	return ans;
}

void tabu(int id) {
	clock_t sTime= clock();
	
	int best = first_fit(weights);	// 初始化最优解
	int LL = 300;					// 迭代次数 

	for(int i = 0; i < LL; ++i) { 
		int *tempWeights = new int[n];
		for(int i = 0; i < n; ++i) 
			tempWeights[i] = weights[i];
		
		int *smallNeighbor = new int[n];
		int isChange = 0;
		for(int j = 0; j < neighbourhoodNum; ++j) {
			generateNeighbor(tempWeights);		// 生成新的排列
			// 计算邻居答案并记录最优邻居 
			if(best > first_fit(tempWeights)) {
				smallNeighbor = tempWeights;
				best = first_fit(tempWeights);
				isChange = 1;
			}
		} 
		if(isChange) {
			for(int i = 0; i < n; ++i) 
				weights[i] = smallNeighbor[i];
		}
	}

	
	cout << "Test " << id << endl;
	cout << "The result is: " << best << endl;
	cout << "The run time is: " << (double)(clock() - sTime)  << "ms" << endl;
}

void read_from_file(string path) {
//	cout << path << endl;
	ifstream data(path);
	string line;
	int cur_row = 0;
	while(getline(data, line)) {
//		cout << line << endl;
		stringstream ss;
		ss << line;
		if(!ss.eof()) {
			if(cur_row == 0)
				ss >> n;
			else if(cur_row == 1)
				ss >> v;
			else
				ss >> weights[cur_row - 2];
			cur_row++;
		}
	} 
}

vector<string> read_from_dir() {
	intptr_t Handle;
	struct _finddata_t FileInfo;
	string p;
	string DirPath = "Data";
	vector<string> filenames;
	if((Handle = _findfirst(p.assign(DirPath).append("\\*").c_str(), &FileInfo)) == -1) {
		cout << "Directory does not exist" << endl;
	} else {
		// cout << FileInfo.name << endl;
		while(_findnext(Handle, &FileInfo) == 0) {
			//cout << FileInfo.name << endl;
			if(strcmp(FileInfo.name, ".") != 0 && strcmp(FileInfo.name, "..") != 0)
				filenames.push_back(FileInfo.name);
		}
		_findclose(Handle);
	} 
	return filenames;
}

int main() {
	vector<string> filenames = read_from_dir();
	string prePath = "Data/";
	for(int i = 0; i < filenames.size(); ++i) {
//		cout << filenames[i] << endl;
		read_from_file(prePath + filenames[i]);
//		cout << n << "  " << v << endl;
//		for(int i = 0; i < n; ++i)
//			cout << weights[i] << endl;
		tabu(i + 1);
	}
		
} 
