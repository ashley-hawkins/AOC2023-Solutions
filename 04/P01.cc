#include <cstdint>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <string_view>

int main()
{
	std::int32_t tally{};
	for (std::string line; std::getline(std::cin, line);)
	{
		auto colon_idx = line.find(':');
		auto pipe_idx = line.find('|');
		auto winning_numbers_sv = std::string_view{line}.substr(colon_idx + 1, pipe_idx);
		auto our_numbers_sv = std::string_view{line}.substr(pipe_idx + 1);

		std::set<std::int32_t> winning_numbers;
		{
			std::istringstream iss{std::string{winning_numbers_sv}};
			for (std::int32_t num{}; iss >> num;)
			{
				winning_numbers.insert(num);
			}
		}
		{
			std::istringstream iss{std::string{our_numbers_sv}};
			std::int32_t this_card_total_winning_numbers{};
			for (std::int32_t num{}; iss >> num;)
			{
				if (winning_numbers.contains(num))
				{
					++this_card_total_winning_numbers;
				}
			}
			if (this_card_total_winning_numbers != 0)
			{
				tally += 1 << (this_card_total_winning_numbers - 1);
			}
		}
	}
	std::cout << tally << '\n';
}
