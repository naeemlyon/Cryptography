#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <windows.h>
#define size  5
#define Nk  4
#define Nb  4
#define Nr 10
#define _sZOfw  Nb *(Nr+1)

//---------------------------------------------------------------------------
char *Padding(int ,int );     //function to pad strings with 0
unsigned int Dec_Val(char );  //used to convert string to hexadecimal
int Power(int ,int );         //Power function eg 2^x;used in String_to_dec
unsigned int string_to_dec(char *str);  //convert a string to hexadecimal
void StateArray(char *[4][4], char **);       //building the State Array S
void KeyExpansion(char **,char **,int );      //Key Expasion (Section 5.2)
char *word(char *,char *,char *,char *);      //used in key expansion 
char *RotWord(char *);                        //used in Key Expansion 
static unsigned int RCon[10] = {0x01000000, 0x02000000, 0x04000000, 0x08000000,
                                0x10000000, 0x20000000, 0x40000000, 0x80000000,
                                0x1b000000, 0x36000000}; //See Section 5.2 & 4.2
static unsigned int SBox(int, int); // Substitution values for byte {xy in hexadecimal format}
static unsigned int SBoxInv(int, int); // Inverse Substitution values {for byte xy in hexadecimal format}
char *SubWord(char *);                          
void AddRoundKey(char *[4][4], char **, int);   
void SubBytes(char *[4][4]);                    
void ShiftRows(char *[4][4]);                   
void MixColumns(char *[4][4]);                  
unsigned int Xtime(unsigned int);               
unsigned int PolyMult(unsigned int , unsigned int );//PolyMult

//decryption
void InvShiftRows(char *[4][4]);                
void InvSubBytes(char *[4][4]);                 
void InvMixColumns(char *[4][4]);               
/////////////////////////////////////////////
void PrintBox(char *S[4][4] , char *);
void SetColor(int);
void En_Cipher(void);
void De_Cipher(void);
void Load_Key(void);
void Read_File(char file_name[1024] , char *);
char *Hex_to_Char(char*, const char* );
void Time_Check(unsigned int , char Purpose[1024]);
void Truncate_File(char file_name[1024]);
void Show_Expand_Key(void);

