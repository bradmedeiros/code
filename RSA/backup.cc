// Implementation of the RSA public-key encryption algorithm
// ECE4893/ECE8893, Fall 2012

#include "gmpxx.h"
#include <inttypes.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
//#include <string>
#include <ctime>
#include <cstdlib>
#include <stdio.h>
#define DEBUG

using namespace std;


//void RSA(int numbits,privateKey priv, publicKey pub, gmp_randclass& random);
//void getRandom(mpz_t& num, int numbits, gmp_randclass& random);

class privateKey{
public:
	mpz_t n;
	mpz_t e;
         
	int sz; 
	privateKey(){
		mpz_init(n); mpz_init(e);
	}
	void encrypt(mpz_t& M, mpz_t& C){
	   mpz_powm(C,M,e,n);
	}

	~privateKey(){
		mpz_clear(n); mpz_clear(e);
	}
};

class  publicKey{
public:
	mpz_t n;
	mpz_t d;
        int sz;
  	publicKey(){
	     mpz_init(n);
      	     mpz_init(d);	
 	}
	void encrypt(mpz_t& M, mpz_t& C){
	   mpz_powm(C,M,d,n);	
	}

      ~publicKey(){
		mpz_clear(n); 
			mpz_clear(d);
	}
 	
	bool operator==(const publicKey& rhs){
	      return ((mpz_cmp(n,rhs.n) == 0) && (mpz_cmp(d,rhs.d)==0));


        }

        bool operator!=(const publicKey& rhs){
	      return (!((*this) == rhs));
        } 
}; 

void RSA(int numbits, publicKey& pub, privateKey& priv, gmp_randclass& random,bool print);
void getRandom(mpz_t& num, int numbits, gmp_randclass& random);

int main()
{
  
   int sz = 32; 
   privateKey key;
   mpz_t M,C, dec;
   mpz_init(M);
   mpz_init(C);
   mpz_init(dec);
   gmp_randclass random(gmp_randinit_default);
   
   random.seed(time(0));
   privateKey priv;
   publicKey pub;
  
   getRandom(M, 16, random);

   int count; 
   while(sz<= 1024){
	count = 0;
 
	for (int i=0;i<10;i++){
	    if (i==9){
		 RSA(sz,pub,priv,random,true); // COMPUTE 10 DIFFERENT KEYS
	    }else{
		RSA(sz,pub,priv,random,false);
	    }
  
	    for (int k=0;k<10;k++){
		getRandom(M,sz/2,random);
		pub.encrypt(M,C);
		priv.encrypt(C,dec);
		if (mpz_cmp(M,dec)!=0){
		   cout<<"ENCRYPTION FAILURE"<<endl;
		   return 1;
		}
  
	    }
	   
	}

	cout << "mpz_class M" << sz << "(\"" << M << "\");" << endl;
	pub.encrypt(M,C);
	cout << "mpz_class C" << sz << "(\"" << C << "\");" << endl;
	priv.encrypt(C,dec);
  	sz*=2;

   }
     
   return 0;
}

void getRandom(mpz_t& num, int numbits, gmp_randclass& random){
     
     mpz_class num_class(0);
     num_class = random.get_z_bits(numbits);
     mpz_t temp;
     mpz_init(temp);
     mpz_add(num, num_class.get_mpz_t(), temp); 
     mpz_clear(temp);
}

void RSA(int numbits,publicKey& pub, privateKey& priv, gmp_randclass& random,bool print){
    mpz_t zero,phi,p,q,n,d,e,C,temp1,temp2;   //declare variables

    pub.sz = numbits;
    priv.sz = numbits;
    
    mpz_init(p);  // initialzie all the variables
    mpz_init(q);
    mpz_init(n);
    mpz_init(d);
    mpz_init(e);
    mpz_init(zero);
   
    mpz_init(phi); 
    mpz_init(temp1);
    mpz_init(temp2);
   
    while(true){
	getRandom(p,numbits,random);
        if (mpz_probab_prime_p(p,10)){
	    break;
        }

    }
    while(true){
	getRandom(q,numbits,random);
        if(mpz_probab_prime_p(q,10)){
		if (mpz_cmp(p,q) != 0){ // if the number are unique
			break;
		}
        }
    }
    

   
   

    mpz_sub_ui(temp1, q,1);
    mpz_sub_ui(temp2, p,1);
    mpz_mul(phi,temp1,temp2);
    mpz_mul(n,p,q);
  

   while(true){
	getRandom(d, numbits*2,random);
        if (mpz_cmp(d,phi) < 0){ //go until d is smaller than phi
		mpz_gcd(temp1,d,phi);
		if (mpz_cmp_d(temp1,1)==0){
			break;
		}
        }

   }

   mpz_invert(e, d,phi);

   mpz_add(pub.n,zero,n);
   mpz_add(pub.d,zero,d);
   mpz_add(priv.n,zero,n);
   mpz_add(priv.e,zero,e);
   
int sz = numbits;
if(print){

cout << "mpz_class p" << sz << "(\"" << p << "\");" << endl;

cout << "mpz_class q" << sz << "(\"" << q << "\");" << endl;

cout << "mpz_class n" << sz << "(\"" << n << "\");" << endl;

cout << "mpz_class d" << sz << "(\"" << d << "\");" << endl;

cout << "mpz_class e" << sz << "(\"" << e << "\");" << endl;

//cout << "mpz_class M" << sz << "(\"" << M << "\");" << endl;

//cout << "mpz_class C" << sz << "(\"" << C << "\");" << endl;

}
  mpz_clear(zero);
  mpz_clear(p); mpz_clear(q); mpz_clear(n); mpz_clear(d);
  mpz_clear(e); mpz_clear(phi); mpz_clear(temp1); mpz_clear(temp2);
}

