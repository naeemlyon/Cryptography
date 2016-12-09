#include <stdio.h>
#include <iostream>
#include <iomanip>
using namespace std;

void Extended_Euclidean_GCD(long , long );
long GCD(long , long);
/////////////////////////////////////////////////////
void main(void)
{
  long a , b , res; char ch;
  cout<<endl<<"====================================================="<<endl;
  cout<<endl<<"============= Simple Euclidian GCD    ============="<<endl;
  cout<<endl<<"=============     Muhammad Naeem    ============="<<endl;
  cout<<endl<<"====================================================="<<endl<<endl;

  do {
  cout<<"Enter Value of a:\t";  cin>>a;
  if (!cin.good())
	{
	 cin.clear();
	 cout<<endl<<"Incorrect value of a!. so i have set a = 420 "<<endl;
	 cin.ignore(10 , '\n');
	 a = 420;
	}	
  cout<<"Enter Value of b:\t";  cin>>b;	
  if (!cin.good())
	{
	 cin.clear();
	 cout<<endl<<"Incorrect value of b!. so i have set b = 20 "<<endl;
	 cin.ignore(10 , '\n');
	 b = 20;
	}	 

  res = GCD(a, b);
  if (res == 1)
   cout<<endl<<"Both "<<a<<" AND "<<b<<" are Co Prime!"<<endl<<endl;
  else
   cout<<endl<<"GCD ("<<a<<" , "<<b<<") : "<<res<<endl<<endl;
  cout<<"Press small 'x' to exit ..any other key to continue..."; cin>>ch;
  }while (ch != 'x');
}

////////////////////////////////////////////////////
long GCD(long a, long b)
{
	// n=numerator  : d=divider : q=Quotient :  r=Remainder
	long n , d , q , r;  
	n= a;    
	d = b;
    q=n/d;
    r=n-(q*d);

   	if(r ==0)
	 return d;

	else
	{
		while(r !=0)
		{
		n=d;
		d = r;
		q = n/d;
		r = n -(q*d);				
		}
	}
	return d;	
}

////////////////////////////////////////////////////
