#include<cstdio>
#include<iostream>
#include"parser.h"
#include"interpreter.h"

int main(int argc ,char ** argv) {

	if (argc < 3) {
		std::cout << "missing parameter\n";
	}
	else {
		std::string t_arg = std::string(argv[1]);
		if (t_arg == "-k" || t_arg == "--key") {
			interpreter inter;
			inter.key_words = argv[2];
			inter.run();
		}
		else {
			std::cout << "wrong parameter\n";
		}
	}

	return 0;
}