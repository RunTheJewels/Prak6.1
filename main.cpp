#include <complex>
#include <cmath>
#include <iostream>
#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

typedef std::complex<float> complexd;

using namespace std;

int kth_bit(int k)
{
    return 1 << k;
}

int ex2(int n)
{
    int r = 1;
    for (int i = 0; i < n; i++)
    {
        r*=2;
    }
    return r;
}

int main(void)
{
    int n,k;
    complexd *a,*b,u[4];
    cin >> n >> k;
    int length = kth_bit(n);
    u[0] = u[1] = u[2] = sqrt(2)/2;
    u[3] = -sqrt(2)/2;
    srand(time(NULL));

    cout << "length:" << length << endl;
    a = new complexd [length];
    b = new complexd [length];
    #pragma omp parallel shared(a)
    {
        unsigned seed = omp_get_thread_num()+time(NULL);
        cout << omp_get_thread_num() << "th thread" << endl;
        #pragma omp for
        for (int i = 0; i < length; i++)
        {
            a[i] = complex <float> ((float)(rand_r(&seed)%201)/100-1,(float)(rand_r(&seed)%201)/100-1);
        }
    }
    for (int i = 0; i < length; i++)
    {
        cout << a[i];
    }

    double start=omp_get_wtime();

    #pragma omp parallel shared(a,b)
    for (int i = 0; i < length; i++)
    {
        if (kth_bit(k) & i == 0)
            b[i]=u[0]*a[i]+u[1]*a[i xor kth_bit(k)];
        else
            b[i]=u[2]*a[i xor kth_bit(k)]+u[3]*a[i];
}

double finish=omp_get_wtime();

for (int i = 0; i < length; i++)
{
    cout << b[i] << " ";
}

delete []a;
delete []b;

FILE *f = fopen("./data.txt", "a");
fprintf(f,"%d %f\n",omp_get_num_procs(),finish-start);
fclose(f);

return 0;
}
