#pragma once
#include <string>
#include <vector>
#include <fstream>
class Toks
{
		// reserved words - with syntactic function
public:
		static std::string IF;
		static std::string AND;
		static std::string DOT;
		static std::string HOLDS;
		static std::string LISTS; // todo
		static std::string IS; // todo

		std::ifstream R;
		 std::string ss;

		static Toks * makeToks(const std::string s, const bool fromFile) {
			Toks * newToks = new Toks();
			if (fromFile) {
				newToks->R.open(s);
				if (!newToks->R.is_open()) {
					printf("Error opening file\n");
					return NULL;
				}
			}
			else
				newToks->ss = s;
			return newToks;
		}

		Toks() {
			ss = "";
		}

		std::string getWord() {
			std::string t = "";
			char c;
			R.get(c);
			while ((c != '_' && !(c >= 'A' && c <= 'Z') && !(c >= 'a' && c <= 'z') && !(c >= '0' && c <= '9')) && !R.eof()) {
				R.get(c);
			}
			std::string sval = "";
			while ((c == '_' || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) && !R.eof()) {
				sval = sval + c;
				R.get(c);
			}
			//R.seekg(0, std::ios::cur);
			if (sval.compare("") == 0)
				return t;
			const char first = sval.at(0);
			
			if ((first >= 'A' && first <= 'Z') || '_' == first) {
				t = "v:" + sval;
			}
			else {
				try {
					const int n = stoi(sval);
					if (abs(n) < 1 << 28) {
						t = "n:" + sval;
					}
					else {
						t = "c:" + sval;
					}
				}
				catch (const std::invalid_argument& e) {
					t = "c:" + sval;
				}
			}
		if(t.compare("")==0)
			t = "" + (char)c;
		return t;
		}

		static std::vector<std::vector<std::vector<std::string>>> toSentences(const std::string s, const bool fromFile) {
			std::vector<std::vector<std::vector<std::string>>> Wsss;
			std::vector<std::vector<std::string>> Wss;
			std::vector<std::string> Ws;
			Toks * newToks = makeToks(s, fromFile);
			int count = 0;
			std::string t = "";
			while ((t = newToks->getWord()).compare("")!=0) {
				printf("%s\n", t.c_str());
				if (DOT.compare(t)==0) {
					Wss.push_back(Ws);
					Wsss.push_back(Wss);
					Wss.clear();
					Ws.clear();
					
				}
				else if (("c:" + IF).compare(t)==0) {

					Wss.push_back(Ws);
				}
				else if (("c:" + AND).compare(t)==0) {
					Wss.push_back(Ws);
				}
				else if (("c:" + HOLDS).compare(t)==0) {
					const std::string w = Ws.at(0);
					Ws[0] = "h:" + w.substr(2);
				}
				else if (("c:" + LISTS).compare(t)==0) {
					const std::string w = Ws.at(0);
					Ws[0] = "l:" + w.substr(2);
				}
				else if (("c:" + IS).compare(t)==0) {
					const std::string w = Ws.at(0);
					Ws[0] =  "f:" + w.substr(2);
				}
				else {
					Ws.push_back(t);
				}
			}
			return Wsss;
		}


		//static void main(const std::string[] args) {
		//	Main.pp(toSentences("prog.nl", true));
		//}
};

