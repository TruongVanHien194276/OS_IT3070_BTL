#ifndef _PAGE_TABLE_
#define _PAGE_TABLE_

#include<iostream>
#include<vector>
#include<iomanip>
using namespace std;

class PageTable
{
 long long int ts;              
 long long int pgFaults;         
 long long int pgReplace(int choice);     

 class PageTableEntry
 {
   public:
     long long int timeStamp;    
     long long int frameNo;      
     int checkFrame;                  
     PageTableEntry()
     {
       timeStamp=0;
       frameNo=-1;
       checkFrame=0;
     }
 };

 vector<PageTableEntry> pgMap;   
 vector<long long int> emptyFrames;   

 public:
   PageTable(long long int log_memSize,long long int phy_memSize);
   bool containsRef(long long int pgNum);
   void accessPg(long long int pgNum,int choice);
   void addPg(long long int pgNum,int choice);
   void displayPageTable();
   long long int getPageFaults();
   void setPageFaults(int pf);
   vector<long long int> fifo_framePgs;
   vector<pair<long long int,long long int> > lfu_framePgs;   


};


PageTable::PageTable(long long int log_memSize,long long int phy_memSize)
{
 emptyFrames.clear();
 pgMap.clear();
 fifo_framePgs.clear();
 for(long long int i=phy_memSize-1;i>=0;i--)
 {
    emptyFrames.push_back(i);
 }
 pgMap.resize(log_memSize);
 ts=1;                             
}


bool PageTable::containsRef(long long int pgNum)
{
 return pgMap[pgNum].checkFrame;
}

void PageTable::accessPg(long long int pgNum,int choice)
{
 long long int frame=pgMap[pgNum].frameNo;

 if(choice==3)      
 {
   for(long long int i=0;i<lfu_framePgs.size();i++)      
   {
     if(lfu_framePgs[i].first==pgNum)
     {
       long long int freq=lfu_framePgs[i].second;
       lfu_framePgs[i].second=freq+1;
     }
   }
 }

 pgMap[pgNum].timeStamp=ts;
 displayPageTable();
 ++ts;
 cout<<"The page "<<pgNum<<" was found at frame "<<frame<<" in physical memory\n\n";
}


void PageTable::addPg(long long int pgNum,int choice)
{

 if(!emptyFrames.empty())
 {
   long long int frame=emptyFrames.back();
   emptyFrames.pop_back();
   pgMap[pgNum].frameNo=frame;
   pgMap[pgNum].checkFrame=1;
   pgMap[pgNum].timeStamp=ts;
   //cout<<"time "<<ts<<"\n";
   cout<<"Page Fault: Adding "<<pgNum<<" at frame "<<frame<<"\n";
 }

 else
 {
   long long int frame=pgReplace(choice);
   pgMap[pgNum].frameNo=frame;
   pgMap[pgNum].checkFrame=1;
   pgMap[pgNum].timeStamp=ts;
   //cout<<"time "<<ts<<"\n";
   cout<<"Page Fault: Adding "<<pgNum<<" at frame "<<frame<<"\n";
 }
 ++pgFaults;
 fifo_framePgs.push_back(pgNum);
 lfu_framePgs.push_back(make_pair(pgNum,1));

}

long long int PageTable::pgReplace(int choice)
{

 if(choice==1)         //Least Recently Used
 {
   long long int min=ts;
   long long int replacePg;
   for(long long int j=0;j<pgMap.size();j++)
   {
     if(pgMap[j].checkFrame==1)    
     {
       if(pgMap[j].timeStamp<min)   
       {
         min=pgMap[j].timeStamp;   
         replacePg=j;             
       }
     }
   }
   long long int frame=pgMap[replacePg].frameNo;
   pgMap[replacePg].checkFrame=0;
   pgMap[replacePg].frameNo=-1;
   return frame;                  
 }

 else if(choice==2)    //First in First Out
 {
   long long int replacePg=fifo_framePgs.at(0);       
   fifo_framePgs.erase(fifo_framePgs.begin()+0);            
   pgMap[replacePg].checkFrame=0;
   long long int frame=pgMap[replacePg].frameNo;
   pgMap[replacePg].frameNo=-1;
   return frame;                  
 }


 else if(choice==3)    //Least frequently used
 {
   long long int min=0;
   for(long long int i=0;i<lfu_framePgs.size();i++)    
   {
     if(lfu_framePgs[i].second<lfu_framePgs[min].second)
       min=i;
   }
   long long int replacePg=lfu_framePgs[min].first;
   lfu_framePgs.erase(lfu_framePgs.begin()+min);
   pgMap[replacePg].checkFrame=0;
   long long int frame=pgMap[replacePg].frameNo;
   pgMap[replacePg].frameNo=-1;
   return frame;                  
 }
 return 1;
}

//display the contents of the page table
void PageTable::displayPageTable()
{
 // printing top border
 cout << left << setfill('-') << setw(1) << "+" << setw(4) << "-" << setw(1) << "+" << setw(15) << "-" << setw(1) << "+" << setw(15) << "-" << setw(1) << "+" << setw(9) << setfill('-') << "-" << right << "+" << setfill(' ')<< setw(21) << "+" << setfill('-') << setw(15) << "-" << endl;
 // printing table record
 cout << right << setw(1) << "|" << "ADR" << setfill(' ') << setw(2) << "|" << left << setw(15) << "Frame no." << setw(1) << "|" << setw(15)  << "Valid" << setw(1) << "|" << setw(5) << "TimeStamp" << setw(1) << "|" << setw(20) << setfill(' ') << " " << "|" << "Empty Frames" << setw(4) << right << "|" << endl;
 // printing bottom border
 cout << left << setfill('-') << setw(4) << "+" << setw(17) << "-" << setw(1) << "+" << setw(15) << "-" << setw(1) << "+" << setw(9) << "-" << setw(1) << "+" << setw(20) << setfill(' ') << " " << setw(1) << setfill(' ') << "+" << setfill('-') << setw(15) << "-" << endl;
 for (unsigned int i = 0; i < pgMap.size(); i++)
 {
   // printing table record
   cout << "|" << setfill(' ') << setw(3) << i << setfill(' ') << setw(1) << " " << "|" << setw(15) << pgMap.at(i).frameNo
     << setw(1) << "|" << setw(15) << pgMap.at(i).checkFrame << setw(1) << "|" << setw(5)
     << pgMap.at(i).timeStamp << setfill(' ') << setw(4) << " " << setw(1) << "|";
   // conditionally print the empty frames
   if (i < emptyFrames.size()) {
     cout << left << setw(20) << setfill(' ') << " " << "|" << setw(5) << emptyFrames.at(i) << setfill(' ') << setw(9) << " " << setw(1) << "|" <<endl;
     //printing bottom border
     cout << setfill('-') << setw(4) << "+" << setw(17) << "-" << setw(1) << "+" << setw(15) << "-" << setw(1) << "+" << setw(9) << "-" << setw(1) << "+" << setw(20) << setfill(' ') << " " << setw(1) << setfill(' ') << "+" << setfill('-') << setw(15) << "-" << endl;
   }
   // if there wasn't a frame printed don't forget to print the bottom border
   else {
     // printing bottom border
     cout << endl << setfill('-') << setw(4) << "+" << setw(17) << "-" << setw(1) << "+" << setw(15) << "-" << setw(1) << "+" << setw(9) << "-" << setw(1) << "+" << endl;
   }
 }

}

void PageTable::setPageFaults(int pf)
{
 pgFaults=pf;
}

long long int PageTable::getPageFaults()
{
 return pgFaults;
}

#endif
