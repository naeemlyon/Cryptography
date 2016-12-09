#include <iostream>
#include <time.h>  // timing calculation
#include <fstream>
#include <conio.h>
#include <windows.h> // color of my life
#include <stdlib.h> //for ltoa()

using namespace std;
#define SZ 51780 // handle Factorial(13950)  

///////////////////////////////////////////////////////////////
class Extra_Large_Number
{
private:

 int Len; //length of Extra_Large_Number string
public:
 char Str[SZ]; //Extra_Large_Number number, as a string 
 Extra_Large_Number Multiply_Digit(const int) const; //prototypes for
 Extra_Large_Number Multiply_by_10(const Extra_Large_Number) const; //private functions
 // above are private tempo...made public...


 Extra_Large_Number() : Len(0) // 0-arg constructor
 { Str[0]='\0'; }
 //-------------------------------------------------------------------
 Extra_Large_Number(const char S[SZ]) // 1-arg constructor
 { strcpy(Str, S); Len=strlen(S); } //for string
 //-------------------------------------------------------------------
 Extra_Large_Number(const unsigned long n) //1-arg constructor
 { //for long int
  ltoa(n, Str, 10); //convert to string
  strrev(Str); //reverse it
  Len=strlen(Str); //find length
 }
 //-------------------------------------------------------------------
 void Display() const; //display Extra_Large_Number
 void Get(); //get Extra_Large_Number from user
 unsigned long Length(); //get Extra_Large_Number from user
 void Write_File(char file_name[1024]);
 Extra_Large_Number operator + (const Extra_Large_Number); //add Extra_Large_Numbers
 Extra_Large_Number operator - (const Extra_Large_Number); //subtract Extra_Large_Numbers
 Extra_Large_Number operator * (const Extra_Large_Number); //multiply Extra_Large_Numbers
};


//////////////////////////////////////////////////////////////////////////////
unsigned long Extra_Large_Number::Length ()
{
  return strlen(Str);
}

//////////////////////////////////////////////////////////////////////////////

void Extra_Large_Number::Write_File(char file_name[1024])
{
  ofstream out(file_name);
  if (out.fail()) 
  {
   cout << "error: could not find file: "<<file_name<< endl;
   exit(1);    // abort program
  }	   
  out.write(strrev(Str) , Len);  
  out.close();
  cout <<endl<<"Result File '"<<file_name<<"' Written"<<endl; 
}


////////////////////////////////////////////////////////////////////////
void Extra_Large_Number::Display() const //display Extra_Large_Number
{
 char temp[SZ];
 strcpy(temp,Str); //make copy
 cout << strrev(temp); //reverse the copy
 cout<<endl<<"================================================================"<<endl;
} //and display it

////////////////////////////////////////////////////////////////////////
void Extra_Large_Number::Get() //get Extra_Large_Number from user
{
 cin >> Str; //get string from user
 Len = strlen(Str); //find its length
 strrev(Str); //reverse it
}

////////////////////////////////////////////////////////////////////////
Extra_Large_Number Extra_Large_Number::operator + (const Extra_Large_Number v) //add Extra_Large_Numbers
{
 char temp[SZ];
 int j; int d1 , d2 , sum;
 //find longest number
 int maxlen = (Len > v.Len) ? Len : v.Len;
 int carry = 0; //set to 1 if sum >= 10
 for(j = 0; j <maxlen; j++) //for each position
 {
  d1 = (j > Len-1) ? 0 : Str[j]-'0'; //get digit
  d2 = (j > v.Len-1) ? 0 : v.Str[j]-'0'; //get digit 
  sum = d1 + d2 + carry; //add digits
 if( sum >= 10 ) //if there’s a carry,
	 { sum -= 10; carry=1; } //decrease sum by 10,
 else //set carry to 1
	 carry = 0; //otherwise carry is 0
 //cout<<endl<<d1<<" + "<<d2<< " = " <<sum<< " (Carry = "<<carry;
 temp[j] = sum+'0'; //insert char in string
}

 if(carry==1) //if carry at end,
 temp[j++] = '1'; //last digit is 1
 temp[j] = '\0'; //terminate string
 //cout<<endl<<temp<<endl<<"============================================================"<<endl;
 return Extra_Large_Number(temp); //return temp Extra_Large_Number
}


