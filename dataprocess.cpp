#include "StdAfx.h"
#include "dataprocess.h"
#include <fstream>
#include <conio.h>
#include <iostream>

#define TRYVECTOR(SENTENCE) try{SENTENCE}catch(std::out_of_range &exc){cout<<"Vector越界，字符串或TroopID等某种ID无法找到"<<endl;std::cerr << exc.what() << endl<<" Line:" << __LINE__ << endl<< " File:" << __FILE__ << endl<<"下标值:"<<value<<endl; 	_getch();}
using namespace std;
extern int iUnrecognized;
extern comment tempcomment;

extern vector<string> qStrs;
extern scriptsfile scripts;
extern triggerfile triggers;
extern missiontplfile missions;
extern menufile menus;
extern stringfile str_file;
extern troopfile trp_file;
extern int sel;

//声音解析用对象
extern map<int, string> opcodemap;
extern vector<string> gVars;
extern map<double, string> triggerinterval;

extern map<int, string> token2state;
extern vector<string> default_states;
std::string& trim(std::string &s)   
{  
	if (s.empty())   
	{  
		return s;  
	}
	for (size_t i=0;i<s.size();i++) {
		if (s[i]=='\"') {
			s.insert(i,string("\\")); 
			i+=2;
		}
	}
	s.erase(0,s.find_first_not_of(" "));  
	s.erase(s.find_last_not_of(" ") + 1);  
	return s;  
}
vector<string> globalVariable;

void split(std::string& s, std::string delim,std::vector< std::string >& ret)  
{  
	size_t last = 0;  
	size_t index=s.find(delim,last); 
	string temp;
	ret.clear();
	while (index!=std::string::npos)  
	{  
		temp=s.substr(last,index-last);
		if(!temp.empty())
			ret.push_back(temp);  
		last=index+delim.length();  
		index=s.find(delim,last);  
	}
	//送入最后一个分割项
	if (index-last>0)  
	{  
		temp=s.substr(last,index-last);
		if(!temp.empty())
			ret.push_back(temp);  
	}  
}
void scriptsfile::load(){
	ifstream ifs;
	script tempscript;
	ifs.open("scripts.txt");
	if (!ifs)  
	{  
		cout<<"错误：打开文件 "<< "scripts.txt" <<" 失败。"<<endl;
		cout<<"请把程序放在mod目录下。"<<endl;
		_getch();
		exit(0);  
	}
	cout<<"正在载入scripts.txt............";
	string tempLine;
	vector<string> words;
	operation tempoperation;
	if(getline(ifs,tempLine))
		header.push_back(trim(tempLine));
	else{
		cout<<"scripts.txt格式不正确"<<endl;
		exit(0);
	}
	if(getline(ifs,tempLine))
		header.push_back(trim(tempLine));
	else{
		cout<<"scripts.txt格式不正确"<<endl;
		exit(0);
	}
	int statementlen=0;
	while(true){
		//获取 储存identify
		if(getline(ifs,tempLine)){
			trim(tempLine);
			if(tempLine.empty())
				break;
			split(tempLine," ",words);
			tempscript.identify=words.front();
			words.clear();
		}else
			break;
		//获取 储存op block
		if(getline(ifs,tempLine)){
			trim(tempLine);
			split(tempLine," ",words);
			//使用operationlist的方法将数字常量储存到oplist
			int i=tempscript.oplist.store(words);
			scripts.push_back(tempscript);
			words.clear();
			tempscript.identify.clear();
			tempscript.oplist.operations.clear();
		}else
			break;
	}
	cout<<"完成"<<endl;
}
void scriptsfile::analyze(){
	for(int s_i=0;s_i<scripts.size();s_i++){
		scripts[s_i].analyze();
	}
}
void scriptsfile::output()
{
	ofstream ofs("reversed\\module_scripts.py");
	cout<<"正在反编译module_scripts.py............";
	ofs<<"from header_common import *\n";
	ofs<<"from header_operations import *\n";
	ofs<<"from module_constants import *\n";
	ofs<<"from module_constants import *\n";
	ofs<<"from header_parties import *\n";
	ofs<<"from header_skills import *\n";
	ofs<<"from header_mission_templates import *\n";
	ofs<<"from header_items import *\n";
	ofs<<"from header_triggers import *\n";
	ofs<<"from header_terrain_types import *\n";
	ofs<<"from header_music import *\n";
	ofs<<"from header_map_icons import *\n";
	ofs<<"from ID_animations import *\n\n";

	ofs<<"from txt2py_constant import *\n\n\n";
	//scripts开始
	ofs << "scripts = ["<<endl;
	for(int s_i=0;s_i<scripts.size();s_i++){
		ofs<<"#**************************************************"<<endl;
		ofs<<"#*               New Script Begin                 *"<<endl;
		ofs<<"#**************************************************"<<endl;
		if(scripts[s_i].identify=="game_receive_url_response")
			cout<<"";
		ofs<<"  (\""<<scripts[s_i].identify<<"\",";
		scripts[s_i].oplist.output(ofs);
		ofs<<"),"<<endl;
	}
	//scripts结束
	ofs<<"]"<<endl;
	cout<<"完成"<<endl;
}
void triggerfile::load()
{
	ifstream ifs;

	ifs.open("triggers.txt");
	if (!ifs)  
	{  
		cout<<"错误：打开文件 "<< "triggers.txt" <<" 失败。"<<endl;
		cout<<"请把程序放在mod目录下。"<<endl;
		_getch();
		exit(0);  
	}
	cout<<"正在载入triggers.txt............";
	string tempLine;
	vector<string> words;

	if(getline(ifs,tempLine))
		header.push_back(trim(tempLine));
	else{
		cout<<"triggers.txt格式不正确"<<endl;
		exit(0);
	}
	if(getline(ifs,tempLine))
		header.push_back(trim(tempLine));
	else{
		cout<<"triggers.txt格式不正确"<<endl;
		exit(0);
	}

	while(getline(ifs,tempLine)){
		trigger temptrigger;
		trim(tempLine); 
		if(tempLine.empty())
			continue;
		split(tempLine," ",words);
		temptrigger.store(words);
		triggers.push_back(temptrigger);
	}
	cout<<"完成"<<endl;
}

