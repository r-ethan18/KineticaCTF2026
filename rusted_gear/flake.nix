{
  description = "Chopped Python LSP";

  inputs.nixpkgs.url = "nixpkgs";

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs {
        inherit system;
      };

      python = pkgs.python314.withPackages (ps: with ps; [
        python-lsp-server
        mypy
        pylint
        pylsp-mypy
      ]);
    in {
      devShells.${system}.default = pkgs.mkShell {
        packages = with pkgs; [
          python
          ruff
          hexedit
        ];

        shellHook = ''
          echo "Python: $(python --version)"
          echo "Pylsp:  $(pylsp --version)"
        '';
      };
    };
}
