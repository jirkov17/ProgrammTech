#include <functional>
#include <limits>
#include <iostream>
#include "Comands.hpp"

int main()
{
  using dictionary = std::multimap< std::string, std::string >;
  std::map< std::string, dictionary > dicts;
  std::map< std::string, std::function< void(std::map< std::string, dictionary >&, std::istream&, std::ostream&) > > commands;
  {
    using namespace std::placeholders;
    commands["help"] = std::bind(jirkov::help, _2, _3);
    commands["create"] = std::bind(jirkov::createDict, _1, _2, _3);
    commands["remove"] = std::bind(jirkov::removeDict, _1, _2, _3);
    commands["print"] = std::bind(jirkov::printDict, _1, _2, _3);
    commands["sort"] = std::bind(jirkov::sortDict, _1, _2, _3);
    commands["write"] = std::bind(jirkov::writeToFile, _1, _2, _3);
    commands["top"] = std::bind(jirkov::printTop, _1, _2, _3);
  }
  auto warningInvCom = std::bind(jirkov::warning, std::placeholders::_1, "<INVALID COMMAND>\n");
  std::string cmd;
  while (std::cin >> cmd && cmd != "save")
  {
    try
    {
      commands.at(cmd)(dicts, std::cin, std::cout);
    }
    catch (const std::invalid_argument& e)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    catch (const std::out_of_range& e)
    {
      warningInvCom(std::cout);
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}
