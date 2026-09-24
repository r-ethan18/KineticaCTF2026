# generate_random_chain.py

import random
import string

MESSAGE = "Girare_gli_ingranaggi"
OUTPUT_FILE = "generated_gears.py"
DECOY_DEPTH = 5

random_generator = random.SystemRandom()


def random_function_name(existing_names):
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


all_names = {"turn_the_gears"}
chain_names = ["turn_the_gears"]

# Names for the functions that print the message
for _ in range(len(MESSAGE) - 1):
    name = random_function_name(all_names)
    all_names.add(name)
    chain_names.append(name)

# Generate one five-function decoy chain for every message function
decoy_chains = {}

for chain_name in chain_names:
    decoy_names = []

    for _ in range(DECOY_DEPTH):
        name = random_function_name(all_names)
        all_names.add(name)
        decoy_names.append(name)

    decoy_chains[chain_name] = decoy_names


lines = [
    "import os",
    "import secrets",
    "",
]

# Generate the decoy functions
for decoy_names in decoy_chains.values():
    for index, decoy_name in enumerate(decoy_names):
        lines.append(f"def {decoy_name}():")
        lines.append("    data = secrets.token_bytes(16)")
        lines.append("    with open(os.devnull, 'wb') as sink:")
        lines.append("        sink.write(data)")

        if index + 1 < len(decoy_names):
            lines.append(f"    {decoy_names[index + 1]}()")

        lines.append("")


# Generate the actual message chain
for index, function_name in enumerate(chain_names):
    decoy_entry = decoy_chains[function_name][0]

    lines.append(f"def {function_name}():")
    lines.append(f"    print({MESSAGE[index]!r}, end='')")
    lines.append(f"    {decoy_entry}()")

    if index + 1 < len(chain_names):
        lines.append(f"    {chain_names[index + 1]}()")

    lines.append("")


lines.extend([
    "if __name__ == '__main__':",
    "    turn_the_gears()",
    "",
])


with open(OUTPUT_FILE, "w", encoding="utf-8") as file:
    file.write("\n".join(lines))

print(f"Generated {OUTPUT_FILE}")
