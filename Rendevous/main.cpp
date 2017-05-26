#include <GL/glut.h>
#include <iostream>
#include <windows.h>
#include <vector>
#include <math.h>
#define TIMER 100
#define GM 0.401408
#define ESCAPEVOLTEX 0.112			//오차값 +-0.4%미만

void SetupRC();
void RenderScene(void);
void ChangeSize(int w, int h);
void TimerFunction(int value);
void processNormalKeys(unsigned char key, int x, int y);
void processMouse(int button, int state, int x, int y);
void processMouseActiveMotion(int x, int y);
float Sphere1_degree = 0.0f;
float rocket_digree = 0;
float multiTime = 10000;
float rotate_theta_x = 0;
float rotate_theta_y = 0;
float rotate_theta_z = 0;
int WIDTH = 1024;
int HEIGHT = 768;
int keyDown = 0;
int nowX, nowY;
int drawX, drawY;
using namespace std;
typedef struct Object {
	float x, y;
	float vx, vy;
	float m;
	vector<pair<float, float>> vec;
}obj;
obj tmpObj = { 0,0,0,0,0 };
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Univers Drawing");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(processNormalKeys);
	glutTimerFunc(TIMER, TimerFunction, 1);
	SetupRC();
	glutMouseFunc(processMouse);
	glutMotionFunc(processMouseActiveMotion);
	glutMainLoop();
	return 0;
}
void SetupRC()
{
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);
}
void RenderScene(void)
{
	// 윈도우를 지정된 색으로 clear한다.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// World Transfrom
	glPushMatrix();
	glLoadIdentity();

	///////////////////Drawing//////////////////////////////////////////////////////
	// Earth
	glPushMatrix();
	glLoadIdentity();
	//glRotatef(30, 0, 1,0);
	glRotatef(Sphere1_degree, 0.2f, 0.3f, 0.5f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glutSolidSphere(64, 50, 50);
	glPopMatrix();
	////////////////////////////////////////////////////////////////////////////////
	//Rocket Point
	float delx = 0;
	if (tmpObj.m > 0) {
		glPushMatrix();
		glLoadIdentity();
		glRotatef(rotate_theta_x, 1, 0, 0);
		glRotatef(rotate_theta_y, 0, 1, 0);
		glRotatef(rotate_theta_z, 0, 0, 1);
		//printf("%f %f %f\n", rotate_theta_x, rotate_theta_y, rotate_theta_z);
		printf("%.4f\t", tmpObj.vy);
		glColor3f(0, 0, 1);
		glTranslated(tmpObj.x, tmpObj.y, 0);
		glutSolidSphere(3+sin(rotate_theta_x/180*3.141592), 50, 50);
		float d = sqrt((tmpObj.x*tmpObj.x) + (tmpObj.y*tmpObj.y));
		float a = GM / (d*d);
		float ax = -a*tmpObj.x / (fabs(tmpObj.x) + fabs(tmpObj.y))*multiTime
			, ay = -a*tmpObj.y / (fabs(tmpObj.x) + fabs(tmpObj.y))*multiTime;
		tmpObj.vx += ax;
		tmpObj.vy += ay;
		tmpObj.x += tmpObj.vx;
		tmpObj.y += tmpObj.vy;
		float dx = (tmpObj.x - tmpObj.vec[tmpObj.vec.size() - 1].first)
			, dy = (tmpObj.y - tmpObj.vec[tmpObj.vec.size() - 1].second);
		float ds = sqrt(dx*dx + dy*dy);
		tmpObj.vec.push_back({ tmpObj.x,tmpObj.y });
		rocket_digree = asin((dy / ds > 1) ? 1 : (dy/ds)) / 3.141592 * 180;
		rocket_digree = dx <= 0 ? (90-rocket_digree): (rocket_digree-90);
		glColor3f(1, 1, 1);
		if (d < 64||tmpObj.x>390||tmpObj.y>390||tmpObj.x<-390||tmpObj.y<-390)tmpObj.m = 0;
		glPopMatrix();
		delx = dx;
	}
	/////////////////////////////////////////// orbit
	if (tmpObj.m > 0) {
		glPushMatrix();
		glLoadIdentity();
		glRotatef(rotate_theta_x, 1, 0, 0);
		glRotatef(rotate_theta_y, 0, 1, 0);
		glRotatef(rotate_theta_z, 0, 0, 1);

		for (int i = tmpObj.vec.size() - 2; i>0; i--) {
			glBegin(GL_LINES);
			glVertex3d(tmpObj.vec[i].first, tmpObj.vec[i].second, 0);
			glVertex3d(tmpObj.vec[i - 1].first, tmpObj.vec[i-1].second, 0.2);
			glEnd();
		}

		glPopMatrix();
	}
	//////////////////////////////////////////
	glPopMatrix();
	/////////////////////////////////////////// Rocket
	if (tmpObj.m > 0) {
		glPushMatrix();
		glLoadIdentity();
		glColor3f(1.0f, 0.0f, 0.0f);
		glRotatef(90, 1, 0, 0);
		glTranslatef(300, 0, -204);
		//if (delx <= 0) {
		//	glRotatef(90 - rocket_digree, 0, 1, 0);
		//}
		//else {
		//	glRotatef(rocket_digree - 90, 0, 1, 0);
		//}
		glRotatef(rocket_digree, 0, 1, 0);
		glTranslatef(0, 0, -20);
		GLUquadric *qobj = gluNewQuadric();
		gluQuadricDrawStyle(qobj, GL_FILL);
		gluCylinder(qobj, 5, 5, 40, 50, 5);
		glColor3f(1, 1, 0);
		glTranslatef(0, 0, -10);
		gluCylinder(qobj, 0, 5, 10, 50, 1);
		glColor3f(0.8, 0, 0);
		glTranslated(0, 0, 50);
		gluCylinder(qobj, 5, 0, 0, 50, 1);
		glPopMatrix();
	}
	////////////////////////////////////////////
	//////////////////////////////////////Throw Power
	if (keyDown == 2) {
		glPushMatrix();
		glLoadIdentity();
		glBegin(GL_LINES);
		glColor3d(1, 1, 1);
		glVertex3d(((double)(nowX - WIDTH / 2) / 1.33), -((double)(nowY - HEIGHT / 2) / 1.33), 0);
		glVertex3d(((double)(drawX - WIDTH / 2) / 1.33), -((double)(drawY - HEIGHT / 2) / 1.33), 1);
		glEnd();
		glPopMatrix();
	}
	/////////////////////////////////////////////////
	// 더블 버퍼링으로 그림 그리기 수행
	glutSwapBuffers();
}
void ChangeSize(int w, int h)
{
	GLfloat nRange = 100.0f;
	// h가 0일 때
	if (h == 0)
		h = 1;
	WIDTH = w, HEIGHT = h;
	// 윈도우의 폭과 넓이 설정
	glViewport(0, 0, w, h);

	//원근투영 사용
	GLfloat fAspect;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	fAspect = (float)w / (float)h;
	gluPerspective(60.0, fAspect, 1.0, 500.0);
	glTranslatef(0.0f, 0.0f, -500.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutSwapBuffers();
}
void TimerFunction(int value)
{
	Sphere1_degree = (int)(Sphere1_degree + 5) % 360;
	glutPostRedisplay();
	glutTimerFunc(TIMER, TimerFunction, 1);
}
void processNormalKeys(unsigned char key, int x, int y)
{
	if (key == 27)//esc
		exit(0);//프로그램 종료
	if (key == ' ' && !(tmpObj.m>0)) {
		//tmpObj = { 0,64,(float)(ESCAPEVOLTEX*sqrt(multiTime)),0,1 };//shoot
		tmpObj = { 0,70,(float)(ESCAPEVOLTEX*sqrt(multiTime)*0.7),0,1 };
		tmpObj.vec.push_back({ tmpObj.x,tmpObj.y });
	}
}
void processMouse(int button, int state, int x, int y)
{
	switch (state)
	{
	case GLUT_DOWN:
		switch (button) {
		case GLUT_LEFT_BUTTON:
			keyDown = 1;
			break;
		case GLUT_RIGHT_BUTTON:
			rotate_theta_x = 0;
			rotate_theta_y = 0;
			rotate_theta_z = 0;
			drawX = x;
			drawY = y;
			keyDown = 2;
			break;
		case GLUT_MIDDLE_BUTTON:
			tmpObj = {};
			break;
		}
		nowX = x;
		nowY = y;
		break;
	case GLUT_UP:
		switch (button) {
		case GLUT_LEFT_BUTTON:
			keyDown = 0;
			break;
		case GLUT_RIGHT_BUTTON:
		if(tmpObj.m<=0){
			int dy = y - nowY
				, dx = nowX - x;
			printf("\n\n%d %d\n\n",nowX-WIDTH/2,nowY-HEIGHT/2);
			tmpObj = { (float)((double)(nowX-WIDTH/2)/1.33),(float)((double)(HEIGHT/2-nowY) / 1.33),(float)dx / 50,(float)dy / 50,10 };
			tmpObj.vec.push_back({ tmpObj.x,tmpObj.y });
		}
			keyDown = 0;
			break;
		}
		break;
	}
}
void processMouseActiveMotion(int x, int y)
{
	switch (keyDown) {
	case 0:break;
	case 1:{			//LBUTTON DOWNED		
		int dy = nowY - y;
		rotate_theta_x -= (float)dy;
		rotate_theta_x = (int)(rotate_theta_x + 360) % 360;
		nowY = y;
		int dx = nowX - x;
		rotate_theta_y -= (float)dx;
		rotate_theta_y = (int)(rotate_theta_y + 360) % 360;
		nowX = x;
	}break;
	case 2: {
		drawX = x;
		drawY = y;
	}break;
	case 3:break;
	default:
		exit(1);
	} 
}