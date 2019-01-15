;******************************************************************************
;* PRU C/C++ Codegen                                              Unix v2.1.5 *
;* Date/Time created: Tue Jan 15 09:58:32 2019                                *
;******************************************************************************
	.compiler_opts --abi=eabi --endian=little --hll_source=on --object_format=elf --silicon_version=3 --symdebug:none 
	.global	__PRU_CREG_PRU_CFG
	.global	__PRU_CREG_PRU_INTC
	.weak	||CT_CFG||
||CT_CFG||:	.usect	".creg.PRU_CFG.noload.near",68,1
	.weak	||CT_INTC||
||CT_INTC||:	.usect	".creg.PRU_INTC.noload.far",5380,1
	.global	||pru_intc_map||
	.sect	".data:pru_intc_map", RW
	.clink
	.align	1
	.elfsym	||pru_intc_map||,SYM_SIZE(4)
||pru_intc_map||:
	.bits	16,8			; pru_intc_map[0].evt @ 0
	.bits	2,8			; pru_intc_map[0].ch @ 8
	.bits	17,8			; pru_intc_map[1].evt @ 16
	.bits	0,8			; pru_intc_map[1].ch @ 24

	.global	||resource_table||
	.sect	".resource_table:retain", RW
	.retain
	.align	1
	.elfsym	||resource_table||,SYM_SIZE(124)
||resource_table||:
	.bits	1,32			; resource_table.base.ver @ 0
	.bits	2,32			; resource_table.base.num @ 32
	.bits	0,32			; resource_table.base.reserved[0] @ 64
	.bits	0,32			; resource_table.base.reserved[1] @ 96
	.bits	24,32			; resource_table.offset[0] @ 128
	.bits	92,32			; resource_table.offset[1] @ 160
	.bits	3,32			; resource_table.rpmsg_vdev.type @ 192
	.bits	7,32			; resource_table.rpmsg_vdev.id @ 224
	.bits	0,32			; resource_table.rpmsg_vdev.notifyid @ 256
	.bits	1,32			; resource_table.rpmsg_vdev.dfeatures @ 288
	.bits	0,32			; resource_table.rpmsg_vdev.gfeatures @ 320
	.bits	0,32			; resource_table.rpmsg_vdev.config_len @ 352
	.bits	0,8			; resource_table.rpmsg_vdev.status @ 384
	.bits	2,8			; resource_table.rpmsg_vdev.num_of_vrings @ 392
	.bits	0,8			; resource_table.rpmsg_vdev.reserved[0] @ 400
	.bits	0,8			; resource_table.rpmsg_vdev.reserved[1] @ 408
	.bits	0,32			; resource_table.rpmsg_vring0.da @ 416
	.bits	16,32			; resource_table.rpmsg_vring0.align @ 448
	.bits	16,32			; resource_table.rpmsg_vring0.num @ 480
	.bits	0,32			; resource_table.rpmsg_vring0.notifyid @ 512
	.bits	0,32			; resource_table.rpmsg_vring0.reserved @ 544
	.bits	0,32			; resource_table.rpmsg_vring1.da @ 576
	.bits	16,32			; resource_table.rpmsg_vring1.align @ 608
	.bits	16,32			; resource_table.rpmsg_vring1.num @ 640
	.bits	0,32			; resource_table.rpmsg_vring1.notifyid @ 672
	.bits	0,32			; resource_table.rpmsg_vring1.reserved @ 704
	.bits	5,32			; resource_table.pru_ints.type @ 736
	.bits	1,32			; resource_table.pru_ints.u.sub_type @ 768
	.bits	20,32			; resource_table.pru_ints.rsc_size @ 800
	.bits	0,16			; resource_table.pru_ints.rsc.pru_ints.reserved @ 832
	.bits	0,8			; resource_table.pru_ints.rsc.pru_ints.channel_host[0] @ 848
	.bits	255,8			; resource_table.pru_ints.rsc.pru_ints.channel_host[1] @ 856
	.bits	2,8			; resource_table.pru_ints.rsc.pru_ints.channel_host[2] @ 864
	.bits	255,8			; resource_table.pru_ints.rsc.pru_ints.channel_host[3] @ 872
	.bits	255,8			; resource_table.pru_ints.rsc.pru_ints.channel_host[4] @ 880
	.bits	255,8			; resource_table.pru_ints.rsc.pru_ints.channel_host[5] @ 888
	.bits	255,8			; resource_table.pru_ints.rsc.pru_ints.channel_host[6] @ 896
	.bits	255,8			; resource_table.pru_ints.rsc.pru_ints.channel_host[7] @ 904
	.bits	255,8			; resource_table.pru_ints.rsc.pru_ints.channel_host[8] @ 912
	.bits	255,8			; resource_table.pru_ints.rsc.pru_ints.channel_host[9] @ 920
	.bits	2,32			; resource_table.pru_ints.rsc.pru_ints.num_evts @ 928
	.bits	||pru_intc_map||,32		; resource_table.pru_ints.rsc.pru_ints.event_channel @ 960

	.global	||rpmsg_receive_buf||
	.common	||rpmsg_receive_buf||,396,1
	.global	||rpmsg_send_buf||
	.common	||rpmsg_send_buf||,396,1
	.data
	.align	4
	.elfsym	||$O1$$||,SYM_SIZE(84)
||$O1$$||:
	.bits	||resource_table|| + 48,32		; $O1$$.status @ 0
	.bits	0,32			; $O1$$.switch1_start_cycle @ 32
	.bits	0,32			; $O1$$.switch1_curr_ms @ 64
	.bits	-1,32			; $O1$$.switch1_last_ms @ 96
	.space	68


;*********************************************************************
;* GROUPED GLOBALS FOR $O1$$                                         *
;*********************************************************************
||status||:	.set	||$O1$$||+0
	.global	||status||
||switch1_start_cycle||:	.set	||$O1$$||+4
	.global	||switch1_start_cycle||
||switch1_curr_ms||:	.set	||$O1$$||+8
	.global	||switch1_curr_ms||
||switch1_last_ms||:	.set	||$O1$$||+12
	.global	||switch1_last_ms||
||rpmsg_src||:	.set	||$O1$$||+16
	.global	||rpmsg_src||
||rpmsg_dst||:	.set	||$O1$$||+18
	.global	||rpmsg_dst||
||rpmsg_receive_len||:	.set	||$O1$$||+20
	.global	||rpmsg_receive_len||
||switch1_last_p8_15||:	.set	||$O1$$||+22
	.global	||switch1_last_p8_15||
||switch2_last_p8_21||:	.set	||$O1$$||+23
	.global	||switch2_last_p8_21||
||rpmsg_transport$0||:	.set	||$O1$$||+24
	.sect	".rodata"
	.align	4
	.elfsym	||$O2$$||,SYM_SIZE(128)
