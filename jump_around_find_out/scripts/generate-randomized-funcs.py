import random

def generate_seeded_chunk_functions(hex_chunks):
    # Set a fixed seed for predictable randomization
    SEED_VALUE = 1337

    for i, chunk in enumerate(hex_chunks):
        # Convert hex chunk to individual bytes
        bytes_list = [chunk[j:j+2] for j in range(0, len(chunk), 2)]

        # Create a local random instance with the fixed seed
        rng = random.Random(SEED_VALUE + i) # Unique seed per chunk or shared

        # Create an index array and shuffle it deterministically
        indices = list(range(len(bytes_list)))
        rng.shuffle(indices)

        # Build the scrambled array based on the shuffle
        scrambled_bytes = [bytes_list[idx] for idx in indices]

        print(f"void fun_chunk_{i}() {{")
        print(f"    // Mangled using fixed seed PRNG")
        print(f"    std::string chunk{i} = \"\";")

        # Output the scrambled bytes
        for b in scrambled_bytes:
            print(f"    chunk{i} += (char)0x{b};")

        # Output the deterministic un-shuffling logic
        print(f"    // Reconstruct original order using the same deterministic indices")
        print(f"    char restored[6];")
        for orig_pos, scrambled_pos in enumerate(indices):
            print(f"    restored[{orig_pos}] = chunk{i}[{scrambled_pos}];")

        print(f"    std::string final_chunk{i} = std::string(restored, 6);")
        print(f"    // TODO: Append final_chunk{i} to your cumulative hash state")
        print(f"}}\n")

if __name__ == "__main__":
    my_chunks = [
        "53616c746172", # Chunk 0: "Saltar"
        "655f655f6469", # Chunk 1: "e_e_di"
        "666963696c65", # Chunk 2: "fficil"
        "655f6d615f69", # Chunk 3: "e_ma_i"
        "6f5f736f6e6f", # Chunk 4: "o_sono"
        "5f4d6172696f"  # Chunk 5: "_Mario"
    ]

    generate_seeded_chunk_functions(my_chunks)
