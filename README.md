## Maple OS
This is a hobby operating system written in C and assembly.
### Building
#### Setting up a cross-compiler
In order to build and run the project, you will need to build GCC as a cross compiler for i386-elf by following <a href="https://wiki.osdev.org/GCC_Cross-Compiler">this</a> tutorial on osdev.
NOTE: consult the <a href="https://wiki.osdev.org/Cross-Compiler_Successful_Builds">successful builds</a> page for version information.

#### Installing dependencies
Adapt for your particular linux distribution:
```
sudo apt install qemu-system-i386 qemu-pc-bin grub-common
```

#### Clone and build the project
```
git clone https://github.com/gradn0/maple-os.git
cd maple-os
./qemu.sh
```
