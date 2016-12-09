#include <iostream> // for cout etc...
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib> // for srand() , rand()
#include <ctime>   // for time for srand()
#include <assert.h> // assertion failure in case of primality test input are fatal
#include <iomanip> // setw  nicely formated tables of miller rabin
#include <conio.h>  // getche();
#include <fstream> // for filing
#include <windows.h> // for coloring purpose only....
#include <time.h> // timinmg...

#define  MAX       100 // P & Q  & E will be smaller than this value alway...

#define  RED       12
#define  LIME      10
#define  MAGENTA   13
#define  YELLOW    14
#define  TORQUOISE 11
#define  WHITE     15
typedef unsigned char UCHAR; // typecasting during dumping everything into out file.
bool Show_Detail;

////////////////////////////////////////////////////
unsigned long Apply_Miller_Rabin(char str[64]);
int MR_Primality_Test(unsigned int , int);
int Generate_Random_Number(int);
unsigned long Exponential_Power(unsigned long, unsigned long, unsigned long);
unsigned long Extended_Euclidean_GCD(unsigned long, unsigned long );
unsigned long GCD(unsigned long , unsigned long);
void Save_Key(char key_file[1024] , unsigned int, unsigned long);
unsigned long Read_Key(char key_file[1024] , char);
void Cipher(char file_in[1024] , char file_out[1024] , unsigned int , unsigned long); 
void Decipher(char file_in[1024] , char file_out[1024] , unsigned int , unsigned long); 
void SetColor(int);
void Time_Check(unsigned int , char Purpose[1024]);
int SystemToDecimal(char * , int);
char *DecimalToSystem(int , int );
long double Raise_To_Power(int , int);

////////////////////////////////////////////////////
using namespace std;
void main(void)
{
 unsigned int P , Q , E , D;  unsigned long N , Phi; 
 unsigned int Start; char opt;
 char file_cipher[1024], file_plaintext[1024] , file_key[1024], file_decipher[1024]; ;

 do
  {    
      SetColor(TORQUOISE);
      cout<<endl<<"========================================================";
      cout<<endl<<"== Ron Rivest, Adi Shamir (RSA) cryptosystem ( via Miller-Rabin-Test + EEU) ==";
      cout<<endl<<"== pseude-code from Wikipedia with same Variable Name ==";
      cout<<endl<<"============        Muhammad Naeem       ===========";
      cout<<endl<<"========================================================";      
	  SetColor(YELLOW);
	  cout<<endl<<"Press\t"; SetColor(WHITE); cout<<"1\t";SetColor(YELLOW); cout<<"for Key Generation";
      cout<<endl<<"Press\t"; SetColor(WHITE); cout<<"2\t";SetColor(YELLOW); cout<<"for Encryption ";
      cout<<endl<<"Press\t"; SetColor(WHITE); cout<<"3\t";SetColor(YELLOW); cout<<"for Decryption ";
      cout<<endl<<"Press\t"; SetColor(WHITE); cout<<"any key\t";SetColor(14); cout<<"Quit appliction ";
	  SetColor(TORQUOISE);
      cout<<endl<<"========================================================"<<endl;
	  cout<<endl<<"Your Choice Please!\t "; cin>>opt;
    switch (opt)
	{

	case '1': // Key Generation and saving it to (Public / Private) Files
		{
	 	 cout<<endl<<"Show Each Step Detail of RSA Key Genertion (1=True , any other=False):  "; cin>>Show_Detail;  	    
         //Show_Detail = 1;  	    
		 Start = clock();          
         P = Apply_Miller_Rabin("P");
		 do 
		{
		Q = Apply_Miller_Rabin("Q");
		} while (Q == P);

		Phi = (P-1)*(Q-1);
		N = P * Q;
 
		do
		{
		E = Apply_Miller_Rabin("E"); 
		D = Extended_Euclidean_GCD(Phi , E);   
		}while ((D > 999) || (D == E) || (D < 2));
        SetColor(RED);		
		cout<<endl<<"RSA Key Pair (M.Naeem MS081009)";
		cout<<endl<<"PQ = {"<<setw(8)<<P<<" , "<<setw(8)<<Q<<" }";
		cout<<endl<<"KU = {"<<setw(8)<<E<<" , "<<setw(8)<<N<<" }";
		cout<<endl<<"KR = {"<<setw(8)<<D<<" , "<<setw(8)<<N<<" }";
		cout<<endl<<"===================================="<<endl;
        Time_Check(Start , "RSA Key Generation");
		cout<<"Both keys will be saved in Base HexaDecimal"<<endl;
		cout<<endl<<"Public Key File Name Please!\t";      cin>>file_key;  
		//strcpy (file_key , "e.key"); //testing purpose   	     		 	    		 
		Save_Key(file_key, E, N);
		cout<<endl<<"Private Key File Name Please!\t";      cin>>file_key;  
		//strcpy (file_key , "d.key"); //testing purpose   	     		 	    		 
		Save_Key(file_key, D, N);		
        break;
		}

    case '2': // Encryption ............ 
		{
        cout<<endl<<"Enter Public Key File:\t";             cin>>file_key; 
		//strcpy (file_key , "e.key"); //testing purpose  
        cout<<endl<<"Enter your Plain Text File Name:\t";   cin>>file_plaintext;  
		//strcpy (file_plaintext , "plain.txt"); // just for temporary purpose to avoid recurring job...
 		cout<<endl<<"Enter your Cipher Text File:\t";       cin>>file_cipher;
        //strcpy (file_cipher , "cipher.rsa");        
        
		cout<<endl<<"Ciphering Start...."<<endl;
        Start = clock(); 
		E = Read_Key(file_key, 'E');
        N = Read_Key(file_key, 'N');		
		Cipher(file_plaintext , file_cipher , E , N); 
		Time_Check(Start , "Encipher");        		
		break;
		}

    case '3':  // Decryption ..........
		{
        cout<<endl<<"Enter Private Key File:\t";             cin>>file_key; 
	    //strcpy (file_key , "d.key"); //testing purpose  
        cout<<endl<<"Enter your Cipher Text File Name:\t";   cin>>file_cipher;   
		//strcpy (file_cipher , "cipher.rsa");          
 	    cout<<endl<<"Enter your Decipher  File Name:\t";     cin>>file_decipher;
        //strcpy (file_decipher , "decipher.txt");        
		
		cout<<endl<<"Deciphering Start...."<<endl;
		Start = clock();
		D = Read_Key(file_key, 'D');
        N = Read_Key(file_key, 'N');
		Decipher(file_cipher , file_decipher , D , N);  
		Time_Check(Start , "Decipher");  
		break;
		}
    
	default:
		return;
	}  
	  SetColor(WHITE);
	  cout<<"Press any key for Menu "; getche();
  }while ((opt != 'x') || (opt !='X'));

}


