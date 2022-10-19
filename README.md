# n e s vis

## Description

Gui for a modular cycle-accurate NES emulator n_e_s (https://github.com/evilcorpltd/n_e_s).

## Building

Clone:

`git clone https://github.com/johnor/n_e_s_vis.git`

Build:

```sh
cmake -S . -B build
cmake --build build/ --config Release --target n_e_s_vis_app
```

Run:

```sh
build/application/Release/n_e_s_vis_app.exe <rom.nes>
```

<p align="center">
<img src="https://camo.githubusercontent.com/94689fdf73e2cf2e25a46ff52bd9439a6b026503362fea8302d26033a403fa0e/68747470733a2f2f6d652e726963617264696375732e73652f36373564343935346237633431303865323036383133623630663734333738642f64656d6f2e706e67" />
</p>
<p align="center"><i>Example of running a ROM</i></p>
