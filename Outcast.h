
#pragma once

#include <string>
#include <vector>

#include "WordNet.h"

class Outcast {
public:
	std::string outcast(WordNet &wn, std::vector<std::string> &nouns) {
		std::vector<int> dists(nouns.size(), 0);
		for (int i = 0; i < nouns.size(); ++i) {
			for (int j = i + 1; j < nouns.size(); ++j) {
				int curDist = wn.distance(nouns[i], nouns[j]);
				dists[i] += curDist;
				dists[j] += curDist;
			}
		}
		int nIdx = 0;
		int maxDist = 0;
		for (int i = 0; i < dists.size(); ++i) {
			int curDist = dists[i];
			if (curDist > maxDist) {
				maxDist = curDist;
				nIdx = i;
			}
		}
		return nouns[nIdx];
	}
};