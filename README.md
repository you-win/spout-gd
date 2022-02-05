# spout-gd
An engine module for Godot 3.4.2 that adds support for [Spout2](https://github.com/leadedge/Spout2). This module uses [SpoutLibrary](https://spoutlibrary-site.netlify.app/#File:SpoutLibrary.cpp).

Spout is Windows-only as noted on their [website](https://spout.zeal.co/).

## Compiling
0. Setup your environment to [compile Godot](https://docs.godotengine.org/en/stable/development/compiling/index.html)
1. Download the SPOUTSDK from the [Spout2](https://github.com/leadedge/Spout2) repository and place into the `thirdparty/` directory. A dummy folder has been provided
2. Copy the folders in this repository into their respective folders in Godot
3. Patch in FBO support to Godot using the patch provided in the `patches/` directory
4. Compile Godot
5. Copy the `SpoutLibrary.dll` located at `thirdparty/SPOUTSDK/SpoutLibrary/Binaries/x64/` next to your new Godot executable. The editor will refuse to run without it
6. `SpoutGD` is now available to be used via GDScript

## Usage
Send the default `icon.png` file over Spout. NOTE: This is cpu-bound and is slow relatively to Spout's full capabilities.
```
var spout: SpoutGD
var image := Image.new()

func _ready():
	image.load("res://icon.png")
	spout = SpoutGD.new()
	spout.set_size(image.get_width(), image.get_height()) # Implicitly used by SpoutGD when sending the image

func _process(delta):
	spout.send_image(image)
```

Send the viewport over Spout. This utilizes the GPU and, in theory, should be very fast.
```
var spout: SpoutGD

var viewport: Viewport
var viewport_fbo: int # The OpenGL framebuffer object (fbo)

func _ready():
	viewport = get_viewport()

	spout = SpoutGD.new()
	spout.set_size(viewport.size.x, viewport.size.y) # According to the Spout docs, this isn't necessary

	# texture_get_fbo must be patched in since this isn't normally exposed by Godot
	viewport_fbo = VisualServer.texture_get_fbo(VisualServer.viewport_get_texture(viewport.get_viewport_rid()))
	
func _process(delta):
	spout.send_screen(viewport_fbo)
```

