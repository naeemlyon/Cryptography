# include <iostream.h>
# include <string.h>
# include <stdio.h>
/////////////////////////////////////////
typedef unsigned char UCHAR;

inline UCHAR get_random_number(void);
void rc4_key_schedule(const char*, int ); 

UCHAR S[256], i, j, t; //  counters                                               

/////////////////////////////////////////////////////////
//  *** RC4 primary focus is on XORING MAGIC      ***////
//  M.Naeem MS081009 ////////////////////////////////////
/////////////////////////////////////////////////////////

void main(int argc, char **argv) 
{
  cout<<endl<<"====================================================="<<endl;
  cout << "============  Rons Code 4 cryptography  ============" << endl;
  cout<<endl<<"=============      Muhammad Naeem   ============="<<endl;
  cout<<endl<<"====================================================="<<endl<<endl;

   FILE *in, *out;
   int counter;

 	if( argc != 4 ) {
		cout << "USAGE: RC4 key source-file dstination-file" << endl;
      return;
   }
	if( strlen(argv[1]) == 0 ) {
   	cout << "OOps: Key length should be 1 Byte long at least" << endl;
      return;
   }

	rc4_key_schedule(argv[1],strlen(argv[1]));	// Initializes key achedule through Substitution Box, i and j

   in = fopen(argv[2],"rb");
   if( in == NULL ) {
 		cout << "OOps! Source file non-existing or corrupt file <" << argv[2] << ">.\n";
      return;
   }
   out = fopen(argv[3],"wb");
   if( out == NULL ) {
   	fclose(in);
 		cout << "OOps!: Failed in opening dstinaton file <" << argv[3] << ">.\n";
      return;
   }

   while( (counter=fgetc(in)) != EOF ) 
	   fputc( ((UCHAR)counter) ^ get_random_number() ,out);
   fclose(in); fclose(out);
   cout << "RC4 applied successfully..." << endl;   
}

/////////////////////////////////////////////////////////////////
inline UCHAR get_random_number() 
{
        ++i;
        j += S[i];
        t = S[i]; S[i] = S[j]; S[j] = t;
        t = S[i] + S[j];
        return S[t];
}
/////////////////////////////////////////////////////////////////
void rc4_key_schedule(const char *key, int keylen) 
{
  int keypos = 0, x;
  UCHAR K[256];

  for( x=0; x<256; ++x ) {
  	S[x] = x;
    K[x] = key[keypos++];
    if( keypos >= keylen ) keypos=0;
   }

   for( j=x=0; x<256; ++x ) {
   	j += S[x] + K[x];
      t = S[x];
      S[x] = S[j];
      S[j] = t;
   }

   i = j = 0;
}
///////////////////////////////////////////////////////////////
