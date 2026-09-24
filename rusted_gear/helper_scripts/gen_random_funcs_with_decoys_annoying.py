# generate_random_chain.py

import random
import string

MESSAGE = "Girare_gli_ingranaggi"
OUTPUT_FILE = "generated_gears.py"
DECOY_DEPTH = 100
DECOY_BYTES = 4096

rng = random.SystemRandom()


def random_function_name(existing):
    first = string.ascii_letters + "_"
    rest = string.ascii_letters + string.digits + "_"

    while True:
        name = rng.choice(first)
        name += "".join(rng.choice(rest) for _ in range(rng.randint(6, 14)))

        if name not in existing:
            return name


names = {"turn_the_gears"}
chain_names = ["turn_the_gears"]

for _ in range(len(MESSAGE) - 1):
    name = random_function_name(names)
    names.add(name)
    chain_names.append(name)


decoy_chains = {}

for chain_name in chain_names:
    decoys = []

    for _ in range(DECOY_DEPTH):
        name = random_function_name(names)
        names.add(name)
        decoys.append(name)

    decoy_chains[chain_name] = decoys


lines = [
    "import secrets",
    "import sys",
    "",
    "link_state = {}",
    "",
]

lines.append(f"def oil_the_gears():")
lines.append(f"    return True")
lines.append("")

# Generate decoy functions
for decoys in decoy_chains.values():
    for index, decoy_name in enumerate(decoys):
        lines.append(f"def {decoy_name}(state_key):")

        if index + 1 < len(decoys):
            lines.append(f"    {decoys[index + 1]}(state_key)")

        lines.append("    if not link_state[state_key]:")
        lines.append(f"        random_bytes = secrets.token_bytes({DECOY_BYTES})")
        lines.append("        sys.stdout.buffer.write(random_bytes)")
        lines.append("        sys.stdout.buffer.flush()")
        lines.append("")

    lines.append("")


# Generate real chain
for index, function_name in enumerate(chain_names):
    state_key = repr(function_name)

    lines.append(f"def {function_name}():")
    lines.append(f"    link_state[{state_key}] = False")
    lines.append(f"    print({MESSAGE[index]!r}, end='')")

    if index + 1 < len(chain_names):
        next_function = chain_names[index + 1]
        decoy_entry = decoy_chains[function_name][0]

        lines.append("    try:")
        lines.append(f"        {next_function}()")
        lines.append(f"        link_state[{state_key}] = True")
        lines.append("    finally:")
        lines.append(f"        if not link_state[{state_key!s}]:")
        lines.append(f"            {decoy_entry}({state_key})")
    else:
        lines.append(f"    link_state[{state_key}] = True")

    lines.append("")


lines.extend([
    "if __name__ == '__main__':",
    "    turn_the_gears()",
    "",
])


with open(OUTPUT_FILE, "w", encoding="utf-8") as file:
    file.write("\n".join(lines))

print(f"Generated {OUTPUT_FILE}")
