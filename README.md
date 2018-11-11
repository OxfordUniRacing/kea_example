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
load tx.elf
```

Note: ask me if you want `.bin` files for programming, I can add support in makefile

### Description


Directory overview:

```
├── include : application header files
│   └── can.h
├── mscan : mscan library
│   ├── cfg
│   ├── inc
│   └── src
├── src : application source files
│   ├── can.c
│   ├── main.c
│   └── MSCAN_Module.c
└── system : system initialization and definitions
    ├── include
    └── src
```

The main action happens in `can.c` and `main.c`

In `do_init` we initialize the can bus, configure tx and rx message buffers,
and waits for the hardware to indicate everything is initalized.

In `do_tx` we load data into the message buffer and puts it into the transmit
queue. Note: `do_tx` return before the packet is actually transmited and acked
so if the message hasn't been acked in time, e.g. no node on can bus to recieve,
subsequent calls to do_tx will fail at load mb because it is still full.

In `do_rx` loop to check if we have recieved any data into our message buffer.


`MSCAN_Module.c` contains the interrupt handlers and contains an initialization
routine to enable interrupts. It is copy pasted from example code with minimal
modification, hence the uglyness.


### Links

[MSCAN driver for Kinetis EA devices](https://www.nxp.com/webapp/sps/download/license.jsp?colCode=MSCAN_KEA_DEVD&Parent_nodeId=1389306867337700597964&Parent_pageType=product)

[OpenSDA JLink firmware](https://www.segger.com/products/debug-probes/j-link/models/other-j-links/opensda-sda-v2/)
