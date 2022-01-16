#include "interpreter.h"
#include<Windows.h>
#include <conio.h>
#include <thread>
#include <chrono>
#include<windows.h>
#include<process.h>
#include<fstream>


HANDLE hThread1;                 //创建线程
#define maxsize 50               //字符串最大长度
char str[maxsize];               //以char的方式存储字符串
std::string input_str = "";      //存放读取的字符串
int flag = 1;                    //flag为1表示可以读取，为0则不能读取

//工作线程，完成读取输入操作
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

//初始化，从database中读数据
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

//翻译过程
void interpreter::run() {
	p.parse_file("test.rsl");
	init();
	tstep = p.Script.steps[p.Script.entry];
	if (var_table.find(key_words) == var_table.end()) {
		std::cout << "用户不存在\n";
		return;
	}
	hThread1 = (HANDLE)_beginthreadex(NULL, 0, Fun1Proc, NULL, 0, NULL);
	SuspendThread(hThread1);
	int repeat = 0;        //repeat记录沉默的次数，连续超过三次则退出
	std::string ans = "";
	while (true) {
		inter_expression(tstep.expression);   //先翻译speak
		std::cout << "\n";
		if (tstep.exit == 1) {                //如果是结束步骤则断循环
			break;
		}
		
		inter_listen(tstep.listen);
		ans = input_str;
		if (ans == "") {                                                //如果什么都没读到为silence
			if (repeat < 3) {
				repeat++;
			}
			else {
				break;
			}
			tstep = p.Script.steps[tstep.silence_to];
		}
		else if (tstep.ans_step.find(ans) != tstep.ans_step.end()) {    //如果在branch中能找到
			repeat = 0;
			tstep = p.Script.steps[tstep.ans_step[ans]];
		}
		else {                                                          //如果无法识别要求则转dafault
			repeat = 0;
			tstep = p.Script.steps[tstep.default_to];
		}
	}
	std::cout << "程序结束\n";
}

//翻译表达式内容
//参数：表达式vector
void interpreter::inter_expression(std::vector<std::string>& exps) {
	for (int i = 0; i < exps.size(); i++) {
		if (exps[i][0] == '"') {         //如果是字符串打印字符串内容
			std::string var(++exps[i].begin(), --exps[i].end());
			std::cout << var;
		}
		else if (exps[i][0] == '$') {   //如果是变量则去符号表中找对应的量
			std::string var(++exps[i].begin(), exps[i].end());
			if (var_table[key_words].find(var) != var_table[key_words].end()) {
				std::cout << var_table[key_words][var];
			}
			else {
				std::cout << "Can't find variable!\n";
			}		
		}
		else {                         //否则则出错
			std::cout << "Wrong!\n";
		}
	}
}

//读取输入操作
//参数：听取时间的vector
void interpreter::inter_listen(std::vector<int> listen) {
	Sleep(listen[0] * 1000);
	std::cout << "滴~~~\n";
	int lastime = listen[1] - listen[0];
	input_str = "";

	//采用键盘识别的方法
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

	flag = 1;                    //置flag为1，激活输入
	ResumeThread(hThread1);      //重新激活线程
	Sleep(lastime * 1000);
	SuspendThread(hThread1);     //悬挂线程

	std::cout << "您的语言说话时间结束\n";
}