void triggerfile::output()
{
	ofstream ofs("reversed\\module_triggers.py");
	cout<<"正在反编译module_triggers.py............";
	ofs<<"from header_common import *\n";
	ofs<<"from header_operations import *\n";
	ofs<<"from header_parties import *\n";
	ofs<<"from header_items import *\n";
	ofs<<"from header_skills import *\n";
	ofs<<"from header_triggers import *\n";
	ofs<<"from header_troops import *\n";
	ofs<<"from module_constants import *\n\n";

	ofs<<"from txt2py_constant import *\n\n\n";

	ofs<<"triggers = ["<<endl;
	for(int t_i=0;t_i<triggers.size();t_i++){
		ofs<<"#**************************************************"<<endl;
		ofs<<"#*              New Trigger Begin                 *"<<endl;
		ofs<<"#**************************************************"<<endl;
		triggers[t_i].output(ofs);
	}
	ofs<<"]";
	cout<<"完成"<<endl;
}

void triggerfile::analyze()
{
	for(int t_i=0;t_i<triggers.size();t_i++){
		triggers[t_i].analyze();
	}
}

int operationlist::store( vector<string> words)
{
	int sentenceslen=0;
	oplen=atoi(words.at(0).c_str());
	int curop=0;
	operation tempoperation;
	int opcodeindex;
	int nextoplistindex;
	if(oplen==0){
		operations.push_back(tempoperation);
		return 1;
	}
	for(opcodeindex=1;opcodeindex<words.size();opcodeindex+=(sentenceslen+2)){
		sentenceslen=atoi(words.at(opcodeindex+1).c_str());
		tempoperation.sentences.push_back(words.at(opcodeindex));
 		for(int sentenceoffset=0;sentenceoffset<sentenceslen;sentenceoffset++){
			tempoperation.sentences.push_back(words.at(opcodeindex+2+sentenceoffset));
		}
		operations.push_back(tempoperation);
		curop++;
		tempoperation.sentences.clear();
		if(curop==oplen)
			break;
	}
	nextoplistindex=(opcodeindex+sentenceslen+2);
	return nextoplistindex;
}
//输出一个代码块
void operationlist::output(ofstream &ofs)
{
	int curdepth=0;
	int nextdepth=0;
	//oplist开始
	ofs<<endl<<"\t[";
	if(oplen!=0){
		ofs<<endl;
		for(int op_i=0;op_i<operations.size();op_i++){
			//sentences开始
			if(operations[op_i].sentences[0]=="try_begin"||operations[op_i].sentences[0]=="try_for_range"||operations[op_i].sentences[0]=="try_for_range_backwards"||operations[op_i].sentences[0]=="try_for_parties"||operations[op_i].sentences[0]=="try_for_agents"){
				nextdepth++;
			}else if(operations[op_i].sentences[0]=="else_try"){
				curdepth--;
			}else if(operations[op_i].sentences[0]=="try_end"){
				curdepth--;
				nextdepth--;
			}
			for(int i=0;i<curdepth;i++)
				ofs<<"\t";
			ofs<<"\t\t(";
			for(int sentence_i=0;sentence_i<operations[op_i].sentences.size();sentence_i++){
				ofs<<operations[op_i].sentences[sentence_i];
				if(sentence_i<operations[op_i].sentences.size()-1)
					ofs<<",";
			}
			ofs<<"),";
			ofs<<endl;

			curdepth=nextdepth;
		}
		ofs<<"\t";
	}
	//oplist结束
	ofs<<"]";

}

void operationlist::analyze()
{
	for(int op_i=0;op_i<oplen;op_i++){
		operations[op_i].analyze();
	}
}

