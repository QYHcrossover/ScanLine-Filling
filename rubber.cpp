#include<windows.h>
#include<GL/glut.h>
#include"datastruct.h"
#include<bits/stdc++.h>

static const  int screenwidth = 1000;  //�Զ��崰�ڿ��
static const int screenheight = 1000; //�Զ��崰�ڸ߶�
vector<point> p; //����ε㼯����
vector<polygon> s; //����������������������Ѿ�����Ķ����
int move_x,move_y; //��굱ǰ����ֵ��������ƶ���̬����ʱʹ��
bool selectt = false; //����η��״̬�жϱ�������Ϊtrueʱ����������Ҽ���������η��
float COLOR[3] = {1.0,0.0,0.0}; //��ǰ��ɫ
bool multiColor = false; //��ǰ�Ƿ��Ƕ�ɫ
int interPixel = 10; //10������Ϊһ����ɫ
float allColor[4][3] = {1,0,0,0,1,0,0,0,1,0,0,0}; //������ɫ��

void init()
{
	glClearColor(1.0,1.0,1.0,0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0,screenwidth,0.0,screenheight);
}

void paint(polygon &pg){
    vector<point> p = pg.p;
    list<edge> AET; //��list�����߱�
    map< int,list<edge> > NET; //��map��������߱�
    //��ʼ������߱�
    for(unsigned i=0;i<p.size();i++){
        point pa = p[i]; //���
        point pb = p[(i+1)%p.size()]; //�յ�
        if(pa.y==pb.y) continue; //ˮƽ�߲�����
        float dx = 1.0*(pa.x-pb.x)/(pa.y-pb.y); //dx�ļ��㹫ʽ
        //����ǰ�������yֵ��С�����ߣ�����������߱���
        if(pa.y<pb.y){
            edge e(pb.y-1,dx,pa.x);
            NET[pa.y].push_back(e);
        }
        else{
            edge e(pa.y-1,dx,pb.x);
            NET[pb.y].push_back(e);
        }
    }
    //���Ա߱�ĳ�ʼ��
    map< int,list<edge> >::iterator it = NET.begin(); //����NET�ĵ�����
    int y = it->first; //��ǰyֵ��ʼ��Ϊ��������ε���͵�
    //������ѭ�����ϸ���AET
    glColor3fv(pg.color);
    int idpixel = 0;
    while(AET.size()||it!=NET.end()){
        //���AET�еı��Ƿ񳬳���Χ
        for(list<edge>::iterator ait=AET.begin();ait!=AET.end();){
            if(y>ait->ymax) ait = AET.erase(ait);
            else ait++;
        }
        //����Ƿ����µı߼���
        if(it!=NET.end()&&y>=it->first){
            AET.insert(AET.begin(),it->second.begin(),it->second.end());
            it++;
        }
        //����
        AET.sort();
        //ƥ��+����
        if(pg.multiColor) glColor3fv(allColor[(idpixel++/interPixel)%4]);
        else glColor3fv(pg.color);
        list<edge>::iterator ait=AET.begin();
        while(ait!=AET.end()){
            float xa = (ait++)->xmin;
            float xb = (ait++)->xmin;
            glBegin(GL_LINES); //���Ѿ�ȷ���ĵ���������
                glVertex2i((int)xa, y);
                glVertex2i((int)xb, y);
            glEnd();
        }
        //�����¸�ɨ����,ÿ���߼���dx
        y++;
        for(list<edge>::iterator ait=AET.begin();ait!=AET.end();ait++){
            ait->xmin += ait->dx;
        }
    }
}

