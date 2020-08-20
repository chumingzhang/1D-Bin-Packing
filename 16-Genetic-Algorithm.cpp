#include <iostream>
#include <io.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <cstdlib> // 产生随机数 
using namespace std;

#define MAXN 250
int n;
int v;
int weights[MAXN];
int bins[MAXN];
int *origins[20];	// 初始种群
int *child[20];	// 存放生成的子代 
int specieCount = 20;

void generation(int k, int *p1, int *p2) {
	int *c1 = new int[n];
	int *c2 = new int[n];
	for(int i = 0; i < n; ++i) {
		c1[i] = p1[i];
		c2[i] = p2[i];
	}
		
	int start = rand() % (n - 11);
	for(int i = start; i < start + 10; ++i) {
		swap(c1[i], c2[i]);
	}
	child[k] = c1;
	child[k + 1] = c2;
}

int* generateOrigins() {
	int *temp = new int[n];
	for(int i = 0; i < n; ++i)
		temp[i] = weights[i];
	
	for(int i = 0; i < 10; ++i) {
		int posx = rand() % n;
		int posy = rand() % n;
		swap(temp[posx], temp[posy]);
	}
	return temp;
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

bool cmp(int a[], int b[]) {
	return a[1] < b[1];
}

void GA(int id) {
	clock_t sTime= clock();
	
	int best;
	// 随机生成20个排序构成基本种群 
	for(int i = 0; i < specieCount; ++i) {
		origins[i] = generateOrigins();
	}
	int LL = 300;					 // 迭代次数 
	srand((unsigned)(time(NULL)));	 //返回指定范围内的随机浮点数 

	for(int i = 0; i < LL; ++i) { 
		// 生成子代 
		for(int j = 0; j < specieCount; j += 2) {
			generation(j, origins[j], origins[j + 1]);
		}
		// 构建筛选条件 
		int **record = new int*[specieCount * 2];
		for(int i = 0; i < specieCount; ++i) {
			int tempParent[] = {i, first_fit(origins[i])};
			int tempChild[] = {i + specieCount, first_fit(child[i])};
			record[i] = tempParent;
			record[i + specieCount] = tempChild;
		} 
		// 真正的筛选
		sort(record, record + (specieCount * 2), cmp);
		best = record[0][1];
		for(int i = 0; i < specieCount; ++i) {
			if(record[i][0] < 20)
				origins[i] = origins[record[i][0]];
			else
				origins[i] = child[record[i][0] - specieCount];
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
		GA(i + 1);
	}
		
} 
