#include <algorithm>
#include <cinttypes>
#include <iostream>

int main()
{
	std::string line;
	std::int32_t total{};
	while (std::getline(std::cin, line))
	{
		auto pred = [](const auto &x) { return '0' <= x && x <= '9'; };
		auto first = std::find_if(line.cbegin(), line.cend(), pred);
		auto last = std::find_if(line.crbegin(), line.crend(), pred);

		if (first != line.cend())
		{
			total += (*first - '0') * 10;
		}
		if (last != line.crend())
		{
			total += (*last - '0');
		}
	}
	std::cout << total << "\n";
}
