; Generated by c68 v4.4.5, 25 Jan 95 (Jan 25 1995) from "./globals.h"
	data
_scancode_table:
	dc.b	0,27,49,50,51,52,53,54,55,56,57,48
	dc.b	45,61,8,9,113,119,101,114,116,121,117,105
	dc.b	111,112,91,93,13,0,97,115,100,102,103,104
	dc.b	106,107,108,59,39,96,0,92,122,120,99,118
	dc.b	98,110,109,44,46,47,0,0,0,32,0,0
	dc.b	0,0,0,0,0,0,0,0,0,0,0,0
	dc.b	0,0,45,0,0,0,43,0,0,0,0,127
	dc.b	0,0,0,0,0,0,0,0,0,0,0,0
	dc.b	0,0,0,40,41,47,42,55,56,57,52,53
	dc.b	54,49,50,51,48,46,13,0,0,0,0,0
	dc.b	0,0,0,0,0,0,0,0
	even
_MUSIC:
	dc.w	568,10,478,10,379,10,358,10
	dc.w	478,10,379,10,358,10,478,10
	dc.w	379,10,568,30,379,30,426,30
	dc.w	379,30,319,30,379,119,568,10
	dc.w	478,10,379,10,358,10,478,10
	dc.w	379,10,358,10,478,10,379,10
	dc.w	568,30,379,30,426,30,379,30
	dc.w	284,30,379,119,568,10,478,10
	dc.w	379,10,358,10,478,10,379,10
	dc.w	358,10,478,10,379,10,239,30
	dc.w	253,30,284,30,319,30,284,30
	dc.w	379,119,568,10,478,10,379,10
	dc.w	358,10,478,10,379,10,358,10
	dc.w	478,10,379,10,239,30,253,30
	dc.w	284,30,319,30,253,30,379,119
_left_button_clicked:
	dc.b	0
	even
_mouse_delta_x:
	dc.w	0
_mouse_delta_y:
	dc.w	0
_process_mouse_data:
	dc.b	0
_mouse_button_data:
	dc.b	0
_input_state:
	dc.b	0
	even
_fillLevel:
	dc.w	0
_buffer_empty:
	dc.w	1
_IKBD_headPtr:
	dc.l	_IKBD_buffer
_IKBD_tailPtr:
	dc.l	255+_IKBD_buffer
_xCoord:
	dc.w	319
_yCoord:
	dc.w	199
_playmusic:
	dc.w	0
_timenow:
	dc.l	0
_currentnote:
	dc.w	-1
_nextnote:
	dc.l	0
	text
L2:
	dc.b	120,32,61,32,37,100,32,121,32,61,32,37
	dc.b	100,32,10,0
L1:
	dc.b	27,102,32,10,0
	even
_main:
	link	a6,#-1362
	movem.l	d3/d4/d5/d6/d7/a3/a4/a5,-(a7)
	lea	-1336(a6),a0
	move.l	a0,d3
	lea	_timenow,a3
	lea	_swapbuffers,a4
	lea	_install_vector,a5
	;line	75
	clr.b	-1(a6)
	clr.l	-6(a6)
	clr.l	-10(a6)
	clr.l	-14(a6)
	clr.l	-18(a6)
	move.w	#1,-1338(a6)
	clr.w	-1340(a6)
	clr.w	-1342(a6)
	clr.w	-1344(a6)
	moveq	#0,d5
	clr.w	-1348(a6)
	clr.w	-1350(a6)
	moveq	#0,d7
	;line	75
	pea	_vblisr
	move.w	#28,-(a7)
	jsr	(a5)
	addq.l	#6,a7
	move.l	d0,-1358(a6)
	;line	76
	pea	_ikbd_isr
	move.w	#70,-(a7)
	jsr	(a5)
	addq.l	#6,a7
	move.l	d0,-1362(a6)
	;line	78
	move.l	#_altbuffer,d4
	;line	79
	move.l	#255,d0
	or.l	d4,d0
	addq.l	#1,d0
	move.l	d0,d4
	;line	80
	jsr	_get_videobase
	move.l	d0,d6
	;line	82
	move.l	d3,-(a7)
	jsr	_initializegame
	addq.l	#4,a7
	;line	83
	clr.w	_renderrequest
	;line	84
	pea	L1
	jsr	_printf
	addq.l	#4,a7
	;line	85
	jsr	_stop_sound
	;line	87
	lea	-1310(a7),a7
	move.l	d3,a0
	move.l	a7,a1
	move.w	#326,d0
