//file; arrays;
#include <iostream>
#include <cstring>
#include <math.h>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <time.h>

using namespace std;



int main()
{   
    long double timeBeg = clock();
    int N = 100000, z = 5;
    double  Rp = 0.0191806, Rj = 0.0000777, t1 = 3, Gamma1 = (0.05654433 * t1) / 0.5, Gamma2 = (0.0002291314 * t1) / 0.5;
    double buf1[5] = {160, 160,  156, 151.99, 147.99},
    buf2[5] = {120.37, 120.37, 124.38, 128.38, 132.39};

    double **x1 = new double *[N];  // 1(1)
    for(int i = 0; i < N; i++)
    {
        x1[i] = new double [z];
    }


    double **y1 = new double *[N];  // 1(2)
    for(int i = 0; i < N; i++)
    {
        y1[i] = new double [z];
    }


    for(int i = 0; i < N; i++)
    {
       x1[i][0] = 160;  // 2(1)
       x1[i][1] = 0; x1[i][2] = 0; x1[i][3] = 0;
       x1[i][4] = 147.99;

       y1[i][0] = 120.37;  // 2(2)
       y1[i][1] = 0; y1[i][2] = 0; y1[i][3] = 0;
       y1[i][4] = 132.39;
    }


    for(int i = 0; i < z; i++)  // 3(1)
    {
       x1[0][i] = buf1[i];
    }

    for(int i = 0; i < z; i++) // 3(2)
    {
       y1[0][i] = buf2[i];
    }

    // DEFINED FOR ALL SCHEMA

    for(int i = 1; i < N; i++)   // n: t
    {
       for(int j = 1; j < z-1; j++)  // i: z
       {
           x1[i][j] = x1[i-1][j] * (-Gamma1 + 1 - (t1 * Rp)) + (Gamma1 * x1[i-1][j-1]) + (t1 * Rp * y1[i-1][(z-1)-j]);
           y1[i][j] = y1[i-1][j] * (-Gamma2 + 1 - (t1 * Rj)) + (Gamma2 * y1[i-1][j-1]) + (t1 * Rj * x1[i-1][(z-1)-j]);
       }
    }


    ofstream foutP("Tpar.txt"); // 4(1)
    for(int i = 0; i < N; i++)
    {
       for(int j = 0; j < z; j++)
       {
           //cout << x1[i][j] << " / ";
           foutP << x1[i][j] << " / ";
       }
       //cout << endl;
       foutP <<"  : " << i*3 <<endl;
    }
    foutP.close();
    //cout << endl << endl;

    ofstream foutJ("Tjyd.txt");  // 4(2)
    for(int i = 0; i < N; i++)
    {
       for(int j = 0; j < z; j++)
       {
           //cout << y1[i][j] << " | ";
           foutJ << y1[i][j] << " | ";
       }
      // cout << endl;
       foutJ << endl;
    }
    foutJ.close();
    cout << endl << endl;


    for(int i = 0; i < N; i++)
    {
        delete[] x1[i];
        delete[] y1[i];
    }

    delete[] x1;
    delete[] y1;

    cout << "Difference time: " << (clock() - timeBeg)/1000 << endl;

    return 0;
}
