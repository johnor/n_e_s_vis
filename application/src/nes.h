#pragma once

#include "core/icpu.h"
#include "core/immu.h"
#include "core/ippu.h"
#include "core/irom.h"

#include <memory>
#include <string>

class Nes {
public:
    Nes();

    void execute();
    void reset();

    void load_rom(const std::string &filepath);

    uint64_t cycle() const;
    n_e_s::core::ICpu &cpu();
    n_e_s::core::IPpu &ppu();
    n_e_s::core::IMmu &mmu();
    n_e_s::core::IMmu &ppu_mmu();
    n_e_s::core::ICpu::Registers &cpu_registers();
    n_e_s::core::IPpu::Registers &ppu_registers();

private:
    std::unique_ptr<n_e_s::core::IMmu> ppu_mmu_;
    n_e_s::core::IPpu::Registers ppu_registers_{};
    std::unique_ptr<n_e_s::core::IPpu> ppu_;

    std::unique_ptr<n_e_s::core::IMmu> mmu_;

    n_e_s::core::ICpu::Registers cpu_registers_{};
    std::unique_ptr<n_e_s::core::ICpu> cpu_;

    uint64_t cycle_{};
};