void missiontplfile::load()
{
	ifstream ifs("mission_templates.txt");
	group tempgroup;
	if (!ifs)  
	{  
		cout<<"错误：打开文件 "<< "mission_templates.txt" <<" 失败。"<<endl;
		cout<<"请把程序放在mod目录下。"<<endl;
		_getch();
		exit(0);  
	}
	cout<<"正在载入mission_templates.txt............";
	string tempLine;
	if(getline(ifs,tempLine))
		header.push_back(trim(tempLine));
	else{
		cout<<"mission_templates.txt格式不正确"<<endl;
		exit(0);
	}
	if(getline(ifs,tempLine))
		header.push_back(trim(tempLine));
	else{
		cout<<"mission_templates.txt格式不正确"<<endl;
		exit(0);
	}
	vector<string> words;
	int i=0;
	while(true){
		missiontpl tempmission;
		if(!getline(ifs,tempLine))
			break;
		trim(tempLine); 
		if(tempLine.empty()||tempLine.find_first_of("mst_")!=0)
			continue;
		split(tempLine," ",words);
		tempmission.id=words.at(0);
		if(tempmission.id=="mst_camera_test")
			cout<<"";
		tempmission.name=words.at(1);
		tempmission.flag=words.at(2);
		tempmission.type=words.at(3);
		words.clear();
		if(!getline(ifs,tempLine)){
			cout<<"error in loading mission_templates.txt unexpected ending, there should be a line."<<endl;
			exit(0);
		}
		trim(tempLine); 
		split(tempLine," ",words);
		tempmission.desc=words.at(0);
		words.clear();
		getline(ifs,tempLine);
		if(!getline(ifs,tempLine)){
			cout<<"error in loading mission_templates.txt unexpected ending, there should be a line."<<endl;
			exit(0);
		}
		trim(tempLine);
		split(tempLine," ",words);
		tempmission.grouplen=words.at(0);
		words.erase(words.begin());
		//push group
		for(int group_i=0;group_i<atoi(tempmission.grouplen.c_str());group_i++){
			tempgroup.entryno=words.at(0);
			tempgroup.spawnflag=words.at(1);
			tempgroup.alterflag=words.at(2);
			tempgroup.aiflag=words.at(3);
			tempgroup.numspawn=words.at(4);
			for(int item_i=6;item_i<words.size();item_i++){
				tempgroup.items.push_back(words.at(item_i));
			}
			tempmission.groups.push_back(tempgroup);
			tempgroup.items.clear();
			words.clear();

			if(!getline(ifs,tempLine)){
				cout<<"error in loading mission_templates.txt unexpected ending, there should be a line."<<endl;
				exit(0);
			}
			trim(tempLine); 
			if(group_i==atoi(tempmission.grouplen.c_str())-1)
				break;
			split(tempLine," ",words);
		}
		//push triggers
		int trignum=atoi(tempLine.c_str());
		if(tempmission.grouplen=="0")
			trignum=atoi(words[0].c_str());
		for(int t_i=0;t_i<trignum;t_i++){
			trigger temptrigger;
			if(!getline(ifs,tempLine)||(trim(tempLine)=="")){
				cout << endl<< "    ----txt修改错误：trigger number有误 已忽略" << endl;
				break;
			}
			//cout << i++ << endl;
			split(tempLine," ",words);
			temptrigger.store(words);
			tempmission.triggerlist.push_back(temptrigger);
			words.clear();
		}
		missiontpls.push_back(tempmission);
	}

	cout<<"完成"<<endl;
}
void missiontplfile::output()
{
	ofstream ofs("reversed\\module_mission_templates.py");
	cout<<"正在反编译module_mission_templates.py............";
	ofs<<"from header_common import *\n";
	ofs<<"from header_operations import *\n";
	ofs<<"from header_mission_templates import *\n";
	ofs<<"from header_animations import *\n";
	ofs<<"from header_sounds import *\n";
	ofs<<"from header_music import *\n";
	ofs<<"from header_items import *\n";
	ofs<<"from module_constants import *\n\n";

	ofs<<"from txt2py_constant import *\n\n\n";

	ofs<<"mission_templates = ["<<endl;
	for(int m_i=0;m_i<missiontpls.size();m_i++){
		ofs<<"#**************************************************"<<endl;
		ofs<<"#*          New Mission Templates Begin           *"<<endl;
		ofs<<"#**************************************************"<<endl;
		//一个任务模板开始
		ofs<<"("<<endl;
		ofs<<"\""<<missiontpls[m_i].name<<"\","<<missiontpls[m_i].flag<<","<<missiontpls[m_i].type<<","<<endl;
		ofs<<"\""<<missiontpls[m_i].desc<<"\","<<endl<<"     [";
		//输出groups
		for(int g_i=0;g_i<missiontpls[m_i].groups.size();g_i++){
			ofs<<"("<<missiontpls[m_i].groups[g_i].entryno<<","<<missiontpls[m_i].groups[g_i].spawnflag<<","\
				<<missiontpls[m_i].groups[g_i].alterflag<<","<<missiontpls[m_i].groups[g_i].aiflag<<","<<\
				missiontpls[m_i].groups[g_i].numspawn<<",";
			//输出list of equipments				
			ofs<<"[";
			for(int e_i=0;e_i<missiontpls[m_i].groups[g_i].items.size();e_i++){
				ofs<<missiontpls[m_i].groups[g_i].items[e_i]<<",";
			}
			ofs<<"]";
			//输出group结尾
			ofs<<"),"<<endl<<"     ";
		}
		ofs<<"],"<<endl;
		//输出triggerlist
		ofs<<"     ["<<endl;
		for(int t_i=0;t_i<missiontpls[m_i].triggerlist.size();t_i++){
			missiontpls[m_i].triggerlist[t_i].output(ofs);
		}
		ofs<<"    ]"<<endl;  //triggerlist结尾
		ofs<<"  ),"<<endl;//missionstpl结尾
	}
	ofs<<"]";
	cout<<"完成"<<endl;
}

void missiontplfile::analyze()
{
	for(int m_i=0;m_i<missiontpls.size();m_i++){
		missiontpls[m_i].analyze();
	}
}

void trigger::store( vector<string> words )
{
	operationlist tempconditions;
	operationlist tempconsequences;
	int condtindex;
	int consqindex;
	checkinterval=words.at(0);
	delayinterval=words.at(1);
	rearminterval=words.at(2);
	condtindex=3;
	if(words.size()>condtindex){
		vector<string> tpcondtwords;
		vector<string> tpconsqwords;
		tpcondtwords.insert(tpcondtwords.end(),words.begin()+condtindex,words.end());
		consqindex=condtindex+conditionsblock.store(tpcondtwords);
		if(words.size()>consqindex){
			tpconsqwords.insert(tpconsqwords.end(),words.begin()+consqindex,words.end());
			consequencesblock.store(tpconsqwords);
		}
	}
}

void trigger::output( ofstream& ofs )
{
	ofs<<"("<<checkinterval<<","<<delayinterval<<","<<rearminterval<<",";
	conditionsblock.output(ofs);
	ofs<<",";
	consequencesblock.output(ofs);
	ofs<<"),"<<endl;
}

