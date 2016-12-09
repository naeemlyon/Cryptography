#include <iostream> // for cout etc...
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib> // for srand() , rand()
#include <ctime>   // for time for srand()
#include <assert.h>
using namespace std;
////////////////////////////////////////////////////
int Test_Primality(int , int);
int Generate_Random_Number(int);
int Exponential_Power(int , int , int);
////////////////////////////////////////////////////
void main(void)
{
    
  int P; // input primer numer or not
  int k = 20; // try it with 20 random numbers (a)...  
  int res; // result if prime or not (0 / 1)
  char ch;
do
{
  cout<<endl<<"========================================================";
  cout<<endl<<"==============    Miller Rabin Test (n)    =============";
  cout<<endl<<"== pseude-code from Wikipedia with same Variable Name ==";
  cout<<endl<<"============           Muhammad Naeem        ===========";
  cout<<endl<<"========================================================"<<endl<<endl;

  cout<<"Enter Possible Prime Numer (P):\t";  cin>>P;
  if (!cin.good())
	{
	 cin.clear();
	 cout<<endl<<"Incorrect value of P!. so i have set P = 5807"<<endl;
	 cin.ignore(10 , '\n');
	 P = 5807;
	}	

  res = Test_Primality(P , 15);
  cout<<endl<<"-------------------------------------------------------------"<<endl;
  if (res == 0 )
  {
      cout<<P<<"\t is Composite."<<endl<<endl;
	  cout<<"Also try it with some prime no. input: 5807 , 2711 , 113 , 9973 , 4639"<<endl<<endl;
  }
  else
	  cout<<P<<"\t is Prime"<<endl<<endl;    

  cout<<"Press small 'x' to exit ..any other key to continue..."; cin>>ch;
}while (ch != 'x');

}


///////////////////////////////////////////////////////


int Generate_Random_Number(int number)
{
    static int check_seed = 0;
    if (check_seed == 0)
    {
        srand(time(NULL));
        check_seed = 1;
    }

    return rand() % number;
}

//////////////////////////////////////////////////////////////////
//// Recursive Function to calcuate exponential power mode value
int Exponential_Power(int base, int expon, int mod)
{
    int res;
    
    if (expon == 0) // if exponent is 0 simply return 1
        return 1;
    if (expon == 1) // if exponent is 1 simply return remainder of badse (div)mode
        return base % mod;
    if (expon % 2 == 0) // if exponent is even number
    {
        res = Exponential_Power(base, expon / 2, mod);
        return (res * res) % mod;
    }
    else // if exponent is odd number
    {
        res = Exponential_Power(base, expon / 2, mod);
        return (((res * res) % mod) * base) % mod;
    }
}

///////////////////////////////////////////////////////////////////////
/// Testing Primalirty (Prime_or_not , Test Count of Random No)
int Test_Primality(int n, int k)
{
    int i, a, r, s, d, b;

    assert(n > 1);
    if (n == 2)    // basic testing....
        return 1;
    s = 0;
    d = n - 1;
    while (d % 2 == 0) // calculating the value of s. i.e. how many exponential possible
    {
        s++;
        d /= 2;
    }
    for (i = 0; i < k; i++) // check it k time against k random numbers
    {
        a = Generate_Random_Number(n - 2) + 1;
		cout<<(i+1)<<" : Random Number = "<<a<<endl;
        b = Exponential_Power(a, d, n);
        if (b == 1)
            continue;
        for (r = 0; r < s; r++)
        {
            if (b == n - 1)
                break;
            b = (b * b) % n;
        }
        if (r != s)
		{
    		cout<<"\tMaybe Prime!..."<<endl;   
			continue;	
		}
        return 0;
    }	
    return 1;
}
//////////////////////////////////////////////////////////
