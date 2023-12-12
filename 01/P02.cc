#include <algorithm>
#include <cinttypes>
#include <iostream>
#include <string_view>
#include <utility>

int main()
{
	std::string line;
	std::int32_t total{};
	while (std::getline(std::cin, line))
	{
		auto pred = [](const auto &x) { return '0' <= x && x <= '9'; };
		auto first = std::find_if(line.cbegin(), line.cend(), pred);
		auto last = std::find_if(line.crbegin(), line.crend(), pred);
		auto first_idx = std::distance(line.cbegin(), first);
		auto last_idx = std::distance(line.crbegin(), last);
		last_idx = line.size() - last_idx - 1;

		auto first_search_area = std::string_view{line}.substr(0, first_idx);
		auto last_search_area = std::string_view{line}.substr(last_idx + 1);

		bool first_digit_found = false;
		std::int8_t first_digit_found_value = 0;
		std::string_view::size_type first_digit_found_idx = 0;
		for (std::int8_t i = 0; const std::string_view digit :
		                        {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"})
		{
			auto idx = first_search_area.find(digit);
			if (idx != std::string_view::npos && (!first_digit_found || idx < first_digit_found_idx))
			{
				first_digit_found = true;
				first_digit_found_value = i;
				first_digit_found_idx = idx;
			}
			++i;
		}

		bool last_digit_found = false;
		std::int8_t last_digit_found_value = 0;
		std::string_view::size_type last_digit_found_idx = 0;
		for (std::int8_t i = 0; const std::string_view digit :
		                        {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"})
		{
			auto idx = last_search_area.rfind(digit);
			if (idx != std::string_view::npos && (!last_digit_found || idx > last_digit_found_idx))
			{
				last_digit_found = true;
				last_digit_found_value = i;
				last_digit_found_idx = idx;
			}
			++i;
		}

		if (first_digit_found)
		{
			total += first_digit_found_value * 10;
		}
		else if (first != line.cend())
		{
			total += (*first - '0') * 10;
		}
		if (last_digit_found)
		{
			total += last_digit_found_value;
		}
		else if (last != line.crend())
		{
			total += (*last - '0');
		}
	}
	std::cout << total << "\n";
}
