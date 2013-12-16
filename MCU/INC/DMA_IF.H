#ifndef _DMA_IF_H_
#define _DMA_IF_H_

#define DMA_MODE_ST             0
#define DMA_MODE_HWR            0x80  // hardware receive
#define DMA_MODE_HWT            0xC0  //hardware send

typedef struct 
{
    U8 ch;
    U32 s_addr;
    U32 d_addr;
    U16 len;
    U8  opt;
}DMA_TYPE;


typedef enum
{
    IS_SOFT = 0x00,
    ISR_USB_EP1_DRQ = 0x20,
    ISR_USB_EP2_DRQ,
    ISR_USB_EP3_DRQ,
    ISR_USB_EP4_DRQ,
    ISR_USB_EP5_DRQ,
    ISR_AD0_SCAN_CONVERSION,
    ISR_AD1_SCAN_CONVERSION,
    ISR_AD2_SCAN_CONVERSION,
    ISR_BASE_TIMER_CH0,
    ISR_BASE_TIMER_CH2,
    ISR_BASE_TIMER_CH4,
    ISR_BASE_TIMER_CH6,
    ISR_MFS_CH0_RX,
    ISR_MFS_CH0_TX,
    ISR_MFS_CH1_RX,
    ISR_MFS_CH1_TX,
    ISR_MFS_CH2_RX,
    ISR_MFS_CH2_TX,
    ISR_MFS_CH3_RX,
    ISR_MFS_CH3_TX,
    ISR_MFS_CH4_RX,
    ISR_MFS_CH4_TX,
    ISR_MFS_CH5_RX,
    ISR_MFS_CH5_TX,
    ISR_MFS_CH6_RX,
    ISR_MFS_CH6_TX,
    ISR_MFS_CH7_RX,
    ISR_MFS_CH7_TX,
    ISR_EXT_CH0,
    ISR_EXT_CH1,
    ISR_EXT_CH2,
    ISR_EXT_CH3
}DMACA_IS_TYPE;

#define MFS_CH0_TDR_ADDR    0x40038008
#define MFS_CH0_RDR_ADDR    0x40038008
#define MFS_CH1_TDR_ADDR    0x40038108
#define MFS_CH1_RDR_ADDR    0x40038108
#define MFS_CH2_TDR_ADDR    0x40038208
#define MFS_CH2_RDR_ADDR    0x40038208
#define MFS_CH3_TDR_ADDR    0x40038308
#define MFS_CH3_RDR_ADDR    0x40038308
#define MFS_CH4_TDR_ADDR    0x40038408
#define MFS_CH4_RDR_ADDR    0x40038408
#define MFS_CH5_TDR_ADDR    0x40038508
#define MFS_CH5_RDR_ADDR    0x40038508
#define MFS_CH6_TDR_ADDR    0x40038608
#define MFS_CH6_RDR_ADDR    0x40038608
#define MFS_CH7_TDR_ADDR    0x40038708
#define MFS_CH7_RDR_ADDR    0x40038708


typedef enum _DMA_STATUS_TYPE_
{
    DMA_STATUS_INITIAL,
    DMA_STATUS_ADDRESS_OVERFLOW,
    DMA_STATUS_TRANSFER_STOP,
    DMA_STATUS_SOURCE_ACCESS_ERR,
    DMA_STATUS_DESTINATION_ACCESS_ERR,
    DMA_STATUS_SUCCESSFUL,
    DMA_STATUS_PAUSE = 0x7,
}DMA_STATUS_TYPE;


void DMA_Config(DMA_TYPE * dma);
U8 DMA_CheckStatus(U8 ch);
void DMA_Handle(U8 ch);
#endif /* _DMA_IF_H_*/
