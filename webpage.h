

#include <string>
using namespace std;

int hashT(string s, int table_size);

struct Page {
  string name;
  Page *next;
  double weight; // this field is only used for main pages
  double new_weight; // this field is only used for main pages
  int linkedpages; // this field is only used for main pages
  int identifier;
  int index; // this field is only used for hyperlinks, storing the index of the corresponding page in the table (-1 if not in table)
  Page(string k, Page *n) { name = k; next = n; weight=0; new_weight=0; linkedpages=0; identifier=0; index=0;}
  Page(string k, Page *n, int m) { name = k; next = n; weight=1.0/m; new_weight=0; linkedpages=0; identifier=0; index=0;}
  Page() { name = ""; next = NULL; weight=0; new_weight=0; linkedpages=0; identifier=0; index=0;}
};

class webPage
{
    public:
	Page **table;
	int size;
	int num_elements;
	webPage(int tablesize);
	~webPage();
	int find(string key);
	void checksize();
};
