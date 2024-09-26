# analyze.S
# Written 2015 by F Lundevall
# Ported 2024/07 by W Szczerek (from MIPS to RISC-V)
# Copyright abandonded - this file is in the public domain.

	.text
	.globl analyze
analyze:
	li	s0, 0x30 			#load immediate 0x30 = 48, to s0
loop:
	mv	a0, s0	 			# copy from s0 to a0
	
	li	a7, 11				# environment call with a7 = 11 will print out
	ecall					# one byte from a0 to the Run I/O window

	addi	s0, s0, 0x03	# what happens if the constant is changed? 0x01
	
	li	t0, 0x5A	#0x5A, A=10, 5*16^1 + 10*16^0 = 90
	ble	s0, t0, loop	#if less than 90, loop back
    	jr 	ra					