char file_key[1024];  
char file_plaintext[1024];       
char file_cipher[1024];  
char file_decipher[1024];  
char *key[16]; // =  {"2b", "7e", "15", "16", "28", "ae", "d2", "a6", "ab", "f7", "15", "88", "09", "cf", "4f", "3c"};    
bool Show_Matrix;
char *S[4][Nb]; //State Array
char *w[_sZOfw]; // 4 * 10+1 = 44 (if key length = 128)
int round;
int K=0; 


  
///////////////////////////////////////////////////////////////////////////////////////////
using namespace std;
void main(void)
{
  unsigned int Start;	char opt;
  do
  {    
	  SetColor(11);
	  cout<<endl<<"========================================================";	  
	  cout<<endl<<"=====  Advanced Encryption Standard (AES) ======";
	  cout<<endl<<"=============        M.Naeem     =====================";
      cout<<endl<<"========================================================";
	  SetColor(14);
	  cout<<endl<<"Press\t"; SetColor(15); cout<<"1\t";SetColor(14); cout<<"for Key Table Genertion ";
      cout<<endl<<"Press\t"; SetColor(15); cout<<"2\t";SetColor(14); cout<<"for Encryption ";
      cout<<endl<<"Press\t"; SetColor(15); cout<<"3\t";SetColor(14); cout<<"for Decryption ";
      cout<<endl<<"Press\t"; SetColor(15); cout<<"any key\t";SetColor(14); cout<<"Quit appliction ";
	  SetColor(11);
      cout<<endl<<"========================================================"<<endl;
	  cout<<endl<<"Your Choice Please!\t "; cin>>opt;
    switch (opt)
	{

	case '1':
		{
         cout<<endl<<"Enter your Key File Name\t";      cin>>file_key; 
		 //strcpy (file_key , "key.txt"); //testing purpose   	     		 	    
		 Start = clock(); 
         Load_Key();
		 cout<<endl<<"Expanded Key (M.Naeem MS081009)" <<endl<<endl;
		 Show_Expand_Key();
		 cout<<"expanded_key.txt  created successfully...";
         Time_Check(Start , "Key Expansion");
		
         
		 break;
		}
    case '2':
		{
        cout<<endl<<"Enter your Plain Text File Name\t"; cin>>file_plaintext;  
		//strcpy (file_plaintext , "plain.txt"); // just for temporary purpose to avoid recurring job...
 		cout<<endl<<"Enter your Key File Name\t";   cin>>file_key; 
		//strcpy (file_key , "key.txt"); //testing purpose  
        //cout<<endl<<"Enter your Cipher Text File Name\t";  //cin>>file_cipher;
        strcpy (file_cipher , "cipher.aes");  
        Truncate_File(file_cipher);
        cout<<endl<<"Show each Box/Marix (1 = True , any other=False)\t"; cin>>Show_Matrix;  	    
		Start = clock(); 
        En_Cipher();     
        Time_Check(Start , "Encipher");
		break;
		}

    case '3':
		{
        cout<<endl<<"Enter your Key File Name\t";   cin>>file_key; 
	    //strcpy (file_key , "key.txt"); //testing purpose  
        cout<<endl<<"Enter your Cipher Text File Name\t";  cin>>file_cipher;   
		//strcpy (file_cipher , "cipher.aes");          
 	    //cout<<endl<<"Enter your Decipher  File Name\t"; //cin>>file_decipher;
        strcpy (file_decipher , "decipher.txt"); 
        Truncate_File(file_decipher);		
        cout<<endl<<"Show each Box/Marix (1 = True , any other=False)\t"; cin>>Show_Matrix;  	    
		Start = clock(); 
        De_Cipher();
        Time_Check(Start , "Decipher");  
		break;
		}
    
	default:
		return;
	}  
	  SetColor(15);
	  cout<<"Press any key for Menu "; getche();
  }while ((opt != 'x') || (opt !='X'));
  
  

  
}

///////////////////////////////////////////////////////////////////////////////////////////
void En_Cipher(void)
{
   char  buf[999999]="";    
   char *hexStr[32] ; //= {"32", "43", "f6", "a8", "88", "5a", "30", "8d", "31", "31", "98", "a2", "e0", "37", "07", "34"};
   int z; char buffer[33]; int c=0; int block_no=1;  
   
   Load_Key();   
   Read_File(file_plaintext , buf);   // cout<<buf<<endl<<endl; cout<<strlen(buf)<<endl;        		
   //return;
   z = 0;
    while ( z < strlen(buf))
	{
     SetColor(6);
	 cout<<endl<<"Ciphering Block No.: "<<block_no++<<endl;
    for (c = 0; c < 16; c++)
	{     
     itoa((int)buf[z++] , buffer , 16);	 	 
	 hexStr[c] = new char[3];
	 hexStr[c][0] = buffer[0];  
	 hexStr[c][1] = buffer[1];  
	 hexStr[c][2] = '\0';		 	      
	}
	//for (int t=0; t<16; t++) 		cout<<endl<<hexStr[t][0]<<hexStr[t][1]<<"\t";
    
    SetColor(10);
	StateArray(S, hexStr); 
    PrintBox(S , "\n\nOriginal String");   //	getche();
   
   KeyExpansion(key,w,Nk);
   AddRoundKey(S,w,0);                 
   
 for (round = 1; round < Nr; round++){
   	if (Show_Matrix == 1) printf("\n\nRound No: %d" , round);
	SubBytes(S); 
	ShiftRows(S);                         
	MixColumns(S);           
	AddRoundKey(S,w,round);  
	}
    if (Show_Matrix == 1) printf("\n\nEncryption Round No: %d" , round);
    SubBytes(S);          
    ShiftRows(S);
    AddRoundKey(S,w,Nr);
    PrintBox(S , "\n\nCipher String:");
    //////////////////////
   fstream of(file_cipher,ios_base::out|ios_base::app);     
   if (!of) { cout<<"Ooops! Error in opening <<file_cipher"<<endl; exit(1); }
   for(int K = 0;K < 4; K++)     
     for (int L = 0; L < 4; L++)
   	 {
		of<<S[L][K];		
	 }	
	 of.close();     
	}	
}


