#pragma once

#include "cpu_widget.h"
#include "log_widget.h"
#include "memory_widget.h"
#include "ppu_helper.h"
#include "ppu_widget.h"

namespace nesvis {

class Gui {
public:
    Gui(n_e_s::nes::Nes *nes, nesvis::PpuHelper *ppu_helper)
            : log_widget(),
              cpu_widget(nes),
              ppu_widget(nes, ppu_helper),
              memory_widget(nes) {}

    void draw() {
        log_widget.draw();
        cpu_widget.draw();
        ppu_widget.draw();
        memory_widget.draw();
    }

    nesvis::LogWidget log_widget;
    nesvis::CpuWidget cpu_widget;
    nesvis::PpuWidget ppu_widget;
    nesvis::MemoryWidget memory_widget;
};

} // namespace nesvis
