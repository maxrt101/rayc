#ifndef _RAYC_UTILS_DIE_H
#define _RAYC_UTILS_DIE_H

#include <cstdlib>
#include <functional>

namespace rayc {

using OnDieCallback = std::function<void()>;

[[noreturn]] void die(int exitCode = EXIT_FAILURE);
void registerOnDie(OnDieCallback cb);

} /* namespace rayc */

#endif