/////////////////////////////////////////////////////////////////////////
///// Start of Decryption .......... 
void De_Cipher(void )	
{	
   char  buf[999999]=""; int block_no = 1;
   int z = 0; int c=0; 
   
   SetColor(7);
   Load_Key();   
   Read_File(file_cipher , buf);  // cout<<buf<<endl<<endl; cout<<strlen(buf)<<endl;        		
   //return;   
   
    while ( z < strlen(buf))
	{
     SetColor(5);
	 cout<<endl<<"Deciphering Block No.: "<<block_no++<<endl;
	 for(int K = 0;K < 4; K++){
      for (int L = 0; L < 4; L++)
	  {
		 S[L][K] = new char[3];
         S[L][K][0] = buf[z++];	 
         S[L][K][1]= buf[z++];	 
         S[L][K][2]= '\0';		 	      		 
	  }
	}	
 
   KeyExpansion(key,w,Nk);			
   AddRoundKey(S,w,Nr);
   if (Show_Matrix == 1) PrintBox(S , "\n\nDecrypt Round Key");   
   
   for (round = Nr-1; round >= 1; round--){
	   if (Show_Matrix == 1) printf("\n\nRound No: %d" , round);
	   InvShiftRows(S);
	   InvSubBytes(S);
	   AddRoundKey(S,w,round);               
       InvMixColumns(S);                           
   }  
   if (Show_Matrix == 1) printf("\n\nDecryption Round No: %d" , round);
   InvShiftRows(S);
   InvSubBytes(S);
   AddRoundKey(S,w,0); 
   
   /////////////////////////////////////////////////////////////////////
   int m=0; char *n;  
   char source[32];
   char dest[4096]; 
   
   for(K = 0;K < 4; K++)     
     for (int L = 0; L < 4; L++)
   	 {
		source[m++] = S[L][K][0];				
        source[m++] = S[L][K][1];
	 }
     source[m] = '\0';
	 n = Hex_to_Char(dest, source);
	 
	 //cout<<printf("%s\n", n);
   /////////////////////////////////////////////////////////////////////
   fstream of(file_decipher,ios_base::out|ios_base::app);     
   if (!of) { cout<<"Ooops! Error in opening <<file_cipher"<<endl; exit(1); }
   for(K = 0;K < 16; K++)         
   	 {
		of<< *n++;
	 }	
	 of.close();     
	} // end of dumping data to decipher file...
}


////////////////////////////////////////////
char *Hex_to_Char(char *dst, const char *src)
{
   char *start = dst;   
   while ( *src )
   {	  
      unsigned value;
      if ( sscanf(src, "%2x", &value) == 1 )
      {
         src += 2;
         *dst++ = value;
		 
      }
   }
   *dst = '\0';
   return start;
}



////////////////////////////////////////////////////////////////
unsigned int Dec_Val(char c)
{
  if (c >= '0' && c <='9')
    return c - 48;
  if (c >= 'A' && c <='Z')
    return (c - 'A' +10);
  if (c >= 'a' && c <='z')
    return (c - 'a' +10);
  return 0;
}

/////////////////////////////////////////////////////////////////
int Power(int x,int n){
    int thePower;
    if (n < 1)
         thePower = 1;
    else
         thePower = Power(x,n-1)*x;
    return(thePower);
}

