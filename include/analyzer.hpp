#pragma once

#include <map>
#include <vector>
#include <string>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <algorithm>

class analyzer final
{
private:
    static std::string const forbidden;
    static std::map<std::string, std::size_t> frequency_ratio;
    static std::vector<std::pair<std::string, std::size_t>> frequency;

public:
    analyzer() = default;

public:
    static void
    read(std::basic_istream<char> &stream)
    {
        auto state = stream.rdstate();
        stream.clear();

        std::string word{};
        while (stream >> std::skipws >> word)
        {
            if (auto filtered{filter(word)};
                !filtered.empty())
            {
                frequency_ratio[filtered]++;
            }
        }

        stream.setstate(state);
    }

    static void
    write(std::basic_ostream<char> &stream)
    {
        auto state = stream.rdstate();
        stream.clear();

        transform_to_sorted();

        stream << "┌───────────┬────────────────┬────────────────────┐\n"
               << "│   index   │    frequency   │        word        │\n"
               << "├───────────┼────────────────┼────────────────────┘\n";

        std::size_t index{0UL};
        for (auto const &[word, freq] : frequency)
        {
            stream << std::left << "| " << std::setw(10) << ++index
                   << std::left << "| " << std::setw(15) << freq
                   << std::left << "| " << std::setw(20) << word
                   << '\n';
        }

        stream << "└───────────┴────────────────┴────────────────────┘\n";

        stream.setstate(state);
    }

private:
    static std::string
    filter(std::string &word)
    {
        auto const left{word.find_first_not_of(forbidden)};
        auto const right{word.find_last_not_of(forbidden)};

        if (left != std::string::npos &&
            right != std::string::npos)
        {
            std::transform(std::next(std::cbegin(word), left),
                           std::next(std::cbegin(word), right + 1UL),
                           std::next(std::begin(word), left),
                           [](unsigned char symbol) -> char
                           {
                               return std::tolower(symbol);
                           });

            return word.substr(left, right - left + 1UL);
        }
        return {};
    }

    static inline void
    transform_to_sorted()
    {
        frequency.reserve(std::size(frequency_ratio));

        std::move(std::cbegin(frequency_ratio),
                  std::cend(frequency_ratio),
                  std::back_inserter(frequency));

        auto compare = [](auto const &left, auto const &right)
        { return left.second > right.second; };

        std::sort(std::begin(frequency),
                  std::end(frequency),
                  compare);
    }
};

std::string const analyzer::forbidden{".,:;!?()[]{}*&^%$#@—-\"\' "};
std::map<std::string, std::size_t> analyzer::frequency_ratio{};
std::vector<std::pair<std::string, std::size_t>> analyzer::frequency;
