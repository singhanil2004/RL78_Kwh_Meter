#include "ior5f10npj_ext.h"
  
    EXTERN _r_dsadc_interrupt

    PUBLIC ___interrupt_0x44

    SECTION `.text`:CODE:NOROOT(0)

___interrupt_0x44:

    SEL     RB3
    MOV     A, CS
    PUSH    AX
    
    CALL    F:_r_dsadc_interrupt
    
    SET1    DTCEN1.5

    POP     AX
    MOV     CS, A
    RETI
  
    END  