||$O2$$||:
	.bits	115,8			; $O2$$.$P$T85$1$0[0] @ 0
	.bits	119,8			; $O2$$.$P$T85$1$0[1] @ 8
	.bits	105,8			; $O2$$.$P$T85$1$0[2] @ 16
	.bits	116,8			; $O2$$.$P$T85$1$0[3] @ 24
	.bits	99,8			; $O2$$.$P$T85$1$0[4] @ 32
	.bits	104,8			; $O2$$.$P$T85$1$0[5] @ 40
	.bits	50,8			; $O2$$.$P$T85$1$0[6] @ 48
	.bits	32,8			; $O2$$.$P$T85$1$0[7] @ 56
	.bits	101,8			; $O2$$.$P$T85$1$0[8] @ 64
	.bits	99,8			; $O2$$.$P$T85$1$0[9] @ 72
	.bits	104,8			; $O2$$.$P$T85$1$0[10] @ 80
	.bits	111,8			; $O2$$.$P$T85$1$0[11] @ 88
	.bits	0,8			; $O2$$.$P$T85$1$0[12] @ 96
	.bits	86,8			; $O2$$.$P$T86$2$0[0] @ 104
	.bits	97,8			; $O2$$.$P$T86$2$0[1] @ 112
	.bits	108,8			; $O2$$.$P$T86$2$0[2] @ 120
	.bits	105,8			; $O2$$.$P$T86$2$0[3] @ 128
	.bits	100,8			; $O2$$.$P$T86$2$0[4] @ 136
	.bits	32,8			; $O2$$.$P$T86$2$0[5] @ 144
	.bits	113,8			; $O2$$.$P$T86$2$0[6] @ 152
	.bits	117,8			; $O2$$.$P$T86$2$0[7] @ 160
	.bits	101,8			; $O2$$.$P$T86$2$0[8] @ 168
	.bits	114,8			; $O2$$.$P$T86$2$0[9] @ 176
	.bits	105,8			; $O2$$.$P$T86$2$0[10] @ 184
	.bits	101,8			; $O2$$.$P$T86$2$0[11] @ 192
	.bits	115,8			; $O2$$.$P$T86$2$0[12] @ 200
	.bits	58,8			; $O2$$.$P$T86$2$0[13] @ 208
	.bits	10,8			; $O2$$.$P$T86$2$0[14] @ 216
	.bits	115,8			; $O2$$.$P$T86$2$0[15] @ 224
	.bits	119,8			; $O2$$.$P$T86$2$0[16] @ 232
	.bits	105,8			; $O2$$.$P$T86$2$0[17] @ 240
	.bits	116,8			; $O2$$.$P$T86$2$0[18] @ 248
	.bits	99,8			; $O2$$.$P$T86$2$0[19] @ 256
	.bits	104,8			; $O2$$.$P$T86$2$0[20] @ 264
	.bits	49,8			; $O2$$.$P$T86$2$0[21] @ 272
	.bits	32,8			; $O2$$.$P$T86$2$0[22] @ 280
	.bits	45,8			; $O2$$.$P$T86$2$0[23] @ 288
	.bits	32,8			; $O2$$.$P$T86$2$0[24] @ 296
	.bits	114,8			; $O2$$.$P$T86$2$0[25] @ 304
	.bits	101,8			; $O2$$.$P$T86$2$0[26] @ 312
	.bits	116,8			; $O2$$.$P$T86$2$0[27] @ 320
	.bits	117,8			; $O2$$.$P$T86$2$0[28] @ 328
	.bits	114,8			; $O2$$.$P$T86$2$0[29] @ 336
	.bits	110,8			; $O2$$.$P$T86$2$0[30] @ 344
	.bits	115,8			; $O2$$.$P$T86$2$0[31] @ 352
	.bits	32,8			; $O2$$.$P$T86$2$0[32] @ 360
	.bits	116,8			; $O2$$.$P$T86$2$0[33] @ 368
	.bits	104,8			; $O2$$.$P$T86$2$0[34] @ 376
	.bits	101,8			; $O2$$.$P$T86$2$0[35] @ 384
	.bits	32,8			; $O2$$.$P$T86$2$0[36] @ 392
	.bits	105,8			; $O2$$.$P$T86$2$0[37] @ 400
	.bits	110,8			; $O2$$.$P$T86$2$0[38] @ 408
	.bits	116,8			; $O2$$.$P$T86$2$0[39] @ 416
	.bits	101,8			; $O2$$.$P$T86$2$0[40] @ 424
	.bits	114,8			; $O2$$.$P$T86$2$0[41] @ 432
	.bits	118,8			; $O2$$.$P$T86$2$0[42] @ 440
	.bits	97,8			; $O2$$.$P$T86$2$0[43] @ 448
	.bits	108,8			; $O2$$.$P$T86$2$0[44] @ 456
	.bits	32,8			; $O2$$.$P$T86$2$0[45] @ 464
	.bits	98,8			; $O2$$.$P$T86$2$0[46] @ 472
	.bits	101,8			; $O2$$.$P$T86$2$0[47] @ 480
	.bits	116,8			; $O2$$.$P$T86$2$0[48] @ 488
	.bits	119,8			; $O2$$.$P$T86$2$0[49] @ 496
	.bits	101,8			; $O2$$.$P$T86$2$0[50] @ 504
	.bits	101,8			; $O2$$.$P$T86$2$0[51] @ 512
	.bits	110,8			; $O2$$.$P$T86$2$0[52] @ 520
	.bits	32,8			; $O2$$.$P$T86$2$0[53] @ 528
	.bits	108,8			; $O2$$.$P$T86$2$0[54] @ 536
	.bits	97,8			; $O2$$.$P$T86$2$0[55] @ 544
	.bits	115,8			; $O2$$.$P$T86$2$0[56] @ 552
	.bits	116,8			; $O2$$.$P$T86$2$0[57] @ 560
	.bits	32,8			; $O2$$.$P$T86$2$0[58] @ 568
	.bits	115,8			; $O2$$.$P$T86$2$0[59] @ 576
	.bits	116,8			; $O2$$.$P$T86$2$0[60] @ 584
	.bits	97,8			; $O2$$.$P$T86$2$0[61] @ 592
	.bits	116,8			; $O2$$.$P$T86$2$0[62] @ 600
	.bits	101,8			; $O2$$.$P$T86$2$0[63] @ 608
	.bits	32,8			; $O2$$.$P$T86$2$0[64] @ 616
	.bits	99,8			; $O2$$.$P$T86$2$0[65] @ 624
	.bits	104,8			; $O2$$.$P$T86$2$0[66] @ 632
	.bits	97,8			; $O2$$.$P$T86$2$0[67] @ 640
	.bits	110,8			; $O2$$.$P$T86$2$0[68] @ 648
	.bits	103,8			; $O2$$.$P$T86$2$0[69] @ 656
	.bits	101,8			; $O2$$.$P$T86$2$0[70] @ 664
	.bits	115,8			; $O2$$.$P$T86$2$0[71] @ 672
	.bits	32,8			; $O2$$.$P$T86$2$0[72] @ 680
	.bits	105,8			; $O2$$.$P$T86$2$0[73] @ 688
	.bits	110,8			; $O2$$.$P$T86$2$0[74] @ 696
	.bits	32,8			; $O2$$.$P$T86$2$0[75] @ 704
	.bits	109,8			; $O2$$.$P$T86$2$0[76] @ 712
	.bits	105,8			; $O2$$.$P$T86$2$0[77] @ 720
	.bits	108,8			; $O2$$.$P$T86$2$0[78] @ 728
	.bits	105,8			; $O2$$.$P$T86$2$0[79] @ 736
	.bits	115,8			; $O2$$.$P$T86$2$0[80] @ 744
	.bits	101,8			; $O2$$.$P$T86$2$0[81] @ 752
	.bits	99,8			; $O2$$.$P$T86$2$0[82] @ 760
	.bits	111,8			; $O2$$.$P$T86$2$0[83] @ 768
	.bits	110,8			; $O2$$.$P$T86$2$0[84] @ 776
	.bits	100,8			; $O2$$.$P$T86$2$0[85] @ 784
	.bits	115,8			; $O2$$.$P$T86$2$0[86] @ 792
	.bits	44,8			; $O2$$.$P$T86$2$0[87] @ 800
	.bits	32,8			; $O2$$.$P$T86$2$0[88] @ 808
	.bits	45,8			; $O2$$.$P$T86$2$0[89] @ 816
	.bits	49,8			; $O2$$.$P$T86$2$0[90] @ 824
	.bits	32,8			; $O2$$.$P$T86$2$0[91] @ 832
	.bits	105,8			; $O2$$.$P$T86$2$0[92] @ 840
	.bits	102,8			; $O2$$.$P$T86$2$0[93] @ 848
	.bits	32,8			; $O2$$.$P$T86$2$0[94] @ 856
	.bits	110,8			; $O2$$.$P$T86$2$0[95] @ 864
	.bits	101,8			; $O2$$.$P$T86$2$0[96] @ 872
	.bits	118,8			; $O2$$.$P$T86$2$0[97] @ 880
	.bits	101,8			; $O2$$.$P$T86$2$0[98] @ 888
	.bits	114,8			; $O2$$.$P$T86$2$0[99] @ 896
	.bits	32,8			; $O2$$.$P$T86$2$0[100] @ 904
	.bits	104,8			; $O2$$.$P$T86$2$0[101] @ 912
	.bits	97,8			; $O2$$.$P$T86$2$0[102] @ 920
	.bits	112,8			; $O2$$.$P$T86$2$0[103] @ 928
	.bits	112,8			; $O2$$.$P$T86$2$0[104] @ 936
	.bits	101,8			; $O2$$.$P$T86$2$0[105] @ 944
	.bits	110,8			; $O2$$.$P$T86$2$0[106] @ 952
	.bits	101,8			; $O2$$.$P$T86$2$0[107] @ 960
	.bits	100,8			; $O2$$.$P$T86$2$0[108] @ 968
	.bits	46,8			; $O2$$.$P$T86$2$0[109] @ 976
	.bits	10,8			; $O2$$.$P$T86$2$0[110] @ 984
	.bits	0,8			; $O2$$.$P$T86$2$0[111] @ 992
	.bits	0,24