//////////////////////////////////////////////////////////////////////
Extra_Large_Number Extra_Large_Number::operator - (const Extra_Large_Number v) //add Extra_Large_Numbers
{
 char temp[SZ]; 
 bool rev=0; // (0 , answer=+ve) , (1 , answer -ve)
 int j; int d1 , d2 , dif; 

 //longest number ?  to decide answer is negavtive or positive number
 if (Len == v.Len)  
 {   
   for (j=1; j<Len; j++)
   {
    d1 =   Str[Len-j] - '0';
    d2 = v.Str[Len-j] - '0';
	if (d1 != d2)   
	{ rev= (d1 < d2) ? 1:0; break;}
   }
 }
 else if (Len < v.Len)
 {
   rev = 1; // -ve number
 }
 //if (rev == 1)    cout<<endl<<"Answer be in Negative.."<<endl;

 int maxlen = (Len > v.Len) ? Len : v.Len;
 int carry = 0; //set to 1 if sum >= 10
 for(j = 0; j <maxlen; j++) //for each position
 {
  if (rev == 1)
  {
   d2 = (j > Len-1) ? 0 : Str[j]-'0'; //get digit
   d1 = (j > v.Len-1) ? 0 : v.Str[j]-'0'; //get digit
  }
  else
  {
   d1 = (j > Len-1) ? 0 : Str[j]-'0'; //get digit
   d2 = (j > v.Len-1) ? 0 : v.Str[j]-'0'; //get digit  
  }
  
 d1 += carry; 
 if( d1 < d2 ) //if carry required,
	 { d1  += 10; carry=-1; } //increase d1 by 10,
 else //set carry to 1
	 carry = 0; //otherwise carry is 0
 dif = d1 - d2 ; //subtract digits
 //cout<<endl<<d1<<" - "<<d2<< " = " <<digitsum;
 temp[j] = dif+'0'; //insert char in string
}

 // if answer is in negative 
 if(rev == 1)  temp[j++] = '-'; //last digit -
 temp[j] = '\0'; //terminate string
 return Extra_Large_Number(temp); //return temp Extra_Large_Number
}


////////////////////////////////////////////////////////////////////////
Extra_Large_Number Extra_Large_Number::operator * (const Extra_Large_Number v) //multiply
{ 
 Extra_Large_Number pprod; //product of one digit
 Extra_Large_Number tempsum; //running total
 for(int j=0; j < v.Len; j++) //for each digit in arg
 {
  int digit = v.Str[j]-'0'; //get the digit
  pprod = Multiply_Digit(digit); //multiply this by digit
  for(int k=0; k < j; k++) //multiply result by
  pprod = Multiply_by_10(pprod); // power of 10
  //cout<<" X 10 = "<<pprod.Str; 
  tempsum = tempsum + pprod; //add product to total
  //cout<<"\t = "<<tempsum.Str<<endl;  
  //cout<<digit;
 }
 return tempsum; //return total of prods
}

////////////////////////////////////////////////////////////////////////
Extra_Large_Number Extra_Large_Number::Multiply_by_10(const Extra_Large_Number v) const //multiply
{ //arg by 10
 char temp[SZ];
 for(int j=v.Len-1; j>=0; j--) //move digits one
 temp[j+1] = v.Str[j]; // position higher
 temp[0] = '0'; //put zero on low end
 temp[v.Len+1] = '\0'; //terminate string
 return Extra_Large_Number(temp); //return result
}

////////////////////////////////////////////////////////////////////////
Extra_Large_Number Extra_Large_Number::Multiply_Digit(const int d2) const
{ //multiply this Extra_Large_Number
 char temp[SZ]; //by digit in argument
 int j, carry = 0;
 ;
 for(j = 0; j < Len; j++) //for each position
 { // in this Extra_Large_Number
  int d1 = Str[j]-'0'; //get digit from this
  int digitprod = d1 * d2; //multiply by that digit 

  digitprod += carry; //add old carry  

  if( digitprod >= 10 ) //if there’s a new carry,
  {
   carry = digitprod/10; //carry is high digit
   digitprod -= carry*10; //result is low digit   
  }
  else
   carry = 0; //otherwise carry is 0
   temp[j] = digitprod+'0'; //insert char in string    
}
  if(carry != 0) //if carry at end,
  temp[j++] = carry+'0'; //it’s last digit
  temp[j] = '\0'; //terminate string
  //cout<<Str<<" X "<<d2<< " = "<<temp<<"\t";
  return Extra_Large_Number(temp); //return Extra_Large_Number
}

