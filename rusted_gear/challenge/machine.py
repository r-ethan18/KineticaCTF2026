from gears import oil_teh_greas
from gears import turn_the_gears
from time import sleep

print("Guzzling down fuel...")
sleep(1)
print("Inhaling the air...")
sleep(1)
print("Oiling the gears...")
sleep(1)
if oil_teh_greas():
    print("Turning the gears...")
    sleep(1)
    print("Here is what you asked for: ", end="")
    print(r"snuc{",end="")
    turn_the_gears()
    print(r"}")
else:
    print("Check the gears! I cannot oil them...")