;*********************************************************************
;* GROUPED GLOBALS FOR $O2$$                                         *
;*********************************************************************
||$P$T85$1$0||:	.set	||$O2$$||+0
||$P$T86$2$0||:	.set	||$O2$$||+13
;	optpru /tmp/TI0C4IDOlxE /tmp/TI0C4clLExm 
;	acpiapru -@/tmp/TI0C4SAUGGd 
;

; The following variables will be grouped together to share a common
; base address.  References in the source interlisting will look like
; "1$$.switch1_start_cycle", "K$1->switch1_start_cycle", "C$1->switch1_start_cycle", "&1$$+4", and "C$1+4".
;
;    --offset--    --size--	--variable--
;
;         0            4	extern volatile unsigned char *status
;         4            4	extern unsigned switch1_start_cycle
;         8            4	extern int switch1_curr_ms
;        12            4	extern int switch1_last_ms
;        16            2	extern unsigned short rpmsg_src
;        18            2	extern unsigned short rpmsg_dst
;        20            2	extern unsigned short rpmsg_receive_len
;        22            1	extern unsigned char switch1_last_p8_15
;        23            1	extern unsigned char switch2_last_p8_21
;        24           60	static struct pru_rpmsg_transport rpmsg_transport$0
;
;

; The following variables will be grouped together to share a common
; base address.  References in the source interlisting will look like
; "2$$.$P$T86$2$0", "K$1->$P$T86$2$0", "C$1->$P$T86$2$0", "&2$$+13", and "C$1+13".
;
;    --offset--    --size--	--variable--
;
;         0           13	static const unsigned char $P$T85$1$0[13]
;        13          112	static const unsigned char $P$T86$2$0[112]
;
	.sect	".text:ui32_to_string"
	.clink
	.global	||ui32_to_string||

;***************************************************************
;* FNAME: ui32_to_string                FR SIZE:  18           *
;*                                                             *
;* FUNCTION ENVIRONMENT                                        *
;*                                                             *
;* FUNCTION PROPERTIES                                         *
;*                            0 Auto, 18 SOE     *
;***************************************************************

||ui32_to_string||:
;* --------------------------------------------------------------------------*
        SUB       r2, r2, 0x12          ; [] 
        SBBO      &r3.b2, r2, 0, 18     ; [] 
        MOV       r4, r15               ; [] |pru0.c:217| buffer
        MOV       r6, r14               ; [] |pru0.c:217| n
;*** 218	-----------------------    if ( n ) goto g3;
        QBNE      ||$C$L1||, r6, 0x00   ; [] |pru0.c:218| n
;* --------------------------------------------------------------------------*
;*** 220	-----------------------    *buffer = 48;
        LDI       r0.b0, 0x30           ; [] |pru0.c:220| 
        SBBO      &r0.b0, r4, 0, 1      ; [] |pru0.c:220| buffer
;*** 221	-----------------------    buffer[1] = 0;
        LDI       r0.b0, 0x00           ; [] |pru0.c:221| 
        SBBO      &r0.b0, r4, 1, 1      ; [] |pru0.c:221| buffer
;*** 222	-----------------------    goto g8;
        JMP       ||$C$L4||             ; [] |pru0.c:222| 
;* --------------------------------------------------------------------------*
||$C$L1||:    
;***	-----------------------g3:
;***  	-----------------------    #pragma LOOP_FLAGS(4096u)
;***  	-----------------------    U$11 = buffer;
        MOV       r7, r4                ; [] $O$U11,buffer
;*** 224	-----------------------    i = 0;
        ZERO      &r5, 4                ; [] |pru0.c:224| i
;* --------------------------------------------------------------------------*
;*   BEGIN LOOP ||$C$L2||
;*
;*   Loop source line                : 225
;*   Loop closing brace source line  : 230
;*   Known Minimum Trip Count        : 1
;*   Known Maximum Trip Count        : 4294967295
;*   Known Max Trip Count Factor     : 1
;* --------------------------------------------------------------------------*
||$C$L2||:    
;***	-----------------------g4:
;*** 227	-----------------------    *U$11 = n%10u+48u;
        LDI       r15, 0x000a           ; [] |pru0.c:227| 
        JAL       r3.w2, ||__pruabi_remu|| ; [] |pru0.c:227| __pruabi_remu
        ADD       r0.b0, r14.b0, 0x30   ; [] |pru0.c:227| 
        SBBO      &r0.b0, r7, 0, 1      ; [] |pru0.c:227| $O$U11
;*** 228	-----------------------    ++U$11;
        ADD       r7, r7, 0x01          ; [] |pru0.c:228| $O$U11,$O$U11
;*** 228	-----------------------    ++i;
        ADD       r5, r5, 0x01          ; [] |pru0.c:228| i,i
;*** 229	-----------------------    if ( n /= 10u ) goto g4;
        MOV       r14, r6               ; [] |pru0.c:229| n
        LDI       r15, 0x000a           ; [] |pru0.c:229| 
        JAL       r3.w2, ||__pruabi_divu|| ; [] |pru0.c:229| __pruabi_divu
        MOV       r6, r14               ; [] |pru0.c:229| n
        QBNE      ||$C$L2||, r6, 0x00   ; [] |pru0.c:229| n
;* --------------------------------------------------------------------------*
;*** 231	-----------------------    *U$11 = 0;
        LDI       r0.b0, 0x00           ; [] |pru0.c:231| 
        SBBO      &r0.b0, r7, 0, 1      ; [] |pru0.c:231| $O$U11
;*** 233	-----------------------    if ( i < 2 ) goto g8;
        MOV       r1, r5                ; [] |pru0.c:233| i
        XOR       r1.b3, r1.b3, 0x80    ; [] |pru0.c:233| 
        LDI       r0, 0x0002            ; [] |pru0.c:233| 
        XOR       r0.b3, r0.b3, 0x80    ; [] |pru0.c:233| 
        QBGT      ||$C$L4||, r1, r0     ; [] |pru0.c:233| 
;* --------------------------------------------------------------------------*
;***  	-----------------------    #pragma MUST_ITERATE(1, 1073741823, 1)
;***  	-----------------------    #pragma LOOP_FLAGS(4096u)
;***  	-----------------------    U$25 = &buffer[i-1];
        ADD       r0, r5, r4            ; [] i,buffer
        SUB       r14, r0, 0x01         ; [] $O$U25
;***  	-----------------------    U$27 = buffer;
;*** 235	-----------------------    L$1 = i/2;
        LSR       r0, r5, 0x1f          ; [] |pru0.c:235| 
        ADD       r0, r5, r0            ; [] |pru0.c:235| 
        LSR       r0, r0, 0x01          ; [] |pru0.c:235| 
        QBBC      ||$C$L3||, r0, 0x1e   ; [] |pru0.c:235| 
;* --------------------------------------------------------------------------*
        FILL      &r1, 4                ; [] |pru0.c:235| 
        LSL       r1, r1, 0x1f          ; [] |pru0.c:235| 
        OR        r0, r0, r1            ; [] |pru0.c:235| 
;* --------------------------------------------------------------------------*
;*   BEGIN LOOP ||$C$L3||
;*
;*   Loop source line                : 233
;*   Loop closing brace source line  : 238
;*   Known Minimum Trip Count        : 1
;*   Known Maximum Trip Count        : 1073741823
;*   Known Max Trip Count Factor     : 1
;* --------------------------------------------------------------------------*
||$C$L3||:    
;***	-----------------------g7:
;*** 235	-----------------------    C$3 = *U$27 ^= *U$25;
        LBBO      &r1.b0, r14, 0, 1     ; [] |pru0.c:235| $O$U25
        LBBO      &r1.b1, r4, 0, 1      ; [] |pru0.c:235| $O$U27
        XOR       r1.b0, r1.b1, r1.b0   ; [] |pru0.c:235| 
        SBBO      &r1.b0, r4, 0, 1      ; [] |pru0.c:235| $O$U27