void trigger::analyze()
{
	//interval的解析
	string str;
	str = triggerinterval[atof(checkinterval.c_str())];
	if (!str.empty())
		checkinterval = str;
	str = triggerinterval[atof(delayinterval.c_str())];
	if (!str.empty())
		delayinterval = str;
	str = triggerinterval[atof(rearminterval.c_str())];
	if (!str.empty())
		rearminterval = str;

	//script块的解析
	conditionsblock.analyze();
	consequencesblock.analyze();
}

void operation::analyze()
{
	string sopcode;
	__int64 key;
	__int64 orinkey;
	char buffer[64];
	string opcode;
	for(int s_i=0;s_i<sentences.size();s_i++){
		//处理opcode
		if(s_i==0){
			key=_atoi64(sentences[0].c_str());
			if((key&0x80000000)==0x80000000){
				sopcode="neg|";
			}
			if((key&0x40000000)==0x40000000){
				sopcode=sopcode+"this_or_next|";
			}
			orinkey=key;
			key=key&0x3FFFFFFF;
			opcode=opcodemap[key&~0x40000000];
			sopcode=sopcode+opcodemap[key&~0x40000000];
			if(opcode.empty()){
				cout<<"ERROR: 未定义的OPCODE!    opcode:"<<orinkey;
				sopcode=_i64toa(orinkey,buffer,10);
			}
			sentences[0]=sopcode;
		}else{
			__int64 i64sentence;
			__int64 mask;
			__int64 value;
			string snewsentence;
			i64sentence=_atoi64(sentences[s_i].c_str());
			mask=i64sentence>>56;
			if(i64sentence<0){
				snewsentence=_i64toa(i64sentence,buffer,10);
				continue;
				//处理其他
			}else{
				value=i64sentence<<8;
				value=value>>8;
#pragma region cases
				switch(mask){
					//no tag
				case 0:
					snewsentence=_i64toa(i64sentence,buffer,10);
					break;
					//register
				case 1:
					snewsentence=string("reg")+_i64toa(value,buffer,10);
					break;
					//global variable
				case 2:
					snewsentence=string("VAR+")+_i64toa(value,buffer,10);
					break;
					//string
				case 3:
					snewsentence=string("\"str_")+str_file.strs.at(value).id+"\"";
					//snewsentence=string("STRING+")+_i64toa(value,buffer,10);
					break;
					//item
				case 4:
					snewsentence=string("ITEM+")+_i64toa(value,buffer,10);
					break;
					//troop
				case 5:
					if (sel == 1){
						TRYVECTOR(snewsentence = string("\"trp_") + trp_file.troops.at(value).id + "\"";)
					}
					else{
						snewsentence = string("TROOP+") + _i64toa(value, buffer, 10);
					}
					break;
					//faction
				case 6:
					snewsentence=string("FACTION+")+_i64toa(value,buffer,10);
					break;
					//quest
				case 7:
					snewsentence=string("QUEST+")+_i64toa(value,buffer,10);
					break;
					//party_tpl
				case 8:
					snewsentence=string("PARTY_TPL+")+_i64toa(value,buffer,10);
					break;
					//party
				case 9:
					snewsentence=string("PARTY+")+_i64toa(value,buffer,10);
					break;
					//scene
				case 10:
					snewsentence=string("SCENE+")+_i64toa(value,buffer,10);
					break;
					//mission_tpl
				case 11:
					TRYVECTOR(snewsentence=string("\"mt_")+missions.missiontpls.at(value).name+"\"";)
						break;
					//menu
				case 12:
					TRYVECTOR(snewsentence="\"mnu_"+menus.menus.at(value).id+"\"";)
						break;
					//script
				case 13:
					TRYVECTOR(snewsentence=string("\"script_")+scripts.scripts.at(value).identify+"\"";)
						break;
					//particle_sys
				case 14:
					snewsentence=string("PARTICLE_SYS+")+_i64toa(value,buffer,10);
					break;
					//scene_prop
				case 15:
					snewsentence=string("SCENE_PROP+")+_i64toa(value,buffer,10);
					break;
					//sound
				case 16:				
					snewsentence=string("SOUND+")+_i64toa(value,buffer,10);
					break;
					//local
				case 17:
					snewsentence=string("\":local_")+_i64toa(value,buffer,10)+"\"";
					break;
					//map_icon
				case 18:
					snewsentence=string("MICON+")+_i64toa(value,buffer,10);
					break;
					//skill
				case 19:
					snewsentence=string("SKILL+")+_i64toa(value,buffer,10);
					break;
					//mesh
				case 20:
					snewsentence=string("MESH+")+_i64toa(value,buffer,10);
					break;
					//presentation
				case 21:
					snewsentence=string("PRESENTATION+")+_i64toa(value,buffer,10);
					break;
					//quick_string
				case 22:
					snewsentence=string("QSTR+")+_i64toa(value,buffer,10);
					break;
					//track
				case 23:
					snewsentence=string("MUSIC+")+_i64toa(value,buffer,10);
					break;
					//table au
				case 24:
					snewsentence=string("TABLEAU+")+_i64toa(value,buffer,10);
					break;
					//animation
					case 25:
					snewsentence=string("ACTION+")+_i64toa(value,buffer,10);
					break;
					case 26:
					snewsentence=string("END_UNRECOGNIZED+")+_i64toa(value,buffer,10);
					break;
					//如果无法解析
				default:
					snewsentence=_i64toa(i64sentence,buffer,10);
					//tempsentence.append("(Unrecognized Tag");
					//rawsentence="UNRECOGNIZED+"+string(_i64toa(value,mystring,10));
					iUnrecognized++;			
				}
#pragma endregion 一堆的解析		
			}
			sentences[s_i]=snewsentence;

		}

	}
}

