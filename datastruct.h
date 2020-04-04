#ifndef DATA_H_
#define DATA_H_
#include<vector>
using namespace std;
class point //���࣬�洢��һ�����������ֵ
{
public:
	int x;
	int y;
};

class polygon //������࣬����һ�������
{
public:
	vector<point> p; //����εĶ���
	float color[3];
	bool multiColor;
};

//���ǻ�߱����������߱���
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
