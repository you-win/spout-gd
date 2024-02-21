#include "spout_texture.h"

void SpoutTexture::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_sender_name", "sender_name"), &SpoutTexture::set_sender_name);
    ClassDB::bind_method(D_METHOD("get_sender_name"), &SpoutTexture::get_sender_name);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "sender_name"), "set_sender_name", "get_sender_name");
}

void SpoutTexture::set_sender_name(String sender_name) {
    _sender_name = sender_name;

    _spout->release_receiver();
    _spout->set_receiver_name(sender_name);
    godot::UtilityFunctions::print("[spout-gd] connecting to sender ", sender_name);
}

String SpoutTexture::get_sender_name() const {
    return _sender_name;
}

void SpoutTexture::rebuild_image(int32_t width, int32_t height) {
    if (width <= 0 || height <= 0) {
        return;
    }

    godot::UtilityFunctions::print("[spout-gd] image dimensions changed ", width, "x", height);

    // keep an internal reference to the mutable image
    // godot's get_image function returns a copy from the GPU, so it's not very performant
    _image = Image::create(width, height, false, Image::Format::FORMAT_RGBA8);
    
    ImageTexture::set_image(_image);
    
    // Godot creates a copy of the data buffer as well because it's safer and prevents unintentional
    // updates and race conditions.  However, this is a controlled scenario and we
    // explicitly do want to update the data, so let's make this as performant as possible.
    // We do this be reusing a shared buffer associated by reference.
    _buffer = _image->get_data();
}

void SpoutTexture::poll_server() {
    if (is_queued_for_deletion()) {
        return;
    }

    // detect if source info has changed
	// Spout SDK states that this is required before every receive call
    if (_spout->is_updated()) {
        int32_t width = _spout->get_sender_width();
        int32_t height = _spout->get_sender_height();
        
        rebuild_image(width, height);
    }
    
    if (_spout->receive_buffer(_buffer, Spout::GLFormat::FORMAT_RGBA, false, false)) {
        _image->set_data(
            _image->get_width(),
            _image->get_height(),
            _image->has_mipmaps(),
            _image->get_format(),
            _buffer
        );
        // tells texture storage that the buffer associated with this image has been updated
        ImageTexture::update(_image);
    }
}

SpoutTexture::SpoutTexture() {
    // create a placeholder image for spout
    _spout = new Spout();
    _sender_name = String("");
    _image = Image::create(int32_t(1), int32_t(1), false, Image::Format::FORMAT_RGBA8);
    _buffer = _image->get_data();
    ImageTexture::set_image(_image);

    RenderingServer::get_singleton()->texture_set_force_redraw_if_visible(get_rid(), true);
    
    auto _update = callable_mp(this, &SpoutTexture::poll_server);

    RenderingServer::get_singleton()->connect(
        "frame_pre_draw",
        _update
    );
}

SpoutTexture::~SpoutTexture() {
    if (_spout != NULL) {
        _spout->release_receiver();
    }
}
