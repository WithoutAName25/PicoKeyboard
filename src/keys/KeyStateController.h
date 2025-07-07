#pragma once

#include "pico/stdlib.h"
#include <functional>
#include <list>
#include <memory>

struct KeyState {
    bool isPressed = false;
    absolute_time_t pressTime = 0;
    absolute_time_t releaseTime = 0;
    uint32_t totalPressCount = 0;
};

enum class ListenerPriority { BEFORE_KEYBOARD_CONTROLLER, WITH_KEYBOARD_CONTROLLER, AFTER_KEYBOARD_CONTROLLER, Count };

class IKeyStateListener {
public:
    virtual ~IKeyStateListener() = default;

    [[nodiscard]] virtual ListenerPriority getPriority() const;

    virtual void onKeyStateChange(uint8_t keyId, const KeyState &state, absolute_time_t timestamp) = 0;
};

using KeyStateListenerReference = std::pair<ListenerPriority, std::list<IKeyStateListener *>::iterator>;

class KeyStateController {
    std::unique_ptr<KeyState[]> keyStates;
    std::array<std::list<IKeyStateListener *>, static_cast<size_t>(ListenerPriority::Count)> listeners;
    std::vector<KeyStateListenerReference> removeQueue;

public:
    explicit KeyStateController(uint8_t numKeys);

    virtual ~KeyStateController() = default;

    virtual void updateKeyState(uint8_t keyId, bool isPressed, absolute_time_t timestamp, bool localKey);

    KeyState &getKeyState(uint8_t keyId);

    KeyStateListenerReference addKeyStateListener(IKeyStateListener *listener);

    void removeKeyStateListener(const KeyStateListenerReference &listenerReference);
};
