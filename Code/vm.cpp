#include<iostream>
#include "pagetable.h"
using namespace std;

bool checkNo(long long int);

int main()
{
  long long int log_memSize, phy_memSize,pgNum,n;
  int choice;

  cout<<"Enter the number of pages in logical memory space \n";
  cin>>log_memSize;
  if(!checkNo(log_memSize))
  {
    cout<<"Error: the number of pages entered is invalid\n";
    exit(1);
  }

  cout<<"Enter the number of frames in physical memory space \n";
  cin>>phy_memSize;
  if(!checkNo(phy_memSize))
  {
    cout<<"Error: the number of frames entered is invalid\n";
    exit(1);
  }

  do {

    PageTable pgTable(log_memSize,phy_memSize);
    cout<<"Choose a page replacement algorithm for paging simulation (1 - 5):\n";
    cout<<"1. Least Recently Used\n";
    cout<<"2. First in First Out\n";
    cout<<"3. Least frequently used\n";
    cout<<"4. Exit\n";
    cin>>choice;
    if(choice==4)
      break;
    pgTable.setPageFaults(0);

    cout<<"Enter the number of pages to be refered\n";
    cin>>n;
    long long int pgArr[n];
    cout<<"Enter all the page numbers to be refered each should belong to this range (0 - "<<log_memSize-1<<"):\n";
    for(long long int i=0;i<n;i++)
    {
          cin>>pgArr[i];
          pgNum=pgArr[i];
          if(pgNum<0 || pgNum>log_memSize-1)       
          {
              cout<<"Error! Enter the page number in range (0 - "<<log_memSize-1<<")\n";
              i=i-1;
          }
          else
          {
              if(pgTable.containsRef(pgNum))       
              {
                pgTable.accessPg(pgNum,choice);           
              }
              else                                 
              {
                pgTable.addPg(pgNum,choice);       
                pgTable.accessPg(pgNum,choice);           
              }
          }

      } 

  cout<<"The page faults occured are : "<<pgTable.getPageFaults()<<"\n";

}while(choice!=4);

  return 0;
}


bool checkNo(long long int size)
{
  if(size<0)
    return false;
    else
      return true;
}
