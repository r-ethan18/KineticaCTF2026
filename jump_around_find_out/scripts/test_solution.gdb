# GDB script to test solving Jump Around & Find Out (C Version)
set pagination off

break main
run

# Step 0: jump_to_venice
call (void)jump_to_venice()
printf "After Venice: step_0_matched=%d, state='%s'\n", step_0_matched, cumulative_state

# Step 1: jump_to_florence
call (void)jump_to_florence()
printf "After Florence: step_1_matched=%d, state='%s'\n", step_1_matched, cumulative_state

# Test wrong jump & undo
call (void)jump_to_milan()
printf "After wrong jump (Milan): step_2_matched=%d, state='%s'\n", step_2_matched, cumulative_state

call (void)undo_jump()
printf "After undo_jump(): step_1_matched=%d, state='%s'\n", step_1_matched, cumulative_state

# Step 2: jump_to_naples
call (void)jump_to_naples()
printf "After Naples: step_2_matched=%d, state='%s'\n", step_2_matched, cumulative_state

# Step 3: jump_to_verona
call (void)jump_to_verona()
printf "After Verona: step_3_matched=%d, state='%s'\n", step_3_matched, cumulative_state

# Step 4: jump_to_palermo
call (void)jump_to_palermo()
printf "After Palermo: step_4_matched=%d, state='%s'\n", step_4_matched, cumulative_state

# Step 5: jump_to_milan
call (void)jump_to_milan()
printf "After Milan: step_5_matched=%d, state='%s'\n", step_5_matched, cumulative_state

printf "All flags: step_0=%d, step_1=%d, step_2=%d, step_3=%d, step_4=%d, step_5=%d\n", step_0_matched, step_1_matched, step_2_matched, step_3_matched, step_4_matched, step_5_matched

quit
