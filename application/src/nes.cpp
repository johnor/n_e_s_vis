#include "nes.h"

#include "core/cpu_factory.h"
#include "core/membank_factory.h"
#include "core/mmu_factory.h"
#include "core/ppu_factory.h"
#include "core/rom_factory.h"

Nes::Nes()
        : ppu_(n_e_s::core::PpuFactory::create(&ppu_registers_)),
          mmu_(n_e_s::core::MmuFactory::create(
                  n_e_s::core::MemBankFactory::create_nes_mem_banks(
                          ppu_.get()))),
          cpu_(n_e_s::core::CpuFactory::create(&cpu_registers_, mmu_.get())) {}

void Nes::execute() {
    if (cycle_++ % 3 == 0) {
        cpu_->execute();
    }

    ppu_->execute();
}

void Nes::reset() {
    cpu_->reset();
}

void Nes::load_rom(const std::string &filepath) {
    n_e_s::core::MemBankList membanks{
            n_e_s::core::MemBankFactory::create_nes_mem_banks(ppu_.get())};

    std::unique_ptr<n_e_s::core::IRom> rom{
            n_e_s::core::RomFactory::from_file(filepath)};
    membanks.push_back(std::move(rom));

    mmu_ = n_e_s::core::MmuFactory::create(std::move(membanks));
    cpu_ = n_e_s::core::CpuFactory::create(&cpu_registers_, mmu_.get());

    reset();
}

uint64_t Nes::cycle() const {
    return cycle_;
}

n_e_s::core::ICpu &Nes::cpu() {
    return *cpu_;
}

n_e_s::core::IPpu &Nes::ppu() {
    return *ppu_;
}

n_e_s::core::IMmu &Nes::mmu() {
    return *mmu_;
}

n_e_s::core::ICpu::Registers &Nes::registers() {
    return cpu_registers_;
}
