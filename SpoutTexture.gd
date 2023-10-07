extends ImageTexture
class_name SpoutTexture

@export var sender_name: String : set = _set_sender_name

var spout: Spout

# receiving image buffer
var image: Image

func _init():
    spout = Spout.new()
    image = Image.create(
        100, 100, # placeholder buffer until we get a real image to show off
        false,
        Image.FORMAT_RGBA8
    )
    set_image(image)
    
    # makes sure to update the data from the spout source
    # while the texture is visible anywhere
    RenderingServer.texture_set_force_redraw_if_visible(get_rid(), true)
    RenderingServer.frame_pre_draw.connect(_update)

func _set_sender_name(name):
    sender_name = name
    
    # in case a connection was already open
    spout.release_receiver()
    
    # connect to new receiver
    spout.set_receiver_name(name)
    
func _rebuild_image():
    if spout.get_sender_width() <= 0 || spout.get_sender_height() <= 0:
        return
    
    image = Image.create(
        spout.get_sender_width(), spout.get_sender_height(),
        false,
        Image.FORMAT_RGBA8
    )
    set_image(image)
    
func _notification(what):
    if what == NOTIFICATION_PREDELETE:
        # in case a connection was already open
        if spout != null:
            spout.release_receiver()
        
func _update():
    if is_queued_for_deletion():
        return
    
    var active_name = spout.get_sender_name()
    if active_name != sender_name:
        _set_sender_name(sender_name)
        return
    
    # detect if source info has changed
    # Spout SDK states that this is required before every receive call
    if spout.is_updated():
        _rebuild_image()
    
    if spout.receive_image(image, Spout.FORMAT_RGBA):
        update(image)
