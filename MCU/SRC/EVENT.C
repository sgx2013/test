//callback
#include <RTL.H>
#include "..\..\CONFIG\INC\CONFIG.H"
#include "..\..\CONFIG\INC\TYPES.H"
#include "..\..\OS\INC\TASK_IF.H"
#include "..\..\SYSTEM\INC\SYS_TASK_IF.H"
#include "..\..\INPUT\INC\ROTARY_IF.H"
#include "..\..\OUTPUT\INC\ILLUM_IF.H"
#include "..\..\POWER\INC\POWER_DRV_IF.H"
//#include "..\..\RECEIVER\INC\SI_TUNER_IF.H"
#include "..\..\RECEIVER\INC\ATOMIC2_TUNER_CMD.H"
#include "..\..\RECEIVER\INC\ATOMIC2_TUNER_IF.H"
#include "..\..\CAR_SVC\INC\SPEED_IF.H"
#ifdef MP5_NAVI_OPTION
#include "..\..\MP5\INC\MP5_SPI_NAVI_IF.H"
#include "..\..\CAR_SVC\INC\IIC_NAVI_IF.H"
#endif//MP5_NAVI_OPTION

#ifdef MP5_PXN9530_OPTION
#include "..\..\MP5\INC\MP5_IIC_9530_IF.H"
#endif //MP5_PXN9530_OPTION  
#include "..\INC\SPI_IF.H"
#include "..\INC\EVENT_IF.H"
#include "..\..\DIAG\INC\ENG_APP_IF.H"
#include "..\..\LIN\INC\LIN_DRV.H" 
#include "..\..\AUDIO\INC\SPI_AKM.H"

/***********************************************************************
*  Name        : mcu_frt_callback
*  Description : is a fuction point array
                 when a interrupt occur,then call
                 or invoke a user function
*                FOR FRT(free run timer)
***********************************************************************/
MCU_EVENT_TYPE mcu_frt_callback[6] =
{
    NULL,/*MFT_0,time0*/
    sys_timer_isr_handler,/*MFT_0,time1*/
    NULL,/*MFT_0,time2*/
#ifdef MAZDA_J53R
#ifdef CT_G4_IPC
    NULL, //888999temp disabled for G4 debugging only, later put back
#else
    illum_timeout_isr,/*MFT_1,time0*/
#endif //CT_G4_IPC
#else
    NULL,
#endif
    NULL,/*MFT_1,time1*/
    NULL /*MFT_1,time2*/

};


/***********************************************************************
*  Name        : mcu_cap_callback
*  Description : is a fuction point array
                 when a interrupt occur,then call
                 or invoke a user function
*                FOR CAP
***********************************************************************/
MCU_EVENT_TYPE mcu_cap_callback[8] =
{
    ( MCU_EVENT_TYPE )spd_pulse_isr,/*CAP_MFT_0,CAP_UNIT_0 for SDVC*/
    NULL,/*CAP_MFT_0,CAP_UNIT_1*/
    NULL,/*CAP_MFT_0,CAP_UNIT_2*/
#ifdef MP5_NAVI_OPTION
    (MCU_EVENT_TYPE)spi_slave_cs_proc,/*CAP_MFT_0,CAP_UNIT_3*/
#else
    NULL,
#endif
    /*CAP_MFT_1,CAP_UNIT_0*/
#ifdef MAZDA_J53R
#ifdef CT_G4_IPC
    NULL, //888999temp disabled for G4 debugging only, later put back
#else
    (MCU_EVENT_TYPE)illum_cap_rise_isr,
#endif //CT_G4_IPC
#else
    NULL,
#endif
#ifdef  ROT_SEL_KEY
    (MCU_EVENT_TYPE)rot_sel_int_isr,/*CAP_MFT_1,CAP_UNIT_1*/
#else
#ifdef MAZDA_J53R
#ifdef CT_G4_IPC
    NULL, //888999temp, temporarily revoved for G4 debugging, put back later
#else
    (MCU_EVENT_TYPE)illum_cap_fall_isr,
#endif //CT_G4_IPC
#else
    NULL,
#endif// MAZDA_J53R
#endif
    NULL,/*CAP_MFT_1,CAP_UNIT_2*/
#ifdef ROT_VOL_KEY
    ( MCU_EVENT_TYPE )rot_vol_int_isr, /*CAP_MFT_1,CAP_UNIT_3 for rot vol*/
#else
    NULL,
#endif
};