void script::analyze()
{
	oplist.analyze();
}

void comment::add( string str )
{
	newcomment.append(str);
}

string comment::getstr()
{
	if(newcomment=="\t\t#")
		return "";
	if(newcomment!=oldcomment){
		oldcomment=newcomment;
		string temp=newcomment;
		newcomment="\t\t#";
		return temp;
	}
}

void comment::init()
{
	newcomment="\t\t#";
}

void missiontpl::analyze()
{
	for(int t_i=0;t_i<triggerlist.size();t_i++){
		triggerlist[t_i].analyze();
	}
}
int findstr(vector<string> vs, string s){
	for each (string ts in vs)
	{
		if (ts == s){
			return 1;
		}
	}
	return 0;
}
void dialogfile::load()
{
	ifstream ifs("conversation.txt");
	string tempLine;
	if (!ifs)  
	{  
		cout<<"错误：打开文件 "<< "conversation.txt" <<" 失败。"<<endl;
		cout<<"请把程序放在mod目录下。"<<endl;
		_getch();
		exit(0);  
	}
	cout<<"正在载入conversation.txt............";
	if(getline(ifs,tempLine))
		header.push_back(trim(tempLine));
	else{
		cout<<"conversation.txt格式不正确"<<endl;
		exit(0);
	}
	if(getline(ifs,tempLine))
		header.push_back(trim(tempLine));
	else{
		cout<<"conversation.txt格式不正确"<<endl;
		exit(0);
	}
	//
	vector<string> v_state;
	while(getline(ifs,tempLine)){
		trim(tempLine);
		if(tempLine.empty())
			continue;
		dialog tempdialog;
		vector<string> words;
		
		split(tempLine," ",words);

		//触发条件
		tempdialog.partner=words[1];
		//input token
		tempdialog.i_token = words[2];
		//条件块开始
		int nextindex=3;
		if(words.size()>nextindex){
			vector<string> condtwords;
			condtwords.insert(condtwords.end(),words.begin()+nextindex,words.end());
			nextindex=nextindex+tempdialog.conditions.store(condtwords);
		}
		//对话文本
		tempdialog.dlgtext=words[nextindex];
		//output token
		tempdialog.o_token = words[nextindex + 1];
		//token读取完 将存入map 保持唯一性
		vector<string> words2;
		static int icount = 0;
		string tempstr1="illegal_txt_defined_state";
		string tempstr2 = "illegal_txt_defined_state";
		char buffer1[16] = { 0 };
		int ok;
		ok = atoi(tempdialog.o_token.c_str());
		char buffer2[16] = { 0 };
		
		if (words[0].find_first_of(":") != string::npos){
			if (words[0].find_first_of("dlga_") == 0)
				words[0].erase(0, 5);
			split(words[0], ":", words2);
			
			if (token2state.count(atoi(tempdialog.i_token.c_str())) == 0){//key不存在,判断state是否已经存在
				int suffix=1;
				char buffer3[16];
				string twords2;
				twords2 = words2[0];
				int i_conflicted = 0;
				while (findstr(v_state, twords2)){
					sprintf(buffer3, "_%d", suffix);
					twords2=words2[0] + buffer3;
					suffix++;
					i_conflicted = 1;
				}
				
				if (i_conflicted){
					cout << "发现重名的state：" << words2[0] << "  已经重命名state 注意汉化修正" << endl;
					i_conflicted = 0;
				}
				words2[0] = twords2;
				
				//token不存在则插入
				token2state.insert(make_pair(atoi(tempdialog.i_token.c_str()), words2[0]));
				v_state.push_back(words2[0]);//插入完记录插入的state
			}


			if (token2state.count(atoi(tempdialog.o_token.c_str())) == 0){//token不存在,判断state是否一样
				int suffix = 1;
				char buffer3[16];
				string twords2;
				twords2 = words2[1];
				int i_conflicted = 0;
				while (findstr(v_state, twords2)&&!findstr(default_states, twords2)){
					sprintf(buffer3, "_%d", suffix);
					twords2 = words2[1] + buffer3;
					suffix++;
					i_conflicted = 1;
				}

				if (i_conflicted){
					cout << "发现重名的state：" << words2[1] << "  已经重命名state 注意汉化修正" << endl;
				}
				words2[1] = twords2;

				//token不存在则插入
				token2state.insert(make_pair(atoi(tempdialog.o_token.c_str()), words2[1]));
				v_state.push_back(words2[1]);//插入完记录插入时的state
			}

		}
		else{//autoid不标准，没办法分出inputstate和outputstate则自定义state，自定义state就不用判断重复问题了
			_itoa(icount++, buffer1, 10);
			tempstr1.append(buffer1);
			token2state.insert(make_pair(atoi(tempdialog.i_token.c_str()), tempstr1));

			_itoa(icount++, buffer2, 10);
			tempstr2.append(buffer2);
			token2state.insert(make_pair(atoi(tempdialog.o_token.c_str()), tempstr2));
		}

		//结果块开始
		vector<string> consqwords;
		consqwords.insert(consqwords.end(),words.begin()+nextindex+2,words.end());
		nextindex=nextindex+2+tempdialog.consequences.store(consqwords);
		tempdialog.voiceover=words[nextindex];
		if(tempdialog.voiceover!="NO_VOICEOVER")
			cout<<"(⊙o⊙)…"<<endl;
		dialogs.push_back(tempdialog);
	}
	cout<<"完成"<<endl;
}

