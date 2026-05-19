#include "KeyProcessor.hpp"

KeyProcessor::KeyProcessor() : currentMode(Mode::ORIGINAL) {}

void KeyProcessor::processKey(int key) {
    switch (key) {
        case '1':
            currentMode = Mode::ORIGINAL;
            break;
        case '2':
            currentMode = Mode::INVERSION;
            break;
        case '3':
            currentMode = Mode::BLUR;
            break;
        case '4':
            currentMode = Mode::CANNY;
            break;
        default:
            break;
    }
}

KeyProcessor::Mode KeyProcessor::getMode() const {
    return currentMode;
}