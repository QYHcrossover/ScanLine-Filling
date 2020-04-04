#include<windows.h>
#include<GL/glut.h>
#include"datastruct.h"
#include<bits/stdc++.h>

static const  int screenwidth = 1000;  //自定义窗口宽度
static const int screenheight = 1000; //自定义窗口高度
vector<point> p; //多边形点集向量
vector<polygon> s; //多边形类向量，用来保存已经画完的多边形
int move_x,move_y; //鼠标当前坐标值，在鼠标移动动态画线时使用
bool selectt = false; //多边形封闭状态判断变量，当为true时即按下鼠标右键，将多边形封闭
float COLOR[3] = {1.0,0.0,0.0}; //当前颜色
bool multiColor = false; //当前是否是多色
int interPixel = 10; //10个像素为一种颜色
float allColor[4][3] = {1,0,0,0,1,0,0,0,1,0,0,0}; //定义颜色表

void init()
{
	glClearColor(1.0,1.0,1.0,0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0,screenwidth,0.0,screenheight);
}

void paint(polygon &pg){
    vector<point> p = pg.p;
    list<edge> AET; //用list定义活化边表
    map< int,list<edge> > NET; //用map定义有序边表
    //初始化有序边表
    for(unsigned i=0;i<p.size();i++){
        point pa = p[i]; //起点
        point pb = p[(i+1)%p.size()]; //终点
        if(pa.y==pb.y) continue; //水平边不考虑
        float dx = 1.0*(pa.x-pb.x)/(pa.y-pb.y); //dx的计算公式
        //根据前后两点的y值大小建立边，并加入有序边表中
        if(pa.y<pb.y){
            edge e(pb.y-1,dx,pa.x);
            NET[pa.y].push_back(e);
        }
        else{
            edge e(pa.y-1,dx,pb.x);
            NET[pb.y].push_back(e);
        }
    }
    //活性边表的初始化
    map< int,list<edge> >::iterator it = NET.begin(); //编历NET的迭代器
    int y = it->first; //当前y值初始化为整个多边形的最低点
    //进入主循环不断更新AET
    glColor3fv(pg.color);
    int idpixel = 0;
    while(AET.size()||it!=NET.end()){
        //检查AET中的边是否超出范围
        for(list<edge>::iterator ait=AET.begin();ait!=AET.end();){
            if(y>ait->ymax) ait = AET.erase(ait);
            else ait++;
        }
        //检查是否有新的边加入
        if(it!=NET.end()&&y>=it->first){
            AET.insert(AET.begin(),it->second.begin(),it->second.end());
            it++;
        }
        //排序
        AET.sort();
        //匹配+画线
        if(pg.multiColor) glColor3fv(allColor[(idpixel++/interPixel)%4]);
        else glColor3fv(pg.color);
        list<edge>::iterator ait=AET.begin();
        while(ait!=AET.end()){
            float xa = (ait++)->xmin;
            float xb = (ait++)->xmin;
            glBegin(GL_LINES); //将已经确定的点连接起来
                glVertex2i((int)xa, y);
                glVertex2i((int)xb, y);
            glEnd();
        }
        //进入下个扫描线,每条边加上dx
        y++;
        for(list<edge>::iterator ait=AET.begin();ait!=AET.end();ait++){
            ait->xmin += ait->dx;
        }
    }
}

