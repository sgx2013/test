#ifndef _DMA_H_
#define _DMA_H_

#define DMACA_EB        0x80000000
#define DMACB_FS        0x02000000
#define DMACB_FD        0x01000000

#define DMACB_RS     0x800000
#define DMACB_RD     0x400000
#define DEMAND_TRANSFER_MOD     0x20000000
#define BLOCK_TRANSFER_MOD      0x00000000 
#define DMA_ENABLE              0x80000000
#define DMACA_BASE              0x40060010
#define DMACB_BASE              0x40060014
#define DMACSA_BASE             0x40060018
#define DMACDA_BASE             0x4006001C

#define  DMACA_EB       0x80000000
#define  DMACA_PB       0x40000000
#define  DMACA_ST       0x20000000
#endif /*_DMA_H_*/

