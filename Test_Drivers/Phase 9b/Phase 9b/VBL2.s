	xdef	_vblisr
	xref	_do_VBL_ISR
		
_vblisr:	movem.l	d0-d2/a0-a2,-(sp)
			move.w	sr,old_sr
			move.w	#$2700,sr
			jsr		_do_VBL_ISR
			move.w	old_sr,sr
			movem.l	(sp)+,d0-d2/a0-a2
			rte
			
old_sr	ds.w	1