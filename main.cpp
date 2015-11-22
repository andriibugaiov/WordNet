
#include "WordNet.h"

using namespace std;

int main() {
	string synsets = "synsets.txt";
	string hypernyms = "hypernyms.txt";
	
	try {
		WordNet wn(synsets, hypernyms);
		wn.UnitTests();
	} catch (const string &msg) {
		cout << msg << endl;
	}
	
	return 0;
}