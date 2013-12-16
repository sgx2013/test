//callback
#ifndef _EVENT_H_
#define _EVENT_H_
#include "..\..\CONFIG\INC\CONFIG.H"
#include "..\..\CONFIG\INC\TYPES.H"

/**** Definition of constants ****/


/**** Definition of types ****/
typedef  void( *const MCU_EVENT_TYPE )( void );

/**** Definition of macros ****/
#ifndef NULL
#define NULL 0
#endif

/**** Declaration of constants ****/
extern MCU_EVENT_TYPE   mcu_frt_callback[6];
extern MCU_EVENT_TYPE   mcu_cap_callback[8];
extern MCU_EVENT_TYPE   mcu_exti_callback[16];
extern MCU_EVENT_TYPE   mcu_lin_tx_callback[8];
extern MCU_EVENT_TYPE   mcu_lin_rx_callback[8];
extern MCU_EVENT_TYPE   mcu_lin_lb_callback[8];
extern MCU_EVENT_TYPE   mcu_lin_err_callback[8];
extern MCU_EVENT_TYPE   mcu_uart_tx_callback[8];
extern MCU_EVENT_TYPE   mcu_uart_rx_callback[8];
extern MCU_EVENT_TYPE   mcu_spi_tx_callback[8];
extern MCU_EVENT_TYPE   mcu_spi_rx_callback[8];
extern MCU_EVENT_TYPE   mcu_i2c_tx_frame_callback[8];
extern MCU_EVENT_TYPE   mcu_i2c_rx_frame_callback[8];
extern MCU_EVENT_TYPE   mcu_i2c_rx_char_callback[8];
extern MCU_EVENT_TYPE   mcu_i2c_slave_rx_char_callback[8];
extern MCU_EVENT_TYPE   mcu_i2c_slave_tx_char_callback[8];
extern MCU_EVENT_TYPE   mcu_i2c_slave_tx_firstchar_callback[8];
extern MCU_EVENT_TYPE   mcu_i2c_slave_rx_firstchar_callback[8];
extern MCU_EVENT_TYPE   mcu_dtim_callback[2];
extern MCU_EVENT_TYPE   mcu_i2c_stop_callback[8];
extern MCU_EVENT_TYPE   mcu_i2c_start_callback[8];
extern MCU_EVENT_TYPE   mcu_dma_callback[8];
extern void mcu_i2c_tx_finish_callback(U8 ch);
extern void mcu_i2c_rx_finish_callback(U8 ch);
/**** Declaration of variables ****/


/**** Declaration of functions ****/

#endif


/**************** END OF FILE *****************************************/


