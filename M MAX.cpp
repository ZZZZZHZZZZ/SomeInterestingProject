#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <random>
using namespace std;
//基于高斯分布的互不相关的分布估计算法
//求解 解空间大小为 S 的 N 维 M max 问题
bool test(const vector<double>& ave, const vector<double>& var, int M);
void printPairVec(const vector<pair<int, int>>& adaptabilityPairVec);
void printSolutionVec(const vector<vector<int>>& temp);
void printVec(const vector<vector<int>>& temp, const vector<double>& ave, const vector<double>& var);
bool cmp(const pair<int, int>& a, const pair<int, int>& b);
void caculateAdaptability(const vector<vector<int>>& solutionVec, vector<pair<int, int>>& adaptabilityPairVec);
void getBetterSolution(int N, const vector<vector<int>>& solutionVec, vector<vector<int>>& betterSolutionVec, const vector<pair<int, int>>& adaptabilityPairVec);
void getAverageAndVariance(const vector<vector<int>>& betterSolutionVec, vector<double>& averageVec, vector<double>& varianceVec);
const double getXiAverage(const vector<int>& temp);
const double getXiVariance(const vector<int>& temp, const double average);
void generateSolution(vector<vector<int>>& solutionVec, const vector<double>& averageVec, const vector<double>& varianceVec, int M);
vector<int> generate(const double average, const double variance, int M);
int S = 0;
int main() {
	cout << "求解解空间大小为 S 的 N 维 M max 问题" << endl;
	cout << "请输入生成的解空间的大小S: " << endl;
	cin >> S;
	int N = 0;//N维
	int M = 1;//M max
	cout << "请输入求解的M max问题的维度N: " << endl;
	cin >> N;
	cout << "请输入求解的M max问题的单元素最大值M: " << endl;
	cin >> M;
	
	vector<vector<int>> solutionVec(N, vector<int>(S, 0));//整体解空间，初始化一个N*100的二维Vector，
	vector<vector<int>> betterSolutionVec(N, vector<int>(S/2, 0));//优势解空间，初始化为一个N*50的二维Vector，
	//为了方便，如果按照二维数组观察，解集中每组解按竖行显示，横行为每一组Xi的解
	//{1， {1， {0， {0， {1  X0
	// 0，  1，  0，  1，  0  X1
	// 1，  0，  1，  0，  1  X2
	// 0，  1，  1，  0，  1  X3
	// 1}， 1}， 1}， 1}， 1} X4
	// 解1 解2	解3	 解4  解5
	vector<double> averageVec(N, M/2);
	vector<double> varianceVec(N, 1);
	vector<pair<int, int>> adaptabilityPairVec(S); //存储格式为<解ID，适应度>，初始化大小为50
	int i = 1;
	while (test(averageVec,varianceVec,M)==false) {
		generateSolution(solutionVec, averageVec, varianceVec, M);//根据高斯分布生成新的解空间
		cout << "新的完整解空间: " << endl;
		printSolutionVec(solutionVec);
		caculateAdaptability(solutionVec, adaptabilityPairVec);//计算每一组解的适应度
		cout << "解的适应度按降序排序: <解ID，适应度>" << endl;
		printPairVec(adaptabilityPairVec);
		getBetterSolution(N, solutionVec, betterSolutionVec, adaptabilityPairVec);//获取优势解
		getAverageAndVariance(betterSolutionVec, averageVec, varianceVec);//计算高斯分布
		cout << "优势解空间: " << endl;
		printVec(betterSolutionVec,averageVec,varianceVec);
		cout << "----------------------------------第" << i << "轮：" << "----------------------------------" << endl;
		++i;
	}
	cout << "已达到最优值，最终解为: " << endl;
	for (int i = 0; i < betterSolutionVec.size(); ++i) {
		cout << betterSolutionVec[i][0] << " ";
	}
	cout << "average: " << averageVec[0] << " variance: " << varianceVec[0] << endl;
	return 0;
}
bool test(const vector<double>& ave, const vector<double>& var, int M) {
	for (auto i : ave) {
		if(i!=M){
			return false;
		}
	}
	for (auto i : var) {
		if (i != 0) {
			return false;
		}
	}
	return true;
}
void printPairVec(const vector<pair<int, int>>& adaptabilityPairVec) {
	for (auto i : adaptabilityPairVec) {
		cout << "<"<<i.first << "," << i.second <<">"<< " ";
	}
	cout << endl;
}
void printSolutionVec(const vector<vector<int>>& temp) {
	for (auto i : temp) {
		for (auto j : i) {
			cout << j << " ";
		}
		cout << endl;
	}
}
void printVec(const vector<vector<int>>& temp,const vector<double>& ave,const vector<double>& var) {
	for (int i = 0; i < temp.size(); ++i) {
		for (int j = 0; j < temp[i].size(); ++j) {
			cout << temp[i][j] << " ";
		}
		cout << "average: "<< ave[i] << " variance: " << var[i] << endl;
	}
}
bool cmp(const pair<int, int>& a, const pair<int, int>& b) {
	//排序用的临时算法
	return a.second > b.second;
}
void caculateAdaptability(const vector<vector<int>>& solutionVec, vector<pair<int, int>>& adaptabilityPairVec) {
	//对当前解空间中的每组解计算适应度并按降序排序
	for (int j = 0; j < solutionVec[0].size(); ++j) {//解ID
		double sum = 0;
		for (int i = 0; i < solutionVec.size(); ++i) {
			sum = sum + solutionVec[i][j];
		}
		adaptabilityPairVec[j] = pair<int, int>(j, sum);
	}
	sort(adaptabilityPairVec.begin(), adaptabilityPairVec.end(), cmp);
}
void getBetterSolution(int N, const vector<vector<int>>& solutionVec, vector<vector<int>>& betterSolutionVec, const vector<pair<int, int>>& adaptabilityPairVec) {
	//按照排序后的适应度Vector中的解ID，选择解空间中的前一半
	for (int j = 0; j < adaptabilityPairVec.size() / 2;  ++j) {
		for (int i = 0; i < solutionVec.size(); ++i) {
			betterSolutionVec[i][j] = solutionVec[i][adaptabilityPairVec[j].first];
		}
	}
}
void getAverageAndVariance(const vector<vector<int>>& betterSolutionVec, vector<double>& averageVec, vector<double>& varianceVec) {
	//根据选优后的解空间计算每一个Xi的高斯分布（均值和方差）
	for (int i = 0; i < betterSolutionVec.size(); ++i) {
		averageVec[i] = getXiAverage(betterSolutionVec[i]);
		varianceVec[i] = getXiVariance(betterSolutionVec[i], averageVec[i]);
	}
}
const double getXiAverage(const vector<int>& temp) {
	//根据Xi的值计算当前Xi的均值
	double sum = 0;
	for (auto i : temp) {
		sum += i;
	}
	return sum / temp.size();
}
const double getXiVariance(const vector<int>& temp, const double average) {
	//根据Xi的值计算当前Xi的方差
	double sum = 0;
	for (auto i : temp) {
		sum += (i - average) * (i - average);
	}
	return sum / temp.size();
}
void generateSolution(vector<vector<int>>& solutionVec, const vector<double>& averageVec, const vector<double>& varianceVec, const int M) {
	//根据整体的高斯分布生成随机数达到整体的矩阵
	for (int i = 0; i < averageVec.size(); ++i) {
		solutionVec[i] = generate(averageVec[i], varianceVec[i], M);
	}
}
vector<int> generate(double average, double variance, const int M) {
	//根据某一维的高斯分布生成这一维
	if (average == M && variance == 0.0) {
		vector<int> vals(S,M);
		return vals;
	}
	random_device rd;
	default_random_engine e{ rd() };
	vector<int> vals(S);
	//如果产生了一维Xi方差为0的值，并且没有达到最优均值，因为高斯分布不能产生方差为0的随机数，因此将此维方差初始化为1
	if (variance == 0.0) {
		//average = M/2;
		variance = 1;
	}
	normal_distribution<double> n(average, variance);
	for (int i = 0; i < S; ++i) {
		unsigned temp = lround(n(e));
		if (0 <= temp && temp <= M) {
			vals[i] = temp;
		}
		else {
			--i;
		}
	}
	return vals;
}