void lineSegment()
{
	glClear(GL_COLOR_BUFFER_BIT);
	int i, j;                 //����ѭ�����Ʊ����������������ѭ��������ѭ���н�����ε��á�
	if(!s.empty())            //��������������Ƿ�Ϊ�գ����жϳ��˵�ǰ���ڻ��Ķ�����Ƿ��������Ѿ����õĶ����
	{
		for( i = 0; i < s.size(); i++)   //�Զ����������ѭ�����������е�ÿ��Ԫ�ش���һ�������
		{
            glColor3fv(s[i].color); //���õ�ǰ��ɫΪ�������ɫ
			int k = s[i].p.size();  //��һ������εĵ�ĸ��������滮�߻��õ�
			for(j = 0; j < s[i].p.size(); j++) //�������
			{
				glBegin(GL_LINES); //����ǰ�ĵ����һ��������
					glVertex2i(s[i].p[j].x, s[i].p[j].y);
					glVertex2i(s[i].p[(j+1)%k].x, s[i].p[(j+1)%k].y);//��ͨ��ȡģ����������Խ�����⣬��˼·��Դ��ѭ������.
				glEnd();
			}
			paint(s[i]);  //Ϊ��ǰ�Ķ���������ɫ
//			s[i].edge.clear(); //��յ�ǰ����εı�����
		}
	}
	i = 0;
	j = p.size() - 1;
	glColor3fv(COLOR);   //�趨��ɫ�������߶���ɫҲ�����ɫ
	while(i < j)           //ѭ����ͼ������ǰ���ڻ��Ķ����
	{
		glBegin(GL_LINES); //���Ѿ�ȷ���ĵ���������
			glVertex2i(p[i].x, p[i].y);
			glVertex2i(p[i+1].x, p[i+1].y);
		glEnd();
		i++;
	}
	if(!p.empty())
	{
//		int i = p.size() - 1; //��ȷ�������һ�����뵱ǰ�������λ�����ߣ�����̬����
		glBegin(GL_LINES);
			glVertex2i(p[j].x, p[j].y);
			glVertex2i(move_x, move_y);
		glEnd();
	}
	if(selectt) //�ж��Ҽ��Ƿ񱻵���
	{
		selectt = false; //��״ֵ̬��Ϊ��
		if(!p.empty())
		{
			glBegin(GL_LINES); //������η��
				glVertex2i(p[j].x, p[j].y);
				glVertex2i(p[0].x, p[0].y);
			glEnd();
			polygon sq;
			//������˵Ķ���α��浽���������
			for(int i = 0; i < p.size(); i++)
				sq.p.push_back(p[i]);
            memcpy(sq.color,COLOR,sizeof(sq.color));
            sq.multiColor = multiColor;
			s.push_back(sq); //����ɵĶ���δ���������������
			paint(sq); //����ǰ����Ķ������ɫ
		}
		p.clear();
	}
	glFlush();
}
void myMouse(int button, int state ,int x, int y) //������¼���Ӧ����
{
	if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)//�������������
	{
		point v;  //����һ���������������Ϊ�Զ����࣬��zl.h�ж���
		v.x = x; //��������ĵ����꣬��x��y��ֵ����v��
		v.y = screenheight - y;
		p.push_back(v); //������Ϣ�������ε㼯����p��
		glutPostRedisplay(); //�ػ洰��
	}

	if(state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON) //������Ҽ������
	{
		selectt = true;
		glutPostRedisplay();
	}


}
void myPassiveMotion(int x, int y) //����ƶ��¼���Ӧ����
{
	move_x = x; //���浱ǰ������ڵ������ֵ
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
    glutAddMenuEntry("��",0);
    glutAddMenuEntry("��",1);
    glutAddMenuEntry("��",2);
    glutAddMenuEntry("��",3);
    glutAddMenuEntry("���",4);
    int submenu2 = glutCreateMenu(processMenuEvents2);
    glutAddMenuEntry("�����ϸ���",0);
    glutAddMenuEntry("������ǰ����",1);
    glutAddMenuEntry("����ϸ������",2);
    glutAddMenuEntry("�������",3);
    int submenu3 = glutCreateMenu(processMenuEvents3);
    glutAddMenuEntry("�洢",0);
    glutAddMenuEntry("��ȡ",1);
    int menu = glutCreateMenu(processMenuEvents2);
    glutAddSubMenu("������ɫ",submenu1);
    glutAddSubMenu("���",submenu2);
    glutAddSubMenu("��ȡ",submenu3);
    glutAttachMenu(GLUT_MIDDLE_BUTTON);
}


int main(int argc, char** argv)
{


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(screenwidth,screenheight);
	glutCreateWindow("�����ɨ������㷨");
	init();
	glutMouseFunc(myMouse); //�������Ϣ��أ����������Ƿ񱻵������������͵���myMouse����
	glutDisplayFunc(lineSegment);
	glutPassiveMotionFunc(myPassiveMotion); //����ƶ���Ϣ��أ����������Ƿ��ƶ������ƶ��͵���myPassiveMotion����
	createGlUTMenus();
	glutMainLoop();

	return 0;

}