/////////////////////////////////////////////////////////////////////////////
//void Time_Check(unsigned int , char Purpose[1024]);
//void SetColor(int);
////////////////////////////////////////////////////////////
void SetColor(int color_num)
{
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_num); 	
}

////////////////////////////////////////////////////////////
void Demo(void)
{
 cout<<"==============================================================="<<endl;
 cout<<endl<<"\t\t\t\t\t"<<"  7193";
 cout<<endl<<"\t\t\t\t\t"<<"X  852";
 cout<<endl<<"\t\t\t\t\t-------";
 cout<<endl<<"\t\t\t\t\t 14386";
 cout<<endl<<"\t\t\t\t\t35965X";
 cout<<endl<<"\t\t\t\t       57544XX";
 cout<<endl<<"\t\t\t\t----------------";
 cout<<endl<<"\t\t\t\t=      6128436";
 cout<<endl<<"\t\t\t\t----------------"<<endl;
 cout<<endl<<"Arr[] = 7193 ---> 3917\t\t Arr[] = 852 ---> 258"<<endl;
 cout<<endl<<"3917 X 2 = 68341\t Append0(0) = 68341      Final[] = 68341"<<endl;
 cout<<endl<<"3917 X 5 = 56953\t Append0(1) = 056953     Final[] = 630473"<<endl;
 cout<<endl<<"3917 X 8 = 44575\t Append0(2) = 0044575    Final[] = 6348216"<<endl; 
 cout<<"==============================================================="<<endl;
 system("pause"); 
 cout<<"\t\t\t Addition"<<endl;
 cout<<endl<<"\t\t\t\t"<<"   374036"<<" ---> "<<"630473";
 cout<<endl<<"\t\t\t\t"<<"+ 5754400"<<" ---> "<<"0044575";
 cout<<endl<<"\t\t\t\t"<<"---------";
 cout<<endl<<"\t\t\t\t"<<"= 6128436"<<" ---> "<<"6348216";
 cout<<endl<<"\t\t\t\t"<<"---------";
 cout<<endl<<"6 + 0 = 6 (Carry = 0)"<<endl;
 cout<<endl<<"3 + 0 = 3 (Carry = 0)"<<endl;
 cout<<endl<<"0 + 4 = 4 (Carry = 0)"<<endl;
 cout<<endl<<"4 + 4 = 8 (Carry = 0)"<<endl;
 cout<<endl<<"7 + 5 = 2 (Carry = 1)"<<endl;
 cout<<endl<<"3 + 7 = 1 (Carry = 1)"<<endl;
 cout<<endl<<"0 + 5 = 6 (Carry = 0)"<<endl;
 cout<<endl<<"----------------------------"<<endl;
 cout<<"\t"<<"6348216";
 cout<<endl<<"----------------------------"<<endl;



}

