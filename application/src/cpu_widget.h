#pragma once

#include <cstdint>

namespace n_e_s::nes {
class Nes;
} // namespace n_e_s::nes

class CpuWidget {
public:
    explicit CpuWidget(n_e_s::nes::Nes *nes);

    void draw();

private:
    uint16_t jump_to_address_{0u};
    n_e_s::nes::Nes *nes_{nullptr};
};