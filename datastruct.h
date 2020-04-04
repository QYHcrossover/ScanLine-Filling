#ifndef DATA_H_
#define DATA_H_
#include<vector>
using namespace std;
class point //点类，存储了一个点的两坐标值
{
public:
	int x;
	int y;
};

class polygon //多边形类，存了一个多边形
{
public:
	vector<point> p; //多边形的顶点
	float color[3];
	bool multiColor;
};

//既是活化边表项，又是有序边表项
struct edge{
    int ymax;
    float dx;
    float xmin;
    edge(int y,float d,float x){ymax=y;dx=d;xmin=x;};
    bool operator<(const edge &other)const{
        return xmin<other.xmin;
    }
};


#endif