;*** 236	-----------------------    C$2 = *U$25 ^= C$3;
        LBBO      &r1.b1, r14, 0, 1     ; [] |pru0.c:236| $O$U25
        XOR       r1.b0, r1.b1, r1.b0   ; [] |pru0.c:236| $O$C3
        SBBO      &r1.b0, r14, 0, 1     ; [] |pru0.c:236| $O$U25
;*** 237	-----------------------    *U$27 ^= C$2;
        LBBO      &r1.b1, r4, 0, 1      ; [] |pru0.c:237| $O$U27
        XOR       r1.b0, r1.b1, r1.b0   ; [] |pru0.c:237| $O$C2
        SBBO      &r1.b0, r4, 0, 1      ; [] |pru0.c:237| $O$U27
;*** 233	-----------------------    --U$25;
        SUB       r14, r14, 0x01        ; [] |pru0.c:233| $O$U25,$O$U25
;*** 233	-----------------------    ++U$27;
        ADD       r4, r4, 0x01          ; [] |pru0.c:233| $O$U27,$O$U27
;*** 233	-----------------------    if ( L$1 = L$1-1 ) goto g7;
        SUB       r0, r0, 0x01          ; [] |pru0.c:233| $O$L1,$O$L1
        QBNE      ||$C$L3||, r0, 0x00   ; [] |pru0.c:233| $O$L1
;***	-----------------------g8:
;***  	-----------------------    return;
;* --------------------------------------------------------------------------*
||$C$L4||:    
        LBBO      &r3.b2, r2, 0, 18     ; [] 
        ADD       r2, r2, 0x12          ; [] 
        JMP       r3.w2                 ; [] 
	.sect	".text:set_initial_switches_state"
	.clink
	.global	||set_initial_switches_state||

;***************************************************************
;* FNAME: set_initial_switches_state    FR SIZE:   0           *
;*                                                             *
;* FUNCTION ENVIRONMENT                                        *
;*                                                             *
;* FUNCTION PROPERTIES                                         *
;*                            0 Auto,  0 SOE     *
;***************************************************************

||set_initial_switches_state||:
;* --------------------------------------------------------------------------*
;*** 134	-----------------------    *((C$1 = &1$$)+22) = __R31>>15&1;
        LDI       r0, ||$O1$$||         ; [] |pru0.c:134| $O$C1,$O1$$
        LSR       r1, r31, 0x0f         ; [] |pru0.c:134| 
        AND       r1.b0, r1.b0, 0x01    ; [] |pru0.c:134| 
        SBBO      &r1.b0, r0, 22, 1     ; [] |pru0.c:134| $O$C1
;*** 135	-----------------------    C$1[23] = __R31>>12&1;
        LSR       r1, r31, 0x0c         ; [] |pru0.c:135| 
        AND       r1.b0, r1.b0, 0x01    ; [] |pru0.c:135| 
        SBBO      &r1.b0, r0, 23, 1     ; [] |pru0.c:135| $O$C1
;***  	-----------------------    return;
        JMP       r3.w2                 ; [] 
	.sect	".text:send_to_arm"
	.clink
	.global	||send_to_arm||

;***************************************************************
;* FNAME: send_to_arm                   FR SIZE:   2           *
;*                                                             *
;* FUNCTION ENVIRONMENT                                        *
;*                                                             *
;* FUNCTION PROPERTIES                                         *
;*                            0 Auto,  2 SOE     *
;***************************************************************

||send_to_arm||:
;* --------------------------------------------------------------------------*
        SUB       r2, r2, 0x02          ; [] 
        SBBO      &r3.b2, r2, 0, 2      ; [] 
        MOV       r17, r14              ; [] |pru0.c:202| message
;***  	-----------------------    #pragma LOOP_FLAGS(4096u)
;***  	-----------------------    U$8 = &message[-1];
        SUB       r0, r17, 0x01         ; [] $O$U8,message
;*** 156	-----------------------    n = 0xffffffffu;  // [0]
        FILL      &r18, 4               ; [] |/usr/share/ti/cgt-pru/include/string.h:156| n
;* --------------------------------------------------------------------------*
;*   BEGIN LOOP ||$C$L5||
;*
;*   Loop source line                : 159
;*   Loop closing brace source line  : 159
;*   Known Minimum Trip Count        : 1
;*   Known Maximum Trip Count        : 4294967295
;*   Known Max Trip Count Factor     : 1
;* --------------------------------------------------------------------------*
||$C$L5||:    
;***	-----------------------g2:
;*** 159	-----------------------    ++n;  // [0]
        ADD       r18, r18, 0x01        ; [] |/usr/share/ti/cgt-pru/include/string.h:159| n,n
;*** 159	-----------------------    if ( *(++U$8) ) goto g2;  // [0]
        ADD       r0, r0, 0x01          ; [] |/usr/share/ti/cgt-pru/include/string.h:159| $O$U8,$O$U8
        LBBO      &r1.b0, r0, 0, 1      ; [] |/usr/share/ti/cgt-pru/include/string.h:159| $O$U8
        QBNE      ||$C$L5||, r1.b0, 0x00 ; [] |/usr/share/ti/cgt-pru/include/string.h:159| 
;* --------------------------------------------------------------------------*
;*** 203	-----------------------    C$2 = &1$$;
        LDI       r0, ||$O1$$||         ; [] |pru0.c:203| $O$C2,$O1$$
;*** 203	-----------------------    pru_rpmsg_send((struct pru_rpmsg_transport *)C$2+24, (unsigned)C$2[9], (unsigned)C$2[8], (void *)message, (unsigned short)n);
        ADD       r14, r0, 0x18         ; [] |pru0.c:203| $O$C2
        LBBO      &r1.w0, r0, 18, 2     ; [] |pru0.c:203| $O$C2
        MOV       r15, r1.w0            ; [] |pru0.c:203| 
        LBBO      &r0.w0, r0, 16, 2     ; [] |pru0.c:203| $O$C2
        MOV       r16, r0.w0            ; [] |pru0.c:203| 
        JAL       r3.w2, ||pru_rpmsg_send|| ; [] |pru0.c:203| pru_rpmsg_send
;***  	-----------------------    return;
        LBBO      &r3.b2, r2, 0, 2      ; [] 
        ADD       r2, r2, 0x02          ; [] 
        JMP       r3.w2                 ; [] 
	.sect	".text:reduce_cycles_and_update_switch1"
	.clink
	.global	||reduce_cycles_and_update_switch1||

;***************************************************************
;* FNAME: reduce_cycles_and_update_switch1 FR SIZE:   0           *
;*                                                             *
;* FUNCTION ENVIRONMENT                                        *
;*                                                             *
;* FUNCTION PROPERTIES                                         *
;*                            0 Auto,  0 SOE     *
;***************************************************************

||reduce_cycles_and_update_switch1||:
;* --------------------------------------------------------------------------*
;*** 148	-----------------------    *(C$3 = (volatile struct $$fake80 *)0x22000u) &= 0xfffffff7u;
        LDI32     r0, 0x00022000        ; [] |pru0.c:148| $O$C3
        LBBO      &r1, r0, 0, 4         ; [] |pru0.c:148| 
        CLR       r1, r1, 0x00000003    ; [] |pru0.c:148| 
        SBBO      &r1, r0, 0, 4         ; [] |pru0.c:148| 
;*** 149	-----------------------    C$2 = (volatile union $$fake166 *)C$3+12;
        ADD       r1, r0, 0x0c          ; [] |pru0.c:149| $O$C2,$O$C3
;*** 149	-----------------------    *C$2 -= 0xee6b2800u;
        LDI32     r14, 0xee6b2800       ; [] |pru0.c:149| 
        LBBO      &r15, r1, 0, 4        ; [] |pru0.c:149| $O$C2
        SUB       r14, r15, r14         ; [] |pru0.c:149| 
        SBBO      &r14, r1, 0, 4        ; [] |pru0.c:149| $O$C2
