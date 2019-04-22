#pragma once

#include "nes.h"

class NesWidget {
public:
    NesWidget(Nes *nes);

    void draw();

private:
    Nes *nes_;
};