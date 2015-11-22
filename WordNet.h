
#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <queue>

class WordNet {
	class Vertex {
	public:
		std::string gloss;
		std::list<int> nIds;
		std::list<int> vIds;
	
		Vertex(std::list<int> &aNIds, std::string &aGloss) {
			gloss = aGloss;
			nIds = aNIds;
		}
		void linkTo(int aVId) {
			vIds.push_back(aVId);
		}
	};
	
	class Noun {
	public:
		int nId;
		std::list<int> vIds;
		
		Noun() : nId(-1) {}
		Noun(int aNId) {
			nId = aNId;
		}
		void linkTo(int aVId) {
			vIds.push_back(aVId);
		}
	};
	
	std::vector<Vertex> digraph;
	std::vector<std::string> vNouns;
	std::unordered_map<std::string, Noun> mpNouns;
	
	// {distance, synset id}
	std::pair<int, int> bfs(const std::list<int> &vIdsA, const std::list<int> &vIdsB) {
		const int A = 0;
		const int B = 1;
		
		// {distance, noun id}
		std::vector<std::pair<int, int>> distTo(digraph.size(), {-1, 0});
		std::queue<int> q;
		for (int vId : vIdsA) {
			distTo[vId] = {0, A};
			q.push(vId);
		}
		for (int vId : vIdsB) {
			if (distTo[vId].first == 0 &&
				distTo[vId].second == A)
				return {0, vId};
			distTo[vId] = {0, B};
			q.push(vId);
		}
		
		while (!q.empty()) {
			int vId = q.front(); q.pop();
			Vertex &v = digraph[vId];
			for (int adjVId : v.vIds) {
				if (distTo[adjVId].first < 0) {
					distTo[adjVId] = {distTo[vId].first + 1, distTo[vId].second};
					q.push(adjVId);
				} else if (distTo[adjVId].second != distTo[vId].second) {
					int dist = distTo[adjVId].first + distTo[vId].first + 1;
					return {dist, adjVId};
				}
			}
		}
		return {-1, -1};
	}
public:
	WordNet(const std::string &synsets, const std::string &hypernyms) {
		std::ifstream inSynsets(synsets);
		std::ifstream inHypernyms(hypernyms);
		if (inSynsets.is_open() && inHypernyms.is_open()) {
			std::string line;
			
			std::list<int> vSynset;
			std::string synset;
			std::string noun;

			std::string gloss;
			while (std::getline(inSynsets, line)) {
				std::stringstream ssLine(line);
				
				int vId;
				ssLine >> vId;
				ssLine.ignore();
				
				std::getline(ssLine, synset, ',');
				std::stringstream ssSynset(synset);
				while (std::getline(ssSynset, noun, ' ')) {
					if (!mpNouns.count(noun)) {
						int nId = int(vNouns.size());
						vNouns.push_back(noun);
						mpNouns[noun] = Noun(nId);
					}
					Noun &n = mpNouns[noun];
					n.linkTo(vId);
					vSynset.push_back(n.nId);
				}
				
				std::getline(ssLine, gloss, '\n');
				
				digraph.push_back(Vertex(vSynset, gloss));
				vSynset.clear();
			}
			inSynsets.close();
			
			while (std::getline(inHypernyms, line)) {
				std::stringstream ssLine(line);
				
				int vId;
				ssLine >> vId;
				ssLine.ignore();
				Vertex &v = digraph[vId];
				
				while (ssLine >> vId) {
					if (ssLine.peek() == ',')
						ssLine.ignore();
					v.linkTo(vId);
				}
			}
			inHypernyms.close();
		} else {
			throw "Unable to open file.";
		}
	}

	const std::vector<std::string> &nouns() {
		return vNouns;
	}
	
	bool isNoun(const std::string &word) {
		return mpNouns.count(word);
	}
	
	int distance(std::string &nounA, std::string &nounB) {
		if (!isNoun(nounA) || !isNoun(nounB))
			return -1;
		const Noun &nA = mpNouns[nounA];
		const Noun &nB = mpNouns[nounB];
		return bfs(nA.vIds, nB.vIds).first;
	}
	
	std::string sap(std::string &nounA, std::string &nounB) {
		if (!isNoun(nounA) || !isNoun(nounB))
			return "";
		const Noun &nA = mpNouns[nounA];
		const Noun &nB = mpNouns[nounB];
		int vId = bfs(nA.vIds, nB.vIds).second;
		Vertex &v = digraph[vId];
		
		std::string synset;
		for (auto &nId : v.nIds)
			synset += vNouns[nId] + " ";
		return synset;
	}
	
// mark -
	
	void UnitTests() {
		std::cout << "Number of nouns : " << nouns().size() << std::endl;
		std::cout << std::endl;
		
		std::cout << "Number of vertecies : " << digraph.size() << std::endl;
		int numEdges = 0;
		for (Vertex &v : digraph)
			numEdges += v.vIds.size();
		std::cout << "Number of edges : " << numEdges << std::endl;
		std::cout << std::endl;
		
		std::vector<std::string> words = {"worm", "bird", "tree"};
		for (auto w : words) {
			if (isNoun(w)) {
				Noun &n = mpNouns[w];
				std::cout << "Synsets that contain " << w << " : " << std::endl;
				for (int vId : n.vIds)
					std::cout << vId << " " << std::endl;;
				std::cout << std::endl;
			} else
				std::cout << w << " is not a noun." << std::endl;
		}
		
		std::vector<std::pair<std::string, std::string>> wordPairs = {
			{"municipality", "region"},
			{"white_marlin", "mileage"},
			{"Black_Plague", "black_marlin"},
			{"American_water_spaniel", "histology"},
			{"Brown_Swiss", "barrel_roll"},
			{"worm", "bird"}
		};
		for (auto wp : wordPairs) {
			std::cout << "The SAP for " << wp.first << " and " << wp.second << " : ";
//			std::cout << distance(wp.first, wp.second) << std::endl;
			std::cout << std::endl << sap(wp.first, wp.second) << std::endl;
		}
	}
};
