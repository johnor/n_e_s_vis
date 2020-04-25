#pragma once

#include "nes/core/imos6502.h"
#include "nes/nes.h"

namespace nesvis {

class Control {
public:
    explicit Control(n_e_s::nes::Nes *nes) : nes_{nes} {}

    bool is_running() const {
        return is_running_;
    };

    void pause() {
        is_running_ = false;
    };

    void run() {
        is_running_ = true;
    };

    // Runs a single step in the ppu/cpu
    void stepi() {
        nes_->execute();
    }

    // Runs until the cpu begins a new instruction
    void step() {
        const auto prev_start_cycle = nes_->cpu().state().start_cycle;
        do {
            nes_->execute();
        } while (nes_->cpu().state().start_cycle == prev_start_cycle);
    }

private:
    bool is_running_{true};
    n_e_s::nes::Nes *nes_{nullptr};
};

} // namespace nesvis
