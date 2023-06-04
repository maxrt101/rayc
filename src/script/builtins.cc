#include <rayc/script/builtins.h>
#include <rayc/script/command.h>
#include <rayc/script/driver.h>
#include <rayc/utils/string.h>

#include <rayc/app.h>
#include <rayc/log.h>
#include <rayc/data/resources.h>

#include <iostream>
#include <fstream>
#include <sstream>

#define _FAIL(ret, msg) \
  do { \
    result.returnCode = ret; \
    result.output = msg; \
    return result; \
  } while (0)


#define _READ2INT() \
  int a = 0, b = 0; \
  do { \
    if (!stoi(args[1], a) || !stoi(args[3], b)) { \
      _FAIL(2, "Invalid numbers"); \
    } \
  } while (0)


rayc::CommandResult rayc::script::builtin_quit(Environment& env, const CommandArgs& args) {
  CommandResult result;

  app::stop();

  return result;
}

rayc::CommandResult rayc::script::builtin_execute(Environment& env, const CommandArgs& args) {
  CommandResult result;

  if (args.size() != 2) {
    _FAIL(1, "Usage: execute FILE\n");
  }

  script::ExecutionContext ctx {
    env,
    [&result](const std::string& s) {
      result.output += s + "\n";
    }
  };

  std::ifstream file(args[1]);
  std::stringstream ss;
  ss << file.rdbuf();
  file.close();

  script::run(&ctx, ss.str());

  return result;
}

rayc::CommandResult rayc::script::builtin_set(Environment& env, const CommandArgs& args) {
  CommandResult result;

  if (args.size() != 3) {
    _FAIL(1, "Usage: set NAME VALUE\n");
  }

  env[args[1]] = args[2];

  return result;
}

rayc::CommandResult rayc::script::builtin_unset(Environment& env, const CommandArgs& args) {
  CommandResult result;

  if (args.size() != 2) {
    _FAIL(1, "Usage: unset NAME\n");
  }

  env.erase(args[1]);

  return result;
}

rayc::CommandResult rayc::script::builtin_env(Environment& env, const CommandArgs& args) {
  CommandResult result;

  if (args.size() == 1) {
    for (auto& [k, v] : env) {
      result.output += k + "=" + v + "\n";
    }
  } else {
    _FAIL(1, "Usage: env\n");
  }

  return result;
}

rayc::CommandResult rayc::script::builtin_test(Environment& env, const CommandArgs& args) {
  CommandResult result;

  if (args.size() < 3) {
    _FAIL(2, "Usage: test [expr]\n");
  }

  if (args.size() == 3) {
    if (args[1] == "-n") {
      result.returnCode = (args[2].size() != 0) ? 0 : 1;
    } else if (args[1] == "-z") {
      result.returnCode = (args[2].size() == 0) ? 0 : 1;
    } else {
      _FAIL(2, "Unimplemented\n");
    }
  } else if (args.size() == 4) {
    if (args[2] == "==" || args[2] == "-eq") {
      result.returnCode = (args[1] == args[3]) ? 0 : 1;
    } else if (args[2] == "!=" || args[2] == "-ne") {
      result.returnCode = (args[1] != args[3]) ? 0 : 1;
    } else if (args[2] == ">" || args[2] == "-gt") {
      _READ2INT();
      result.returnCode = (a > b) ? 0 : 1;
    } else if (args[2] == "<" || args[2] == "-lt") {
      _READ2INT();
      result.returnCode = (a < b) ? 0 : 1;
    } else {
      _FAIL(2, "Unimplemented\n");
    }
  } else {
    _FAIL(2, "Unimplemented\n");
  }

  return result;
}

rayc::CommandResult rayc::script::builtin_expr(Environment& env, const CommandArgs& args) {
  CommandResult result;

  if (args.size() < 3) {
    _FAIL(2, "Usage: test [expr]\n");
  }

  if (args.size() == 3) {
    if (args[1] == "-") {
      int x = 0;
      if (!stoi(args[2], x)) {
        _FAIL(2, "Invalid number");
      }
      result.output = std::to_string(-x);
    } else {
      _FAIL(2, "Unimplemented\n");
    }
  } else if (args.size() == 4) {
    if (args[2] == "+") {
      _READ2INT();
      result.output = std::to_string(a + b);
    } else if (args[2] == "-") {
      _READ2INT();
      result.output = std::to_string(a - b);
    } else if (args[2] == "*") {
      _READ2INT();
      result.output = std::to_string(a * b);
    } else if (args[2] == "/") {
      _READ2INT();
      result.output = std::to_string(a / b);
    } else if (args[2] == "#") {
      int x = 0;
      if (!stoi(args[3], x)) {
        _FAIL(2, "Invalid number");
      }
      result.output += args[1][x];
    } else if (args[2] == "&") {
      _READ2INT();
      result.output = std::to_string(a & b);
    } else if (args[2] == "|") {
      _READ2INT();
      result.output = std::to_string(a | b);
    } else if (args[2] == ">>") {
      _READ2INT();
      result.output = std::to_string(a >> b);
    } else if (args[2] == "<<") {
      _READ2INT();
      result.output = std::to_string(a << b);
    } else {
      _FAIL(2, "Unimplemented\n");
    }
  } else {
    _FAIL(2, "Unimplemented\n");
  }

  return result;
}

