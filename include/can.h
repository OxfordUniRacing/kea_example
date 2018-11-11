#ifndef _CAN_H
#define _CAN_H

int do_init(void);
int do_tx(void);
int do_rx(void);

void MSCAN_ModuleEn(void);
void MSCAN_RX_IRQHandler(void);
void MSCAN_TX_IRQHandler(void);

#endif
