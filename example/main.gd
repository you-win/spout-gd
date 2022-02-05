extends CanvasLayer

var spout: SpoutGD

#var image := Image.new()

var viewport: Viewport
var viewport_fbo: int

###############################################################################
# Builtin functions                                                           #
###############################################################################

func _ready() -> void:
#	image.load("res://icon.png")
	viewport = get_viewport()
	
	spout = SpoutGD.new()
#	spout.set_size(image.get_width(), image.get_height())
	spout.set_size(viewport.size.x, viewport.size.y)
	
#	spout.set_frame_count(true)
#	print(spout.is_frame_count_enabled())
	
	viewport_fbo = VisualServer.texture_get_fbo(VisualServer.viewport_get_texture(viewport.get_viewport_rid()))

func _process(_delta: float) -> void:
#	spout.send(viewport.get_texture().get_data())
	
#	spout.send_screen(VisualServer.texture_get_fbo(VisualServer.viewport_get_texture(viewport_rid)))
	spout.send_screen(viewport_fbo)

###############################################################################
# Connections                                                                 #
###############################################################################

###############################################################################
# Private functions                                                           #
###############################################################################

###############################################################################
# Public functions                                                            #
###############################################################################
