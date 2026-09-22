def generate_cpp_chunk_functions(hex_chunks):
    for i, chunk in enumerate(hex_chunks):
        print(f"void fun_chunk_{i}() {{")
        print(f"    std::string chunk{i} = \"\";")

        # Step through the hex string 2 characters (1 byte) at a time
        for j in range(0, len(chunk), 2):
            byte_hex = chunk[j:j+2]
            print(f"    chunk{i} += (char)0x{byte_hex};")

        print(f"    // TODO: Append chunk{i} to your global cumulative state")
        print(f"    // update_state(chunk{i});")
        print(f"}}\n")

if __name__ == "__main__":
    # Your balanced 6 hex chunks (6 bytes / 12 hex chars each)
    my_chunks = [
        "53616c746172", # 0
        "655f655f6469", # 1
        "666963696c65", # 2
        "655f6d615f69", # 3
        "6f5f736f6e6f", # 4
        "5f4d6172696f"  # 5
    ]

    generate_cpp_functions = generate_cpp_chunk_functions(my_chunks)
