import binascii

def generate_chunks():
    target_string = "Saltare_e_difficile_ma_io_sono_Mario"
    
    # Convert string to hex encoding
    flag_hex = binascii.hexlify(target_string.encode('utf-8')).decode('utf-8')
    
    chunk_size = 12 # 6 bytes = 12 hex characters per chunk
    chunks = []
    
    print(f"Full Hex: {flag_hex}\n")
    print("--- 6 Non-Overlapping Chunks ---")
    
    for i in range(0, len(flag_hex), chunk_size):
        chunk = flag_hex[i:i + chunk_size]
        chunks.append(chunk)
        print(f"Chunk {len(chunks) - 1}: {chunk}")
        
    return chunks

if __name__ == "__main__":
    generate_chunks()