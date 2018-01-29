		xdef	_getbase
		xdef	_setbase
	
base	equ		8
	

;SUBROUTINE: _getbase
;
;	PURPOSE: Gets the middle bytes of the address of the video base
;
;	OUTPUT: Returns a word value that represents the middle bytes of the
;			address of the video base
	
_getbase:	link	a6,#0
			movem.l	a1,-(sp)
			clr.l	d0
			movea.l	#$FFFF8201,a1
			movep.w	(a1),d0
			lsl.l	#8,d0
			movem.l	(sp)+,a1
			unlk	a6
			rts
			
			


;SUBROUTINE: _setbase
;
;	PURPOSE: Sets both bytes in memory that hold the high and middle
;			 byte of the video base address,
;
;	INPUT: Takes in 2 bytes of an address
_setbase:	link	a6,#0
			movem.l	d1/a1,-(sp)
			clr.l	d1
			move.l	base(a6),d1
			lsr.l	#8,d1
			movea.l	#$FFFF8201,a1
			movep.w	d1,(a1)
			movem.l	(sp)+,d1/a1
			unlk	a6
			rts