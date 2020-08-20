#include <iostream>
#include <io.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <algorithm>
using namespace std;

#define MAXN 250
int n;
int v;
int weights[MAXN];
int bins[MAXN];
int ans;

bool cmp(int a, int b) {
	return a < b;
}

void first_fit_ascend(int id) {
	clock_t sTime= clock();
	sort(weights, weights + n, cmp);
	ans = 0;
	
	int last_bin = 0;
	for(int i = 0; i < n; ++i) {
		int j = last_bin;
		for(; j < ans; ++j) {
			if(bins[j] >= weights[i]) {
				bins[j] -= weights[i];
				last_bin = j;
				break;
			}
		}
		if(j == ans) {
			bins[ans] = v - weights[i];
			last_bin = ans;
			ans++;
		}
	}
	cout << "Test " << id << endl;
	cout << "The result is: " << ans << endl;
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
		first_fit_ascend(i + 1);
	}
		
} 
