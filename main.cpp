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
    return 1 << (k-1);
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

int main(int argc, char** argv)
{
    int n,k,threads;
    complexd *a,*b,u[4];
    long long int length;
    unsigned seed;
    float arg;

    if (argc < 3)
    {
        cout << "Недостаточно аргументов";
        return -1;
    }
    n = atoi(argv[1]);
    k = atoi(argv[2]);
    threads = atoi(argv[3]);

    u[0] = u[1] = u[2] = sqrt(2)/2;
    u[3] = -sqrt(2)/2;

    length = round(pow(2,n));
    a = new complexd [length];
    b = new complexd [length];

    #pragma omp parallel num_threads(threads) private(seed,arg) shared(a)
    {
        seed = omp_get_thread_num()+time(NULL);
        #pragma omp for
        for (int i = 0; i < length; i++)
        {
            arg = (float)rand_r(&seed);
            a[i] = complex <float> (cos(arg),sin(arg));
        }
    }

    double start=omp_get_wtime();

    #pragma omp parallel for shared(a,b)
    for (int i = 0; i < length; i++)
    {
        if (kth_bit(k) & i == 0)
            b[i]=u[0]*a[i]+u[1]*a[i xor kth_bit(k)];
        else
            b[i]=u[2]*a[i xor kth_bit(k)]+u[3]*a[i];
    }

    double finish=omp_get_wtime();

    // for (int i = 0; i < length; i++)
    // {
    //     cout << a[i] << " ";
    // }
    cout << endl;
    for (int i = 0; i < length; i++)
    {
        cout << b[i] << " ";
    }

    delete []a;
    delete []b;

    FILE *f = fopen("./data.txt", "a");
    fprintf(f,"%d %d %d %f\n",threads,n,k,finish-start);
    fclose(f);

    return 0;
}
