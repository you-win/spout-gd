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
}

String SpoutTexture::get_sender_name() const {
    return _sender_name;
}

void SpoutTexture::rebuild_image() {
    int32_t width = _spout->get_sender_width();
    int32_t height = _spout->get_sender_height();

    if (width <= 0 || height <= 0) {
        return;
    }

    _image = Image::create(width, height, false, Image::Format::FORMAT_RGBA8);

    ImageTexture::set_image(_image);
}

void SpoutTexture::poll_server() {
    if (is_queued_for_deletion()) {
        return;
    }

    String active_name = _spout->get_sender_name();
    if (active_name != _sender_name) {
        set_sender_name(active_name);
    }

    // detect if source info has changed
	// Spout SDK states that this is required before every receive call
    if (_spout->is_updated()) {
        rebuild_image();
    }

    if (_spout->receive_image(_image, Spout::GLFormat::FORMAT_RGBA, false, false)) {
        ImageTexture::update(_image);
    }
}

SpoutTexture::SpoutTexture() {
    // create a placeholder image for spout
    _spout = new Spout();
    _image = Image::create(int32_t(100), int32_t(100), false, Image::Format::FORMAT_RGBA8);
    _sender_name = String("");
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