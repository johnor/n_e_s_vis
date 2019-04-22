#pragma once

#include "core/cpu_factory.h"
#include "core/icpu.h"
#include "core/immu.h"
#include "core/ippu.h"
#include "core/irom.h"
#include "core/membank_factory.h"
#include "core/mmu_factory.h"
#include "core/ppu_factory.h"
#include "core/rom_factory.h"

#include <memory>

class Nes {
public:
    Nes()
            : ppu_(n_e_s::core::PpuFactory::create(&ppu_registers_)),
              mmu_(n_e_s::core::MmuFactory::create(
                      n_e_s::core::MemBankFactory::create_nes_mem_banks(
                              ppu_.get()))),
              cpu_(n_e_s::core::CpuFactory::create(&cpu_registers_,
                      mmu_.get())) {}

    void execute() {
        if (cycle_++ % 3 == 0) {
            cpu_->execute();
        }

        ppu_->execute();
    }

    void reset() {
        cpu_->reset();
    }

    void load_rom(const std::string &filepath) {
        n_e_s::core::MemBankList membanks{
                n_e_s::core::MemBankFactory::create_nes_mem_banks(ppu_.get())};

        std::unique_ptr<n_e_s::core::IRom> rom{
                n_e_s::core::RomFactory::from_file(filepath)};
        membanks.push_back(std::move(rom));

        mmu_ = n_e_s::core::MmuFactory::create(std::move(membanks));
        cpu_ = n_e_s::core::CpuFactory::create(&cpu_registers_, mmu_.get());

        reset();
    }

    uint64_t cycle() const {
        return cycle_;
    }

    n_e_s::core::IPpu::Registers ppu_registers_{};
    std::unique_ptr<n_e_s::core::IPpu> ppu_;

    std::unique_ptr<n_e_s::core::IMmu> mmu_;

    n_e_s::core::ICpu::Registers cpu_registers_{};
    std::unique_ptr<n_e_s::core::ICpu> cpu_;

    uint64_t cycle_{};
};