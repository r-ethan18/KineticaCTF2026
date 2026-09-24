# generate_random_chain.py

import random
import string

MESSAGE = "Girare_gli_ingranaggi"
OUTPUT_FILE = "generated_gears.py"

random_generator = random.SystemRandom()


def random_function_name(existing_names):
    letters = string.ascii_letters
    first_characters = string.ascii_letters + "_"
    remaining_characters = string.ascii_letters + string.digits + "_"

    while True:
        name = random_generator.choice(first_characters)
        name += "".join(
            random_generator.choice(remaining_characters)
            for _ in range(random_generator.randint(6, 14))
        )

        if name not in existing_names:
            return name


function_names = ["turn_the_gears"]

for _ in range(len(MESSAGE) - 1):
    function_names.append(random_function_name(function_names))

lines = []

lines.append(f"def oil_the_gears():")
lines.append(f"    return True")
lines.append("")


for index, function_name in enumerate(function_names):
    lines.append(f"def {function_name}():")
    lines.append(f"    print({MESSAGE[index]!r}, end='')")

    if index + 1 < len(function_names):
        lines.append(f"")
        lines.append(f"    {function_names[index + 1]}()")

    lines.append("")

lines.extend([
    "if __name__ == '__main__':",
    "    turn_the_gears()",
    "",
])

with open(OUTPUT_FILE, "w", encoding="utf-8") as file:
    file.write("\n".join(lines))

print(f"Generated {OUTPUT_FILE}")
