#pragma once

#include "nes.h"

class PpuWidget {
public:
    explicit PpuWidget(Nes *nes);

    void draw();

private:
    uint8_t try_get_ppu_mem(const uint16_t addr);
    Nes *nes_{nullptr};
};