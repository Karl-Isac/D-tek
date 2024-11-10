.text 
    
    addi t0, zero, 5           
    beq t0, zero, end_loop_base  
    addi gp, zero, 1       
    beq gp, t0, end_loop_base  # snabbt checka att n = [0,1] = 1
    addi gp,t0,0 # 5
    addi tp,zero,1
    
factorial_loop:
    beq t0, tp, end_loop # t0 == 1?
    addi t1,t0 ,-1          # 4
    addi t2, zero, 0           # SOLUTION!

multiply_loop:
    beq t1, zero, next_step  
    add t2, t2, gp        
    addi t1, t1, -1          
    beq zero, zero, multiply_loop  

next_step:
    addi t0, t0, -1  #this is the problem
    addi gp,t2,0     #20,     
    beq zero, zero, factorial_loop 

end_loop:
    beq zero, zero, end_loop   # Inga j, bara beq

end_loop_base:
    addi t2, zero, 1          
    beq zero, zero, end_loop 
