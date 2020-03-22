;Z-80 Simple Adding program
;2/21/03
start:
    ;in	a,(0)	;First number from input port 0
	ld	b,a	;Put in register b
	LD bc, 10
	;in	a,(1)	;Second number from input port 1
	add	a,b	;Add to first number
	;out	(0),a	;Output result to output port 0
	jp	start	;start over.
	jp z, start
	jp nz, start
	jp	start	;start over.

	call start
	ret