/***********************************************************************
*  Name        : mcu_exti_callback
*  Description : is a fuction point array
                 when a interrupt occur,then call
                 or invoke a user function
*                FOR EXTI
***********************************************************************/

MCU_EVENT_TYPE mcu_exti_callback[16] =
{
    isr_6v5_handler,/* EXTI_00 */
    NULL,/* EXTI_01 */
    isr_9v_handler,/* EXTI_02 */
    NULL,/* EXTI_03 */
    NULL,/* EXTI_04 */
#ifdef TUNER_SI475X
    si_tuner_int_handler,/* EXTI_05 */
#else
    NULL,/* EXTI_05 */
#endif/*TUNER_SI475X*/
    NULL,/* EXTI_06 */
    NULL,/* EXTI_07 */
#ifdef POWER_INSERT_DECK_OPTION
    //isr_disc_insert_handler,/* EXTI_08 */
    NULL,/* EXTI_08 */
#else
    NULL,/* EXTI_08 */
#endif/*POWER_INSERT_DECK_OPTION*/

    NULL,/* EXTI_09 */
    NULL,/* EXTI_10 */

    isr_can_inh_handler,/* EXTI_11 */

#ifdef POWER_EJECT_DECK_OPTION
    //isr_disc_eject_handler,/* EXTI_12 */
    NULL,/* EXTI_12 */
#else
    NULL,/* EXTI_12 */
#endif/*POWER_EJECT_DECK_OPTION*/
    isr_power_key_handler,//,/* EXTI_13 */
    NULL,//isr_acc_handler,/* EXTI_14 */
    isr_rvc_handler,/* EXTI_15 */
};

/***********************************************************************
*  Name        : lin_tx_callback
*  Description : is a fuction point array
                 when a interrupt occur,then call
                 or invoke a user function
*
***********************************************************************/
MCU_EVENT_TYPE mcu_lin_tx_callback[8] =
{
#ifdef LIN_BUS_OPTION
    isr_lin_on_tx_char,/* LIN_0_TX */
#else
    NULL,/* LIN_0_TX */
#endif /* LIN_BUS_OPTION */
    NULL,/* LIN_1_TX */
    NULL,/* LIN_2_TX */
    NULL,/* LIN_3_TX */
    NULL,/* LIN_4_TX */
    NULL,/* LIN_5_TX */
    NULL,/* LIN_6_TX */
    NULL /* LIN_7_TX */
};

/***********************************************************************
*  Name        : lin_rx_callback
*  Description : is a fuction point array
                 when a interrupt occur,then call
                 or invoke a user function
*
***********************************************************************/
MCU_EVENT_TYPE  mcu_lin_rx_callback[8] =
{
#ifdef LIN_BUS_OPTION
    isr_lin_on_rx_char,/* LIN_0_RX */
#else
    NULL,/* LIN_0_RX */
#endif /* LIN_BUS_OPTION */
    NULL,/* LIN_1_RX */
    NULL,/* LIN_2_RX */
    NULL,/* LIN_3_RX */
    NULL,/* LIN_4_RX */
    NULL,/* LIN_5_RX */
    NULL,/* LIN_6_RX */
    NULL /* LIN_7_RX */
};

/***********************************************************************
*  Name        : lin_rx_callback
*  Description : is a fuction point array
                 when a interrupt occur,then call
                 or invoke a user function
*
***********************************************************************/
MCU_EVENT_TYPE mcu_lin_lb_callback[8] =
{
#ifdef LIN_BUS_OPTION
    isr_lin_lb_handler,/* LIN_0_RX */
#else
    NULL,/* LIN_0_RX */
#endif /* LIN_BUS_OPTION */
    NULL,/* LIN_1_RX */
    NULL,/* LIN_2_RX */
    NULL,/* LIN_3_RX */
    NULL,/* LIN_4_RX */
    NULL,/* LIN_5_RX */
    NULL,/* LIN_6_RX */
    NULL /* LIN_7_RX */
};

/***********************************************************************
*  Name        : lin_err_callback
*  Description : is a fuction point array
                 when a interrupt occur,then call
                 or invoke a user function
*
***********************************************************************/
MCU_EVENT_TYPE mcu_lin_err_callback[8] =
{
#ifdef LIN_BUS_OPTION
    isr_lin_err_handler,/* LIN_0_ERR */
#else
    NULL,/* LIN_0_ERR */
#endif /* LIN_BUS_OPTION */
    NULL,/* LIN_1_ERR */
    NULL,/* LIN_2_ERR */
    NULL,/* LIN_3_ERR */
    NULL,/* LIN_4_ERR */
    NULL,/* LIN_5_ERR */
    NULL,/* LIN_6_ERR */
    NULL /* LIN_7_ERR */
};

