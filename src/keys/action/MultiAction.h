#pragma once

#include "IKeyAction.h"

class MultiAction final : public IKeyAction {
    std::vector<std::unique_ptr<IKeyAction>> actions;

    template<typename T, typename... Args>
    [[maybe_unused]] void addInstance(T instance, Args... args) {
        actions.emplace_back(std::make_unique<T>(std::move(instance)));
        addInstance(args...);
    }

    static void addInstance() {}

public:
    template<typename... Args>
    explicit MultiAction(Args... args) {
        addInstance(args...);
    }

    void execute(uint8_t keyId, const KeyState* state, absolute_time_t timestamp) override;
};
