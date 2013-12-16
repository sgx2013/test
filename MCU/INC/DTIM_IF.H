#ifndef   _DTIM_IF_H_
#define   _DTIM_IF_H_
#include "..\..\CONFIG\INC\CONFIG.H"
#include "..\..\CONFIG\INC\TYPES.H"

/**** Definition of constants ****/


/**** Definition of types ****/
typedef struct
{
    volatile U32 RLD;
    volatile U32 VALR;
    volatile U32 CRLR;
    volatile U32 INTCR;
    volatile U32 INTSR;
    volatile U32 INTMR;
    volatile U32 BRLD;
} DTIM_TYPE;

/**** Definition of macros ****/
#define DTIM_ENABLE         0x80
#define DTIM_MODE_PERIOD    0x40
#define DTIM_IT_ENABLE      0x20
#define DTIM_DIV_1          0x00
#define DTIM_DIV_16         0x04
#define DTIM_DIV_256        0x08
#define DTIM_SIZE_32B       0x02
#define DTIM_SIZE_16B       0x00
#define DTIM_ONE_SHOT       0x01

#define DTIM_1  ((DTIM_TYPE *)DTIM1_BASE)
#define DTIM_2  ((DTIM_TYPE *)DTIM2_BASE)

#define IS_DTIM_TYPE(dtim)      ((dtim==DTIM_1)||((dtim==DTIM_2)))
/**** Declaration of constants ****/
#define DTIM1_BASE 0x40015000UL
#define DTIM2_BASE 0x40015020UL

/**** Declaration of variables ****/


/**** Declaration of functions ****/
extern void DTIM_Config( DTIM_TYPE *dtim, U32 period, U8 options );
extern void DTIM_Cmd( DTIM_TYPE *dtim, FUNCTION_STATE state );
extern void DTIM_SetPeriod( DTIM_TYPE *dtim, U32 period );
void DTIM_Hander( void );

#endif /*_DTIM_IF_H_*/
