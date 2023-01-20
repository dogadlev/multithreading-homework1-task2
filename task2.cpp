#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>
#include <cmath>
#include <exception>


void sumVector(std::vector<int>& vec1, std::vector<int>& vec2, std::vector<int>& vec3, size_t fIndex, size_t sIndex)
{
	for (size_t i = fIndex; i <= sIndex; ++i)
		vec3.at(i) = ((vec1.at(i) + vec2.at(i)));
}

void calc(std::vector<int>& vec1, std::vector<int>& vec2, std::vector<int>& vec3, size_t&& threadNum)
{
	if (vec1.size() != vec2.size() || vec1.empty() || vec2.empty() || vec3.empty() || threadNum == 0)
	{
		throw std::runtime_error("Wrong inputs!");
	}
	else
	{
		std::vector<std::thread> thVec;
		size_t firstIndex = 0;
		size_t secondIndex = vec1.size() / threadNum - 1;
		for (size_t i = 0; i < threadNum; ++i)
		{
			thVec.push_back(std::thread(sumVector, ref(vec1), ref(vec2), ref(vec3), firstIndex, secondIndex));
			firstIndex = secondIndex + 1;
			if (i < threadNum - 2)
				secondIndex += (vec1.size() / threadNum);
			else
				secondIndex = vec1.size() - 1;
		}

		for (auto& t : thVec)
			t.join();
	}
}

std::once_flag infoFlag;
void printInfo()
{
	std::cout << "Hardware core quantity - " << std::thread::hardware_concurrency() << "\n\n";
	std::cout << "\t\t1000\t\t10000\t\t100000\t\t1000000" << std::endl;
}

int main(int argc, char** argv)
{
	try
	{
		int power = 0;
		std::string output = "";
		do {
			int threadsNumber = pow(2, power);
			for (int i = 1000; i <= 1000000; i *= 10)
			{
				call_once(infoFlag, printInfo);

				std::vector<int> v1(i, rand() % 10);
				std::vector<int> v2(i, rand() % 10);
				std::vector<int> v3(i);

				auto start = std::chrono::high_resolution_clock::now();
				calc(v1, v2, v3, threadsNumber);
				auto end = std::chrono::high_resolution_clock::now();

				std::chrono::duration<double, std::ratio<1, 1000>> time = end - start;
				output += (std::to_string(time.count()) + "ms" + "\t");
			}
			std::cout << threadsNumber << " thread(s) " << "\t" << output << std::endl;
			output = "";
			++power;
		} while (power <= 4);
	}
	catch (std::runtime_error& e) { std::cout << e.what() << std::endl; }
	catch (...) { std::cout << "Some exception occured!" << std::endl; }

	return 0;
}