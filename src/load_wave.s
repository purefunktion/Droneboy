.globl _load_wave

_load_wave:
    ld  l,  e   ; first arg in function call gets loaded into DE
    ld  h,  d   ; sdcc calling conventions in the sdcc manual "SM83 calling conventions"
    ; Read about irp in the ASXXXX manual https://shop-pdp.net/ashtml/asmcro.htm#SECT5
    .irp ofs,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
        ld a, (hl+)
        ldh (__AUD3WAVERAM+ofs), a
    .endm
    ret