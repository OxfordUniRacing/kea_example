## Kinetis KEA Sucks

### Quick start guide

Step 0: Get linux

Step 1: Get the source code

```
git clone https://github.com/OxfordUniRacing/kea_example.git
cd kea_example
```

Step 2: Compile

Building tx firmware:
```
make clean
make DEFS=-DTX=1 EXE=tx.elf
```

Building rx firmware:
```
make clean
make DEFS=-DRX=1 EXE=rx.elf
```

Step 3: Upload firmware

What I did is I reprogrammed on board bootloader to jlink,
and uploaded with jlink drivers.  
Your preferred programming method may vary.

```
JLinkGDBServer -if SWD -device S9KEAZ128xxxx
arm-none-eabi-gdb -ex 'target extended-remote 127.0.0.1:2331'
```

and in gdb

```
(gdb) load tx.elf
```
