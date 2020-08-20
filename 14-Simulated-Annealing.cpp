#include <iostream>
#include <io.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <cmath>
#include <cstdlib> // ��������� 
using namespace std;

#define MAXN 250
int n;
int v;
int weights[MAXN];
int bins[MAXN];
int ans;

void generateNew(int *temp) {
	for(int i = 0; i < 10; ++i) {
		int posx = rand() % n;
		int posy = rand() % n;
		swap(temp[posx], temp[posy]);
	}
}

int first_fit(int *temp) {
	ans = 0;
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

void simulatedFire(int id) {
	clock_t sTime= clock();
	
	int best = first_fit(weights);	// ��ʼ�����Ž�
	int LL = 200;					// �������� 
	double r = 0.95; 				// ���ڿ��ƽ��µĿ�����0 < r < 1 ��rԽ�󣬽���Խ������֮����Խ�� 
	double T = 200; 				// ϵͳ���¶ȣ�ϵͳ��ʼӦ��Ҫ����һ�����µ�״̬  
	double T_min = 10;				// �¶ȵ����ޣ����¶�T�ﵽT_min����ֹͣ����  
	
	int de;	
	srand((unsigned)(time(NULL)));	 //����ָ����Χ�ڵ���������� 
	int *tempWeights = new int[n];
	for(int i = 0; i < n; ++i) {
		tempWeights[i] = weights[i];
//		cout << tempWeights[i] << " ";
	}
//	cout << endl;
	 
	while(T > T_min) {
		for(int i = 0; i < LL; ++i) {
			generateNew(tempWeights);	// �����µ�����
			int temp = first_fit(tempWeights);
			de = temp - best;
			if(de < 0) {				// ������������ 
				best = temp;
				for(int i = 0; i < n; ++i)
					weights[i] = tempWeights[i];
			} else {					// ����ϲ�����ʽ��� 
				if(exp((de + rand() % 2) / (T * 0.1)) > (rand() / ((double)RAND_MAX + 2.0) * 2)) {
					for(int i = 0; i < n; ++i)
						weights[i] = tempWeights[i];
				} 
			} 
		}
		T = r * T;			// �˻� 
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
		simulatedFire(i + 1);
	}
		
} 