////////////////////////////////////////////////////////////////////
unsigned int string_to_dec(char *str){
 unsigned int val = 0;
 int len = strlen(str);
 for (int i = len; i> 0;--i)
   val +=Dec_Val(str[i-1])* (Power(16,len - i));
 return val;
}

//split the strings into blocks of 8  ...method simmilar to bblocks in DES
char **a_Blocks(char hexArr[]){
   int i,j;
   int chkCnt =0;
   char **DaSplit;
   char *Temp;
   int intCnt;

   Temp = (char *)malloc(sizeof (char));
   DaSplit = (char **)malloc (sizeof(char*));
   intCnt = strlen(hexArr);
   intCnt = (intCnt ) / 8;      //calculate the number of blocks
   intCnt--;
   for (i = 0; i <= intCnt; i++){
      chkCnt = 0;
      for (j =(i* 8);j <= (i *8)+7;j++){
         Temp[chkCnt] = hexArr[j];
         if (chkCnt == 7) {
            Temp[chkCnt+1] = '\0';
            DaSplit = (char **)realloc (DaSplit,(size+2)*sizeof(char *));
            DaSplit[i] = (char *) malloc (strlen(Temp)+1*sizeof(char));
            strcpy (DaSplit[i],Temp);
            chkCnt = 0;
         }
         else chkCnt ++;
      }
   }
  free(Temp);
  return (DaSplit);
}

///  one dimentional array into 2-dimentional array S     ////////////////
void StateArray(char *SArr[4][4],char **In){
   int r,c;
   for (r = 0; r < 4; r++)
      for(c = 0; c < 4; c++) {
        SArr[r][c] = (char *) malloc (sizeof(char *) * 2);
        SArr[r][c] = In[r + (4 * c)];
      }
}


////////////////////////////////////////////////////////////////
char *Padding(int lowerlim,int Upperlim){
   char *paddedStr; int i;
   paddedStr =(char *)malloc ((Upperlim+1) *2);

   for (i = lowerlim; i <Upperlim ;i++ )
      paddedStr[i] =  '0';
   paddedStr[i] = '\0';  //please end the string
   return (paddedStr);
}


////////////////////////////////////////////////////////////////
void KeyExpansion(char **key,char **w, int _Nk){
   char *temp;
   unsigned int A;
   temp = (char *) malloc ((strlen(key[0])+1) * 5);
   int i = 0;
   while (i < _Nk){
     w[i] = (char *) malloc ((strlen(key[4*i])+1) * 5);
     w[i] = word(key[4*i],key[4*i+1],key[4*i+2], key[4*i+3]);
     i++;
   }
   i = _Nk;
   //nb =4 nk=4 nr = 10
   while(i < Nb *(Nr + 1)){
      // strcpy(temp,"");
      strcpy(temp,w[i-1]);
      if (i % _Nk == 0){
         strcpy(temp,RotWord(temp));
         strcpy(temp,SubWord(temp));
         A = string_to_dec(temp)^ RCon[(i/_Nk)-1];
         itoa(A,temp,16);
      }
      else if( (_Nk > 6) && (i % _Nk == 4))
         strcpy(temp,SubWord(temp));
      //w[i] = w[i - _Nk]^temp;
      A = string_to_dec(w[i - _Nk])^string_to_dec(temp);
       w[i] = (char *) malloc ((strlen(key[0])+1) * 5);
      itoa(A,w[i],16);
      i++;
   }
}


//////////////////////////////////////////////////////////////
void Truncate_File(char file_name[1024])
{
	ofstream of(file_name , ios::out | ios::trunc); 
}

///////////////////////////////////////////////////////////////
char *word(char *keyA,char *keyB,char *keyC,char *keyD){
   char *tempStr;
   tempStr = (char *) malloc((strlen(keyA)+1) * 4);
   strcpy(tempStr,"");
   strcat(tempStr, keyA); strcat(tempStr, keyB);
   strcat(tempStr, keyC); strcat(tempStr, keyD);
   return(tempStr);
}

