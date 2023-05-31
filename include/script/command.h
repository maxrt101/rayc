#ifndef _RAYC_SCRIPT_COMMAND_H_
#define _RAYC_SCRIPT_COMMAND_H_ 1

#include <functional>
#include <string>
#include <vector>
#include <map>

namespace rayc {

using Environment = std::map<std::string, std::string>;
using CommandArgs = std::vector<std::string>;

struct CommandResult {
  int returnCode = 0;
  std::string output;
};

using Command = std::function<CommandResult(Environment&, const CommandArgs&)>;

void registerCommand(const std::string& name, Command command);

} /* namespace rayc */

#endif /* _RAYC_SCRIPT_COMMAND_H_ */