void dialogfile::output()
{
	ofstream ofs("reversed\\module_dialogs.py");
	cout<<"正在反编译module_dialogs.py............";
	ofs<<"from header_common import *\n";
	ofs<<"from header_dialogs import *\n";
	ofs<<"from header_operations import *\n";
	ofs<<"from header_parties import *\n";
	ofs<<"from header_item_modifiers import *\n";
	ofs<<"from header_skills import *\n";
	ofs<<"from header_triggers import *\n";
	ofs<<"from ID_troops import *\n";
	ofs<<"from ID_party_templates import *\n\n";

	ofs<<"from txt2py_constant import *\n\n\n";

	ofs<<"dialogs = ["<<endl;
	for(int d_i=0;d_i<dialogs.size();d_i++){
		ofs<<"#**************************************************"<<endl;
		ofs<<"#*               New Dialog Begin                 *"<<endl;
		ofs<<"#**************************************************"<<endl;
		dialogs[d_i].output(ofs);
		ofs<<","<<endl;
	}
	ofs<<"]"<<endl;
	cout<<"完成"<<endl;
}

void dialogfile::analyze()
{
	for(int d_i=0;d_i<dialogs.size();d_i++){
		dialogs[d_i].analyze();
	}
}

void dialog::output( ofstream& ofs )
{
	ofs << "[" << partner << ",\"" << token2state[atoi(i_token.c_str())] << "\",";
	conditions.output(ofs);
	ofs<<","<<endl<<"    \"";
	ofs<<dlgtext<<"\",";
	if (!token2state[atoi(o_token.c_str())].empty()){
		ofs << "\"" << token2state[atoi(o_token.c_str())] << "\"";
		ofs<<",";
	}
	else{
		cout << "ERROR: 发现没有output token --"<<partner<<" "<<i_token << endl;
	}
	consequences.output(ofs);
	if(voiceover!="NO_VOICEOVER")
		ofs<<voiceover;
	ofs<<"]";
}

void dialog::analyze()
{
	__int64 ipartner=_atoi64(partner.c_str());
	string snewparter="";
	__int64 trpid;
	__int64 othertrdid;
	char buffer[64];
	if((ipartner&0x00020000)==0x00020000)
		snewparter+="party_tpl|";
	if((trpid=(ipartner&0x00000fff))==0x00000fff)
		snewparter+="anyone|";
	else
		snewparter+=string(_itoa(trpid,buffer,10))+"|";
	if((ipartner&0x00001000)==0x00001000)
		snewparter+="repeat_for_factions|";
	if((ipartner&0x00002000)==0x00002000)
		snewparter+="repeat_for_parties|";
	if((ipartner&0x00003000)==0x00003000)
		snewparter+="repeat_for_troops|";
	if((ipartner&0x00004000)==0x00004000)
		snewparter+="repeat_for_100|" ;
	if((ipartner&0x00005000)==0x00005000)
		snewparter+="repeat_for_1000|";
	if((ipartner&0x00010000)==0x00010000)
		snewparter+="plyr|";
	if((ipartner&0x00040000)==0x00040000)
		snewparter+="auto_proceed|";
	if((ipartner&0x00080000)==0x00080000)
		snewparter+="multi_line|";
	if((othertrdid=ipartner&4293918720)!=0){
		othertrdid=othertrdid>>20;
		snewparter=snewparter+string("other(")+_i64toa(othertrdid,buffer,10)+")";
	}
	if(snewparter.find_last_of("|")==(snewparter.length()-1))
		snewparter.erase(snewparter.length()-1);
	partner=snewparter;
	conditions.analyze();
	consequences.analyze();
}

void menufile::load()
{
	ifstream ifs("menus.txt");
	string tempLine;
	if (!ifs)  
	{  
		cout<<"错误：打开文件 "<< "conversation.txt" <<" 失败。"<<endl;
		cout<<"请把程序放在mod目录下。"<<endl;
		_getch();
		exit(0);  
	}
	cout<<"正在载入menus.txt............";
	getline(ifs,tempLine);
	trim(tempLine);
	header.push_back(tempLine);
	getline(ifs,tempLine);
	trim(tempLine);
	header.push_back(tempLine);
	while(getline(ifs,tempLine)){
		menu tempmenu;
		vector<string> words;
		trim(tempLine);
		if (tempLine.empty()){       //父menu行不能为空
			cout << "上级menu不存在，不可预料的空行" << endl;
			break;
		}
		split(tempLine," ",words);
		words[0].erase(0,5);
		tempmenu.id=words[0];
		tempmenu.flags=words[1];
		tempmenu.text=words[2];
		tempmenu.meshname=words[3];
		vector<string> vsopblock;
		int isublen=0;
		vsopblock.insert(vsopblock.end(),words.begin()+4,words.end());
		isublen=4+tempmenu.opblock.store(vsopblock);
		tempmenu.optslen=words[isublen];
		getline(ifs,tempLine);
		if(!tempLine.empty()){
		words.clear();
		split(tempLine," mno_",words);
		for(int m_i=0;m_i<words.size();m_i++){
			menuoption tempopt;
			tempopt.store(words[m_i]);
			tempmenu.opts.push_back(tempopt);
		}
		}
		menus.push_back(tempmenu);
		
	}
	cout<<"完成"<<endl;
}

