#include <cinttypes>
#include <cstdlib>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <string>

int main()
{
	std::map<std::pair<std::size_t, std::size_t>, std::vector<int32_t>> gears;
	std::deque<std::string> lines;
	std::size_t line_num{};
	auto process_line = [&line_num, &lines, &gears](std::size_t idx) {
		auto is_symbol = [](char c) {
			return c == '*';
		};
		std::string_view theLine = lines.at(idx);
		std::string_view aboveLine;
		std::string_view belowLine;
		if (0 < idx)
		{
			aboveLine = lines.at(idx - 1);
		}
		if (idx < lines.size() - 1)
		{
			belowLine = lines.at(idx + 1);
		}

		std::int32_t current_number{};
		std::vector<std::pair<std::size_t, std::size_t>> current_gears;
		auto check = [line_num, &current_gears, &is_symbol, &theLine, &aboveLine, &belowLine](std::size_t i) {
			bool result = false;
			if (i < theLine.size())
			{
				if (is_symbol(theLine.at(i)))
				{
					current_gears.emplace_back(line_num, i);
					result = true;
				}
				if (!aboveLine.empty() && is_symbol(aboveLine.at(i)))
				{
					current_gears.emplace_back(line_num - 1, i);
					result = true;
				}
				if (!belowLine.empty() && is_symbol(belowLine.at(i)))
				{
					current_gears.emplace_back(line_num + 1, i);
					result = true;
				}
			}
			return result;
		};
		for (std::size_t i = 0; auto c : theLine)
		{
			if (std::isdigit(c, std::locale::classic()))
			{
				constexpr auto radix = 10;
				current_number *= radix;
				current_number += c - '0';
				check(i - 1);
			}
			else
			{
				if (i == 0 || std::isdigit(theLine.at(i - 1), std::locale::classic()))
				{
					check(i - 1);
					check(i);
				}

				for (const auto &gear_pos : current_gears)
				{
					gears[gear_pos].push_back(current_number);
				}
				current_number = 0;
				current_gears.clear();
			}
			++i;
		}
		for (const auto &gear_pos : current_gears)
		{
			gears[gear_pos].push_back(current_number);
		}
		++line_num;
	};
	std::string line;
	if (std::getline(std::cin, line))
	{
		lines.push_back(line);
	}
	if (std::getline(std::cin, line))
	{
		lines.push_back(line);
		process_line(0);
	}
	while (std::getline(std::cin, line))
	{
		lines.push_back(line);
		process_line(1);
		lines.pop_front();
	}
	process_line(1);

	std::int32_t tally{};
	for (const auto &[_, num_vec] : gears)
	{
		if (num_vec.size() == 2)
		{
			tally += num_vec.at(0) * num_vec.at(1);
		}
	}
	std::cout << tally << '\n';
}
