#include <SKEAZ1284.h>
#include <stdint.h>

#include "msCANdrv.h"

#include "can.h"

void MSCAN_ModuleEn(void);

int do_init(void) {
	MSCAN_ModuleEn();

	__enable_irq();

	int ret = 0;

	ret = Init_CAN(0, FAST);
	if (ret)
		return ret;

	// configure message buffer 1 for transmit
	ret = Config_CAN_MB(0, 1, TXDF, 0);
	if (ret)
		return ret;

	// configure message buffer 0 for transmit
	ret = Config_CAN_MB(0, 0, RXDF, 0);
	if (ret)
		return ret;

	uint8_t can_status[3];
	do {
		ret = Check_CAN_Status(0, can_status);
		if (ret)
			return ret;
	} while(!(can_status[0] & SYNCH));

	return 0;
}

int do_tx(void) {
	// first byte is length, next length bytes is the data
	uint8_t data[9] = { 1, 0xff };

	int ret = 0;

	ret = Load_CAN_MB(0, 1, data);
	if (ret)
		return ret;

	ret = Transmit_CAN_MB(0, 1);
	if (ret)
		return ret;

	return 0;
}

int do_rx(void) {
	uint8_t data[9];

	int ret = 0;

	uint8_t buf_status[2];
	do {
		ret = Check_CAN_MB_Status(0, 0, buf_status);
		if (ret)
			return ret;
	} while (buf_status[0] != NEWDATA);

	ret = Read_CAN_MB_Data(0, 0, data);
	if (ret)
		return ret;

	// put breakpoint here to see data
	return 0;
}
