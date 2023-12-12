#include <algorithm>
#include <asm-generic/errno.h>
#include <cinttypes>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <optional>
#include <sstream>
#include <string_view>

std::optional<std::int32_t> get_game_power(std::string_view list)
{
	std::int32_t max_red = 0;
	std::int32_t max_green = 0;
	std::int32_t max_blue = 0;
	while (!list.empty())
	{
		auto next_semicolon_idx = list.find(';');
		auto this_set = std::string_view{list}.substr(0, next_semicolon_idx);
		if (next_semicolon_idx == std::string_view::npos)
		{
			list = "";
		}
		else
		{
			list = std::string_view{list}.substr(next_semicolon_idx + 1);
		}
		while (!this_set.empty())
		{
			auto next_comma_idx = this_set.find(',');
			auto this_element = std::string_view{this_set}.substr(0, next_comma_idx);
			if (next_comma_idx == std::string_view::npos)
			{
				this_set = "";
			}
			else
			{
				this_set = std::string_view{this_set}.substr(next_comma_idx + 1);
			}
			std::istringstream iss{std::string(this_element)};
			std::int32_t amount;
			std::string colour;
			iss >> amount >> colour;
			if (!iss)
			{
				return std::nullopt;
			}
			if (colour == "red")
			{
				max_red = std::max(max_red, amount);
			}
			else if (colour == "green")
			{
				max_green = std::max(max_green, amount);
			}
			else if (colour == "blue")
			{
				max_blue = std::max(max_blue, amount);
			}
		}
	}
	return max_red * max_green * max_blue;
}

int main()
{
	std::int32_t tally{};
	std::string line;
	while (std::getline(std::cin, line))
	{
		auto colon_idx = line.find(':');
		auto first_part = std::string_view{line}.substr(sizeof("Game") - 1, colon_idx);
		auto second_part = std::string_view{line}.substr(colon_idx + 1);

		std::int32_t game_id;
		{
			std::istringstream iss{std::string(first_part)};
			iss >> game_id;
			if (!iss)
			{
				return EXIT_FAILURE;
			}
		}
		auto res = get_game_power(second_part);
		if (res)
		{
			tally += *res;
		}
		else
		{
			return EXIT_FAILURE;
		}
	}
	std::cout << tally << '\n';
}
