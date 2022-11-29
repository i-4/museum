#include "museum.h"
#include "solverBase.h"
#include <time.h>
#include<iostream>

int main()
{
	try {
		std::string fileName;
		std::cout << "ファイルのパスを指定してください。拡張子は省略してください。" << std::endl;
		std::cout << "例：Quiz\\ex5" << std::endl;
		std::cin >> fileName;
		auto museum = make_museum_form_csv(fileName + ".csv");
		const auto start = clock();
		const auto solvedMuseum = SolverBase(museum).solve();
		std::cout << "問題を解き終わりました。" << std::endl;
		std::cout << "経過時間は" << double(clock() - start) / CLOCKS_PER_SEC << "[s] でした" << std::endl;
		output_museum_to_csv(solvedMuseum, fileName + "_solved.csv");
	}
	catch (const std::exception e) {
		std::cout << e.what() << std::endl;
	}
}