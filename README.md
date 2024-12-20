# Spout GD

[Godot](https://github.com/godotengine/godot) 4.1.1 bindings for [Spout](https://github.com/leadedge/Spout2).

The Godot 3 engine module is located on the `godot-3` branch.

## Status

The sender/receiver apis have been implemented but are mostly untested. Please open an issue
if something doesn't seem to work.

## Building

Prerequisites:

* Windows (Spout only works on Windows)
* git
* bash
* scons
* a C++ compiler capable of [compiling Godot](https://docs.godotengine.org/en/stable/contributing/development/compiling/compiling_for_windows.html)
* cmake

Clone the repository: `git clone --recurse-submodules https://github.com/you-win/spout-gd.git`

Run `build.sh` in a bash-compatible terminal. This will:

* Compile Spout2
* Compile the gdextension bindings
* Compile spout-gd

Once compilation is finished, the following files should be moved to your project's `addons/spout-gd` folder (create it manually if it does not exist):

* `spout_gd.gdextension`
* `Spout2/Binaries/x64/SpoutLibrary.dll`
* `out/spout_gd.windows.template_debug.dll`
* `out/spout_gd.windows.template_release.dll`

Like this:
```
Godot_project_root
├───addons
│   ├───spout-gd
│   │    ├───SpoutLibrary.dll
│   │    ├───spout_gd.gdextension
│   │    ├───spout_gd.windows.template_debug.dll
│   │    ├───spout_gd.windows.template_release.dll
```

## Basic usage

```gdscript
var spout: Spout
var my_image: Image = load_from_somewhere_idk() # Provide your own image somehow
func _ready():
    spout = Spout.new()
    
    spout.send_image(my_image, image.get_width(), image.get_height())
```