////////////////////////////////////////////////////////////
void Time_Check(unsigned int Start , char Purpose[1024])
{
 unsigned int End; float Diff; int Min; float Sec ; int rem;	 
 End = clock();
 Diff = (End - Start); 
 
 SetColor(15);
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



////////////////////////////////////////////////////////
void main(void)
{
 unsigned int Start;	char opt;
 unsigned long num , j;
 Extra_Large_Number First=1; Extra_Large_Number Second=1; Extra_Large_Number Result=1; 
 

  do
  {    
	  SetColor(11);
	  cout<<endl<<"========================================================";	  
	  cout<<endl<<"=======      Extra Large Number   =========";
	  cout<<endl<<"============    Muhammad Naeem   ==================";
      cout<<endl<<"========================================================";
	  SetColor(14);
	  cout<<endl<<"Press\t"; SetColor(15); cout<<"9\t";SetColor(14); cout<<"Demo (How it works)";
      cout<<endl<<"Press\t"; SetColor(15); cout<<"+\t";SetColor(14); cout<<"Addition             Max Digit: 51761 after Sum";
      cout<<endl<<"Press\t"; SetColor(15); cout<<"-\t";SetColor(14); cout<<"Subtraction          Max Digit: 51761 per Number";
      cout<<endl<<"Press\t"; SetColor(15); cout<<"*\t";SetColor(14); cout<<"Multiplication       Max Digit  51761 for Result";
      cout<<endl<<"Press\t"; SetColor(15); cout<<"1\t";SetColor(14); cout<<"Exponential Power    Base 9 digits  , Exp 3 or 4 digits";
	  cout<<endl<<"Press\t"; SetColor(15); cout<<"2\t";SetColor(14); cout<<"Facorial             Max No: 13950";      
      cout<<endl<<"Press\t"; SetColor(15); cout<<"any key\t";SetColor(14); cout<<"Quit appliction ";
	  SetColor(11);
      cout<<endl<<"========================================================"<<endl;
	  cout<<endl<<"Your Choice Please!\t "; cin>>opt;
    switch (opt)
	{
    case '9': { Demo(); break; }       
    

	case '+':
		{         
         cout << "\nFirst 1st Number plz:\t";  First.Get();
         cout << "\nEnter 2nd Number plz:\t";  Second.Get();                   
		 cout<<endl<<"Processing\t";
		 Start = clock(); 
		  Result = First + Second; 
		 Time_Check(Start , "Addition of Two Numbers");		        		 
		 cout<<endl<<"Digits in 1st Number:\t"<<First.Length();
		 cout<<endl<<"Digits in 2nd Number:\t"<<Second.Length();
		 cout<<endl<<"Digits in Result Number:\t"<<Result.Length()<<endl;			 
		 Result.Write_File("out.naeem"); 		 
		 break;
		}

	case '-':
		{         
         cout << "\nFirst 1st Number plz:\t";  First.Get();
         cout << "\nEnter 2nd Number plz:\t";  Second.Get();                   
		 cout<<endl<<"Processing\t";
		 Start = clock(); 
		  Result = First - Second; 
		 Time_Check(Start , "Subtraction of Two Numbers");		        		 
		 cout<<endl<<"Digits in 1st Number:\t"<<First.Length();
		 cout<<endl<<"Digits in 2nd Number Digits:\t"<<Second.Length();
		 cout<<endl<<"Total Digits in Result Number:\t"<<Result.Length()<<endl;			 		 
		 Result.Write_File("out.naeem"); 		 
		 break;
		}

	case '*':
		{         
         cout << "\nFirst 1st Number plz:\t";  First.Get();
         cout << "\nEnter 2nd Number plz:\t";  Second.Get();                   
		 cout<<endl<<"Processing\t";
		 Start = clock(); 		  
		  Result = First * Second; 
		 Time_Check(Start , "Multiplication of Two Numbers");		        		 
		 cout<<endl<<"Digits in 1st Number:\t"<<First.Length();
		 cout<<endl<<"Digits in 2nd Number:\t"<<Second.Length();
		 cout<<endl<<"Total Digits in Result Number:\t"<<Result.Length()<<endl;			 		 		 
		 Result.Write_File("out.naeem"); 		 
		 break;
		}

	case '1' :
		{
         cout << "\n\nEnter Base  Number: ";    First.Get();
		 cout << "\n\nEnter Expon Number:  ";    cin >> num; 
		 cout<<endl<<"Processing\t";
		 Start = clock(); 		 
         for(j=num; j>0; j--)     Result = Result * First; 
	     Time_Check(Start , "Base ^ Exp");		 
		 cout<<endl<<"Digits in Base   Number:\t"<<First.Length();		 
		 cout<<endl<<"Digits in Result Number:\t"<<Result.Length()<<endl;			 
		 Result.Write_File("out.naeem");		 
		 break;
		}
    
	case '2':
		{         
         cout << "\n\nEnter Number for Factorial: ";
         cin >> num; 
		 cout<<endl<<"Processing\t";
		 Start = clock(); 		 
         for(j=num; j>0; j--)     Result = Result * j; 
	     Time_Check(Start , "Factorial");		 
		 cout<<endl<<"Digits in Result Number:\t"<<Result.Length()<<endl;			 		 
		 Result.Write_File("out.naeem");		 
		 break;
		}
    
	default:
		return;
	}  
	    SetColor(15);   
		Result=1;  First=1;  Second=1;
	    cout<<"Press any key for Menu "; getche();
  }while ((opt != 'x') || (opt !='X'));
  
  
}
//////////////////////////////////////////////////////////////////////////////////
