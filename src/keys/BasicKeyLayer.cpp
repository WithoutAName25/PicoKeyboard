#include "BasicKeyLayer.h"

#include <utility>

extern RGBController rgbController;

IKeyAction *BasicKeyLayer::getAction(const uint8_t keyId) const { return actions[keyId].get(); }

void BasicKeyLayer::enable() {
    if (effect != nullptr) {
        if (enabled)
            return;

        overlayEffect = rgbController.addOverlayEffect(effect);
        enabled = true;
    }
}

void BasicKeyLayer::disable() {
    if (!enabled)
        return;

    rgbController.removeOverlayEffect(overlayEffect);
    enabled = false;
}

BasicKeyLayer::BasicKeyLayer(const uint8_t numKeys, std::shared_ptr<IRGBEffect> effect)
    : KeyLayer(numKeys), actions(std::make_unique<std::unique_ptr<IKeyAction>[]>(numKeys)), effect(std::move(effect)) {}
