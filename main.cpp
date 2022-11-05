#include <cmath>
#include <functional>
#include <iostream>

double singleParamScheme(
    double y_0,
    std::function<double()> step,
    size_t numIter)
{
	for (size_t i = 0; i < numIter; ++i)
		y_0 = (std::abs(y_0 <= std::numeric_limits<double>::epsilon())) ? 0.0 : y_0 * step();

	return y_0;
}

double doubleParamScheme(
    double y_0,
    double y_1,
    std::function<double(double, double)> step,
    size_t numIter)
{
	for (size_t i = 0; i < numIter; ++i) {
		double tmp = y_1;
		y_1 = step(y_0, y_1);
		y_0 = tmp;
	}
	return y_1;
}

void printResult(double schemeResult, double trueResult)
{
	std::cout << std::scientific
	          << std::abs(schemeResult - trueResult)
	          << std::endl;
}

int main(int argc, char** argv)
{
	if (argc != 4)
		return -1;

	size_t scheme = std::stoull(argv[1]);
	size_t n = std::stoull(argv[2]);
	double A = std::stod(argv[3]);
	double h = std::pow(10.0, -static_cast<double>(n));
	size_t numIters = std::pow(10, n);

	std::vector<std::function<double()>> singleStep = {
		[A, h]() { return 1.0 - A * h; },
		[A, h]() { return 1 / (A * h + 1.0); },
		[A, h]() { return (1.0 - A * h / 2.0) / (1.0 + A * h / 2.0); },
	};

	std::vector<std::function<double(double, double)>> doubleStep = {
		[A, h](double y_0, double y_1) { return y_0 - 2.0 * h * A * y_1; },
		[A, h](double y_0, double y_1) { return (2 * y_1 - 0.5 * y_0) / (1.5 + A * h); },
		[A, h](double y_0, double y_1) { return (2 * y_1 + (A * h - 1.5) * y_0) / 0.5; },
	};

	switch (scheme) {
	case 1 ... 3:
		printResult(singleParamScheme(1.0, singleStep[scheme - 1], numIters), std::exp(-A));
		break;
	case 4 ... 6:
		printResult(doubleParamScheme(1.0, 1.0 - A * h, doubleStep[scheme - 4], numIters - 1), std::exp(-A));
		break;
	}

	return 0;
}