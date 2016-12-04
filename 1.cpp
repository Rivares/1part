//  Graphic;  Vectors; Threads
//  Linear Heat exchanger Mathematical Model
#include <iostream>
#include <cstring>
#include <math.h>
#include <iomanip>
#include <cstdlib>
#include <GL/glut.h>
#include <vector>
#include <thread>
#include <cstdarg>
#include <stdio.h>
#include <time.h>
 
using namespace std;


void x1Thread(int N, short z, vector<vector<double> > &x1, double buf1[]);
void y1Thread(int N, short z, vector<vector<double> > &y1, double buf2[]);


//void x1Thread(int N,short z, double **x1, double buf1[]);
//void y1Thread(int N,short z, double **y1, double buf2[]);


long double timeBeg = clock();


void output(GLfloat x, GLfloat y, char const *text)
{
    glPushMatrix();
    glTranslatef(x, y, 0);
	  GLfloat ficks = 0.1f;
	  glScalef(ficks, ficks, 0);
	  
    for( char const *p = text; *p; p++)
    {
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *p);
    }
    glPopMatrix();
}


void displayMe()
{
    unsigned int N = 100000;
    short z = 5, t1 = 3;
  	GLfloat grphpnts[N][z];
    char md = '0';
    double  Rp = 0.0191806, Rj = 0.0000777, Gamma1 = (0.05654433 * t1) / 0.5, Gamma2 = (0.0002291314 * t1) / 0.5,
	  buf1[5] = {160, 160, 156, 151.99, 147.99}, buf2[5] = {120.37, 120.37, 124.38, 128.38, 132.39};


    vector<double> bmp(z, 0);
    vector<vector<double> > x1(N,bmp);
    vector<vector<double> > y1(N,bmp);

    thread t0(x1Thread, N, z, ref(x1), buf1);
    thread t9(y1Thread, N, z, ref(y1), buf2);

    t0.join();
    t9.join();

/*
    double **x1 = new double *[N];
    double **y1 = new double *[N];
    double **grphpnts = new double *[N];
	  for(int i = 0; i < N; i++)
    {
        x1[i] = new double [z];
        y1[i] = new double [z];
        grphpnts[i] = new double [z];
    }

    thread t0(x1Thread, N, z, x1, buf1);
    thread t9(y1Thread, N, z, y1, buf2);

    t0.join();
    t9.join();


    for(int i = 0; i < N; i++)
    {
       x1[i][0] = 160;  // 2(1)
       x1[i][1] = 0; x1[i][2] = 0; x1[i][3] = 0;
       x1[i][z-1] = 147.99;
       y1[i][0] = 120.37;  // 2(2)
       y1[i][1] = 0; y1[i][2] = 0; y1[i][3] = 0;
       y1[i][z-1] = 132.39;
    }


    for(int i = 0; i < 5; i++)  // 3(1)
    {
       x1[0][i] = buf1[i];
    }

    for(int i = 0; i < 5; i++) // 3(2)
    {
       y1[0][i] = buf2[i];
    }
*/

    // DEFINED FOR ALL SCHEMA

    for(unsigned int i = 1; i < N; i++)   // n: t
    {
       for(short j = 1; j < (z-1); j++)  // i: z
       {
          x1[i][j] = x1[i-1][j] * (-Gamma1 + 1 - (t1 * Rp)) + (Gamma1 * x1[i-1][j-1]) + (t1 * Rp * y1[i-1][(z-1)-j]);
		      y1[i][j] = y1[i-1][j] * (-Gamma2 + 1 - (t1 * Rj)) + (Gamma2 * y1[i-1][j-1]) + (t1 * Rj * x1[i-1][(z-1)-j]);   	   
       }
    }

	//---------------------------------Switch need function:----------------------------------
	
  float cP = 4.358974358, kP = 0.66666, cG = 5.132075471, cDT = 0;  // coefficients for scalability
	char const *T0, *T1, *T2;
	cout << "Switch need function! P or G: ";
  
  //cin >> md;
  md = 'P';

	if (md == 'P')
	{
		for(unsigned int i = 0; i < N; i++) // 1(0)
		{
		  for(short j = 0; j < z; j++)
		   {
			   grphpnts[i][j] = cP * x1[i][j];
		   }
		   cout << endl;
		}
		cDT = 1;
		T0 = "160", T1 = "156", T2 = "151.99";
	}
	else
	{
	  if (md == 'G')
	  {
		    for(unsigned int i = 0; i < N; i++) // 1(0)
		    {
		        for(short j = 0; j < z; j++)
		        {
			         grphpnts[i][j] = cG * y1[i][j];
		        }
		        cout << endl;
		    }

		    cDT = 0.1;
		    T0 = "128.38", T1 = "124.38", T2 = "120.37";
    }
	  else
	  {
		  cout << "Error input!!!" << endl;
		  return;
	  }
	}


	//------------------------------------------------------------------------------------------------
	
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_LINES);

		//Drawing axis coordinates:
	  
    short x0 = 25, xN = 1000, y0 = 5, yN = 680;
	    
    glVertex2f(x0, y0);                          // one point of coordinates axis x
    glVertex2f(xN, y0);

		glVertex2f(xN, y0);							// one part point of axis x
    glVertex2f(xN-10, y0+5);
		glVertex2f(xN, y0);							// two part point of axis x
    glVertex2f(xN-10, y0-5);

    glVertex2f(x0, y0);							// one point of coordinates axis y
	  glVertex2f(x0, yN);
    
		glVertex2f(x0, yN);							// one part point of axis y
    glVertex2f(x0-4, yN-10);
		glVertex2f(x0, yN);							// two part point of axis y
    glVertex2f(x0+4, yN-10);		


    //Drawing lines on axis y:     ( x = 5.132075471 - coeff scalability;  156 * x = 680 ); 5.132075471*0.66666*grphpnts[0][1]

	  glVertex2f( x0+3, kP*grphpnts[0][1] );		// T0
    glVertex2f( x0-3, kP*grphpnts[0][1] );

		glVertex2f( x0+3, grphpnts[0][2]/2 );	    // T1
    glVertex2f( x0-3, grphpnts[0][2]/2 );

		glVertex2f( x0+3, grphpnts[0][3]/3 );		  // T2
    glVertex2f( x0-3, grphpnts[0][3]/3 );


		for(short i = 25; i < xN; i += 59)
		{
		    glVertex2f(i, y0+3);					 // other lines on axis x
        glVertex2f(i, y0-3);
		}
	
	  //Drawing function to lines:
		
    for(short i = 1; i < (z-1); ++i)  // 1(1)
		{
			double xbuf = 25;

			for(unsigned int m = 0; m < N; ++m)
			{
				double buf1 = 0;			
				buf1 = grphpnts[m][i];

				if (i == 1)										     // for scalability
					glVertex2f(xbuf, buf1*kP/i);
				else 
					glVertex2f(xbuf, buf1/i);	

				xbuf += cDT;									     // step
        buf1 = grphpnts[m+1][i];

				if (i == 1)
					glVertex2f(xbuf, buf1*kP/i);
        else 
					glVertex2f(xbuf, buf1/i);
			}
    }
		
	
		
	glEnd();
	
  char yCoord[] = {'T', ',', 'C', '\0'};    // we don't forget symbol for end of string
  char xCoord[] = {'t', '*', '1', '0', '^', '3', ' ', 's', 'e', 'c', '\0'};
  char cCoord[] = {'0', '\0'};
	output(35, 660, yCoord);  // 2(0)
	output(880, 50, xCoord);
	output(10, 5, cCoord);
	
	output(x0+1, (kP*grphpnts[0][1])+12 , T0);
	output(x0+1, (grphpnts[0][2]/2)+12 , T1);
	output(x0+1, (grphpnts[0][3]/3)+12 , T2);
	
