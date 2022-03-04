#ifndef _RAYC_CORE_COMPONENTS_FUNCTIONAL_H
#define _RAYC_CORE_COMPONENTS_FUNCTIONAL_H

#include "core/component.h"

namespace rayc {

class FunctionalComponent : public Component {
 public:
  FunctionalComponent(Context* ctx, int priority, const std::string& name);
  ~FunctionalComponent();

 public:
  virtual void onFrameUpdate(float frameTime) = 0;
};

}; /* namespace rayc */

#endif
