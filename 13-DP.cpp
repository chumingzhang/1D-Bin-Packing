#include <iostream>
#include <io.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <memory.h>
using namespace std;

#define MAXN 250
int n;
int v;
int weights[MAXN];
int bins[MAXN];
int ans;

int vis[MAXN][10005];
int used;	// 已经装了多少个物品 

void beibao() {
	int dp[10005] = {0};
	memset(vis, 0, sizeof(vis));
    for(int i = 0; i < n; ++i) {
    	if(weights[i] != 0) {
    		for(int j = v; j >= weights[i]; --j) {
    			dp[j] = max(dp[j], dp[j - weights[i]] + weights[i]);
    			vis[i][j] = 1;
			}
		}
	}
	
	// 输出方案 
    for(int i = 0, j = dp[v]; i < n; i++) {
    	if(vis[i][j]) {
//            cout << weights[i] << endl;
            used++;
            j -= weights[i];
            weights[i] = 0;
        }
	}         
//    cout << dp[v];
}

void DP(int id) {
	clock_t sTime= clock();
	used = 0;
	ans = 0;
	while(used != n) {
		beibao();
		ans++;
	}
	cout << "Test " << id << endl;
	cout << "The result is: " << ans << endl;
	cout << "The run time is: " << (double)(clock() - sTime) << "ms" << endl;
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
		DP(i + 1);
	}
		
} 
