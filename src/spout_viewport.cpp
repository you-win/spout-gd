#include "spout_viewport.h"

void SpoutViewport::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_sender_name", "sender_name"), &SpoutViewport::set_sender_name);
    ClassDB::bind_method(D_METHOD("get_sender_name"), &SpoutViewport::get_sender_name);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "sender_name"), "set_sender_name", "get_sender_name");
}

void SpoutViewport::set_sender_name(String sender_name) {
    _sender_name = sender_name;

    _spout->release_receiver();
    _spout->set_receiver_name(sender_name);
}

String SpoutViewport::get_sender_name() const {
    return _sender_name;
}

void SpoutViewport::poll_server() {
    if (is_queued_for_deletion()) {
        return;
    }

    String active_name = _spout->get_sender_name();
    if (active_name != _sender_name) {
        _spout->set_sender_name(_sender_name);
    }

    auto image = get_texture()->get_image();

    _spout->send_image(
        image,
        image->get_width(),
        image->get_height()
    );
}

SpoutViewport::SpoutViewport() {
    // create a placeholder image for spout
    _spout = new Spout();
    _sender_name = String("");

    auto _update = callable_mp(this, &SpoutViewport::poll_server);

    RenderingServer::get_singleton()->connect(
        "frame_post_draw",
        _update
    );
}

SpoutViewport::~SpoutViewport() {
    if (_spout != NULL) {
        _spout->release_receiver();
    }
}