///////////////////////////////////////////////////////////////
void Cipher(char source_file[1024] , char dest_file[1024] , unsigned int Exp , unsigned long Mod)
{
 FILE *in;
 int counter; int Ret; 
 int Code = Exponential_Power(Mod , 1 , 32); //Base System (2-32) used for coding cipher file...
 if (Code < 2) Code = 31;  // if 0 or 1 then 31 is my choice of base

 in = fopen(source_file ,"rb");
   if(in == NULL ) {
 	cout << "OOps! Source file "<<source_file<<" non-existing or corrupt file\n";
    return;
   }
    ofstream out(dest_file);
	if (out.fail()) {
        cout << "error: could not find file "<<dest_file<<endl;
			fclose(in);
        exit(1);    // abort program
    }
	   
   while( (counter=fgetc(in)) != EOF ) 
   {
    Ret = Exponential_Power(counter, Exp, Mod);  	 
    out<<DecimalToSystem(Ret, Code)<<" ";
   }
   fclose(in); out.close();
   cout<<endl<<dest_file << " Encoded on Base <"<<Code<<">.....calculated on N mod 32. i.e. 2-31"<<endl;
}


///////////////////////////////////////////////////////////////
void Decipher(char source_file[1024] , char dest_file[1024] , unsigned int Exp , unsigned long Mod)
{
 FILE *out;
 char val[33]; int Ret; 
 int Code = Exponential_Power(Mod , 1 , 32);  //Base System (2-32) used for reading coding cipher file...
 if (Code < 2) Code = 31; // if 0 or 1 then 31 is my choice of base

   ifstream input(source_file);
    if (input.fail()) {
        cout << "error: could not find file "<<source_file<<endl;
        exit(1);    // abort program
    }    
 
  out = fopen(dest_file ,"wb");
  if(out == NULL ) {
 	cout << "OOps! Dest file "<<dest_file<<" non-existing or corrupt file\n";
    return;
   }
 	   
  input >> val;  
  Ret = SystemToDecimal(val , Code);  
    while (!input.eof()) 
	{
     Ret = Exponential_Power(Ret, Exp, Mod); 	 
	 if (Ret < 1) Ret = Mod + Ret;
     input >> val;
	 fputc( ((UCHAR)Ret), out);
	 Ret = SystemToDecimal(val , Code);
    }   
   input.close();
   fclose(out);
}


