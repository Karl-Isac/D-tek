  # hexmain.S
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

	.text
	.globl hex2asc

hex2asc:
	li	a0, 15		# test number (from 0 to 15)
	
	addi    sp,sp,-4	#ra is 4 bytes´
	sw      ra,0(sp)	
	
	jal	hexasc		# call hexasc
	
	li	a7, 11	# write a0 to stdout
	ecall

	lw      ra,0(sp)
	addi    sp,sp,4
	jr      ra	

  # You can write your own code for hexasc here
  #
hexasc:
    andi a0, a0, 0xF       # Maskera de lägsta 4 bitarna av a0
    li   t1, 9             # Siffra 9 (decimal)
    
    ble  a0, t1, zero_to_nine  # Om a0 är 0-9, gå till zero_to_nine
    
    # Om a0 är 10-15 (A-F)
    addi a0, a0, 0x37      # Konvertera till ASCII ('A' - 'F')
    ret                    # Returnera från subrutin

zero_to_nine:
    addi a0, a0, 0x30      # Konvertera till ASCII ('0' - '9')
    ret                    # Returnera från subrutin

