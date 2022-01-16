#pragma once
#include"parser.h"

class interpreter
{
	std::unordered_map<std::string,std::unordered_map<std::string, std::string>> var_table;    //������
	parser p;      //parserʵ���������﷨��
	step tstep;    //��ǰstep
public:
	std::string key_words;     //��ʱ�����ʶ������
	void init();               //��ʼ������database�ж�����
	void run();                //�������
	void inter_expression(std::vector<std::string>& exps);      //������ʽ����
	void inter_listen(std::vector<int> listen);                 //��ȡ�������
};