////////////////////////////////////////////////////////////////////////
//// Apply Miller Rabin ()
unsigned long Apply_Miller_Rabin(char Str[64]){
  unsigned long Num;
  int k= 15; // no of times miller rabin will test the primarility...
  int res; // result if prime or not (0 / 1)  
  do
  {
   SetColor(LIME);
   Num = Generate_Random_Number(MAX);   
   if (Num > 2)  
   {
	   if (Show_Detail == 1) 
	   {
		   cout<<endl<<" Applying Miller Rabin on  "<<Str<<"="<<Num<<endl;
	   }
	   res = MR_Primality_Test(Num , k);
	   if (Show_Detail == 1) 
	   {
		   SetColor(WHITE);
		   if ( (res ==0) || (Num <  2) ) cout<<Str<<"="<<Num<<"..Rejected"<<endl;
	   }
   }
   
  }while ( (res ==0) || (Num <  2) );  

  if (Show_Detail == 1)  
  {
   SetColor(WHITE);
   cout<<Num <<" Accepted as "<<Str;
   cout<<endl<<"-------------------------------------------------------------"<<endl;
  }
  return Num;
}


///////////////////////////////////////////////////////////////
int Generate_Random_Number(int number){
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
unsigned long Exponential_Power(unsigned long base, unsigned long expon, unsigned long mod)
{
    unsigned long res;
    
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
int MR_Primality_Test(unsigned int n, int k)
{
    int i, a, r, s, d, b;

    assert(n > 1); // failure if n = 1 , 0; 
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
		if (Show_Detail == 1)   	cout<<(i+1)<<" : Random Number = "<<a<<endl;
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
    		if (Show_Detail == 1)  cout<<"\tMaybe Prime!..."<<endl;   
			continue;	
		}
        return 0;
    }	
    return 1;
}
//////////////////////////////////////////////////////////




////////////////////////////////////////////////////
unsigned long Extended_Euclidean_GCD(unsigned long m, unsigned long n)
{
  unsigned long Q, A1 , A2 , A3 , B1, B2, B3 , T1 , T2 , T3;
  A1 = 1;  A2=0;  A3=m;
  B1 = A2; B2=A1; B3=n;
  Q=-1;  
  SetColor(MAGENTA);    
  if (Show_Detail == 1)
  {   
   cout<<"--------------------------------------------------------------------"<<endl;
   cout<<setw(6)<<"Q"<<setw(7)<<"A1"<<setw(7)<<"A2"<<setw(7)<<"A3"<<setw(7)<<"B1"<<setw(7)<<"B2"<<setw(7)<<"B3"<<endl;
   cout<<"--------------------------------------------------------------------"<<endl;
   cout<<setw(6)<<"/";
   printf(" %6ld %6ld %6ld %6ld %6ld %6ld", A1 , A2 , A3 , B1 , B2 , B3);
   cout<<endl<<endl;
  }
  while (B3 >= 0) {
  if (B3 == 0) {
   A3 = GCD(m,n);
   if (Show_Detail == 1)
   {
   	cout<<"--------------------------------------------------------------------"<<endl;
    cout<<"GCD :\t"<<A3<<endl;	
    cout<<"No Inverse at all"<<endl;	
   	cout<<"--------------------------------------------------------------------"<<endl;
   }
    return 0;
  }
 if (B3 == 1) {
    A3 = GCD(m,n);
   if (Show_Detail == 1)
   {   
   	cout<<"--------------------------------------------------------------------"<<endl;
	cout<<"GCD :\t"<<A3<<endl;	
   }
	if (B2 < 0) B2 = m + B2;
   if (Show_Detail == 1)
   {     
	cout<<"Inverse of "<<n<< " in GF("<<m<<") : "<<B2<<endl;	
	cout<<"--------------------------------------------------------------------"<<endl;
   }
	return B2;
  }     
     
   Q = A3 / B3, 
   T1 = A1 - (Q*B1);     T2 = A2 - (Q*B2);     T3 = A3 - (Q*B3);
   A1 = B1;              A2 = B2;	   		   A3 = B3;
   B1 = T1;			     B2 = T2;              B3 = T3;	    
   if (Show_Detail == 1)
   {
    printf("%6ld %6ld %6ld %6ld %6ld %6ld %6ld", Q , A1 , A2 , A3 , B1 , B2 , B3);
    cout<<endl<<endl;   
   }
  } // end of while   
  printf("-------------------------------------------------\n");
} // end of function..


