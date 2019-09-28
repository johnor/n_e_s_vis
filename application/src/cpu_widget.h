#pragma once

#include "nes.h"

class CpuWidget {
public:
    explicit CpuWidget(Nes *nes);

    void draw();

private:
    uint16_t jump_to_address_{0u};
    Nes *nes_{nullptr};
};