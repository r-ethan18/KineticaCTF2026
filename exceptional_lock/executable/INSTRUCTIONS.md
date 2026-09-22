Run these commands if compiled on nixos:

    patchelf --set-interpreter /lib64/ld-linux-x86-64.so.2 exceptional-locking-mechanism
    patchelf --remove-rpath exceptional-locking-mechanism
