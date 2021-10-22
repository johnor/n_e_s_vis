#pragma once

#include <cstdint>

namespace n_e_s::nes {
class Nes;
} // namespace n_e_s::nes

namespace nesvis {

class Control;

class CpuWidget {
public:
    CpuWidget(n_e_s::nes::Nes *nes, Control *control);

    void draw();

private:
    int renderer_{0};
    uint16_t jump_to_address_{0u};
    n_e_s::nes::Nes *nes_{nullptr};
    Control *control_{nullptr};
};

} // namespace nesvis
