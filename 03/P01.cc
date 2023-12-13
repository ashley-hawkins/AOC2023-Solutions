#include <cinttypes>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>

int main()
{
	std::deque<std::string> lines;
	std::int32_t tally{};
	auto process_line = [&lines, &tally](std::size_t idx) {
		auto is_symbol = [](char chr) {
			return chr != '.' && !std::isalnum(chr, std::locale::classic());
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

		bool is_part_number = false;
		std::int32_t current_number{};
		auto check = [&is_symbol, &theLine, &aboveLine, &belowLine](std::size_t pos) {
			if (pos != decltype(pos)(-1))
			{
				if ((is_symbol(theLine.at(pos))) || (!aboveLine.empty() && is_symbol(aboveLine.at(pos))) ||
				    (!belowLine.empty() && is_symbol(belowLine.at(pos))))
				{
					return true;
				}
			}
			if (pos < theLine.size())
			{
				if ((is_symbol(theLine.at(pos))) || (!aboveLine.empty() && is_symbol(aboveLine.at(pos))) ||
				    (!belowLine.empty() && is_symbol(belowLine.at(pos))))
				{
					return true;
				}
			}
			return false;
		};
		for (std::size_t i = 0; auto c : theLine)
		{
			if (std::isdigit(c, std::locale::classic()))
			{
				constexpr auto radix = 10;
				current_number *= radix;
				current_number += c - '0';
				if (!is_part_number && check(i - 1))
				{
					is_part_number = true;
				}
			}
			else
			{
				if (!is_part_number && (check(i - 1) || check(i)))
				{
					is_part_number = true;
				}

				if (is_part_number)
				{
					tally += current_number;
				}
				current_number = 0;
				is_part_number = false;
			}
			++i;
		}
		if (is_part_number)
		{
			tally += current_number;
		}
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
	std::cout << tally << '\n';
}