/***********************************************************************
*  Name        : uart_tx_callback
*  Description : is a fuction point array
                 when a interrupt occur,then call
                 or invoke a user function
*
***********************************************************************/
MCU_EVENT_TYPE mcu_uart_tx_callback[8] =
{
    NULL,/* UART_0_TX */
    NULL,/* UART_1_TX */
    NULL,/* UART_2_TX */
    isr_eng_on_tx_char,/* UART_3_TX *///ENG_UART
    NULL,/* UART_4_TX */
    NULL,/* UART_5_TX */
    NULL,/* UART_6_TX */
    NULL /* UART_7_TX */
};


/***********************************************************************
*  Name        : uart_rx_callback
*  Description : is a fuction point array
                 when a interrupt occur,then call
                 or invoke a user function
*
***********************************************************************/
extern void dsp_spi_read_proc(void);
MCU_EVENT_TYPE  mcu_uart_rx_callback[8] =
{
    NULL,/* UART_0_RX */
    NULL,/* UART_1_RX */
    NULL,/* UART_2_RX */
    isr_eng_on_rx_char,/* UART_3_RX *///ENG_UART
    NULL,/* UART_4_RX */
    NULL,/* UART_5_RX */
    NULL,/* UART_6_RX */
    NULL /* UART_7_RX */
};

/***********************************************************************
*  Name        : spi_tx_callback
*  Description : is a fuction point array
                 when a interrupt occur,then call
                 or invoke a user function
*
***********************************************************************/
extern void dsp_spi_write_proc(void);
MCU_EVENT_TYPE   mcu_spi_tx_callback[8] =
{
    NULL,/* SPI_0_TX */
    NULL,/* SPI_1_TX */
    NULL,/* SPI_2_TX */
    NULL,/* SPI_3_TX */
    NULL,/* SPI_4_TX */
#ifdef MP5_NAVI_OPTION
#ifdef CT_G4_IPC
    spi_navi_fifotx_proc,//new interrupt processing routine with hw fifo
#else
    spi_navi_tx_proc,/* SPI_5_TX */
#endif //CT_G4_IPC
#else //MP5_NAVI_OPTION
    NULL, /* SPI_5_TX */
#endif //MP5_NAVI_OPTION
    NULL,/* SPI_6_TX */
    dsp_spi_write_proc/* SPI_7_TX */
};


/***********************************************************************
*  Name        : spi_rx_callback
*  Description : is a fuction point array
                 when a interrupt occur,then call
                 or invoke a user function
*
***********************************************************************/
MCU_EVENT_TYPE   mcu_spi_rx_callback[8] =
{
    NULL,/* SPI_0_RX */
    NULL,/* SPI_1_RX */
    NULL,/* SPI_2_RX */
    NULL,/* SPI_3_RX */
    NULL,/* SPI_4_RX */
#ifdef MP5_NAVI_OPTION
#ifdef CT_G4_IPC
    spi_navi_fiforx_proc, //new interrupt processing routine with hw fifo
#else
    spi_navi_rx_proc,/* SPI_5_RX */
#endif //CT_G4_IPC
#else //MP5_NAVI_OPTION
    NULL, /* SPI_5_RX */
#endif //MP5_NAVI_OPTION
    NULL,/* SPI_6_RX */
    dsp_spi_read_proc /* SPI_7_RX */
};


/***********************************************************************
*  Name        : mcu_i2c_tx_finish_callback
*  Description : 
*
***********************************************************************/
void mcu_i2c_tx_finish_callback(U8 ch)
{
   if(ch < 8) //channel 4 is tuner
   {
        isr_sem_send(i2c_sem_wait[ch]); 
   }          
}

/***********************************************************************
*  Name        : mcu_i2c_rx_finish_callback
*  Description : 
*
***********************************************************************/
void mcu_i2c_rx_finish_callback(U8 ch)
{
   if(ch < 8) //channel 4 is tuner
   {
        isr_sem_send(i2c_sem_wait[ch]); 
   } 
}