void lineSegment()
{
	glClear(GL_COLOR_BUFFER_BIT);
	int i, j;                 //两个循环控制变量，在下面的向量循环和数组循环中将被多次调用。
	if(!s.empty())            //看多边形类向量是否为空，即判断除了当前正在画的多边形是否还有曾经已经画好的多边形
	{
		for( i = 0; i < s.size(); i++)   //对多边形类向量循环，该向量中的每个元素代表一个多边形
		{
            glColor3fv(s[i].color); //设置当前颜色为多边形颜色
			int k = s[i].p.size();  //将一个多边形的点的个数，后面划线会用到
			for(j = 0; j < s[i].p.size(); j++) //画多边形
			{
				glBegin(GL_LINES); //将当前的点与后一个点连线
					glVertex2i(s[i].p[j].x, s[i].p[j].y);
					glVertex2i(s[i].p[(j+1)%k].x, s[i].p[(j+1)%k].y);//，通过取模操作来避免越界问题，该思路来源于循环队列.
				glEnd();
			}
			paint(s[i]);  //为当前的多边形填充颜色
//			s[i].edge.clear(); //清空当前多边形的边向量
		}
	}
	i = 0;
	j = p.size() - 1;
	glColor3fv(COLOR);   //设定颜色，既是线段颜色也是填充色
	while(i < j)           //循环画图，将当前正在画的多边形
	{
		glBegin(GL_LINES); //将已经确定的点连接起来
			glVertex2i(p[i].x, p[i].y);
			glVertex2i(p[i+1].x, p[i+1].y);
		glEnd();
		i++;
	}
	if(!p.empty())
	{
//		int i = p.size() - 1; //将确定的最后一个点与当前鼠标所在位置连线，即动态画线
		glBegin(GL_LINES);
			glVertex2i(p[j].x, p[j].y);
			glVertex2i(move_x, move_y);
		glEnd();
	}
	if(selectt) //判断右键是否被点下
	{
		selectt = false; //将状态值置为假
		if(!p.empty())
		{
			glBegin(GL_LINES); //将多边形封闭
				glVertex2i(p[j].x, p[j].y);
				glVertex2i(p[0].x, p[0].y);
			glEnd();
			polygon sq;
			//将封闭了的多边形保存到多边形类中
			for(int i = 0; i < p.size(); i++)
				sq.p.push_back(p[i]);
            memcpy(sq.color,COLOR,sizeof(sq.color));
            sq.multiColor = multiColor;
			s.push_back(sq); //将绘成的多边形存入多边形类向量中
			paint(sq); //给当前画完的多边形填色
		}
		p.clear();
	}
	glFlush();
}
void myMouse(int button, int state ,int x, int y) //鼠标点击事件响应函数
{
	if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)//当鼠标左键被点击
	{
		point v;  //申请一个点类变量，点类为自定义类，在zl.h中定义
		v.x = x; //将点击处的点坐标，即x和y的值存入v中
		v.y = screenheight - y;
		p.push_back(v); //将点信息存入多边形点集向量p中
		glutPostRedisplay(); //重绘窗口
	}

	if(state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON) //当鼠标右键被点击
	{
		selectt = true;
		glutPostRedisplay();
	}


}
void myPassiveMotion(int x, int y) //鼠标移动事件响应函数
{
	move_x = x; //保存当前鼠标所在的坐标的值
	move_y = screenheight - y;
	glutPostRedisplay();

}

void processMenuEvents1(int option){
    if(option<4){memcpy(COLOR,allColor[option],sizeof(COLOR));
        multiColor = false;}
    else multiColor=true;
}

void processMenuEvents2(int option){
    if(option==0){
        if(!p.empty())
            p.pop_back();
    }
    else if(option==1) p.clear();
    else if(option==2){
        if(!s.empty()){
            s.pop_back();
        }
    }
    else{
        p.clear();
        s.clear();
    }
}
void save(){
    freopen("data","w",stdout);
    cout<<s.size()<<endl;
    for(int i=0;i<s.size();i++){
        cout<<s[i].p.size();
        for(int j=0;j<s[i].p.size();j++)
            cout<<" "<<s[i].p[j].x<<" "<<s[i].p[j].y;
        cout<<endl;
        for(int j=0;j<3;j++)
            cout<<(int)s[i].color[j]<<" ";
        cout<<endl;
        cout<<s[i].multiColor;
        cout<<endl;
    }
    fclose(stdout);
}

void load(){
    freopen("data","r",stdin);
    s.clear();
    int icase; cin>>icase;
    for(int i=0;i<icase;i++){
        polygon pg;
        int ipoints; cin>>ipoints;
        for(int j=0;j<ipoints;j++){
            point temp;
            cin>>temp.x>>temp.y;
            pg.p.push_back(temp);
        }
        for(int j=0;j<3;j++)
            cin>>pg.color[j];
        cin>>pg.multiColor;
        s.push_back(pg);
    }
    fclose(stdin);
    glFlush();
}

void processMenuEvents3(int option){
    if(!option) save();
    else load();
}

void processMenuEvents(int option){
}



void createGlUTMenus(){
    int submenu1 = glutCreateMenu(processMenuEvents1);
    glutAddMenuEntry("红",0);
    glutAddMenuEntry("绿",1);
    glutAddMenuEntry("蓝",2);
    glutAddMenuEntry("黑",3);
    glutAddMenuEntry("多彩",4);
    int submenu2 = glutCreateMenu(processMenuEvents2);
    glutAddMenuEntry("撤销上个点",0);
    glutAddMenuEntry("撤销当前所有",1);
    glutAddMenuEntry("清除上个多边形",2);
    glutAddMenuEntry("清除所有",3);
    int submenu3 = glutCreateMenu(processMenuEvents3);
    glutAddMenuEntry("存储",0);
    glutAddMenuEntry("读取",1);
    int menu = glutCreateMenu(processMenuEvents2);
    glutAddSubMenu("更改颜色",submenu1);
    glutAddSubMenu("清除",submenu2);
    glutAddSubMenu("存取",submenu3);
    glutAttachMenu(GLUT_MIDDLE_BUTTON);
}


int main(int argc, char** argv)
{


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(screenwidth,screenheight);
	glutCreateWindow("多边形扫描填充算法");
	init();
	glutMouseFunc(myMouse); //鼠标点击消息监控，即监控鼠标是否被点击，若被点击就调用myMouse函数
	glutDisplayFunc(lineSegment);
	glutPassiveMotionFunc(myPassiveMotion); //鼠标移动消息监控，即监控鼠标是否移动，若移动就调用myPassiveMotion函数
	createGlUTMenus();
	glutMainLoop();

	return 0;

}