/*  union cnvrt
	{
	    float dt;
		char bdt[sizeof(float)];
	};
    cnvrt it;
*/


	char it[]={"0.625 1.25  1.875 2.5  3.125 3.75 4.375  5  5.625  6.25  6.875 7.5  8.125 8.75 9.375  10"};    // supposably for two functions
	output(x0+(35), y0+10,  it);


  glFlush();


  cout << "Difference time: " << (clock() - timeBeg) / 1000 << endl;

/*
    for(int i = 0; i < N; i++)
    {
       for(int j = 0; j < z; j++)
       {
		   cout << x1[i][j] << " / ";
       }
       cout << endl;
    }
	cout << endl << endl;
	for(int i = 0; i < N; i++)
    {
       for(int j = 0; j < z; j++)
       {
		   cout << y1[i][j] << " / ";
       }
       cout << endl;
    }	
*/

}



int main(int argc, char** argv)
{
	
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(800, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Simulation:");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1000, 0, 680);

    glutDisplayFunc(displayMe);
    glutMainLoop();

		
    return 0;
}












void x1Thread(int N, short z, vector<vector<double> > &x1, double buf1[])
{
    for(int i = 0; i < N; i++)
    {
       x1[i][0] = 160;  // 2(1)
       x1[i][1] = 0; x1[i][2] = 0; x1[i][3] = 0;
       x1[i][z-1] = 147.99;
    }

    for(short i = 0; i < z; i++)  // 3(1)
    {
       x1[0][i] = buf1[i];
    }
    //cout << "1" << endl;
}

void y1Thread(int N, short z, vector<vector<double> > &y1, double buf2[])
{
    for(int i = 0; i < N; i++)
    {
       y1[i][0] = 120.37;  // 2(2)
       y1[i][1] = 0; y1[i][2] = 0; y1[i][3] = 0;
       y1[i][z-1] = 132.39;
    }

    for(short i = 0; i < z; i++) // 3(2)
    {
       y1[0][i] = buf2[i];
    }
    //cout << "2" << endl;
}


/*

void x1Thread(int N, short z, double **x1, double buf1[])
{
    for(int i = 0; i < N; i++)
    {
       x1[i][0] = 160;  // 2(1)
       x1[i][1] = 0; x1[i][2] = 0; x1[i][3] = 0;
       x1[i][z-1] = 147.99;
    }

    for(short i = 0; i < z; i++)  // 3(1)
    {
       x1[0][i] = buf1[i];
    }
    cout << "1" << endl;
}

void y1Thread(int N, short z, double **y1, double buf2[])
{
    for(int i = 0; i < N; i++)
    {
       y1[i][0] = 120.37;  // 2(2)
       y1[i][1] = 0; y1[i][2] = 0; y1[i][3] = 0;
       y1[i][z-1] = 132.39;
    }

    for(short i = 0; i < z; i++) // 3(2)
    {
       y1[0][i] = buf2[i];
    }
    cout << "2" << endl;
}
*/