///////////////////////////////////////////////////////////////
char *RotWord(char *Str){
  //permute ArrayStr now; ie(a0a1a2a3)->(a1a2a3a0)
   char *RotStr;
   char *T;
   RotStr = (char *) malloc (strlen(Str)+2);
   T = (char *) malloc (strlen(Str));

   strcpy(RotStr,"");
   strcpy(T,"");

   memcpy(RotStr,Str+2,6);
   RotStr[6] ='\0';
   memcpy(T,Str,2);
   T[2] ='\0';
   strcat(RotStr,T);

  free(T);
   return(RotStr);
}

/////////////////////////////////////////////////////////////////
static unsigned int SBox(int x,  int y){
    register unsigned int SBoxRtn[16][16] = {
      {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76},
      {0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0},
      {0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15},
      {0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75},
      {0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84},
      {0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf},
      {0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8},
      {0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2},
      {0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73},
      {0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb},
      {0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79},
      {0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08},
      {0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a},
      {0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e},
      {0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf},
      {0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}
   };

  return (SBoxRtn[x][y]);
}

///////////////////////////////////////////////////////////////
char *SubWord(char *aStr){
   int A[8];  int j;
   char *teetee; char *tempVal;char *t; char *Pad;
   teetee = (char *) malloc(sizeof(char));

   Pad = (char *) malloc(sizeof(char));
   t = (char *) malloc(sizeof(char));
   tempVal = (char *) malloc((sizeof(char)+1)* 2);
   strcpy(tempVal,"");
   for (j = 0; j < 8; j++){
      strcpy(teetee, "");
      *teetee = aStr[j];  teetee[1] ='\0';
      A[j] = string_to_dec(teetee);
   }
   j = 0;
   while (j <= 6){
         strcpy(t,"");
         itoa(SBox(A[j],A[j+1]),t,16);
         if (strlen(t) < 2){
            strcpy(Pad,"");
            Pad = Padding(0,1);
            strcat(Pad,t);
            strcpy(t,Pad);
         }
         strcat(tempVal,t);
         j += 2;
   }
   return(tempVal);
}

//////////////////////////////////////////////////////////////
void AddRoundKey(char *SArr[4][4], char **w,int rnd){
//void AddRoundKey(char *SArr[4][4], char **w){
   int Cnt;
  int r,c,k;  //r = row; c= column
   unsigned int A;
   char *temp;char *tempStr; char *Pad;
   temp = (char *) malloc (sizeof(char));
   tempStr = (char *) malloc (sizeof(char));
   Pad = (char *) malloc(sizeof(char));

   //char *SArr[4][4] = SArray;
   Cnt = 1;
   for(r = 0; r < 4; r++){
      k= (rnd * Nb) + Cnt;
      for(c = 0; c < 4; c++){
        strcpy(tempStr,"");
         strcpy(temp,"");
         memcpy(tempStr,(w[k-1])+(c*2),2); tempStr[2] ='\0';
         strcpy(temp,SArr[c][r]);
         A = string_to_dec(temp)^ string_to_dec(tempStr);
         itoa(A,temp,16);
         if (strlen(temp) < 2){
            Pad = Padding(0,1);
            strcat(Pad,temp);
            strcpy(temp,Pad);
         }
         
		SArr[c][r] =(char *) malloc ((sizeof(char)));		 
		strcpy(SArr[c][r],temp);
      }
      Cnt++;
   }
   PrintBox(SArr , "\n\nAdd Round Key");
   //free(temp);// free(Pad); free(tempStr); 
}

