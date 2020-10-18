# n e s vis

## Description

Gui for the modular cycle-accurate NES emulator n_e_s: https://github.com/evilcorpltd/n_e_s

## Building

Clone!

`git clone https://github.com/johnor/n_e_s_vis.git`

Build!

```sh
cmake -S . -B build
cmake --build build/ --config Release --target n_e_s_vis_app
```

Run!
```sh
build/application/Release/n_e_s_vis_app.exe <rom.nes>
```