void menufile::output()
{
	ofstream ofs("reversed\\module_game_menus.py");
	cout<<"正在反编译module_menus.py............";
	ofs<<"from header_game_menus import *\n";
	ofs<<"from header_parties import *\n";
	ofs<<"from header_items import *\n";
	ofs<<"from header_mission_templates import *\n";
	ofs<<"from header_music import *\n";
	ofs<<"from header_terrain_types import *\n\n";
	ofs<<"from module_constants import *\n\n";

	ofs<<"from txt2py_constant import *\n\n\n";

	ofs<<"game_menus = ["<<endl;

	for(int m_i=0;m_i<menus.size();m_i++){
		ofs<<"#**************************************************"<<endl;
		ofs<<"#*                New Menu Begin                  *"<<endl;
		ofs<<"#**************************************************"<<endl;
		ofs<<"(\""<<menus[m_i].id<<"\","<<menus[m_i].flags<<","<<endl;
		ofs<<"\""<<menus[m_i].text<<"\","<<endl;
		ofs<<"\""<<menus[m_i].meshname<<"\",";
		menus[m_i].opblock.output(ofs);
		ofs<<","<<endl;
		ofs<<"  [ #Option List Begin"<<endl;;
		for(int o_i=0;o_i<menus[m_i].opts.size();o_i++){
			menus[m_i].opts[o_i].output(ofs);
		}
		ofs<<"  ]),"<<endl;;
	}
	ofs<<"]"<<endl;
	cout<<"完成"<<endl;
}

void menufile::analyze()
{
	for(int m_i=0;m_i<menus.size();m_i++){
		menus[m_i].analyze();
	}
}

void menuoption::store(string str)
{
	vector<string> words;
	vector<string> words2;
	str.erase(str.end()-1,str.end());
	trim(str);
	if(str.empty()){
		cout<<"menuoption为空，错误"<<endl;
		exit(0);
	}
	split(str," ",words);
	id=words[0];
	words2.insert(words2.end(),words.begin()+1,words.end());
	int next_i=1+conditions.store(words2);
	text=words[next_i];
	words2.clear();
	words2.insert(words2.end(),words.begin()+next_i+1,words.end());
	next_i=next_i+1+consquences.store(words2);
	doorname=words[next_i];
}

void menuoption::output( ofstream &ofs )
{
	ofs<<"\t(\""<<id<<"\",";
	conditions.output(ofs);
	ofs<<","<<endl;
	ofs<<"\t\""<<text<<"\",";
	consquences.output(ofs);
	if(doorname!="."){
		ofs<<",\""<<doorname<<"\"";
	}
	ofs<<"),"<<endl;
}

void menuoption::analyze()
{
	conditions.analyze();
	consquences.analyze();
}

void menu::analyze()
{
	opblock.analyze();
	for(int o_i=0;o_i<opts.size();o_i++){
		opts[o_i].analyze();
	}
}

void stringfile::load()
{
	ifstream ifs("strings.txt");
	string tempLine;
	if (!ifs)  
	{  
		cout<<"错误：打开文件 "<< "strings.txt" <<" 失败。"<<endl;
		cout<<"请把程序放在mod目录下。"<<endl;
		_getch();
		exit(0);  
	}
	cout<<"正在载入strings.txt............";
	getline(ifs,tempLine);
	trim(tempLine);
	header.push_back(tempLine);
	getline(ifs,tempLine);
	trim(tempLine);
	header.push_back(tempLine);
	while(getline(ifs,tempLine)){
		trim(tempLine);
		if(tempLine.empty())
			continue;
		normalstr tempstr;
		vector<string> words;
		split(tempLine," ",words);
		words[0].erase(0,4);
		tempstr.id=words[0];
		if(words.size()==2)
			tempstr.str=words[1];
		strs.push_back(tempstr);	
	}
	cout<<"完成"<<endl;
}

void stringfile::output()
{
		ofstream ofs("reversed\\module_strings.py");
		cout<<"正在反编译module_strings.py............";
		ofs<<"strings = ["<<endl;
		for(int s_i=0;s_i<str_file.strs.size();s_i++){
			ofs<<"\t"<<"(\""<<str_file.strs[s_i].id<<"\",\""<<str_file.strs[s_i].str<<"\"),"<<endl;
		}
		ofs<<"]";
		cout<<"完成"<<endl;;
}

void troopfile::load()
{
	ifstream ifs("troops.txt");
	cout<<"正在载入troops.txt............";
	string tempLine;
	getline(ifs,tempLine);
	trim(tempLine);
	header.push_back(tempLine);
	getline(ifs,tempLine);
	trim(tempLine);
	header.push_back(tempLine);
	while(getline(ifs,tempLine)){
		if((tempLine.find("trp_"))>3)
			continue;
		trim(tempLine);
		troop tempTroop;
		vector<string> words;
		split(tempLine," ",words);
		words[0].erase(0,4);
		tempTroop.id=words[0];
		tempTroop.name=words[1];
		tempTroop.pluname=words[2];
		tempTroop.image=words[3];
		tempTroop.flags=words[4];
		tempTroop.scene=words[5];
		tempTroop.reversed=words[6];
		tempTroop.faction=words[7];
		tempTroop.upgrade1=words[8];
		tempTroop.upgrade2=words[9];
		getline(ifs,tempLine);
		trim(tempLine);
		split(tempLine," 0",tempTroop.itemlist);
		getline(ifs,tempLine);
		trim(tempLine);
		split(tempLine," ",tempTroop.attributes);
		getline(ifs,tempLine);
		trim(tempLine);
		split(tempLine," ",tempTroop.wp);
		getline(ifs,tempLine);
		trim(tempLine);
		split(tempLine," ",tempTroop.skillswords);
		getline(ifs,tempLine);
		trim(tempLine);
		split(tempLine," ",tempTroop.facekey);
		troops.push_back(tempTroop);
	}
	cout<<"完成"<<endl;;
}

