{
  description = "C++";

  inputs.nixpkgs.url = "nixpkgs";

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
    in {
      devShells.${system}.default = pkgs.mkShell {
        nativeBuildInputs = with pkgs; [
          pkg-config
          cmake
          gnumake
          clang-tools
        ];
        buildInputs = with pkgs; [
          gcc
          gdb
        ];
      };
    };
}
