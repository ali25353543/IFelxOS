
# IFelxOS-v1.0.0 — Symbolic Kernel for x86

IFelxOS is a lightweight, modular kernel built for educational and symbolic computing environments. Designed for i386 architecture, it prioritizes control, independence, and symbolic execution across a minimal, freestanding runtime.

---

## ⚙️ Features

- **Pure x86 Kernel** — built from scratch using NASM, GCC, and LD for 32-bit targets.
- **VGA Output** — direct text rendering with cursor and color control.
- **Keyboard Input** — interrupt-driven scancode processing with command buffer.
- **Command Interface** — symbolic interaction via built-in shell (`clear`, `info`, etc.).
- **Mini String Library** — essential utilities for formatting, comparison, and parsing.
- **Heap Management** — dynamic memory allocation for runtime flexibility.

---

## 🛠️ Build Instructions

### Requirements:
- `gcc`, `nasm`, `i386-elf-ld`, `grub-mkrescue` , `qemu-system`

### Build Commands:
```bash
make              # Builds kernel.bin
make iso          # Generates bootable IFelxOS.iso
make clean        # Cleans build artifacts