;*** 150	-----------------------    *C$3 |= 0x8u;
        LBBO      &r14, r0, 0, 4        ; [] |pru0.c:150| $O$C3
        SET       r14, r14, 0x00000003  ; [] |pru0.c:150| 
        SBBO      &r14, r0, 0, 4        ; [] |pru0.c:150| $O$C3
;*** 151	-----------------------    *((C$1 = &1$$)+4) = *C$2;
        LDI       r0, ||$O1$$||         ; [] |pru0.c:151| $O$C1,$O1$$
        LBBO      &r1, r1, 0, 4         ; [] |pru0.c:151| $O$C2
        SBBO      &r1, r0, 4, 4         ; [] |pru0.c:151| $O$C1
;*** 152	-----------------------    *((int *)C$1+8) += 20000;
        LDI       r1, 0x4e20            ; [] |pru0.c:152| 
        LBBO      &r14, r0, 8, 4        ; [] |pru0.c:152| $O$C1
        ADD       r1, r14, r1           ; [] |pru0.c:152| 
        SBBO      &r1, r0, 8, 4         ; [] |pru0.c:152| $O$C1
;***  	-----------------------    return;
        JMP       r3.w2                 ; [] 
	.sect	".text:are_cycles_past_threshold"
	.clink
	.global	||are_cycles_past_threshold||

;***************************************************************
;* FNAME: are_cycles_past_threshold     FR SIZE:   0           *
;*                                                             *
;* FUNCTION ENVIRONMENT                                        *
;*                                                             *
;* FUNCTION PROPERTIES                                         *
;*                            0 Auto,  0 SOE     *
;***************************************************************

||are_cycles_past_threshold||:
;* --------------------------------------------------------------------------*
;*** 142	-----------------------    return *((volatile union $$fake166 *)(volatile struct $$fake88 *)0x22000u+12) > 0xee6b2800u;
        LDI       r14.b0, 0x00          ; [] |pru0.c:142| 
        LDI32     r0, 0xee6b2800        ; [] |pru0.c:142| 
        LDI32     r1, 0x0002200c        ; [] |pru0.c:142| 
        LBBO      &r1, r1, 0, 4         ; [] |pru0.c:142| 
        QBLE      ||$C$L6||, r0, r1     ; [] |pru0.c:142| 
;* --------------------------------------------------------------------------*
        LDI       r14.b0, 0x01          ; [] |pru0.c:142| 
;* --------------------------------------------------------------------------*
||$C$L6||:    
        JMP       r3.w2                 ; [] 
	.sect	".text:handle_switch1_p8_15_change"
	.clink
	.global	||handle_switch1_p8_15_change||

;***************************************************************
;* FNAME: handle_switch1_p8_15_change   FR SIZE:  18           *
;*                                                             *
;* FUNCTION ENVIRONMENT                                        *
;*                                                             *
;* FUNCTION PROPERTIES                                         *
;*                            0 Auto, 18 SOE     *
;***************************************************************

||handle_switch1_p8_15_change||:
;* --------------------------------------------------------------------------*
        SUB       r2, r2, 0x12          ; [] 
        SBBO      &r3.b2, r2, 0, 18     ; [] 
;*** 157	-----------------------    curr_cycle = *((volatile union $$fake166 *)(volatile struct $$fake88 *)0x22000u+12);
        LDI32     r0, 0x0002200c        ; [] |pru0.c:157| 
        LBBO      &r5, r0, 0, 4         ; [] |pru0.c:157| curr_cycle
;*** 158	-----------------------    *((C$3 = &1$$)+22) = switch1_curr_p8_15;
        LDI       r4, ||$O1$$||         ; [] |pru0.c:158| $O$C3,$O1$$
        SBBO      &r14.b0, r4, 22, 1    ; [] |pru0.c:158| $O$C3,switch1_curr_p8_15
;*** 159	-----------------------    C$1 = (int *)C$3+8;
        ADD       r7, r4, 0x08          ; [] |pru0.c:159| $O$C1,$O$C3
;*** 159	-----------------------    C$2 = (unsigned *)C$3+4;
        ADD       r6, r4, 0x04          ; [] |pru0.c:159| $O$C2,$O$C3
;*** 159	-----------------------    *((int *)C$3+12) = (int)((curr_cycle-*C$2)/200000u)+*C$1;
        LBBO      &r0, r6, 0, 4         ; [] |pru0.c:159| $O$C2
        RSB       r14, r0, r5           ; [] |pru0.c:159| curr_cycle
        LDI32     r15, 0x00030d40       ; [] |pru0.c:159| 
        JAL       r3.w2, ||__pruabi_divu|| ; [] |pru0.c:159| __pruabi_divu
        LBBO      &r0, r7, 0, 4         ; [] |pru0.c:159| $O$C1
        ADD       r0, r14, r0           ; [] |pru0.c:159| 
        SBBO      &r0, r4, 12, 4        ; [] |pru0.c:159| $O$C3
;*** 161	-----------------------    *C$2 = curr_cycle;
        SBBO      &r5, r6, 0, 4         ; [] |pru0.c:161| $O$C2,curr_cycle
;*** 162	-----------------------    *C$1 = 0;
        LDI       r0, 0x0000            ; [] |pru0.c:162| 
        SBBO      &r0, r7, 0, 4         ; [] |pru0.c:162| $O$C1
;***  	-----------------------    return;
        LBBO      &r3.b2, r2, 0, 18     ; [] 
        ADD       r2, r2, 0x12          ; [] 
        JMP       r3.w2                 ; [] 
	.sect	".text:attempt_receive_from_arm"
	.clink
	.global	||attempt_receive_from_arm||

;***************************************************************
;* FNAME: attempt_receive_from_arm      FR SIZE:   2           *
;*                                                             *
;* FUNCTION ENVIRONMENT                                        *
;*                                                             *
;* FUNCTION PROPERTIES                                         *
;*                            0 Auto,  2 SOE     *
;***************************************************************

||attempt_receive_from_arm||:
;* --------------------------------------------------------------------------*
        SUB       r2, r2, 0x02          ; [] 
        SBBO      &r3.b2, r2, 0, 2      ; [] 
;*** 168	-----------------------    C$1 = &1$$;
        LDI       r0, ||$O1$$||         ; [] |pru0.c:168| $O$C1,$O1$$
;*** 168	-----------------------    return (unsigned char)pru_rpmsg_receive((struct pru_rpmsg_transport *)C$1+24, C$1+16, C$1+18, &rpmsg_receive_buf, C$1+20);
        ADD       r14, r0, 0x18         ; [] |pru0.c:168| $O$C1
        ADD       r15, r0, 0x10         ; [] |pru0.c:168| $O$C1
        ADD       r16, r0, 0x12         ; [] |pru0.c:168| $O$C1
        ADD       r18, r0, 0x14         ; [] |pru0.c:168| $O$C1
        LDI       r17, ||rpmsg_receive_buf|| ; [] |pru0.c:168| rpmsg_receive_buf
        JAL       r3.w2, ||pru_rpmsg_receive|| ; [] |pru0.c:168| pru_rpmsg_receive
        LBBO      &r3.b2, r2, 0, 2      ; [] 
        ADD       r2, r2, 0x02          ; [] 
        JMP       r3.w2                 ; [] 
;

; The following local variables in handle_query_from_arm() will be grouped together
; to share stack space among distinct scoping blocks.  References
; in the source interlisting will look like "O$1.s3_1.l3_2" or "&$O$O1+0".
;
;    --offset--    --reference--		 --variable--
;
;         0	  O$1.s3_1.l3_2                  unsigned char message[112]  [pru0.c:194]
;         0	  O$1.s3_3.l3_4                  unsigned char message[13]  [pru0.c:188]
;
	.sect	".text:handle_query_from_arm"
	.clink
	.global	||handle_query_from_arm||

;***************************************************************
;* FNAME: handle_query_from_arm         FR SIZE: 118           *
;*                                                             *
;* FUNCTION ENVIRONMENT                                        *
;*                                                             *
;* FUNCTION PROPERTIES                                         *
;*                           112 Auto,  6 SOE     *
;***************************************************************

||handle_query_from_arm||:
;* --------------------------------------------------------------------------*
        SUB       r2, r2, 0x76          ; [] 
        SBBO      &r3.b2, r2, 112, 6    ; [] 
