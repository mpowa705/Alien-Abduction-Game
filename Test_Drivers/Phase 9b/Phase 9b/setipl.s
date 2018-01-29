            xdef _set_ipl

_set_ipl:	move.w	sr,d0
			move.w d0,-(sp) 
			lsr.w #8,d0 
			and.w #$0007,d0 
			andi.w #$F8FF,(sp)
			move.w d1,-(sp)
			move.w 8(sp),d1 
			lsl.w #8,d1
			and.w #$0700,d1
			or.w d1,2(sp) 
			move.w (sp)+,d1
			rte