////////////////////////////////////////////
unsigned long GCD(unsigned long m, unsigned long b)
{
	unsigned long n , d , q , r;
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


////////////////////////////////////////////////////////////
void Time_Check(unsigned int Start , char Purpose[1024])
{
 unsigned int End; float Diff; int Min; float Sec ; int rem;	 
 End = clock();
 Diff = (End - Start); 
 
 SetColor(WHITE);
 if  (float(Diff / 1000) > 60.0)
 {
   rem = (Diff / 1000);
   Min = (rem / 60);
   Sec = (rem % 60);   
   cout<<endl<<"Total Time (" << Purpose <<") = "<<Min<<" Min : "<<Sec<<" Sec "<<endl;   	 
 }
 else if  (float(Diff / 1000) > 1.0)
 {
   Sec = (Diff / 1000);
   cout<<endl<<"Total Time (" << Purpose <<") = "<<Sec<< " Seconds" <<endl;   	 
 }
 else
 {
   cout<<endl<<"Total Time (" << Purpose <<") = "<<Diff<< " Milli Seconds" <<endl;   	 
 } 
}


////////////////////////////////////////////////////////////
void SetColor(int color_num)
{
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_num); 	
}


//////////////////////////////////////////////////////////////////////////////
void Save_Key(char key_file[1024] , unsigned int ed , unsigned long n)
{
  ofstream out(key_file);
  if (out.fail()) 
  {
   cout << "error: could not find file: "<<key_file<< endl;
   exit(1);    // abort program
  }	   
  out<<DecimalToSystem(ed, 16)<<"\n";
  out<<DecimalToSystem(n , 16)<<"\n";  
  out.close();
}


////////////////////////////////////////////////////////////////////////////////////
unsigned long Read_Key(char key_file[1024] , char ch)
{
  char ed[33] , n[33];
  
  ifstream input(key_file);
  if (input.fail()) {
    cout << "error: could not find file: "<<key_file<<endl;
    exit(1);    // abort program
  }    	   
  input >> ed; 
  input >> n;   
  input.close();  

  switch (ch)
  {
   case 'E': return SystemToDecimal(ed , 16); 
   case 'D': return SystemToDecimal(ed , 16);
   case 'N': return SystemToDecimal(n  , 16);   
  }
}
////////////////////////////////////////////////////////////////////////////////////

long double Raise_To_Power(int base , int ex)
{
  long double b;
  b = base;
  if  (ex == 0) return 1;
  if  (ex == 1) return base;

  while (ex-- > 1)
  {    
     b = b * base;  	 	 
  }
  return b;
}

////////////////////////////////////////////////////////////////////
int SystemToDecimal(char * charInput, int sys)
{
 int intInput=0;
 int intReturn=0;
 int intDigit=0;
 int i=0;
 int j=0;

  strupr(charInput);
  i=strlen(charInput);
  charInput += (i-1) ;

 for(;i; i--, charInput--){

  if(*charInput > 57)
  {
	intInput = *charInput - 55;
  }
  else 
  {
   intInput = *charInput - 48;
  }
  intDigit = Raise_To_Power(sys , j++);	//	cout<<sys<<" of "<<j<<" = "<<intDigit<<endl;   
   intDigit *= intInput;
   intReturn +=  intDigit;
 } // end of for

 return intReturn ;
}


////////////////////////////////////////////////////////
char *DecimalToSystem(int intInput, int sys)
{
  static char a[64];
  strupr(itoa(intInput, a, sys));
  return a;
}
///////////////////////////////////////////////////////