;***  	-----------------------    #pragma MUST_ITERATE(396, 396, 396)
;***  	-----------------------    #pragma LOOP_FLAGS(0u)
;***  	-----------------------    K$6 = 0;
        ZERO      &r1, 4                ; [] $O$K6
;*** 327	-----------------------    m = &rpmsg_send_buf[0];  // [3]
        LDI       r0, ||rpmsg_send_buf|| ; [] |/usr/share/ti/cgt-pru/include/string.h:327| m,rpmsg_send_buf
;***  	-----------------------    L$1 = 396u;
        LDI       r1.w1, 0x018c         ; [] $O$L1
;***	-----------------------g2:
;***	-----------------------g22:
        LOOP      ||$C$L8||, r1.w1      ; [] $O$L1
;* --------------------------------------------------------------------------*
;*   BEGIN LOOP ||$C$L7||
;*
;*   Loop source line                : 329
;*   Loop closing brace source line  : 329
;*   Known Minimum Trip Count        : 396
;*   Known Maximum Trip Count        : 396
;*   Known Max Trip Count Factor     : 396
;* --------------------------------------------------------------------------*
||$C$L7||:    
;*** 329	-----------------------    *m = K$6;  // [3]
        SBBO      &r1.b0, r0, 0, 1      ; [] |/usr/share/ti/cgt-pru/include/string.h:329| m,$O$K6
;*** 329	-----------------------    ++m;  // [3]
        ADD       r0, r0, 0x01          ; [] |/usr/share/ti/cgt-pru/include/string.h:329| m,m
;*** 329	-----------------------    if ( --L$1 != 0 ) goto g22;  // [3]
        ; END LOOP ; [] |/usr/share/ti/cgt-pru/include/string.h:329| 
;* --------------------------------------------------------------------------*
||$C$L8||:    
;***  	-----------------------    #pragma LOOP_FLAGS(4352u)
;*** 181	-----------------------    string1 = K$18 = &rpmsg_receive_buf[0];
        LDI       r0, ||rpmsg_receive_buf|| ; [] |pru0.c:181| $O$K18,rpmsg_receive_buf
        MOV       r14, r0               ; [] |pru0.c:181| string1,$O$K18
;*** 181	-----------------------    string2 = (const unsigned char *)"switch1";
        LDI32     r1, $C$SL1            ; [] |pru0.c:181| string2
;*** 327	-----------------------    K$1 = &rpmsg_send_buf[0];  // [3]
        LDI       r4, ||rpmsg_send_buf|| ; [] |/usr/share/ti/cgt-pru/include/string.h:327| $O$K1,rpmsg_send_buf
;* --------------------------------------------------------------------------*
;*   BEGIN LOOP ||$C$L9||
;*
;*   Loop source line                : 260
;*   Loop closing brace source line  : 266
;*   Known Minimum Trip Count        : 1
;*   Known Maximum Trip Count        : 4294967295
;*   Known Max Trip Count Factor     : 1
;* --------------------------------------------------------------------------*
||$C$L9||:    
;***	-----------------------g4:
;*** 263	-----------------------    C$4 = *string1;  // [2]
        LBBO      &r16.b0, r14, 0, 1    ; [] |/usr/share/ti/cgt-pru/include/string.h:263| $O$C4,string1
;*** 263	-----------------------    res = C$4-*string2;  // [2]
        LBBO      &r15.b0, r1, 0, 1     ; [] |/usr/share/ti/cgt-pru/include/string.h:263| string2
        RSB       r15, r15.b0, r16.b0   ; [] |/usr/share/ti/cgt-pru/include/string.h:263| res,$O$C4
;*** 265	-----------------------    if ( C$4 ) goto g15;  // [2]
        QBNE      ||$C$L15||, r16.b0, 0x00 ; [] |/usr/share/ti/cgt-pru/include/string.h:265| $O$C4
;* --------------------------------------------------------------------------*
;*** 268	-----------------------    if ( res ) goto g16;  // [2]
        QBNE      ||$C$L16||, r15, 0x00 ; [] |/usr/share/ti/cgt-pru/include/string.h:268| res
;* --------------------------------------------------------------------------*
;***  	-----------------------    #pragma LOOP_FLAGS(4352u)
;*** 186	-----------------------    string1 = K$18;
;*** 186	-----------------------    string2 = (const unsigned char *)"switch2";
        LDI32     r1, $C$SL2            ; [] |pru0.c:186| string2
;* --------------------------------------------------------------------------*
;*   BEGIN LOOP ||$C$L10||
;*
;*   Loop source line                : 260
;*   Loop closing brace source line  : 266
;*   Known Minimum Trip Count        : 1
;*   Known Maximum Trip Count        : 4294967295
;*   Known Max Trip Count Factor     : 1
;* --------------------------------------------------------------------------*
||$C$L10||:    
;***	-----------------------g7:
;*** 263	-----------------------    C$3 = *string1;  // [2]
        LBBO      &r15.b0, r0, 0, 1     ; [] |/usr/share/ti/cgt-pru/include/string.h:263| $O$C3,string1
;*** 263	-----------------------    res = C$3-*string2;  // [2]
        LBBO      &r14.b0, r1, 0, 1     ; [] |/usr/share/ti/cgt-pru/include/string.h:263| string2
        RSB       r14, r14.b0, r15.b0   ; [] |/usr/share/ti/cgt-pru/include/string.h:263| res,$O$C3
;*** 265	-----------------------    if ( C$3 ) goto g11;  // [2]
        QBNE      ||$C$L12||, r15.b0, 0x00 ; [] |/usr/share/ti/cgt-pru/include/string.h:265| $O$C3
;* --------------------------------------------------------------------------*
;*** 268	-----------------------    if ( res ) goto g12;  // [2]
        QBNE      ||$C$L13||, r14, 0x00 ; [] |/usr/share/ti/cgt-pru/include/string.h:268| res
;* --------------------------------------------------------------------------*
;*** 194	-----------------------    O$1.s3_1.l3_2 = *(&2$$+13);
        ADD       r14, r2, 0            ; [] |pru0.c:194| $O$O1,$O$O1
        LDI       r15, ||$O2$$||+13     ; [] |pru0.c:194| $O2$$
        LDI       r16, 0x0070           ; [] |pru0.c:194| 
        JAL       r3.w2, ||memcpy||     ; [] |pru0.c:194| memcpy
;***  	-----------------------    #pragma LOOP_FLAGS(4096u)
;***  	-----------------------    U$57 = &O$1-1;
        ADD       r0, r2, 0             ; [] $O$O1,$O$O1
        SUB       r0, r0, 0x01          ; [] $O$U57
;***  	-----------------------    U$61 = &K$1[-1];
        SUB       r1, r4, 0x01          ; [] $O$U61,$O$K1
;* --------------------------------------------------------------------------*
;*   BEGIN LOOP ||$C$L11||
;*
;*   Loop source line                : 170
;*   Loop closing brace source line  : 170
;*   Known Minimum Trip Count        : 1
;*   Known Maximum Trip Count        : 4294967295
;*   Known Max Trip Count Factor     : 1
;* --------------------------------------------------------------------------*
||$C$L11||:    
;***	-----------------------g10:
;*** 170	-----------------------    T$1 = *(++U$57);  // [1]
        ADD       r0, r0, 0x01          ; [] |/usr/share/ti/cgt-pru/include/string.h:170| $O$U57,$O$U57
        LBBO      &r14.b0, r0, 0, 1     ; [] |/usr/share/ti/cgt-pru/include/string.h:170| $O$T1,$O$U57
;*** 170	-----------------------    *(++U$61) = T$1;  // [1]
        ADD       r1, r1, 0x01          ; [] |/usr/share/ti/cgt-pru/include/string.h:170| $O$U61,$O$U61
        SBBO      &r14.b0, r1, 0, 1     ; [] |/usr/share/ti/cgt-pru/include/string.h:170| $O$U61,$O$T1
;*** 170	-----------------------    if ( T$1 ) goto g10;  // [1]
        QBNE      ||$C$L11||, r14.b0, 0x00 ; [] |/usr/share/ti/cgt-pru/include/string.h:170| $O$T1
