#include "interpreter.h"
#include<Windows.h>
#include <conio.h>
#include <thread>
#include <chrono>
#include<windows.h>
#include<process.h>
#include<fstream>


HANDLE hThread1;                 //�����߳�
#define maxsize 50               //�ַ�����󳤶�
char str[maxsize];               //��char�ķ�ʽ�洢�ַ���
std::string input_str = "";      //��Ŷ�ȡ���ַ���
int flag = 1;                    //flagΪ1��ʾ���Զ�ȡ��Ϊ0���ܶ�ȡ

//�����̣߳���ɶ�ȡ�������
unsigned __stdcall Fun1Proc(void* pArguments) {

	while (1) {
		if (flag == 1) {
			gets_s(str, maxsize-1);
			input_str = std::string(str);
			flag = 0;
		}
	}

	return 0;
}

//��ʼ������database�ж�����
void interpreter::init() {
	std::fstream fp;
	fp.open("database.txt");
	std::string pn1, na;

	int var_size = p.Script.vars.size();

	while (!fp.eof()) {
		fp >> pn1;
		for (int i = 0; i < var_size && !fp.eof(); i++) {
			fp >> na;
			var_table[pn1][p.Script.vars[i]] = na;
		}
	}
}

//�������
void interpreter::run() {
	p.parse_file("test.rsl");
	init();
	tstep = p.Script.steps[p.Script.entry];
	if (var_table.find(key_words) == var_table.end()) {
		std::cout << "�û�������\n";
		return;
	}
	hThread1 = (HANDLE)_beginthreadex(NULL, 0, Fun1Proc, NULL, 0, NULL);
	SuspendThread(hThread1);
	int repeat = 0;        //repeat��¼��Ĭ�Ĵ��������������������˳�
	std::string ans = "";
	while (true) {
		inter_expression(tstep.expression);   //�ȷ���speak
		std::cout << "\n";
		if (tstep.exit == 1) {                //����ǽ����������ѭ��
			break;
		}
		
		inter_listen(tstep.listen);
		ans = input_str;
		if (ans == "") {                                                //���ʲô��û����Ϊsilence
			if (repeat < 3) {
				repeat++;
			}
			else {
				break;
			}
			tstep = p.Script.steps[tstep.silence_to];
		}
		else if (tstep.ans_step.find(ans) != tstep.ans_step.end()) {    //�����branch�����ҵ�
			repeat = 0;
			tstep = p.Script.steps[tstep.ans_step[ans]];
		}
		else {                                                          //����޷�ʶ��Ҫ����תdafault
			repeat = 0;
			tstep = p.Script.steps[tstep.default_to];
		}
	}
	std::cout << "�������\n";
}

//������ʽ����
//���������ʽvector
void interpreter::inter_expression(std::vector<std::string>& exps) {
	for (int i = 0; i < exps.size(); i++) {
		if (exps[i][0] == '"') {         //������ַ�����ӡ�ַ�������
			std::string var(++exps[i].begin(), --exps[i].end());
			std::cout << var;
		}
		else if (exps[i][0] == '$') {   //����Ǳ�����ȥ���ű����Ҷ�Ӧ����
			std::string var(++exps[i].begin(), exps[i].end());
			if (var_table[key_words].find(var) != var_table[key_words].end()) {
				std::cout << var_table[key_words][var];
			}
			else {
				std::cout << "Can't find variable!\n";
			}		
		}
		else {                         //���������
			std::cout << "Wrong!\n";
		}
	}
}

//��ȡ�������
//��������ȡʱ���vector
void interpreter::inter_listen(std::vector<int> listen) {
	Sleep(listen[0] * 1000);
	std::cout << "��~~~\n";
	int lastime = listen[1] - listen[0];
	input_str = "";

	//���ü���ʶ��ķ���
	//while (true) {
	//	if (_kbhit()) {
	//		//std::cin.ignore((std::numeric_limits< std::streamsize >::max)(), '\n');
	//		//std::cin >> input_str;		
	//		scanf_s("%s", str, 49);
	//		input_str = std::string(str);
	//		//FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	//		return input_str;
	//	}
	//	else if (time(0) - timeBegin >= lastime) {
	//		break;
	//	}
	//};

	flag = 1;                    //��flagΪ1����������
	ResumeThread(hThread1);      //���¼����߳�
	Sleep(lastime * 1000);
	SuspendThread(hThread1);     //�����߳�

	std::cout << "��������˵��ʱ�����\n";
}