/***********************************************************************
*  Name        : mcu_i2c_tx_frame_callback
*  Description : is a fuction point array
                 when a interrupt occur,then call
                 or invoke a user function
*
***********************************************************************/
MCU_EVENT_TYPE mcu_i2c_tx_frame_callback[8] =
{
    NULL,/* I2C_0_TX_FRAME */
    NULL,/* I2C_1_TX_FRAME */
    NULL,/* I2C_2_TX_FRAME */
    NULL,/* I2C_3_TX_FRAME */
    NULL,/* I2C_4_TX_FRAME */
    NULL,/* I2C_5_TX_FRAME */
#ifdef CD_DECK_CDMM10
    iic_deck_frame_sent,/* I2C_TX_FRAME *///NULL,/* I2C_4_TX_FRAME */
#else
    NULL,
#endif
    NULL, /* I2C_7_TX_FRAME */
};


/***********************************************************************
*  Name        : mcu_i2c_rx_frame_callback
*  Description : is a fuction point array
                 when a interrupt occur,then call
                 or invoke a user function
*
***********************************************************************/
MCU_EVENT_TYPE mcu_i2c_rx_frame_callback[8] =
{
    NULL,/* I2C_0_RX_FRAME */
    NULL,/* I2C_1_RX_FRAME */
    NULL,/* I2C_2_RX_FRAME */
    NULL,/* I2C_3_RX_FRAME */
    NULL,/* I2C_4_RX_FRAME */
    NULL,/* I2C_5_RX_FRAME */
#ifdef CD_DECK_CDMM10
    iic_deck_frame_received,/* I2C_RX_FRAME *///NULL,/* I2C_4_RX_FRAME */
#else
    NULL,
#endif
    NULL /* I2C_7_RX_FRAME */
};


/***********************************************************************
*  Name        : mcu_i2c_rx_char_callback
*  Description : is a fuction point array
                 when a interrupt occur,then call
                 or invoke a user function
*
***********************************************************************/
MCU_EVENT_TYPE mcu_i2c_rx_char_callback[8] =
{
    NULL,/* I2C_0_RX_CHAR */
    NULL,/* I2C_1_RX_CHAR */
    NULL,/* I2C_2_RX_CHAR */
    NULL,/* I2C_3_RX_CHAR */
    NULL,/* I2C_4_RX_CHAR */
    NULL,/* I2C_5_RX_CHAR */
#ifdef CD_DECK_CDMM10
    iic_deck_rec_first_char,//NULL,/* I2C_4_RX_CHAR */
#else
    NULL,
#endif
    NULL /* I2C_7_RX_CHAR */
};


/***********************************************************************
*  Name        : mcu_i2c_slave_rx_char_callback
*  Description : is a fuction point array
                 when a interrupt occur,then call
                 or invoke a user function
*
***********************************************************************/
MCU_EVENT_TYPE mcu_i2c_slave_rx_char_callback[8] =
{
    NULL,/*I2C_0_Slave_rx_char*/
#ifdef MP5_NAVI_OPTION
    iic_navi_handle_msg_char,/*I2C_1_Slave_rx_char*/
#endif // MP5_NAVI_OPTION   
#ifdef MP5_PXN9530_OPTION
    iic_9530_on_rx_char,/*I2C_1_Slave_rx_char*/
#endif // MP5_PXN9530_OPTION      
    NULL,/*I2C_2_Slave_rx_char*/
    NULL,/*I2C_3_Slave_rx_char*/
    NULL,/*I2C_4_Slave_rx_char*/
    NULL,/*I2C_5_Slave_rx_char*/
    NULL,/*I2C_6_Slave_rx_char*/
    NULL /*I2C_7_Slave_rx_char*/
};


/***********************************************************************
*  Name        : mcu_i2c_slave_tx_char_callback
*  Description : is a fuction point array
                 when a interrupt occur,then call
                 or invoke a user function
*
***********************************************************************/
MCU_EVENT_TYPE mcu_i2c_slave_tx_char_callback[8] =
{
    NULL,/*I2C_0_Slave_tx_char*/
#ifdef MP5_NAVI_OPTION
    iic_navi_send_char,/*I2C_1_Slave_tx_char*/
#endif // MP5_NAVI_OPTION   
#ifdef MP5_PXN9530_OPTION
    iic_9530_on_tx_char,/*I2C_1_Slave_tx_char*/
#endif // MP5_PXN9530_OPTION  
    NULL,/*I2C_2_Slave_tx_char*/
    NULL,/*I2C_3_Slave_tx_char*/
    NULL,/*I2C_4_Slave_tx_char*/
    NULL,/*I2C_5_Slave_tx_char*/
    NULL,/*I2C_6_Slave_tx_char*/
    NULL /*I2C_7_Slave_tx_char*/
};


