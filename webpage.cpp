

#include <iostream>
#include <string.h>
#include <assert.h>
#include "webpage.h"

using namespace std;

/* Return a hashT for the string s in the range 0..table_size-1 */
int hashT(string s, int table_size)
{
  unsigned int i, h = 0;
  for (i=0; i<s.length(); i++)
    h = (h * 2917 + (unsigned int)s[i]) % table_size;
  return h;
}

/* Allocate a table of pointers to Page structs */
Page **allocate_table(int size)
{
  Page **table = new Page *[size];
  for (int i=0; i<size; i++)
    table[i] = NULL;
  return table;
}

webPage::webPage(int tablesize)
{  
  size=tablesize;
  table = allocate_table(size);
  num_elements=0;
}

webPage::~webPage()
{
  for (int i=0; i<size; i++) {
    while (table[i] != NULL) {
      Page *temp = table[i];
      table[i] = table[i]->next;
      delete temp;
    }
  }
  delete[] table;
}
int webPage::find(string key)
{
  int h = hashT(key, size);
  Page *n = table[h];
  while (n != NULL) {
    if (n->name == key) return(h);
    h++;
    if(h==size){
	h=0;
    }
    n = table[h];
  }
  return -1;
}

// checks how full the hashT table is, allocating a new table and rehashTing if necessary
// (only used for the words hashT table)
void webPage::checksize()
{
		num_elements++;
		if (num_elements == 0.5*size) {
    		Page **newtable = allocate_table(size*2);
		int i;	
		Page *newwrd;
		for(i=0;i<size;i++)
        	{
            		if(table[i]!=NULL)
                        {
                                int h = hashT(table[i]->name,size*2);
				while(newtable[h]!=NULL)
  				{
					h++;
					if(h==size*2)
					{
						h=0;
					}
				}
				newwrd = new Page(table[i]->name,NULL);
  				newtable[h]=newwrd;				
				table[i]=table[i]->next;
				while(table[i]!=NULL)
				{
					Page *newpage = new Page(table[i]->name,NULL);
					newwrd->next=newpage;
					newwrd=newpage;
					table[i]=table[i]->next;
				}
                        }         
		}
		for (int i=0; i<size; i++) {
    				while (table[i] != NULL) {
      					Page *deleter = table[i];
      					table[i] = table[i]->next;
      					delete deleter;
    				}
			}
  		delete[] table;
  		size=size*2;
  		table=newtable;
		}
}
