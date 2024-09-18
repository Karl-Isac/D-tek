# timetemplate.S
# Written 2015 by F Lundevall
# Ported 2024/06 by W Szczerek (from MIPS to RISC-V)
# Copyright abandonded - this file is in the public domain.

#############################################################
# Choose the macro syntax for RARS or DTEK-V board.         #
# The syntax for RARS is probably due to its MIPS heritage. #
#############################################################
###################
# Macros for RARS #
###################
#.macro	PUSH (%reg)
#	addi	sp,sp,-4
#	sw	%reg,0(sp) 
#.end_macro

#.macro	POP (%reg)
#	lw	%reg,0(sp)
#	addi	sp,sp,4
#.end_macro
###################
# Macros for DTEK #
###################
.macro	PUSH reg
	addi sp,sp,-4
	sw \reg,0(sp) 
.endm

.macro	POP reg
	lw	\reg,0(sp)
	addi	sp,sp,4
.endm
#############################################################

	.data
	.align 2
mytime:	.word 	0x5957
timstr:	.asciz 	"text more text lots of text\0"
test_string: .asciz "Hello, World!\n"
	.text
	.globl timetemplate, tick, time2string, 

#
#test_display_string:
#    la a0, test_string    # Ladda adressen till teststrängen
#    jal display_string    # Anropa display_string
#    j   end_program       # Hoppa till slutet av programmet (eller till nästa del)
    
    
#end_program:
    # Du kan här lägga till kod för att avsluta programmet eller andra tester
    # Om du har en slinga som kallar andra funktioner, säkerställ att du har en väg ut.
#    j end_program 


# Function for displaying a string with a newline at the end	
display_string:	
	li a7,4
	ecall
	li a0, 10
	li a7,11
	ecall
	jr ra
	
timetemplate:
	la	a0, timstr
	jal     display_string
	
	# wait a little
	li	a0, 50		# ms
	jal	delay
	
	# call tick
	la 	a0, mytime
	jal	tick
	
	# call your function time2string
	la	a0, timstr
	la	t0, mytime
	lw	a1, 0(t0)
	jal	time2string
	
	# go back and do it all again
	j	timetemplate

	
# tick: update time pointed to by $a0
tick:	lw	t0, 0(a0)	# get time
	addi	t0, t0, 1	# increase
	andi	t1, t0, 0xf	# check lowest digit
	sltiu	t2, t1, 0xa	# if digit < a, okay
	bnez	t2, tiend
	addi	t0, t0, 0x6	# adjust lowest digit
	
	andi	t1, t0, 0xf0	# check next digit
	sltiu	t2, t1, 0x60	# if digit < 6, okay
	bnez	t2, tiend
	addi	t0, t0, 0xa0	# adjust digit
	
	li	t3, 0xF
	slli	t3, t3, 0x8
	and	t1, t0, t3	# check minute digit
	addi	t3, x0, 0xA
	slli	t3, t3, 0x8
	slt	t2, t1, t3	# if digit < a, okay
	bnez	t2, tiend
	addi	t0, t0, 0x600	# adjust digit - this one's okay, it's lower than 0x7FF 
	
	li	t3, 0xF
	slli	t3, t3, 0xC
	and	t1, t0, t3	# check last digit
	addi	t3, x0, 0x6
	slli	t3, t3, 0xC
	slt	t2, t1, t3	# if digit < 6, okay
	bnez	t2, tiend
	
	li	t3, 0xA
	slli	t3, t3, 0xC
	add	t0, t0, t3	# adjust last digit
tiend:	sw	t0,0(a0)	# save updated result
	jr	ra		# return

#########################################################
# Place for your functions: time2string, hex2asc, delay.#
#########################################################


hex2asc:
	li	a0, 12		# test number (from 0 to 15)
	
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
    li   t1, 11             # Siffra 9 (decimal)
    
    ble  a0, t1, zero_to_nine  # Om a0 är 0-9, gå till zero_to_nine
    
    # Om a0 är 10-15 (A-F)
    addi a0, a0, 0x37      # Konvertera till ASCII ('A' - 'F')
    ret                    # Returnera från subrutin

zero_to_nine:
    addi a0, a0, 0x30      # Konvertera till ASCII ('0' - '9')
    ret                    # Returnera från subrutin

          
    
time2string:
    PUSH ra               # Save return address
    PUSH s0               # Save s0
    PUSH s2               # Save s2

    mv s0, a1             # s0 = time value (a1 holds time)
    mv s2, a0             # s2 = address to store the result string (a0)

    # Extract digits for minutes
    srli a0, s0, 12       # Shift to get 'minutes_high' (bits 12-15)
    jal hexasc            # Convert to ASCII
    sb a0, 0(s2)          # Store result at memory address

    srli a0, s0, 8        # Shift to get 'minutes_low' (bits 8-11)      # Mask the lower 4 bits
    jal hexasc            # Convert to ASCII
    sb a0, 1(s2)          # Store result at memory address

    li t1, 0x3A           # Use s1 for ASCII code for ':'
    sb t1, 2(s2)          # Store ':' in memory


    srli a0, s0, 4        # Shift to get 'seconds_high' (bits 4-7)
    jal hexasc            # Convert to ASCII
    sb a0, 3(s2)          # Store result at memory address

          
    jal hexasc            # Convert to ASCII
    sb a0, 4(s2)          # Store result at memory address
    sb zero, 5(s2)        # Null-terminate the string

    POP s2                # Restore s2
    POP s0                # Restore s0
    POP ra                # Restore return address

    jr ra                 # Return from subroutine


##################### Delay

return:
    ret

delay:
    ble    a0, zero, return

    li    t1, 10000 # Easy to change

    addi    a0, a0, -1
for:
    addi    t1, t1, -1
    bnez     t1, for
    j    delay