rayc::CommandResult rayc::script::builtin_clear(Environment& env, const CommandArgs& args) {
  CommandResult result;

  console::clear();

  return result;
}

rayc::CommandResult rayc::script::builtin_print(Environment& env, const CommandArgs& args) {
  CommandResult result;

  for (int i = 1; i < args.size(); i++) {
    result.output += args[i];
    if (i + 1 < args.size()) {
      result.output += " ";
    }
  }

  return result;
}

rayc::CommandResult rayc::script::builtin_read(Environment& env, const CommandArgs& args) {
  CommandResult result;

  _FAIL(2, "Unimplemented");

  return result;
}

rayc::CommandResult rayc::script::builtin_log(Environment& env, const CommandArgs& args) {
  CommandResult result;

  if (args.size() < 2) {
    _FAIL(1, "Usage: log <LEVEL> <ARGS>");
  }

  std::string msg;
  for (size_t i = 2; i < args.size(); i++) {
    msg += args[i] + " ";
  }

  logf(stringToLogLevel(args[1]), msg);

  return result;
}

rayc::CommandResult rayc::script::builtin_loglvl(Environment& env, const CommandArgs& args) {
  CommandResult result;

  if (args.size() == 1) {
    result.output = logLevelToString(getLogLevel());
  } else if (args.size() == 2) {
    setLogLevel(stringToLogLevel(args[1]));
  } else {
    _FAIL(1, "Usage: loglvl [level]");
  }

  return result;
}

rayc::CommandResult rayc::script::builtin_map(Environment& env, const CommandArgs& args) {
  CommandResult result;

  if (args.size() < 2) {
    _FAIL(2, "Usage: map FILE");
  }

  auto view = app::getView("renderer");

  if (!view) {
    _FAIL(3, "No such view");
  }

  view->onEvent("run", {{"map", args[1]}});

  return result;
}

rayc::CommandResult rayc::script::builtin_data(Environment& env, const CommandArgs& args) {
  CommandResult result;

  if (args.size() < 2) {
    _FAIL(2, "Usage: data [load|show|clear] <ARGS>");
  }

  if (args[1] == "load") {
    if (args.size() != 3) {
      _FAIL(2, "Usage: data load FILE");
    }
    loadPak(args[2]);
  } else if (args[1] == "show") {
    printPak();
  } else if (args[1] == "clear") {
    clearResources();
  } else {
    _FAIL(2, "Unimplemented");
  }

  return result;
}

rayc::CommandResult rayc::script::builtin_view(Environment& env, const CommandArgs& args) {
  CommandResult result;

  if (args.size() < 3) {
    _FAIL(2, "Usage: view toggle <VIEW>");
  }

  if (args[1] == "toggle") {
    auto view = app::getView(args[2]);
    if (!view) {
      _FAIL(3, "No such view");
    }
    view->setActive(!view->isActive());
  } else {
    _FAIL(2, "Unimplemented");
  }

  return result;
}

rayc::CommandResult rayc::script::builtin_sendEvent(Environment& env, const CommandArgs& args) {
  CommandResult result;

  if (args.size() < 3) {
    _FAIL(2, "Usage: send-event <VIEW> <EVENT> <ARGS>");
  }

  auto viewName = args[1];
  auto eventName = args[2];

  std::map<std::string, std::string> eventArgs;

  if (args.size() == 4) {
    for (auto& kvstr : strsplit(args[3], ",")) {
      auto kv = strsplit(kvstr, "=");
      eventArgs[kv[0]] = kv[1];
    }
  }

  auto view = app::getView(viewName);

  if (!view) {
    _FAIL(3, "No such view");
  }

  view->onEvent(eventName, eventArgs);

  return result;
}

rayc::CommandResult rayc::script::builtin_alert(Environment& env, const CommandArgs& args) {
  CommandResult result;

  if (args.size() < 3) {
    _FAIL(2, "Usage: alert <DURATION> <MSG> ");
  }

  auto duration = args[1];
  auto msg = args[2];


  auto view = app::getView("alert");

  if (!view) {
    _FAIL(3, "No view 'alert' found");
  }

  view->onEvent("add", {{"msg", msg}, {"duration", duration}});

  return result;
}

rayc::CommandResult rayc::script::builtin_tp(Environment& env, const CommandArgs& args) {
  CommandResult result;

  _FAIL(2, "Unimplemented");

  return result;
}

rayc::CommandResult rayc::script::builtin_spawn(Environment& env, const CommandArgs& args) {
  CommandResult result;

  _FAIL(2, "Unimplemented");

  return result;
}

rayc::CommandResult rayc::script::builtin_kill(Environment& env, const CommandArgs& args) {
  CommandResult result;

  _FAIL(2, "Unimplemented");

  return result;
}

rayc::CommandResult rayc::script::builtin_ai(Environment& env, const CommandArgs& args) {
  CommandResult result;

  _FAIL(2, "Unimplemented");

  return result;
}