void troopfile::analyze()
{
	for(int t_i=0;t_i<troops.size();t_i++){
		troops[t_i].analyze();
	}
}

void troopfile::output()
{
	ofstream ofs("reversed\\module_troops.py.因为关联过多_该文件只可以用来阅读.txt");
	cout<<"正在反编译module_troops.py............";
	ofs<<"import random\n\n";
	ofs<<"from header_common import *\n";
	ofs<<"from header_items import *\n";
	ofs<<"from header_troops import *\n";
	ofs<<"from header_skills import *\n";
	ofs<<"from ID_factions import *\n";
	ofs<<"from ID_items import *\n";
	ofs<<"from ID_scenes import *\n\n\n";

	ofs<<"troops = ["<<endl;
	for(int t_i=0;t_i<trp_file.troops.size();t_i++){
		ofs<<"#=====================New Troop Begin====================="<<endl;
		ofs<<"  "<<"[\""<<troops[t_i].id<<"\",\""<<troops[t_i].name<<"\",\""<<troops[t_i].pluname<<"\","<<troops[t_i].flags<<","<<troops[t_i].scene<<","<<troops[t_i].reversed<<","<<troops[t_i].faction<<","<<endl;
		ofs<<"  [";
		for(int i_i=0;i_i<troops[t_i].itemlist.size();i_i++){
			if(troops[t_i].itemlist[i_i]!="-1")
				ofs<<troops[t_i].itemlist[i_i];
			else
				continue;
			if(i_i!=troops[t_i].itemlist.size()-1)
				ofs<<",";
		}
		ofs<<"],"<<endl;
		ofs<<"  "<<troops[t_i].strattributes<<","<<troops[t_i].strwp<<","<<endl;
		ofs<<"  "<<troops[t_i].strskillswords<<","<<endl;
		ofs<<"  "<<troops[t_i].face1;
		if(troops[t_i].face2!="0x0000000000000000000000000000000000000000000000000000000000000000")
			ofs<<","<<troops[t_i].face2;
		if(troops[t_i].image!="0")
			ofs<<","<<troops[t_i].image;
		ofs<<"],"<<endl;
	}
	ofs<<"]"<<endl<<endl;
	for(int t_i2=0;t_i2<troops.size();t_i2++){
		if(!troops[t_i2].strupgrade.empty())
			ofs<<troops[t_i2].strupgrade<<endl;
	}
	cout<<"完成"<<endl;;
}

void troop::analyze()
{
	strwp="wp_one_handed("+wp[0]+")|wp_two_handed("+wp[1]+")|wp_polearm("+wp[2]+")|wp_archery("+wp[3]+")|wp_crossbow("+wp[4]+")|wp_throwing("+wp[5]+")|wp_firearm("+wp[6]+")";
	strattributes="("+attributes[0]+")|("+attributes[1]+"<<8"+")|("+attributes[2]+"<<16"+")|("+attributes[3]+"<<24"+")|level("+attributes[4]+")";
	strskillswords="("+skillswords[0]+")|("+skillswords[1]+"<<32"+")|("+skillswords[2]+"<<64"+")|("+skillswords[3]+"<<96"+")|("+skillswords[4]+"<<128"+")|("+skillswords[5]+"<<160"+")";
	_int64 iface11;
	_int64 iface12;
	_int64 iface13;
	_int64 iface14;
	_int64 iface21;
	_int64 iface22;
	_int64 iface23;
	_int64 iface24;
	iface11=_atoi64(facekey[3].c_str());
	iface12=_atoi64(facekey[2].c_str());
	iface13=_atoi64(facekey[1].c_str());
	iface14=_atoi64(facekey[0].c_str());
	iface21=_atoi64(facekey[7].c_str());
	iface22=_atoi64(facekey[6].c_str());
	iface23=_atoi64(facekey[5].c_str());
	iface24=_atoi64(facekey[4].c_str());
	char buffer[512];
	sprintf(buffer,"0x%016I64X%016I64X%016I64X%016I64X",iface14,iface13,iface12,iface11);
	face1=buffer;
	sprintf(buffer,"0x%016I64X%016I64X%016I64X%016I64X",iface24,iface23,iface22,iface21);
	face2=buffer;
	if(upgrade1!="0"){
		upgrade1=trp_file.troops[atoi(upgrade1.c_str())].id;
		if(upgrade2=="0"){
			strupgrade="upgrade(troops,\""+id+"\",\""+upgrade1+"\")";
		}else{
			upgrade2=trp_file.troops[atoi(upgrade2.c_str())].id;
			strupgrade="upgrade2(troops,\""+id+"\",\""+upgrade1+"\",\""+upgrade2+"\")";
		}
	}
	for(int i_i=0;i_i<itemlist.size();i_i++){
		trim(itemlist[i_i]);
	}
}

void partyfile::load()
{
	ifstream ifs("parties.txt");
	cout<<"正在载入parties.txt............";
	string tempLine;
	getline(ifs,tempLine);
	header.push_back(tempLine);
	getline(ifs,tempLine);
	while(getline(ifs,tempLine)){
		if(!tempLine.find("p_"))
			continue;
		vector<string> words;
		party tempParty;
		trim(tempLine);
		split(tempLine," ",words);
		tempParty.id=words[3];
		tempParty.name=words[4];
		tempParty.flags=words[5];
		tempParty.menuid=words[6];
		tempParty.partytpl=words[7];
		tempParty.faction=words[8];
		tempParty.personality=words[9];//words[9]=words[10]
		tempParty.aibehavior=words[11];
		tempParty.aitargetparty=words[12];//words[12]=words[13]
		tempParty.coordinates.push_back(words[14]);
		tempParty.coordinates.push_back(words[15]);

	}
}
