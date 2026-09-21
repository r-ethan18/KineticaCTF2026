# Exceptional Lock — CTF Challenge Documentation

**Exceptional Lock** is a beginner-to-intermediate C++ reverse engineering CTF challenge. Standard password input validation appears to reject non-matching inputs, but supplying a specifically constructed 213-character input triggers a custom C++ exception (`panic`) that redirects execution into a hidden flag-decryption routine.

---

## Technical Overview & Architecture

### Key Components

1. **Custom Exception Control Flow**:
   - In `validator.cpp`, password inputs are checked against a trigger function `is_panic_trigger(pwd)`.
   - When triggered, it raises a custom lightweight struct exception (`throw panic();`).
   - `main()` catches `const panic&` and redirects execution into `resolve_fault()`, bypassing standard password hashing.

2. **Anti-Debugging Protection**:
   - `generate_dynamic_key()` in `backup.cpp` contains an inlined anti-debugging check using `ptrace(PTRACE_TRACEME, 0, 1, 0)`.
   - If a debugger (e.g., GDB, LLDB) is attached, `ptrace` returns `-1`, modifying the internal key mask from `0x5A` to `0xA5`.
   - This silently corrupts key reconstruction without exiting or crashing, frustrating naive dynamic analysis in a debugger.

3. **Dynamic Key Reconstruction**:
   - The 32-byte Libsodium key is derived dynamically in stack memory at runtime.
   - Masked byte transformations use bitwise operations `(a & ~m) | (~a & m)` (equivalent to XOR with mask `0x5A`) combined with state noise mixing to defeat static string searches.

4. **Obfuscated Symbol Naming**:
   - The hidden decryption handler is named `resolve_fault()` (instead of `secret_path` or `flag_decrypt`) to appear as routine error handling during static analysis.

---

## Build & Testing Instructions

### Prerequisites
- `cmake` (version 3.22+)
- `gcc` / `g++`
- `libsodium`

### Building the Project
```bash
cmake -B build
cmake --build build
```

This generates two binaries:
- `./build/decrypt-str`: The main challenge binary distributed to CTF players.
- `./build/encrypt-str`: Helper utility used by challenge author to re-encrypt new flags/passwords.

---

## Solutions & Credentials Reference

### Option A: Standard Password Path
- **Input**: `PD}7G:NCSN1bq1!INatl5:!£`
- **Behavior**: Standard execution path. Hashes input via Libsodium `crypto_pwhash` and decrypts flag.

### Option B: Exception Trigger Path (Intended CTF Solution)
- **Input**: A 213-character string containing the symbol sequence `!@#$%^&*+=?~!@#$%^&*+=` placed at index intervals `0, 10, 20, 30, ..., 210`.
- **Sample Payload**:
  ```text
  !AAAAAAAAAA@AAAAAAAAAA#AAAAAAAAAA$AAAAAAAAAA%AAAAAAAAAA^AAAAAAAAAA&AAAAAAAAAA*AAAAAAAAAA+AAAAAAAAAA=AAAAAAAAAA?AAAAAAAAAA~AAAAAAAAAA!AAAAAAAAAA@AAAAAAAAAA#AAAAAAAAAA$AAAAAAAAAA%AAAAAAAAAA^AAAAAAAAAA&AAAAAAAAAA*AAAAAAAAAA+AAAAAAAAAA=AA
  ```
- **Flag**: `snuc{Davinci_dice_Sono_stanco_capo}`

---

## Player Solution Walkthrough (Hypothetical Writeup)

Below is how a CTF player would reverse engineer and solve this challenge:

### Step 1: Initial Static Analysis & Disassembly
When loading the binary into **Ghidra** or **IDA Pro**:
- The main function reads a line of input and passes it into `decrypt(password)`.
- Inside `decrypt()`, `validator(password)` is called.
- Decompiling `validator()` reveals a validation function `is_panic_trigger()` checking two conditions on the input string:
  1. `input.length() == 213`
  2. Input characters at index positions `0, 10, 20, 30, ..., 210` match the pattern string `!@#$%^&*+=?~!@#$%^&*+=`.

### Step 2: Discovering Exception Redirection
- Analyzing `validator()` shows that if `is_panic_trigger()` evaluates to true, it throws an empty struct exception `throw panic()`.
- Checking `main()` reveals a `try-catch` block:
  ```cpp
  try {
      decrypt(password);
  }
  catch (const panic&) {
      resolve_fault();
  }
  ```
- `catch (const panic&)` intercepts the exception and invokes `resolve_fault()`.

### Step 3: Analyzing `resolve_fault()` & Anti-Debugging
- Inspecting `resolve_fault()` shows it calls `generate_dynamic_key(key_out)` to reconstruct a 32-byte Libsodium key in stack memory and decrypts `ciphertext_hex`.
- Inspecting `generate_dynamic_key()` reveals a `ptrace(PTRACE_TRACEME, 0, 1, 0)` call.
- **Gotcha**: If the player attempts to step through `resolve_fault()` in GDB, `ptrace` fails with `-1`, altering the mask calculation (`m = 0xA5`), producing a corrupted key and failing decryption.
- **Bypass**: The player must either patch out the `ptrace` check in the binary, override `ptrace` via `LD_PRELOAD`, or run the payload natively outside of a debugger.

### Step 4: Crafting Payload & Retrieving Flag
The player generates a 213-character string containing the required symbols at indices `0, 10, 20, ..., 210`:

```python
symbols = "!@#$%^&*+=?~!@#$%^&*+="
buf = list("A" * 213)
for i, char in enumerate(symbols):
    buf[i * 10] = char
payload = "".join(buf)

print(payload)
```

Running the challenge binary natively with the payload:
```bash
python3 solve.py | ./decrypt-str
```

**Output**:
```text
Enter password: Decrypted: snuc{Davinci_dice_Sono_stanco_capo}
```
Flag captured: `snuc{Davinci_dice_Sono_stanco_capo}`!