/***********************************************************************
*  Name        : mcu_i2c_slave_tx_firstchar_callback
*  Description : is a fuction point array
                 when a interrupt occur,then call
                 or invoke a user function
*
***********************************************************************/
MCU_EVENT_TYPE mcu_i2c_slave_tx_firstchar_callback[8] =
{
    NULL,/*I2C_0_Slave_tx_firstchar*/
#ifdef MP5_NAVI_OPTION
    iic_navi_start_send,/*I2C_1_Slave_tx_firstchar*/
#endif // MP5_NAVI_OPTION  
#ifdef MP5_PXN9530_OPTION
    iic_9530_on_send_req,/*I2C_1_Slave_tx_firstchar*/
#endif // MP5_PXN9530_OPTION  
    NULL,/*I2C_2_Slave_tx_firstchar*/
    NULL,/*I2C_3_Slave_tx_firstchar*/
    NULL,/*I2C_4_Slave_tx_firstchar*/
    NULL,/*I2C_5_Slave_tx_firstchar*/
    NULL,/*I2C_6_Slave_tx_firstchar*/
    NULL /*I2C_7_Slave_tx_firstchar*/
};


/***********************************************************************
*  Name        : mcu_i2c_slave_rx_firstchar_callback
*  Description : is a fuction point array
                 when a interrupt occur,then call
                 or invoke a user function
*
***********************************************************************/
MCU_EVENT_TYPE mcu_i2c_slave_rx_firstchar_callback[8] =
{
    NULL,/*I2C_0_Slave_rx_firstchar*/
#ifdef MP5_NAVI_OPTION
    NULL,/*I2C_1_Slave_rx_firstchar*/
#endif // MP5_NAVI_OPTION  
#ifdef MP5_PXN9530_OPTION
    iic_9530_on_read_req, /*I2C_1_Slave_rx_firstchar*/
#endif // MP5_PXN9530_OPTION  
    NULL,/*I2C_2_Slave_rx_firstchar*/
    NULL,/*I2C_3_Slave_rx_firstchar*/
    NULL,/*I2C_4_Slave_rx_firstchar*/
    NULL,/*I2C_5_Slave_rx_firstchar*/
    NULL,/*I2C_6_Slave_rx_firstchar*/
    NULL /*I2C_7_Slave_rx_firstchar*/
};

MCU_EVENT_TYPE  mcu_i2c_stop_callback[8] =
{
    NULL,/*I2C_0_Stop*/
#ifdef MP5_NAVI_OPTION
    iic_navi_iic_stop_isr,/*I2C_1_Stop*/
#endif // MP5_NAVI_OPTION          
#ifdef MP5_PXN9530_OPTION
    iic_9530_on_frame_end, /*I2C_1_Stop*/
#endif // MP5_PXN9530_OPTION   
    NULL,/*I2C_2_Stop*/
    NULL,/*I2C_3_Stop*/
    NULL,/*I2C_4_Stop*/
    NULL,/*I2C_5_Stop*/
    NULL,/*I2C_6_Stop*/
    NULL /*I2C_7_Stop*/
};

MCU_EVENT_TYPE  mcu_i2c_start_callback[8] =
{
    NULL,/*I2C_0_Start*/
    NULL,/*I2C_1_Start*/
    NULL,/*I2C_2_Start*/
    NULL,/*I2C_3_Start*/
    NULL,/*I2C_4_Start*/
    NULL,/*I2C_5_Start*/
    NULL,/*I2C_6_Start*/
    NULL /*I2C_7_Start*/
};


/***********************************************************************
*  Name        : mcu_dtim_callback
*  Description : is a fuction point array
                 when a interrupt occur,then call
                 or invoke a user function
*
***********************************************************************/
MCU_EVENT_TYPE  mcu_dtim_callback[2] =
{
    NULL,/* DTimer 1*/
    NULL /* DTimer 2*/
};

extern void dsp_spi_dma_proc(void);
extern void eng_uart_dma_proc(void);
/***********************************************************************
*  Name        : mcu_dma_callback
*  Description : is a fuction point array
                 when a interrupt occur,then call
                 or invoke a user function
*
***********************************************************************/
MCU_EVENT_TYPE  mcu_dma_callback[8] =
{
    dsp_spi_dma_proc,/* DMAC 0*/
    eng_uart_dma_proc,/* DMAC 1*/
    NULL,/* DMAC 2*/
    NULL,/* DMAC 3*/
    NULL,/* DMAC 4*/
    NULL,/* DMAC 5*/
    NULL,/* DMAC 6*/
    NULL,/* DMAC 7*/
};
