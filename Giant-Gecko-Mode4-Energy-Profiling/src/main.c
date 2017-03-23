/**************************************************************************//**
 * @file main.c
 * @brief Demo for energy mode current consumption testing.
 * @version 4.4.0
 ******************************************************************************
 * @section License
 * <b>Copyright 2015 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/


#include "emodes.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_assert.h"
#include <stdint.h>

/**************************************************************************//**
 * @brief  Main function
 *****************************************************************************/
int main(void)
{
  /* Chip errata */
  CHIP_Init();

  EMU_EnterEM4();

  // Should not be reached.
  EFM_ASSERT(false);
}
