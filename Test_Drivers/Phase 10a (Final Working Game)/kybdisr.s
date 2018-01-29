;SUBROUTINE: IKBD_ISR
;
;	PURPOSE: The intelligent keyboard service routine function below is called by its vector # when a keyboard interrupt
;            is detected.
;
;	INPUT: n/a
;
;	OUTPUT: n/a
;

            xdef	_ikbd_isr
            xref    _do_IKBD_ISR

_ikbd_isr:	movem.l	d0-d2/a0-a2,-(sp)
            jsr		_do_IKBD_ISR
			movem.l	(sp)+,d0-d2/a0-a2
			rte
