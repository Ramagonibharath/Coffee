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
  
  Clause( int len, int *hgs, int base, int neck,int *xs)
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
int main()
{
int str1[]={1,2,4};
int str3[]={6,7,9};
int x;
Clause b(10,&str1[0],7,3,&str3[0]);
 
  cin>>x;
  return 0;
}

