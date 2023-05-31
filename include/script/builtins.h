#ifndef _RAYC_SCRIPT_BUILTINS_H_
#define _RAYC_SCRIPT_BUILTINS_H_ 1

#include <rayc/script/command.h>

namespace rayc {
namespace script {

CommandResult builtin_quit(Environment& env, const CommandArgs& args);
// Basic Commands
CommandResult builtin_execute(Environment& env, const CommandArgs& args);
CommandResult builtin_set(Environment& env, const CommandArgs& args);
CommandResult builtin_unset(Environment& env, const CommandArgs& args);
CommandResult builtin_env(Environment& env, const CommandArgs& args);
CommandResult builtin_test(Environment& env, const CommandArgs& args);
CommandResult builtin_expr(Environment& env, const CommandArgs& args);
CommandResult builtin_clear(Environment& env, const CommandArgs& args);
CommandResult builtin_print(Environment& env, const CommandArgs& args);
CommandResult builtin_read(Environment& env, const CommandArgs& args);
CommandResult builtin_log(Environment& env, const CommandArgs& args);
CommandResult builtin_loglvl(Environment& env, const CommandArgs& args);
// Meta Game Commands
CommandResult builtin_map(Environment& env, const CommandArgs& args);
CommandResult builtin_data(Environment& env, const CommandArgs& args);
CommandResult builtin_view(Environment& env, const CommandArgs& args);
CommandResult builtin_sendEvent(Environment& env, const CommandArgs& args);
// Game Commands
CommandResult builtin_alert(Environment& env, const CommandArgs& args);
CommandResult builtin_tp(Environment& env, const CommandArgs& args);
CommandResult builtin_spawn(Environment& env, const CommandArgs& args);
CommandResult builtin_kill(Environment& env, const CommandArgs& args);
CommandResult builtin_ai(Environment& env, const CommandArgs& args);

} /* namespace script */
} /* namespace rayc */

#endif /* _RAYC_SCRIPT_BUILTINS_H_ */