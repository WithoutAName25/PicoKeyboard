#pragma once

class IPressReleaseAction {
public:
    virtual void press() = 0;

    virtual void release() = 0;
};
