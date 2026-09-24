# GDB script to test solving Jump Around & Find Out (C Version)
set pagination off

break main
run

# Step 0: jump_to_venice
call (void)jump_to_venice()
p (char)step_0_matched
x/s &cumulative_state
printf "\n"

# Step 1: jump_to_florence
call (void)jump_to_florence()
p (char)step_1_matched
x/s &cumulative_state
printf "\n"

# Test wrong jump & undo
call (void)jump_to_milan()
p (char)step_2_matched
x/s &cumulative_state
printf "\n"

call (void)undo_jump()
p (char)step_1_matched
x/s &cumulative_state
printf "\n"

# Step 2: jump_to_naples
call (void)jump_to_naples()
p (char)step_2_matched
x/s &cumulative_state
printf "\n"

# Step 3: jump_to_verona
call (void)jump_to_verona()
p (char)step_3_matched
x/s &cumulative_state
printf "\n"

# Step 4: jump_to_palermo
call (void)jump_to_palermo()
p (char)step_4_matched
x/s &cumulative_state
printf "\n"

# Step 5: jump_to_milan
call (void)jump_to_milan()
p (char)step_5_matched
x/s &cumulative_state
printf "\n"

quit
