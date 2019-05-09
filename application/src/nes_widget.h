#pragma once

#include "nes.h"

class NesWidget {
public:
    NesWidget(Nes *nes);

    void draw();

private:
    uint32_t jump_to_address_{0u};
    Nes *nes_{nullptr};
};