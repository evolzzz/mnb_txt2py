#include <string>
#include <vector>
#include <map>
using namespace std;

struct comment{
	string oldcomment;
	string newcomment;
	void init();
	void add(string str);
	string getstr();
};
struct operation{
	vector<string> sentences;
	void analyze();
};
struct operationlist{
	int oplen;
	vector<operation> operations;
	int store(vector<string> words);
	void output(ofstream& ofs);
	void analyze();
};
struct script{
	string identify;
	operationlist oplist;
	void analyze();
};
struct group{
	string entryno;
	string spawnflag;
	string alterflag;
	string aiflag;
	string numspawn;
	vector <string> items;
};
struct scriptsfile{
	vector<string> header;
	vector<script> scripts;
	void load();
	void analyze();
	void output();
};

struct dialog{
	//记住映射关系
	string autoid;

	string partner;
	string i_token;//对话之间建立联系使用
	operationlist conditions;
	string dlgtext;
	string o_token;//对话之间建立联系使用
	operationlist consequences;
	string voiceover;
	void output(ofstream& ofs);
	void analyze();
};

struct trigger{
	void store(vector<string> words);
	string checkinterval;
	string delayinterval;
	string rearminterval;
	operationlist conditionsblock;
	operationlist consequencesblock;
	void output(ofstream& ofs);
	void analyze();
};
struct triggerfile{
	vector<string> header;
	vector<trigger> rawtriggers;
	vector<trigger> triggers;
	void load();
	void output();
	void analyze();
};
struct missiontpl{
	string id;
	string name;
	string flag;
	string type;
	string desc;
	string grouplen;
	vector<group> groups;
	vector<trigger> triggerlist;
	void analyze();
};
struct missiontplfile{
	vector<string> header;
	void load();
	void analyze();
	vector<missiontpl> missiontpls;
	void output();
};

struct dialogfile{
	vector<string> header;
	vector<dialog> dialogs;
	void load();
	void output();
	void analyze();
};
struct menuoption{
	string id;
	operationlist conditions;
	string text;
	operationlist consquences;
	string doorname;
	void store(string str);
	void output(ofstream &ofs);
	void analyze();
};
struct menu{
	string id;
	string flags;
	string text;
	string meshname;
	operationlist opblock;
	vector<menuoption> opts;
	string optslen;
	void analyze();
};
struct menufile{
	vector<string> header;
	vector<menu> menus;
	void load();
	void output();
	void analyze();
};
struct normalstr{
	string id;
	string str;
};
struct stringfile{
	vector<string> header;
	vector<normalstr> strs;
	void load();
	void output();
};
struct troop{
	string id;
	string name;
	string pluname;
	string flags;
	string scene;
	string reversed;
	string faction;
	vector<string> itemlist;
	vector<string> attributes;
	string strattributes;
	string strwp;
	string strskillswords;
	string strupgrade;
	vector<string> wp;
	vector<string> skillswords;
	vector<string> facekey;
	string face1;
	string face2;
	string image;
	string upgrade1;
	string upgrade2;
	void analyze();
};
struct troopfile{
	vector<string> header;
	vector<troop> troops;
	void load();
	void output();
	void analyze();
};
struct party{
	string id;
	string name;
	string flags;
	string menuid;
	string partytpl;
	string faction;
	string personality;
	string aibehavior;
	string aitargetparty;
	vector<string> coordinates;
	vector<vector<string>> trooplist;
	string direction;
};
struct partyfile{
	vector<string> header;
	vector<party> parties;
	void load();
	void output();
};
std::string& trim(std::string &s);
void split(std::string& s, std::string delim,std::vector< std::string >& ret);