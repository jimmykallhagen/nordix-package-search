# Nordix Package Search Tools

Enhanced package search utilities for Arch Linux that provide a cleaner, more informative output than the default `pacman -Ss` and `paru -Ss` commands.

Perfect when you want to install a package even though you have tired eyes.

## Features

- **Formatted output** - Clear visual separation between packages for easier reading
- **Package sizes** - Shows installed size for each package (fetched via `expac`)
- **Search statistics** - Displays total found, installed count, and available disk space
- **Auto-scroll** - Automatically scrolls to top after search so you see results from the beginning
- **Color coded** - Visual distinction between package info, descriptions, and metadata
- **AUR detection** - `nordix-paru` distinguishes between AUR and repo packages

## Installation

### Dependencies

- `pacman` (for nordix-pacman)
- `paru` (for nordix-paru)
- `expac` - for fetching package sizes

### Build

```bash
# nordix-pacman
clang -march=native -mtune=native -O3 -flto=full -o nordix-pacman nordix-pacman.c
# nordix-paru
clang -march=native -mtune=native -O3 -flto=full -o nordix-paru nordix-paru.c
```

### Install

```bash
sudo cp nordix-pacman nordix-paru /usr/bin/
```

### Permissions (for auto-scroll)

The auto-scroll feature uses `/dev/uinput`. Add your user to the `input` group:

```bash
sudo usermod -aG input $USER
```

Create udev rule for uinput access:

```bash
echo 'KERNEL=="uinput", MODE="0660", GROUP="input"' | sudo tee /etc/udev/rules.d/99-uinput.rules
sudo udevadm control --reload-rules
sudo udevadm trigger
```

Log out and back in for changes to take effect.

## Usage

### nordix-pacman

Search official repositories:

```bash
nordix-pacman <search term>
```

Example:
```bash
nordix-pacman firefox
nordix-pacman linux headers
```

### nordix-paru

Search official repositories and AUR:

```bash
nordix-paru <search term>
```

Example:
```bash
nordix-paru spotify
nordix-paru --no-size hyprland
```

## Options

| Option       | Description                                  |
|--------------|----------------------------------------------|
| `--scroll-0` | Disable auto-scroll                          |
| `--scroll-1` | Scroll up after search (default)             |
| `--scroll-2` | Scroll down after search                     |
| `--no-size`  | Skip size lookup (faster) - nordix-paru only |
| `--help, -h` | Show help message                            |

## Output Example

```
𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹
❄️ Nordix package search  Pacman: firefox
Scroll mode: Up (default)
Found: 359 Installed: 6 Diskspace: 164 GB
𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹

cachyos-znver4/firefox-pure 148.0-1
    Fast, Private & Safe Web Browser
  Installed Size: 264.53 MiB

▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔
cachyos-znver4/firefox-pure-debug 148.0-1
    Detached debugging symbols for firefox-pure
  Installed Size: 0.04 MiB

▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔
cachyos-znver4/firefox-wayland-cachy-hg 149.0a1.r947823.00dfa34-1
    Standalone web browser from mozilla.org (mozilla-unified hg, release branding, targeting wayland)
  Installed Size: 281.64 MiB

▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔
cachyos-extra-znver4/curl-impersonate 1.5.0-1.1
    A build of curl that impersonates four major browsers: Chrome, Edge, Safari & Firefox
  Installed Size: 4.06 MiB

▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔
```

For `nordix-paru`, AUR packages are marked:

```
aur/firefox-nightly 130.0a1-1
    Fast, Private & Safe Web Browser - Nightly build
  AUR package (size unknown until build)
```

## How It Works

1. Runs `pacman -Ss` or `paru -Ss` with your search terms
2. Parses the output to extract package names
3. Fetches installed sizes using `expac`
4. Displays formatted output with separators and statistics
5. Auto-scrolls to top so you see the header and first results

## Shell Integration

### Fish

Add to `~/.config/fish/config.fish`:

```fish
# Nordix package search aliases
function pacman
    if test "$argv[1]" = "-Ss"
        nordix-pacman --scroll-1 $argv[2..-1]
    else
        command pacman $argv
    end
end

## aur options        --sortby <votes|popularity|id|baseid|name|base|submitted|modified>
function paru
    if test "$argv[1]" = "-Ss"
        nordix-paru --aur --sortby popularity $argv[2..-1]
    else
        command paru $argv
    end
end

### Bash and ZSH

```bash
pacman() {
    if [ "$1" = "-Ss" ]; then
        shift
        nordix-pacman --scroll-1 "$@"
    else
        command pacman "$@"
    fi
}

paru() {
    if [ "$1" = "-Ss" ]; then
        shift
        nordix-paru --aur --sortby popularity "$@"
    else
        command paru "$@"
    fi
}
```


This lets you use the normal `pacman -Ss` and `paru -Ss` syntax while getting the enhanced nordix output.

---

## License

 * SPDX-License-Identifier: PolyForm-Noncommercial-1.0.0                            
 * [**Nordix - license**](https://polyformproject.org/licenses/noncommercial/1.0.0) 
 * Copyright (c) 2025 Jimmy Källhagen                                               
 * Part of Nordix - https://github.com/jimmykallhagen/Nordix                        
 * Nordix and Yggdrasil are trademarks of Jimmy Källhagen 

---

## Author

>*Jimmy Källhagen*
