#pragma once
#include"parser.h"

class interpreter
{
	std::unordered_map<std::string,std::unordered_map<std::string, std::string>> var_table;    //变量表
	parser p;      //parser实例，传递语法树
	step tstep;    //当前step
public:
	std::string key_words;     //此时输入的识别主键
	void init();               //初始化，从database中读数据
	void run();                //翻译过程
	void inter_expression(std::vector<std::string>& exps);      //翻译表达式内容
	void inter_listen(std::vector<int> listen);                 //读取输入操作
};

