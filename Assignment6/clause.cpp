#include <iostream>
#include <bits/stdc++.h>
using namespace std;
class Clause
{
public:
  int len;
  int * hgs;
  int base;
  int neck;
  int * xs;

  Clause( int len, int hgs[], int base, int neck,int xs[])
  {
    this->hgs=hgs; // head+goals pointing to cells in cs
    this->base=base; // heap where this starts
    this->len=len; // length of heap slice
    this->neck=neck; // first after the end of the head
    this->xs= xs; // indexables in head
    cout<<"Length :";
    cout<<len;
    cout<<"\n";
    cout<<"Base ";
    cout<<base;
    cout<<"\n";
    cout<<"Index xs : \n";
    for(int i=0;i<3;i++)
{
    cout << hgs[i];
    cout<<" ";

}
  cout<<"\n";
  cout<<"Neck : ";
  cout<<neck;
   cout<<"\n";
    cout<<"Heap : \n";
    for(int j=0;j<3;j++)
{
    cout << xs[j];
    cout<<" ";

}

  }
};


int * randomsdq(int seed){


        static int data[3];int random;

        for (int i=100,j=0;j<3;i=i*seed,j++){
            random=i*seed+seed*(i-seed);
            data[j]=random;
        }
        return data;

}
int main()
{
  int *str1;int *str3;
 int dad[3];
  int seed=24;
  str1=randomsdq(23);
  for (int h=0;h<3;h++){
      dad[h]=str1[h];
  }
  str3=randomsdq(24);
Clause b(seed+10,dad,seed+12,seed+35,str3);
  return 0;
}