//////////////////////////////////////////////////////////////////
void SubBytes(char *SArr[4][4]){
   int r,c;
   char xlook[2];char ylook[2]; char hexStr[2]; char *Pad;
   int intxlook,intylook;
   char *temp;
   int subbyte;
   Pad = (char *)malloc (sizeof(char));
   temp = (char *)malloc (sizeof(char));

   for (r = 0; r < 4; r++)
      for(c = 0; c < 4; c++){
            strcpy(temp,"");
            strcpy(xlook,"");strcpy(ylook,"");
            strcpy(temp,SArr[r][c]);
            xlook[0] = temp[0]; xlook[1] = '\0';
            ylook[0] = temp[1];ylook[1] = '\0';
            intxlook = string_to_dec(xlook);
            intylook = string_to_dec(ylook);

         subbyte = SBox(intxlook,intylook);
         itoa(subbyte,hexStr,16);

         if (strlen(hexStr) < 2){
            strcpy(Pad,"");
            Pad = Padding(0,1);
            strcat(Pad,hexStr);	 
			strcpy(SArr[r][c],Pad);
         }
         else
            strcpy(SArr[r][c],hexStr);
		 
      }

      PrintBox(SArr , "\n\nSubs Bytes");
      //free(temp); free(Pad);
}     //end of void

///////////////////////////////////////////////////////////////
void ShiftRows(char *SArr[4][4]){
   int c;
   char *temp,*temp1;

   temp = (char *) malloc (sizeof(char));
   temp1 = (char *) malloc (sizeof(char));
   strcpy(temp,""); strcpy(temp1,"");
   //shift(1,4) = 1
   strcpy(temp,SArr[1][0]);
   for (c = 1; c <= 3; c++ )
      strcpy(SArr[1][c-1],SArr[1][c]);
   strcpy(SArr[1][3],temp);

   //shift(2,4) = 2
   strcpy(temp,SArr[2][0]);
   strcpy(temp1,SArr[2][1]);
   for (c = 2; c <= 3; c++ )
      strcpy(SArr[2][c-2],SArr[2][c]);
   strcpy(SArr[2][2],temp);
   strcpy(SArr[2][3],temp1);

   //shift(3,4) = 3
   strcpy(temp,SArr[3][3]);
   for (c = 2; c >= 0; c-- )
      strcpy(SArr[3][c+1],SArr[3][c]);
   strcpy(SArr[3][0],temp);

   PrintBox(SArr , "\n\nShift Rows");             

   //free(temp); free(temp1);
}

//Didn't use this Array this is supposed to be the official mix Array
static unsigned int MixArray( unsigned int x,  unsigned int y){
   unsigned int MArr[4][4] = {
      {0x02, 0x03, 0x01, 0x01},
      {0x01, 0x02, 0x03, 0x01},
      {0x01, 0x01, 0x02, 0x03},
      {0x03, 0x01, 0x01, 0x02}
   };
   return (MArr[x][y]);
}

///////////////////////////////////////////////////////////////////////////////
void MixColumns(char *SArr[4][4]){
   int r,c;                    //r = row and c= column
   unsigned int A[4][4];       //A = Hex Array
   char *TStr; char *Pad;
   TStr = (char *) malloc (sizeof(char)); //Temporary Str for re-assigning
   Pad = (char *) malloc (sizeof(char));  //Pad =Zero Pad Str

   for (r = 0; r < 4; r++)
      for (c = 0; c < 4; c++)
         A[c][r] = PolyMult(0x02,string_to_dec(SArr[c][r]))
                   ^ PolyMult(0x03,string_to_dec(SArr[(c+1)% 4][r]))
                   ^ string_to_dec(SArr[(c+2)% 4][r])
                   ^ string_to_dec(SArr[(c+3)% 4][r]);

   for (r = 0; r < 4; r++)    //convert the hexadecimal Array to a string Array
      for (c = 0; c < 4; c++){
         strcpy(TStr,"");
         itoa(A[r][c],TStr,16);
         if (strlen(TStr) < 2){
            Pad = Padding(0,1);
            strcat(Pad,TStr);
			//SArr[c][r] =(char *) malloc ((sizeof(char)));		 
            strcpy(SArr[r][c],Pad);
         }
         else {
			 //SArr[c][r] =(char *) malloc ((sizeof(char)));		 
			 strcpy(SArr[r][c],TStr);
		 }
       }
    PrintBox(SArr , "\n\nMix Columns");
   //free(Pad); free(TStr);
}

