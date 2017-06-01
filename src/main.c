#include <stdint.h>
#include <stdbool.h>

#include "chip.h"
#include "Serial.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

const uint32_t OscRateIn = 12000000;

#define SERIAL_BAUDRATE 115200

volatile uint32_t msTicks;

/*****************************************************************************/

 /* Private function */
void SysTick_Handler(void) {
  msTicks++;
}

/****************************************************************************/

int main(void) {

  SystemCoreClockUpdate();

  if (SysTick_Config (SystemCoreClock / 1000)) {
    //Error
    while(1);
  }

  Serial_Init(SERIAL_BAUDRATE);

  // Wheel Speed 1
  Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_5, IOCON_FUNC0 | IOCON_MODE_INACT);
  Chip_GPIO_SetPinDIRInput(LPC_GPIO, 1, 5);
  Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_5, IOCON_FUNC0 | IOCON_MODE_INACT);

  // Wheel Speed 2
  Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_0, IOCON_FUNC1 | IOCON_MODE_INACT);
  Chip_GPIO_SetPinDIRInput(LPC_GPIO, 1, 0);
  Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_0, IOCON_FUNC1 | IOCON_MODE_INACT);

  Serial_Println("Started up");

  uint32_t timestamp = 0;

  while (1) {
    if (timestamp + 1000 < msTicks) {
      timestamp = msTicks;
      bool wheel_2_state = Chip_GPIO_GetPinState(LPC_GPIO, 1, 0);
      Serial_Print(wheel_2_state ? "2_HIGH" : "2_LOW");
      Serial_Print(", ");
      bool wheel_1_state = Chip_GPIO_GetPinState(LPC_GPIO, 1, 5);
      Serial_Print(wheel_1_state ? "1_HIGH" : "1_LOW");
      Serial_Print("\n");
    }
  }
}