;* --------------------------------------------------------------------------*
;*** 170	-----------------------    goto g14;  // [1]
        JMP       ||$C$L17||            ; [] |/usr/share/ti/cgt-pru/include/string.h:170| 
;* --------------------------------------------------------------------------*
||$C$L12||:    
;***	-----------------------g11:
;*** 263	-----------------------    ++string2;  // [2]
        ADD       r1, r1, 0x01          ; [] |/usr/share/ti/cgt-pru/include/string.h:263| string2,string2
;*** 262	-----------------------    ++string1;  // [2]
        ADD       r0, r0, 0x01          ; [] |/usr/share/ti/cgt-pru/include/string.h:262| string1,string1
;*** 265	-----------------------    if ( !res ) goto g7;  // [2]
        QBEQ      ||$C$L10||, r14, 0x00 ; [] |/usr/share/ti/cgt-pru/include/string.h:265| res
;* --------------------------------------------------------------------------*
||$C$L13||:    
;***	-----------------------g12:
;*** 188	-----------------------    O$1.s3_3.l3_4 = *&2$$;
        LDI       r0, ||$O2$$||         ; [] |pru0.c:188| $O2$$
        LBBO      &r14.b0, r0, 0, 13    ; [] |pru0.c:188| 
        SBBO      &r14.b0, r2, 0, 13    ; [] |pru0.c:188| $O$O1
;***  	-----------------------    #pragma LOOP_FLAGS(4096u)
;***  	-----------------------    U$76 = &O$1-1;
        ADD       r0, r2, 0             ; [] $O$O1,$O$O1
        SUB       r0, r0, 0x01          ; [] $O$U76
;***  	-----------------------    U$80 = &K$1[-1];
        SUB       r1, r4, 0x01          ; [] $O$U80,$O$K1
;* --------------------------------------------------------------------------*
;*   BEGIN LOOP ||$C$L14||
;*
;*   Loop source line                : 170
;*   Loop closing brace source line  : 170
;*   Known Minimum Trip Count        : 1
;*   Known Maximum Trip Count        : 4294967295
;*   Known Max Trip Count Factor     : 1
;* --------------------------------------------------------------------------*
||$C$L14||:    
;***	-----------------------g13:
;*** 170	-----------------------    T$2 = *(++U$76);  // [1]
        ADD       r0, r0, 0x01          ; [] |/usr/share/ti/cgt-pru/include/string.h:170| $O$U76,$O$U76
        LBBO      &r14.b0, r0, 0, 1     ; [] |/usr/share/ti/cgt-pru/include/string.h:170| $O$T2,$O$U76
;*** 170	-----------------------    *(++U$80) = T$2;  // [1]
        ADD       r1, r1, 0x01          ; [] |/usr/share/ti/cgt-pru/include/string.h:170| $O$U80,$O$U80
        SBBO      &r14.b0, r1, 0, 1     ; [] |/usr/share/ti/cgt-pru/include/string.h:170| $O$U80,$O$T2
;*** 170	-----------------------    if ( T$2 ) goto g13;  // [1]
        QBNE      ||$C$L14||, r14.b0, 0x00 ; [] |/usr/share/ti/cgt-pru/include/string.h:170| $O$T2
;* --------------------------------------------------------------------------*
        JMP       ||$C$L17||            ; [] |pru0.c:191| 
;* --------------------------------------------------------------------------*
||$C$L15||:    
;***	-----------------------g15:
;*** 263	-----------------------    ++string2;  // [2]
        ADD       r1, r1, 0x01          ; [] |/usr/share/ti/cgt-pru/include/string.h:263| string2,string2
;*** 262	-----------------------    ++string1;  // [2]
        ADD       r14, r14, 0x01        ; [] |/usr/share/ti/cgt-pru/include/string.h:262| string1,string1
;*** 265	-----------------------    if ( !res ) goto g4;  // [2]
        QBEQ      ||$C$L9||, r15, 0x00  ; [] |/usr/share/ti/cgt-pru/include/string.h:265| res
;* --------------------------------------------------------------------------*
||$C$L16||:    
;***	-----------------------g16:
;*** 183	-----------------------    ui32_to_string((unsigned)*(&1$$+12), K$1);
        LDI       r0, ||$O1$$||+12      ; [] |pru0.c:183| $O1$$
        LBBO      &r14, r0, 0, 4        ; [] |pru0.c:183| 
        MOV       r15, r4               ; [] |pru0.c:183| $O$K1
        JAL       r3.w2, ||ui32_to_string|| ; [] |pru0.c:183| ui32_to_string
;* --------------------------------------------------------------------------*
||$C$L17||:    
;*** 184	-----------------------    send_to_arm(K$1);
        MOV       r14, r4               ; [] |pru0.c:184| $O$K1
        JAL       r3.w2, ||send_to_arm|| ; [] |pru0.c:184| send_to_arm
;***	-----------------------g17:
;***  	-----------------------    return;
        LBBO      &r3.b2, r2, 112, 6    ; [] 
        ADD       r2, r2, 0x76          ; [] 
        JMP       r3.w2                 ; [] 
	.sect	".text:main"
	.clink
	.global	||main||

;***************************************************************
;* FNAME: main                          FR SIZE:  14           *
;*                                                             *
;* FUNCTION ENVIRONMENT                                        *
;*                                                             *
;* FUNCTION PROPERTIES                                         *
;*                            0 Auto, 14 SOE     *
;***************************************************************

||main||:
;* --------------------------------------------------------------------------*
        SUB       r2, r2, 0x0e          ; [] 
        SBBO      &r3.b2, r2, 0, 14     ; [] 
;*** 69	-----------------------    __R30 = __R30|0x8000u;
        SET       r30, r30, 0x0000000f  ; [] |pru0.c:69| 
;*** 70	-----------------------    __R30 = __R30|0x2000u;
        SET       r30, r30, 0x0000000d  ; [] |pru0.c:70| 
;*** 73	-----------------------    *(&CT_CFG+4) &= 0xffffffefu;
        LBCO      &r0, __PRU_CREG_PRU_CFG, $CSBREL(||CT_CFG||+4), 4 ; [] |pru0.c:73| CT_CFG
        CLR       r0, r0, 0x00000004    ; [] |pru0.c:73| 
        SBCO      &r0, __PRU_CREG_PRU_CFG, $CSBREL(||CT_CFG||+4), 4 ; [] |pru0.c:73| CT_CFG
;*** 76	-----------------------    K$8 = &CT_INTC+36;
        LDI32     r5, ||CT_INTC||+36    ; [] |pru0.c:76| $O$K8,CT_INTC
;*** 76	-----------------------    *(volatile unsigned *)K$8 = *(volatile unsigned *)K$8&0xfffffc00u|0x11u;
        LDI32     r0, 0xfffffc00        ; [] |pru0.c:76| 
        LBBO      &r1, r5, 0, 4         ; [] |pru0.c:76| $O$K8
        AND       r0, r1, r0            ; [] |pru0.c:76| 
        OR        r0, r0, 0x11          ; [] |pru0.c:76| 
        SBBO      &r0, r5, 0, 4         ; [] |pru0.c:76| $O$K8
;*** 79	-----------------------    *&1$$ = (C$3 = &resource_table)+48;
        LDI       r0, ||resource_table|| ; [] |pru0.c:79| $O$C3,resource_table
        ADD       r1, r0, 0x30          ; [] |pru0.c:79| $O$C3
        LDI       r14, ||$O1$$||        ; [] |pru0.c:79| $O1$$
        SBBO      &r1, r14, 0, 4        ; [] |pru0.c:79| 
;*** 79	-----------------------    K$16 = C$3;
;* --------------------------------------------------------------------------*
;*   BEGIN LOOP ||$C$L18||
;*
;*   Loop source line                : 80
;*   Loop closing brace source line  : 80
;*   Known Minimum Trip Count        : 1
;*   Known Maximum Trip Count        : 4294967295
;*   Known Max Trip Count Factor     : 1
;* --------------------------------------------------------------------------*
||$C$L18||:    
;***	-----------------------g2:
;*** 80	-----------------------    if ( !(K$16[48]&4) ) goto g2;
        LBBO      &r1.b0, r0, 48, 1     ; [] |pru0.c:80| $O$K16
        QBBC      ||$C$L18||, r1.b0, 0x02 ; [] |pru0.c:80| 
