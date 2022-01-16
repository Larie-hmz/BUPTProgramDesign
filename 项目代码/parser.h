#pragma once

#include<cstdio>
#include<string>
#include<iostream>
#include<vector>
#include<unordered_map>


class step {
public:
	std::vector<std::string> expression;                    //�洢speak�ı���
	std::vector<int> listen;                                //�洢listen�Ŀ�ʼ��ʱ���ͽ�����ʱ��
	std::unordered_map<std::string, std::string>  ans_step; //ǰ��Ϊ�ش𣬺���Ϊ������
	std::string silence_to;									//silence����Ĳ�����
	std::string default_to;								    //default����Ĳ�����
	int exit = 0;                                           //Ϊ0��ʾ��ʱ�������˳�����Ϊ1���ʾ��ʱ��Ҫ�˳�
};

class script {
public:
	std::unordered_map<std::string, step> steps;           //�洢����step��ǰ��Ϊstep��������Ϊstep
	std::string entry;                                     //������stepid
	std::vector<std::string> vars;                         //������б���
};

class parser
{
public:
	void parse_file(std::string filename);  //��ȡ�ļ�
	script Script;                          //Scriptʵ��
private:
	std::string old_stepid="";              //��һ��step_id��

	step tempstep;                          //��ǰstep
	void trim(std::string& s);              //ɾ����β�հ��ַ�
	void parse_line(std::string line);      //����һ�� 
	void process_tokens(std::vector<std::string>& tokens);                 //����һ���е�token���
	void process_step(std::string stepid);                                 //step��������
	void process_speak(std::vector<std::string>& tokens);                  //�����������
	void process_expression(std::vector<std::string>& tokens);             //����������
	void process_listen(int start_timer,int stop_timer);                   //��ȡ��������
	void process_branch(std::string answer, std::string next_stepid);      //��֧����
	void process_silence(std::string next_stepid);                         //��������
	void process_default(std::string next_stepid);                         //Ĭ�Ϲ���
	void process_exit();                                                   //�˳�����
	
	int string_to_int(std::string str);     //�ַ���ת����
	void test_parser();						//����parser
};