L3:
	move.l	(a0)+,(a1)+
	dbra	d0,L3
	move.w	(a0)+,(a1)+
	jsr	_rendersplash
	lea	1310(a7),a7
	move.b	d0,-1(a6)
	;line	88
	cmp.b	#49,-1(a6)
	bne	L4
	;line	90
	moveq	#70,d0
	add.l	(a3),d0
	move.l	d0,-6(a6)
	;line	91
	lea	140,a0
	add.l	(a3),a0
	move.l	a0,-10(a6)
	;line	93
	lea	-1310(a7),a7
	move.l	d3,a0
	move.l	a7,a1
	move.w	#326,d0
L5:
	move.l	(a0)+,(a1)+
	dbra	d0,L5
	move.w	(a0)+,(a1)+
	jsr	_renderfirst
	lea	1310(a7),a7
	;line	94
	move.w	#1,_playmusic
	;line	96
	move.l	d3,-(a7)
	move.l	d4,-(a7)
	move.l	d6,-(a7)
	jsr	(a4)
	lea	12(a7),a7
	;line	98
	moveq	#1,d5
	;line	99
	lea	-1310(a7),a7
	move.l	d3,a0
	move.l	a7,a1
	move.w	#326,d0
L6:
	move.l	(a0)+,(a1)+
	dbra	d0,L6
	move.w	(a0)+,(a1)+
	jsr	_renderfirst
	lea	1310(a7),a7
	;line	101
	bra	L8
L7:
	;line	103
	move.l	d3,-(a7)
	jsr	_nextframe
	addq.l	#4,a7
	;line	104
	cmp.w	#1,-638(a6)
	bne	L10
	move.l	(a3),d0
	cmp.l	-14(a6),d0
	bcs	L10
	;line	106
	move.l	d3,-(a7)
	jsr	_stopfiring
	addq.l	#4,a7
L10:
	;line	108
	tst.w	-1314(a6)
	bne	L11
	move.l	(a3),d0
	cmp.l	-18(a6),d0
	bcs	L11
	;line	110
	move.w	#1,-1314(a6)
L11:
	;line	113
	tst.w	_fillLevel
	bls	L12
	;line	115
	jsr	_userinput
	move.b	d0,-1353(a6)
	;line	121
	cmp.b	#32,-1353(a6)
	bne	L13
	;line	123
	pea	-150(a6)
	pea	-146(a6)
	pea	-126(a6)
	pea	-76(a6)
	pea	-1334(a6)
	move.l	d3,-(a7)
	jsr	_requestfirebeam
	lea	24(a7),a7
	move.w	d0,-1342(a6)
	;line	124
	move.w	#2,-(a7)
	pea	0
	pea	__iob
	jsr	_fseek
	lea	10(a7),a7
	;line	125
	cmp.w	#1,-1342(a6)
	bne	L14
	;line	127
	moveq	#20,d0
	add.l	(a3),d0
	move.l	d0,-14(a6)
	;line	128
	lea	245,a0
	add.l	(a3),a0
	move.l	a0,-18(a6)
L14:
	bra	L15
L13:
	;line	133
	pea	-1312(a6)
	pea	-1334(a6)
	move.l	d3,-(a7)
	move.b	-1353(a6),d0
	ext.w	d0
	and.w	#255,d0
	move.w	d0,-(a7)
	jsr	_requestmoveufo
	lea	14(a7),a7
	;line	134
	move.w	#2,-(a7)
	pea	0
	pea	__iob
	jsr	_fseek
	lea	10(a7),a7
L15:
L12:
	;line	138
	move.l	(a3),d0
	cmp.l	-6(a6),d0
	bcs	L16
	tst.w	-1350(a6)
	bne	L16
	;line	140
	move.l	-6(a6),d0
	add.l	#70,d0
	move.l	d0,-6(a6)
	;line	141
	cmp.w	#1,-1338(a6)
	bne	L17
	;line	143
	move.w	-634(a6),-(a7)
	pea	-1312(a6)
	pea	-626(a6)
	pea	-526(a6)
	move.w	#1,-(a7)
	jsr	_spawn_plane
	lea	16(a7),a7
	;line	145
	clr.w	-1338(a6)
	bra	L18
L17:
	;line	149
	move.w	-634(a6),-(a7)
	pea	-1312(a6)
	pea	-630(a6)
	pea	-606(a6)
	clr.w	-(a7)
	jsr	_spawn_plane
	lea	16(a7),a7
	;line	151
	move.w	#1,-1338(a6)
L18:
L16:
	;line	155
	move.l	(a3),d0
	cmp.l	-10(a6),d0
	bcs	L19
	tst.w	-1348(a6)
	bne	L19
	;line	157
	move.l	-10(a6),d0
	add.l	#140,d0
	move.l	d0,-10(a6)
	;line	158
	cmp.w	#1,-1340(a6)
	bne	L20
	;line	160
	pea	-150(a6)
	pea	-126(a6)
	move.w	#1,-(a7)
	jsr	_spawn_soldier
	lea	10(a7),a7
	;line	161
	clr.w	-1340(a6)
	bra	L21