;* --------------------------------------------------------------------------*
;*** 81	-----------------------    K$35 = C$2 = (struct pru_rpmsg_transport *)(K$21 = &1$$)+24;
        LDI       r6, ||$O1$$||         ; [] |pru0.c:81| $O$K21,$O1$$
        ADD       r14, r6, 0x18         ; [] |pru0.c:81| $O$C2,$O$K21
        MOV       r4, r14               ; [] |pru0.c:81| $O$K35,$O$C2
;*** 81	-----------------------    pru_rpmsg_init(C$2, (struct fw_rsc_vdev_vring *)K$16+52, (struct fw_rsc_vdev_vring *)K$16+72, 16u, 17u);
        ADD       r15, r0, 0x34         ; [] |pru0.c:81| $O$K16
        ADD       r16, r0, 0x48         ; [] |pru0.c:81| $O$K16
        LDI       r17, 0x0010           ; [] |pru0.c:81| 
        LDI       r18, 0x0011           ; [] |pru0.c:81| 
        JAL       r3.w2, ||pru_rpmsg_init|| ; [] |pru0.c:81| pru_rpmsg_init
;* --------------------------------------------------------------------------*
;*   BEGIN LOOP ||$C$L19||
;*
;*   Loop source line                : 88
;*   Loop closing brace source line  : 94
;*   Known Minimum Trip Count        : 1
;*   Known Maximum Trip Count        : 4294967295
;*   Known Max Trip Count Factor     : 1
;* --------------------------------------------------------------------------*
||$C$L19||:    
;***	-----------------------g4:
;*** 88	-----------------------    if ( pru_rpmsg_channel(0u, K$35, "rpmsg-pru", "Channel 32", 32) ) goto g4;
        LDI       r14.b0, 0x00          ; [] |pru0.c:88| 
        MOV       r15, r4               ; [] |pru0.c:88| $O$K35
        LDI32     r16, $C$SL3           ; [] |pru0.c:88| 
        LDI32     r17, $C$SL4           ; [] |pru0.c:88| 
        LDI       r18, 0x0020           ; [] |pru0.c:88| 
        JAL       r3.w2, ||pru_rpmsg_channel|| ; [] |pru0.c:88| pru_rpmsg_channel
        QBNE      ||$C$L19||, r14.w0, 0x00 ; [] |pru0.c:88| 
;* --------------------------------------------------------------------------*
;*** 97	-----------------------    set_initial_switches_state();
        JAL       r3.w2, ||set_initial_switches_state|| ; [] |pru0.c:97| set_initial_switches_state
;*** 100	-----------------------    *((volatile union $$fake166 *)(C$1 = (volatile struct $$fake88 *)0x22000u)+12) = 0u;
        LDI32     r0, 0x00022000        ; [] |pru0.c:100| $O$C1
        LDI       r1, 0x0000            ; [] |pru0.c:100| 
        SBBO      &r1, r0, 12, 4        ; [] |pru0.c:100| $O$C1
;*** 101	-----------------------    *(volatile unsigned *)C$1 |= 0x8u;
        LBBO      &r1, r0, 0, 4         ; [] |pru0.c:101| $O$C1
        SET       r1, r1, 0x00000003    ; [] |pru0.c:101| 
        SBBO      &r1, r0, 0, 4         ; [] |pru0.c:101| $O$C1
;* --------------------------------------------------------------------------*
;*   BEGIN LOOP ||$C$L20||
;*
;*   Loop source line                : 104
;*   Loop closing brace source line  : 127
;*   Known Minimum Trip Count        : 1
;*   Known Maximum Trip Count        : 4294967295
;*   Known Max Trip Count Factor     : 1
;* --------------------------------------------------------------------------*
||$C$L20||:    
;***	-----------------------g6:
;*** 106	-----------------------    if ( !are_cycles_past_threshold() ) goto g8;
        JAL       r3.w2, ||are_cycles_past_threshold|| ; [] |pru0.c:106| are_cycles_past_threshold
        QBEQ      ||$C$L21||, r14.b0, 0x00 ; [] |pru0.c:106| 
;* --------------------------------------------------------------------------*
;*** 108	-----------------------    reduce_cycles_and_update_switch1();
        JAL       r3.w2, ||reduce_cycles_and_update_switch1|| ; [] |pru0.c:108| reduce_cycles_and_update_switch1
;* --------------------------------------------------------------------------*
||$C$L21||:    
;***	-----------------------g8:
;*** 111	-----------------------    switch1_curr_p8_15 = __R31>>15&1;
        LSR       r0, r31, 0x0f         ; [] |pru0.c:111| 
        AND       r14.b0, r0.b0, 0x01   ; [] |pru0.c:111| switch1_curr_p8_15
;*** 112	-----------------------    if ( switch1_curr_p8_15 == *((unsigned char *)K$21+22) ) goto g10;
        LBBO      &r0.b0, r6, 22, 1     ; [] |pru0.c:112| $O$K21
        QBEQ      ||$C$L22||, r0.b0, r14.b0 ; [] |pru0.c:112| switch1_curr_p8_15
;* --------------------------------------------------------------------------*
;*** 114	-----------------------    handle_switch1_p8_15_change(switch1_curr_p8_15);
        JAL       r3.w2, ||handle_switch1_p8_15_change|| ; [] |pru0.c:114| handle_switch1_p8_15_change
;* --------------------------------------------------------------------------*
||$C$L22||:    
;***	-----------------------g10:
;*** 118	-----------------------    if ( !(__R31&0x40000000u) ) goto g6;
        QBBC      ||$C$L20||, r31, 0x1e ; [] |pru0.c:118| 
;* --------------------------------------------------------------------------*
;*** 121	-----------------------    *(volatile unsigned *)K$8 = *(volatile unsigned *)K$8&0xfffffc00u|0x11u;
        LDI32     r0, 0xfffffc00        ; [] |pru0.c:121| 
        LBBO      &r1, r5, 0, 4         ; [] |pru0.c:121| $O$K8
        AND       r0, r1, r0            ; [] |pru0.c:121| 
        OR        r0, r0, 0x11          ; [] |pru0.c:121| 
        SBBO      &r0, r5, 0, 4         ; [] |pru0.c:121| $O$K8
;* --------------------------------------------------------------------------*
;*   BEGIN LOOP ||$C$L23||
;*
;*   Loop source line                : 122
;*   Loop closing brace source line  : 125
;*   Known Minimum Trip Count        : 1
;*   Known Maximum Trip Count        : 4294967295
;*   Known Max Trip Count Factor     : 1
;* --------------------------------------------------------------------------*
||$C$L23||:    
;***	-----------------------g12:
;*** 122	-----------------------    if ( attempt_receive_from_arm() ) goto g6;
        JAL       r3.w2, ||attempt_receive_from_arm|| ; [] |pru0.c:122| attempt_receive_from_arm
        QBNE      ||$C$L20||, r14.b0, 0x00 ; [] |pru0.c:122| 
;* --------------------------------------------------------------------------*
;*** 124	-----------------------    handle_query_from_arm();
        JAL       r3.w2, ||handle_query_from_arm|| ; [] |pru0.c:124| handle_query_from_arm
;*** 124	-----------------------    goto g12;
        JMP       ||$C$L23||            ; [] |pru0.c:124| 
;* Inlined function references:
;* [  0] strlen
;* [  1] strcpy
;* [  2] strcmp
;* [  3] memset
;******************************************************************************
;* STRINGS                                                                    *
;******************************************************************************
	.sect	".rodata:.string"
||$C$SL1||:	.string	"switch1",0
||$C$SL2||:	.string	"switch2",0
||$C$SL3||:	.string	"rpmsg-pru",0
||$C$SL4||:	.string	"Channel 32",0
;*****************************************************************************
;* UNDEFINED EXTERNAL REFERENCES                                             *
;*****************************************************************************
	.global	||pru_rpmsg_init||
	.global	||pru_rpmsg_receive||
	.global	||pru_rpmsg_send||
	.global	||pru_rpmsg_channel||
	.global	||__pruabi_remu||
	.global	||__pruabi_divu||
	.global	||memcpy||
