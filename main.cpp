//Homework 1
//Name: Parth Patel
//CPSC 2120
//09/21/2015

#include <cstring>
#include <fstream>
#include <iostream>
#include "webpage.h"

// I used the same Page struct (in webpage.h) for main pages, links, and words.
// I used the identifier field to differentiate them. Identifer = 0 means
// it's a main page, identifier = 1 means it's a hyperlink, and identifier=
// 2 means it's a word. Each cell in the first hash table created below
// holds a main page and points to a linked list of all the words and hyperlinks
// on that page. Each cell in the second hash table created below
// holds a word and points to a linked list of all the pages the word appears on.

using namespace std;
int main()
{
	cout<<"Just a second..."<<endl;	
	ifstream fin;
	string s;
	int count=0;
	int counter, i, index;	
	Page *temp=NULL;
	Page *foundpage=NULL;
	Page *newPage=NULL;
	Page *tracker=NULL;
	fin.open("webpages.txt");

	// count the number of pages in the file
	while (fin >> s) {
		if(s=="NEWPAGE")
		{
			count++;
		}
	}
	fin.close();

	// allocate a hash table twice the size of the number of pages found
	webPage Pages(count*2);

	// Load the pages into the hash table and build each page's linked list
	fin.open("webpages.txt");
	Page *wordorlink=NULL;
	cout<<"Building page hash table..."<<endl;
	while (fin >> s) 
	{
		if(s=="NEWPAGE")
		{
			fin >> s;
			index=hashT(s,Pages.size);
			newPage=new Page(s,NULL,count);
			while(Pages.table[index]!=NULL)
			{
				index++;
				if(index==Pages.size)
				{
					index=0;
				}
			}
			Pages.table[index]=newPage;
			temp=Pages.table[index];
		}
		else
		{
			int found=s.find("http://");
			if(found != string::npos)
			{
				wordorlink=new Page(s,NULL);
				temp->next=wordorlink;
               	         	temp=wordorlink;
				wordorlink->identifier=1;
			}
			else
			{
				wordorlink=new Page(s,NULL);
				temp->next=wordorlink;
				wordorlink->identifier=2;
	                        temp=wordorlink;
			}
		}
	}
	fin.close();

	// Set the linkedpages field of each page, which is how many hyperlinks are
	// on that page, only counting hyperlinks that point to pages with corresponding
	// NEWPAGE records for them
	for(i=0;i<Pages.size;i++)
		{
			if(Pages.table[i]!=NULL)
			{
			tracker=Pages.table[i]->next;
			while(tracker!=NULL)
			{
				if(tracker->identifier==1)
				{
					tracker->index = Pages.find(tracker->name);
					if(tracker->index!=-1)
					{
						Pages.table[i]->linkedpages++;
					}
				}
			tracker=tracker->next;
			}
		}
	}

	// Run Pagerank algorithm
	cout<<"Running Pagerank algorithm..."<<endl;
	for(counter=0;counter<50;counter++)
	{ 
		for(i=0;i<Pages.size;i++)
        	{
        		if(Pages.table[i]!=NULL)                                                
               		{	
				Pages.table[i]->new_weight=0.1/count;
			}
		}
		for(i=0;i<Pages.size;i++)
		{
			if(Pages.table[i]!=NULL)
			{
				tracker=Pages.table[i]->next;
				while(tracker!=NULL)
				{
					if(tracker->identifier==1)
					{
						if(tracker->index!=-1)
						{
							foundpage=Pages.table[tracker->index];
							foundpage->new_weight=foundpage->new_weight+0.9*
							Pages.table[i]->weight/Pages.table[i]->linkedpages;
						}
					}
				tracker=tracker->next;
				}
			}		
		}
		for(i=0;i<Pages.size;i++)
        	{
        		if(Pages.table[i]!=NULL)                                                
               		{
				Pages.table[i]->weight=Pages.table[i]->new_weight;
			}
		}
	}

	// allocate and build hash table of the words pointing to pages

	// none of the fields except name and next matter in this table
	// since we are not running a Pagerank algorithm with this table
	string mainpage;
	webPage Words(1000);
	fin.open("webpages.txt");
	cout<<"Building word hash table..."<<endl;
        while (fin >> s) {
                if(s=="NEWPAGE")
                {
			fin >> s;
			mainpage=s;
		}
		else{
			int found=s.find("http://"); //hyperlinks are ignored
                	if(found != string::npos)
                	{
				continue;
			}
			else
			{
				Words.checksize(); // if hash table gets half full, a new hash table twice the size is created
						   // and everything is rehashed into the new table
				if(Words.find(s)==-1){
					Page *newWord=new Page(s,NULL);
					int wordindex=hashT(s,Words.size);
                			while(Words.table[wordindex]!=NULL)
                			{
                       				wordindex++;
                        			if(wordindex==Words.size)
                        			{
                                			wordindex=0;
                        			}
                			}
                			Words.table[wordindex]=newWord;
                			Words.table[wordindex]->next= new Page(mainpage,NULL);
				}
        			else
        			{
                			Page *alreadyintable = Words.table[Words.find(s)];
                			Page *pushback = alreadyintable->next;
                			alreadyintable->next= new Page(mainpage,NULL);
					alreadyintable->next->next=pushback;
        			}
			}
		}
	}
	fin.close();

	// Start reading in search terms
	string term;
	int numpages;
	cout << "Enter the word you want to search (Enter 'Iwanttostop' to end): ";
	cin >> term;
	while(term!="Iwanttostop"){
		numpages=0;
		if(Words.find(term)==-1)
		{
			cout<<"Word not found"<<endl;
		}
		else{
		Page *foundterm = Words.table[Words.find(term)];
		foundterm=foundterm->next;
		while(foundterm!=NULL)
		{
			cout << int (Pages.table[Pages.find(foundterm->name)]->weight *7100000)
			<< " " << foundterm->name << endl;
			foundterm=foundterm->next;
			numpages++;
		}
		}
		cout<<"Pages found: "<<numpages<<endl;
		cout << "Enter the word you want to search (Enter 'Iwanttostop' to end): ";
		cin >> term;
	}
	return 0;
}
