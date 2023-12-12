#include <algorithm>
#include <cinttypes>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <optional>
#include <sstream>
#include <string_view>

constexpr int max_red = 12;
constexpr int max_green = 13;
constexpr int max_blue = 14;

std::optional<bool> check_setlist_validity(std::string_view list)
{

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
			if (colour == "red" && amount > max_red)
			{
				return false;
			}
			else if (colour == "green" && amount > max_green)
			{
				return false;
			}
			else if (colour == "blue" && amount > max_blue)
			{
				return false;
			}
		}
	}
	return true;
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
		auto res = check_setlist_validity(second_part);
		if (res)
		{
			if (*res)
			{
				tally += game_id;
			}
		}
		else
		{
			return EXIT_FAILURE;
		}
	}
	std::cout << tally << '\n';
}
