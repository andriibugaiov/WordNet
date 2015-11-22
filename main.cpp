
#include "WordNet.h"
#include "Outcast.h"

#include <vector>
#include <string>

using namespace std;

int main() {
	string synsets = "synsets.txt";
	string hypernyms = "hypernyms.txt";
	
	try {
		WordNet wn(synsets, hypernyms);
		wn.UnitTests();
		
		Outcast oc;
		vector<vector<string>> nouns = {
			{"horse", "zebra", "cat", "bear", "table"},
			{"water" "soda", "bed", "orange_juice", "milk apple_juice", "tea", "coffee"},
			{"apple", "pear", "peach", "banana", "lime", "lemon", "blueberry", "strawberry", "mango", "watermelon", "potato"}
		};
		
		cout << endl;
		for (auto &ns : nouns) {
			cout << "Outcast for nouns : ";
			for (auto &n : ns)
				cout << n << " ";
			cout << endl;
			cout << oc.outcast(wn, ns) << endl;
		}
	} catch (const string &msg) {
		cout << msg << endl;
	}
	
	return 0;
}