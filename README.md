# Swamp Surveyor

## Build
- Linux: `make`
- MacOS: `make macos && ./macos_installer.sh`
  - Optionally: `./macos_packager.sh`

## Debug Mode
- Run `./swamp.out -d` to run in debug mode. This shows player position, framerate, and highlights interactive features in the map
- Use `-c` and `-b` options to change the location of the configuration file
  - The configuration is in `resources/`
    - The MacOS installer puts this in the user's application support folder
