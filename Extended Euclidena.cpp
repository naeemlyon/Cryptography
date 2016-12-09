#include <stdio.h>
#include <iostream>
#include <iomanip>
using namespace std;

void Extended_Euclidean_GCD(long , long );
long GCD(long , long);
/////////////////////////////////////////////////////
void main(void)
{
  long m , n; char ch;
  do{
  cout<<endl<<"=====================================================";
  cout<<endl<<"============= Extended Euclidian GCD    =============";
  cout<<endl<<"===========        Muhammad Naeem       =============";
  cout<<endl<<"====================================================="<<endl<<endl;

  cout<<"Enter Value of m:\t";  cin>>m;
  if (!cin.good())
	{
	 cin.clear();
	 cout<<endl<<"Incorrect value of m!. so i have set m = 420 "<<endl;
	 cin.ignore(10 , '\n');
	 m = 420;
	}	
  cout<<"Enter Value of n:\t";  cin>>n;	
  if (!cin.good())
	{
	 cin.clear();
	 cout<<endl<<"Incorrect value of n!. so i have set n = 20 "<<endl;
	 cin.ignore(10 , '\n');
	 n = 20;
	}	 

  Extended_Euclidean_GCD(m, n);
  cout<<"Press small 'x' to exit ..any other key to continue..."; cin>>ch;
  }while (ch != 'x');

}
////////////////////////////////////////////////////
void Extended_Euclidean_GCD(long m, long n)
{
  long Q, A1 , A2 , A3 , B1, B2, B3 , T1 , T2 , T3;
  A1 = 1;  A2=0;  A3=m;
  B1 = A2; B2=A1; B3=n;
  Q=-1;
  cout<<"--------------------------------------------------------------------"<<endl;
  cout<<setw(6)<<"Q"<<setw(7)<<"A1"<<setw(7)<<"A2"<<setw(7)<<"A3"<<setw(7)<<"B1"<<setw(7)<<"B2"<<setw(7)<<"B3"<<endl;
  cout<<"--------------------------------------------------------------------"<<endl;
  cout<<setw(6)<<"/";
  printf(" %6ld %6ld %6ld %6ld %6ld %6ld", A1 , A2 , A3 , B1 , B2 , B3);
  cout<<endl<<endl;
  while (B3 >= 0) {
  if (B3 == 0) {
	A3 = GCD(m,n);
	cout<<"--------------------------------------------------------------------"<<endl;
    cout<<"GCD :\t"<<A3<<endl;	
    cout<<"No Inverse at all"<<endl;	
	cout<<"--------------------------------------------------------------------"<<endl;
    return;
  }
 if (B3 == 1) {
    A3 = GCD(m,n);
	cout<<"--------------------------------------------------------------------"<<endl;
	cout<<endl<<"GCD :\t"<<A3<<endl;	
	cout<<"Inverse of "<<n<< " in GF("<<m<<") : "<<B2<<endl;	
	cout<<"--------------------------------------------------------------------"<<endl;
	return;
  }     
     
   Q = A3 / B3, 
   T1 = A1 - (Q*B1);     T2 = A2 - (Q*B2);     T3 = A3 - (Q*B3);
   A1 = B1;              A2 = B2;	   		   A3 = B3;
   B1 = T1;			     B2 = T2;              B3 = T3;	    
   printf("%6ld %6ld %6ld %6ld %6ld %6ld %6ld", Q , A1 , A2 , A3 , B1 , B2 , B3);
   cout<<endl<<endl;   
  } // end of while   
  printf("-------------------------------------------------\n");
} // end of function..

////////////////////////////////////////////
////////////////////////////////////////////
long GCD(long m, long b)
{
	long n , d , q , r;
	n= m;    
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

