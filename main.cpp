#include "museum.h"
#include "solverBase.h"
#include <time.h>
#include<iostream>

int main()
{
	try {
		std::string fileName;
		std::cout << "�t�@�C���̃p�X���w�肵�Ă��������B�g���q�͏ȗ����Ă��������B" << std::endl;
		std::cout << "��FQuiz\\ex5" << std::endl;
		std::cin >> fileName;
		auto museum = make_museum_form_csv(fileName + ".csv");
		const auto start = clock();
		const auto solvedMuseum = SolverBase(museum).solve();
		std::cout << "���������I���܂����B" << std::endl;
		std::cout << "�o�ߎ��Ԃ�" << double(clock() - start) / CLOCKS_PER_SEC << "[s] �ł���" << std::endl;
		output_museum_to_csv(solvedMuseum, fileName + "_solved.csv");
	}
	catch (const std::exception e) {
		std::cout << e.what() << std::endl;
	}
}