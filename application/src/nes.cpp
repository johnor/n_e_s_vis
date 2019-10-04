#include "nes.h"

#include "core/cpu_factory.h"
#include "core/membank_factory.h"
#include "core/mmu_factory.h"
#include "core/ppu_factory.h"
#include "core/rom_factory.h"

namespace {

class MemBankReference : public n_e_s::core::IMemBank {
public:
    MemBankReference(IMemBank *const membank) : membank_(membank) {}

    bool is_address_in_range(uint16_t addr) const override {
        return membank_->is_address_in_range(addr);
    }

    uint8_t read_byte(uint16_t addr) const override {
        return membank_->read_byte(addr);
    }
    void write_byte(uint16_t addr, uint8_t byte) override {
        membank_->write_byte(addr, byte);
    }

private:
    IMemBank *membank_;
};

} // namespace

Nes::Nes()
        : ppu_mmu_(n_e_s::core::MmuFactory::create(
                  n_e_s::core::MemBankFactory::create_nes_ppu_mem_banks())),
            ppu_(n_e_s::core::PpuFactory::create(&ppu_registers_, ppu_mmu_.get())),
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
    std::unique_ptr<n_e_s::core::IRom> rom{
            n_e_s::core::RomFactory::from_file(filepath)};

    n_e_s::core::MemBankList ppu_membanks{
            n_e_s::core::MemBankFactory::create_nes_ppu_mem_banks()};

    ppu_membanks.push_back(std::make_unique<MemBankReference>(rom.get()));

    ppu_mmu_ = n_e_s::core::MmuFactory::create(std::move(ppu_membanks));
    ppu_ = n_e_s::core::PpuFactory::create(&ppu_registers_, ppu_mmu_.get());

    n_e_s::core::MemBankList cpu_membanks{
            n_e_s::core::MemBankFactory::create_nes_mem_banks(ppu_.get())};
    cpu_membanks.push_back(std::move(rom));

    mmu_ = n_e_s::core::MmuFactory::create(std::move(cpu_membanks));
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

n_e_s::core::IMmu &Nes::ppu_mmu() {
    return *ppu_mmu_;
}

n_e_s::core::ICpu::Registers &Nes::cpu_registers() {
    return cpu_registers_;
}

n_e_s::core::IPpu::Registers &Nes::ppu_registers() {
    return ppu_registers_;
}
