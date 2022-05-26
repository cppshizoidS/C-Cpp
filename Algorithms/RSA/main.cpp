#include <iostream>
#include <cstring>
#include <ctime>
#include <cstdlib>
using namespace std;


int Plaintext [100];
long long Ciphertext [100];
int n, e = 0, d;

int BianaryTransform(int num, int bin_num[])
{

    int i = 0,  mod = 0;

    while(num != 0)
    {
        mod = num%2;
        bin_num[i] = mod;
        num = num/2;
        i++;
    }
    return i;
}


long long Modular_Exonentiation(long long a, int b, int n)
{
    int c = 0, bin_num[1000];
    long long d = 1;
    int k = BianaryTransform(b, bin_num)-1;

    for(int i = k; i >= 0; i--)
    {
        c = 2*c;
        d = (d*d)%n;
        if(bin_num[i] == 1)
        {
            c = c + 1;
            d = (d*a)%n;
        }
    }
    return d;
}

// generation of prime numbers
int ProducePrimeNumber(int prime[])
{
    int c = 0, vis[1001];
    memset(vis, 0, sizeof(vis));
    for(int i = 2; i <= 1000; i++)if(!vis[i])
        {
            prime[c++] = i;
            for(int j = i*i; j <= 1000; j+=i)
                vis[j] = 1;
        }

    return c;
}


//Extended Evklid algos
int Exgcd(int m,int n,int &x)
{
    int x1,y1,x0,y0, y;
    x0=1; y0=0;
    x1=0; y1=1;
    x=0; y=1;
    int r=m%n;
    int q=(m-r)/n;
    while(r)
    {
        x=x0-q*x1; y=y0-q*y1;
        x0=x1; y0=y1;
        x1=x; y1=y;
        m=n; n=r; r=m%n;
        q=(m-r)/n;
    }
    return n;
}


void RSA_Initialize()
{
    int prime[5000];
    int count_Prime = ProducePrimeNumber(prime);

    srand((unsigned)time(NULL));
    int ranNum1 = rand()%count_Prime;
    int ranNum2 = rand()%count_Prime;
    int p = prime[ranNum1], q = prime[ranNum2];

    n = p*q;

    int On = (p-1)*(q-1);


    for(int j = 3; j < On; j+=1331)
    {
        int gcd = Exgcd(j, On, d);
        if( gcd == 1 && d > 0)
        {
            e = j;
            break;
        }

    }

}

//  RSA
void RSA_Encrypt()
{
    cout<<"Public Key (e, n) : e = "<<e<<" n = "<<n<<'\n';
    cout<<"Private Key (d, n) : d = "<<d<<" n = "<<n<<'\n'<<'\n';

    int i = 0;
    for(i = 0; i < 100; i++)
        Ciphertext[i] = Modular_Exonentiation(Plaintext[i], e, n);

    cout<<"Use the public key (e, n) to encrypt:"<<'\n';
    for(i = 0; i < 100; i++)
        cout<<Ciphertext[i]<<" ";
    cout<<'\n'<<'\n';
}

// Decrypt RSA
void RSA_Decrypt()
{
    int i = 0;
    for(i = 0; i < 100; i++)
        Ciphertext[i] = Modular_Exonentiation(Ciphertext[i], d, n);

    cout<<"Use private key (d, n) to decrypt:"<<'\n';
    for(i = 0; i < 100; i++)
        cout<<Ciphertext[i]<<" ";
    cout<<'\n'<<'\n';
}


// Initialisation of algos
void Initialize()
{
    int i;
    srand((unsigned)time(NULL));
    for(i = 0; i < 100; i++)
        Plaintext[i] = rand()%1000;

    cout<<"Generate 100 random numbers:"<<'\n';
    for(i = 0; i < 100; i++)
        cout<<Plaintext[i]<<" ";
    cout<<'\n'<<'\n';
}

int main()
{
    Initialize();

    while(!e)
        RSA_Initialize();

    RSA_Encrypt();

    RSA_Decrypt();

    return 0;
}
