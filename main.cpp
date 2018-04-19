#include <stdint.h>
#include "mbed.h"
#include "lm7580.h"

#define PIN_SDL p27
#define PIN_SDA p28

//Instantiate a new I2C object on the defined pins.
static I2C i2c_ctx(PIN_SDA, PIN_SDL);

/**
 * Configures the LM75X board for temperature access over I2C.
 * The register addresses for both configure and temperatue base are in
 * lm7580.h and are described in https://www.nxp.com/docs/en/data-sheet/LM75B.pdf (section 7.4).
 * @return void
 */
I2C setup_lm7580_i2c() {
	char packet[2];

	packet[0] = LM75_CONFIGURE;
	packet[1] = LM7580_TEMP_ADDR;
	i2c_ctx.write(LM7580_BASE_ADDR, packet, 2);
}

/**
 * Reads a single byte from the LM75X board that has already been setup for LM7580
 * access. The request is stored in the first byte of the packet, and the response
 * data is stored in the second byte.
 * @return float  The computed temperature value in degrees celsius.
 */
float read_lm5780_temp() {
	char packet[2];

	//Note that the second byte of packet[] is used to store the byte received
	//from the devices itself.
	packet[0] = LM7580_TEMP_ADDR;

	//Request the temperature.
	i2c_ctx.write(LM7580_BASE_ADDR, packet, 1);

	//Read the temperature back into packet[1], i.e. (2 -1)
	i2c_ctx.read(LM7580_BASE_ADDR, packet, 2);

	//Compute the result.
	return (float((packet[0]<<8)|packet[1]) / 0x100);
}

int main() {
	setup_lm7580_i2c();
	while (1) {
		printf("The temperature is %f.\n", read_lm5780_temp());
		wait_ms(500);
	}

	return 0;
}