L20:
	;line	165
	pea	-146(a6)
	pea	-76(a6)
	clr.w	-(a7)
	jsr	_spawn_soldier
	lea	10(a7),a7
	;line	166
	move.w	#1,-1340(a6)
L21:
L19:
	;line	170
	clr.w	-1348(a6)
	;line	171
	clr.w	-1350(a6)
	;line	173
	pea	-1334(a6)
	move.l	d3,-(a7)
	jsr	_requestmovesoldiers
	addq.l	#8,a7
	move.w	d0,-1348(a6)
	;line	174
	pea	-1312(a6)
	pea	-1334(a6)
	move.l	d3,-(a7)
	jsr	_requestmoveplanes
	lea	12(a7),a7
	move.w	d0,-1350(a6)
	;line	177
	move.w	_yCoord,-(a7)
	move.w	_xCoord,-(a7)
	pea	L2
	jsr	_printf
	addq.l	#8,a7
	;line	179
	tst.w	d5
	bne	L22
	;line	181
	move.l	d3,-(a7)
	move.l	d4,-(a7)
	move.l	d6,-(a7)
	jsr	(a4)
	lea	12(a7),a7
	;line	182
	moveq	#1,d5
	bra	L23
L22:
	;line	186
	move.l	d3,-(a7)
	move.l	d6,-(a7)
	move.l	d4,-(a7)
	jsr	(a4)
	lea	12(a7),a7
	;line	187
	moveq	#0,d5
L23:
	;line	190
	cmp.w	#1,-636(a6)
	bne	L24
	cmp.w	#1,d7
	bne	L24
	;line	192
	clr.w	-636(a6)
	;line	193
	moveq	#0,d7
	bra	L25
L24:
	;line	195
	cmp.w	#1,-636(a6)
	bne	L26
	tst.w	d7
	bne	L26
	;line	197
	moveq	#1,d7
L26:
L25:
L8:
	tst.w	-1334(a6)
	beq	L27
	cmp.b	#27,-1353(a6)
	bne	L7
L27:
L9:
	;line	201
	tst.w	d5
	bne	L28
	;line	202
	move.l	d3,-(a7)
	move.l	d4,-(a7)
	move.l	d6,-(a7)
	jsr	(a4)
	lea	12(a7),a7
L28:
	;line	203
	jsr	_stop_sound
L4:
	;line	206
	move.l	-1358(a6),-(a7)
	move.w	#28,-(a7)
	jsr	(a5)
	addq.l	#6,a7
	;line	207
	move.l	-1362(a6),-(a7)
	move.w	#70,-(a7)
	jsr	(a5)
	;line	208
	moveq	#0,d0
L29:
	movem.l	-1394(a6),d3/d4/d5/d6/d7/a3/a4/a5
	unlk	a6
	rts
_swapbuffers:
	link	a6,#0
	movem.l	d3/a3,-(a7)
	move.l	12(a6),d3
	move.l	16(a6),a3
L30:
L31:
	;line	221
	tst.w	_renderrequest
	beq	L30
L32:
	;line	223
	move.l	8(a6),-(a7)
	jsr	_set_video_base
	;line	224
	move.l	d3,24(a3)
	;line	225
	move.l	d3,28(a3)
	;line	226
	move.l	d3,32(a3)
	;line	227
	clr.w	_renderrequest
	movem.l	-8(a6),d3/a3
	unlk	a6
	rts
_pollclock:
	link	a6,#-12
	;line	242
	move.l	#1122,-4(a6)
	;line	242
	pea	0
	move.w	#32,-(a7)
	jsr	__trap_1_wl
	addq.l	#6,a7
	move.l	d0,-12(a6)
	;line	243
	move.l	-4(a6),a0
	move.l	(a0),-8(a6)
	;line	244
	move.l	-12(a6),-(a7)
	move.w	#32,-(a7)
	jsr	__trap_1_wl
	;line	245
	move.l	-8(a6),d0
L33:
	unlk	a6
	rts
	globl	_swapbuffers
	globl	_pollclock
	globl	_xCoord
	globl	_yCoord
	globl	_fillLevel
	bss
_IKBD_buffer:
	ds.b	256
	globl	_IKBD_buffer
	globl	_IKBD_tailPtr
	globl	_IKBD_headPtr
_altbuffer:
	ds.b	32256
	globl	_altbuffer
	even
_renderrequest:
	ds.b	2
	globl	_renderrequest
	globl	_playmusic
	globl	_currentnote
	globl	_timenow
	globl	_nextnote
	globl	_input_state
	globl	_mouse_button_data
	globl	_process_mouse_data
	globl	_left_button_clicked
	globl	_mouse_delta_x
	globl	_mouse_delta_y
	globl	_scancode_table
	globl	_buffer_empty
	globl	_main
