#include "parser.h"
#include<fstream>

//�����ű�
//�������ļ���
void parser::parse_file(std::string filename) {
	std::fstream f;
	f.open(filename);
	while (!f.eof()) {
		std::string line;
		std::cin.unsetf(std::ios::skipws);   //��ȡʱ�����հ�
		std::getline(f, line);
		trim(line);     //������β�Ŀո�
		if (line != "" && line[0] != '#' && line != "\n" && line != "\t") {  //���ֿ��л�ע��������
			parse_line(line);
		}
	}
	if (old_stepid != "") {
		Script.steps[old_stepid] = tempstep;
	}
	f.close();
	//test_parser();
}

//ɾ����β�հ��ַ�
//������Ҫ������ַ���
void parser::trim(std::string & s)
{
     if (!s.empty())
     {
         s.erase(0, s.find_first_not_of(" "));
         s.erase(s.find_last_not_of(" ") + 1);
     }
}

//����һ��
//������ĳһ���ַ���
void parser::parse_line(std::string line) {
	std::vector<std::string> tokens;
	std::string temp="";
	for (int i = 0; i < line.size(); i++) {
		if (line[i] == '#') {      //���ʶ����βע����ֱ���˳�
			break;
		}
		if (line[i] !=  ' ') {
			temp += line[i];
		}
		else {
			if (temp != "") {
				tokens.push_back(temp);
			}
			temp = "";
		}
	}
	tokens.push_back(temp);
	process_tokens(tokens);
}

//����һ���е�token���
//������һ���е�����token
void parser::process_tokens(std::vector<std::string>& tokens) {
	std::string token_0 = tokens[0];  //�Ƚ�token[0]��¼��������ֹ�������
	if (token_0 == "Step") {
		process_step(tokens[1]);
	}
	else if (token_0 == "Speak") {
		std::vector<std::string> var(++tokens.begin(), tokens.end());
		process_speak(var);
	}
	else if (token_0 == "Listen") {
		process_listen(string_to_int(tokens[1]), string_to_int(tokens[2]));
	}
	else if (token_0 == "Branch") {
		process_branch(tokens[1], tokens[2]);
	}
	else if (token_0 == "Silence") {
		process_silence(tokens[1]);
	}
	else if (token_0 == "Default") {
		process_default(tokens[1]);
	}
	else if (token_0 == "Exit") {
		process_exit();
	}
	else {
		std::cout << "Wrong string!\n";
	}
}

//step��������
//������������
void parser::process_step(std::string stepid) {
	if (old_stepid != "") {
		Script.steps[old_stepid] = tempstep;		
	}
	step newstep;
	//�����ʱ�ǵ�һ��step����������Ϊ���
	tempstep = newstep;
	old_stepid = stepid;
	if (Script.steps.size() == 0) {
		Script.entry = stepid;
	}
}

//�����������
//��������Ҫ���������token
void parser::process_speak(std::vector<std::string>& tokens) {
	process_expression(tokens);
}

//����������
//��������Ҫ���������token
void parser::process_expression(std::vector<std::string>& tokens) {
	for (int i = 0; i < tokens.size(); i++) {
		//�����ʱ�Ǳ���
		if (tokens[i][0] == '$') {
			Script.vars.push_back(tokens[i].substr(1));
			tempstep.expression.push_back(tokens[i]);

		}
		//�����ʱ�ǡ������ַ���
		else if (tokens[i][0] == '"') {
			tempstep.expression.push_back(tokens[i]);
		}
		//���Ե�+��
		else if (tokens[i][0] == '+') {
			continue;
		}
		else {
			std::cout << "Wrong expression!\n";
		}
	}
}

//��ȡ��������
//��������ʼ������������������
void parser::process_listen(int start_timer, int stop_timer) {
	tempstep.listen.resize(2);
	tempstep.listen[0] = start_timer;
	tempstep.listen[1] = stop_timer;
}

//��֧����
//�������ش���ת������
void parser::process_branch(std::string answer, std::string next_stepid) {
	std::string var(++answer.begin(), --answer.end());   //ȥ��ǰ������
	tempstep.ans_step[var] = next_stepid;
}

//��������
//����������ʱ��ת������
void parser::process_silence(std::string next_stepid) {
	tempstep.silence_to = next_stepid;
}

//Ĭ�Ϲ���
//������Ĭ��ʱ��ת������
void parser::process_default(std::string next_stepid) {
	tempstep.default_to = next_stepid;
}

//�˳�����
void parser::process_exit() {
	tempstep.exit = 1;
}

//�ַ���ת����
//��������ת�ַ���
int parser::string_to_int(std::string str) {
	int ans = 0;
	for (int i = 0; i < str.size(); i++) {
		ans = ans * 10 + str[i] - '0';
	}
	return ans;
}

//����parser
void parser::test_parser() {
	auto it = Script.steps.begin();
	for (it; it != Script.steps.end(); it++) {
		step tstep=(*it).second;
		std::cout << tstep.expression.size() << "\n" << tstep.listen.size()<<"\n";
		std::cout << tstep.silence_to << "\n" << tstep.default_to << "\n\n";

	}
}