////////////////////////////////////////////////////////////
unsigned int Xtime(unsigned int poly){
   poly <<= 1;
   return((poly & 0x100) ? (poly ^ 0x11b):(poly));
   //check if the polynomial <8
}

/////////////////////////////////////////////////////////////
unsigned int PolyMult(unsigned int x, unsigned int y){
   unsigned int bit;
   int  result = 0;
   for (bit  = 1; bit < 256; bit *= 2, y = Xtime(y)){
      if (x & bit)
         result ^= y;
   }
   return (result);
}

/****************************************************************************
Decryption steps
(a) InvShifRows : Cyclically shifts the last 3 rows in the State
(b) InvSubBytes
(c) InvMixColumns
****************************************************************************/
void InvShiftRows(char *SArr[4][4]){
   int c;
   char *temp,*temp1;

   temp = (char *) malloc (sizeof(char));
   temp1 = (char *) malloc (sizeof(char));
   strcpy(temp,""); strcpy(temp1,"");
   //shift(1,4) = 1
   strcpy(temp,SArr[1][3]);
   for (c = 3; c  >= 1; c-- )
      strcpy(SArr[1][c],SArr[1][c - 1]);
   strcpy(SArr[1][0],temp);

   //shift(2,4) = 2
   strcpy(temp,SArr[2][2]);
   strcpy(temp1,SArr[2][3]);
   for (c = 2; c <= 3; c++ )
      strcpy(SArr[2][c],SArr[2][c-2]);
   strcpy(SArr[2][0],temp);
   strcpy(SArr[2][1],temp1);

   //shift(3,4) = 3
   strcpy(temp,SArr[3][0]);
   for (c = 1; c <= 3; c++ )
      strcpy(SArr[3][c - 1],SArr[3][c]);
   strcpy(SArr[3][3],temp);
   PrintBox(SArr , "\n\nInverse Shift Rows");
   //free(temp); free(temp1);
}

static unsigned int SBoxInv(int x,  int y){
     register unsigned int SBoxRtnInv[16][16] = {
      {0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb},
      {0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb},
      {0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e},
      {0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25},
      {0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92},
      {0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84},
      {0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06},
      {0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b},
      {0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73},
      {0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e},
      {0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b},
      {0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4},
      {0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f},
      {0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef},
      {0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61},
      {0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d}
   };
   return (SBoxRtnInv[x][y]);
}

//////////////////////////////////////////////////////////////////////////////////
void InvMixColumns(char *SArr[4][4]){
   int r,c;                    //r = row and c= column
   unsigned int A[4][4];       //A = Hex Array
   char *TStr; char *Pad;
   TStr = (char *) malloc (sizeof(char)); //Temporary Str for re-assigning
   Pad = (char *) malloc (sizeof(char));  //Pad =Zero Pad Str

   for (r = 0; r < 4; r++)
      for (c = 0; c < 4; c++)
         A[c][r] = PolyMult(0x0e,string_to_dec(SArr[c][r]))
                   ^ PolyMult(0x0b,string_to_dec(SArr[(c+1)% 4][r]))
                   ^ PolyMult(0x0d,string_to_dec(SArr[(c+2)% 4][r]))
                   ^ PolyMult(0x09,string_to_dec(SArr[(c+3)% 4][r]));

   for (r = 0; r < 4; r++)    //convert the hexadecimal Array to a string Array
      for (c = 0; c < 4; c++){
         strcpy(TStr,"");
         itoa(A[r][c],TStr,16);
         if (strlen(TStr) < 2){
            Pad = Padding(0,1);
            strcat(Pad,TStr);
            strcpy(SArr[r][c],Pad);
         }
         else strcpy(SArr[r][c],TStr);
       }
     PrintBox(SArr , "\n\nInverse Mix Columns");
  // free(Pad); free(TStr);
}

