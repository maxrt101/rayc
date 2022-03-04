#include "core/components/functional.h"

rayc::FunctionalComponent::FunctionalComponent(Context* ctx, int priority, const std::string& name)
    : Component(ComponentType::FUNCTIONAL, priority, ctx, name) {}

rayc::FunctionalComponent::~FunctionalComponent() {}