///////////////////////////////////////////////////////////////////////////////
void InvSubBytes(char *SArr[4][4]){
   int r,c;
   char xlook[2];char ylook[2]; char hexStr[2]; char *Pad;
   int intxlook,intylook;
   char *temp;
   int subbyte;
   Pad = (char *)malloc (sizeof(char));
   temp = (char *)malloc (sizeof(char));

   for (r = 0; r < 4; r++)
      for(c = 0; c < 4; c++){
            strcpy(temp,"");
            strcpy(xlook,"");strcpy(ylook,"");
            strcpy(temp,SArr[r][c]);
            xlook[0] = temp[0]; xlook[1] = '\0';
            ylook[0] = temp[1];ylook[1] = '\0';
            intxlook = string_to_dec(xlook);
            intylook = string_to_dec(ylook);

         subbyte = SBoxInv(intxlook,intylook);
         itoa(subbyte,hexStr,16);

         if (strlen(hexStr) < 2){
            strcpy(Pad,"");
            Pad = Padding(0,1);
            strcat(Pad,hexStr);
            strcpy(SArr[r][c],Pad);
         }
         else
            strcpy(SArr[r][c],hexStr);
      }
      PrintBox(SArr , "\n\nInverse Sub Bytes");
//      free(temp); free(Pad);
}     //end of void

/////////////////////////////////////////////////////////////////////
void Load_Key(void)
{
  int z=0;  
  ifstream F(file_key);
  if (F.fail())
  {	 
	 SetColor(3); 
     cout<<"Oooops! File "<<file_key << " is NOT Existant...."<<endl<<"Aborting System"<<endl;
	 return ;
  }
    while (!F.eof())       
	{ 
	 key[z] = new char[3];
	 key[z][0] = F.get();
	 key[z][1] = F.get();
	 key[z][2] = '\0';	 
	 z++;	 
	 if (z > 15) break;
	} 
   F.close();
}

////////////////////////////////////////////////////////////
void PrintBox(char *S[4][4] , char *Str ){
	if (Show_Matrix == 0) return;
   printf("%s" , Str);
   for(int K = 0;K < 4; K++){
     printf("\n");
     for (int L = 0; L < 4; L++)
         printf("%s ",S[K][L]);
   }
}
////////////////////////////////////////////////////////////
void SetColor(int color_num)
{
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_num); 	
}


///////////////////////////////////////////////////////////
void Show_Expand_Key()
{
   KeyExpansion(key,w,Nk);			
   fstream of("expanded_key.txt",ios_base::out);
   if (!of) { cout<<"Ooops! Error in opening expanded_key.txt file"<<endl; exit(1); } 

   of<<"Expanded Key (M.Naeem MS081009)" <<endl<<endl;
   SetColor(12);
     for (int K= 0;K<44;K++)
	 {
		of<<w[K]<<endl;  cout<<w[K]<<endl;
		if (((K+1) % 4) == 0) {of<<"\n"<<endl;  cout<<endl;  }
	 }
	 of.close();    
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


///////////////////////////////////////////////////////////
void Read_File(char file_name[1024] , char *dest)
{
 char ch; 
 ifstream inf(file_name);
 inf.seekg(0,ios::end); // go to 0 bytes from end
 int pos = inf.tellg();
 //cout<<" TellG() : "<<pos<<endl;
 inf.seekg(0); // go to start of the file.
 for (int i=0; i<pos; i++)
 {
   inf.get(ch);	 
   *dest++ = ch;
 }
 inf.close();
/*
 int m = (i % 16);  
 while (m-- > 0)
 {
   *dest++ = 'z';   
 } */ 

}

//////////////////////////////////////////////////////////////
