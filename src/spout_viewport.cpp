#include "spout_viewport.h"

void SpoutViewport::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_sender_name", "sender_name"), &SpoutViewport::set_sender_name);
    ClassDB::bind_method(D_METHOD("get_sender_name"), &SpoutViewport::get_sender_name);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "sender_name"), "set_sender_name", "get_sender_name");
}

void SpoutViewport::set_sender_name(String sender_name) {
    _sender_name = sender_name;
}

String SpoutViewport::get_sender_name() const {
    return _sender_name;
}

void SpoutViewport::poll_server() {
    if (is_queued_for_deletion()) {
        return;
    }

    if (!is_inside_tree()) {
        return;
    }

    if (_spout == nullptr) {
        return;
    }

    if (_spout->get_sender_name() != _sender_name) {
        _spout->release_sender();
        _spout->set_sender_name(_sender_name);
    }

    auto rs = RenderingServer::get_singleton();
    auto size = get_size();
    
    // spout's API only supports GL texture handles at this moment.  If it exposes DX12 or Vulkan resource handles,
    // switch to use RenderingDevice's get_driver_resource and send the texture handle directly
    if (_using_gl_renderer) {
        _spout->send_texture(
            rs->texture_get_native_handle(get_viewport_rid()),
            0x0DE1, // GL_TEXTURE_2D
            size.x,
            size.y,
            false
        );
    }
    // potentially slow, copies from GPU to CPU to send as pixels
    else {
        _spout->send_image(
            get_texture()->get_image(),
            size.x,
            size.y,
            has_transparent_background() ? Spout::GLFormat::FORMAT_RGBA : Spout::GLFormat::FORMAT_RGB,
            false
        );
    }
}

void SpoutViewport::_notification(int p_what) {
    if (p_what == NOTIFICATION_READY && !Engine::get_singleton()->is_editor_hint()) {
        _spout = Ref(memnew(Spout));

        auto _update = callable_mp(this, &SpoutViewport::poll_server);

        RenderingServer::get_singleton()->connect(
            "frame_post_draw",
            _update
        );
    }
    else if (p_what == NOTIFICATION_PREDELETE) {
        if (_spout != nullptr) {
            _spout->release_sender();
        }    
    }
}

SpoutViewport::SpoutViewport() {
    // create a placeholder image for spout
    _sender_name = String("");
    // detect renderer type to know if we can send textures directly over spout
    _using_gl_renderer = RenderingServer::get_singleton()->get_current_rendering_method() == String("gl_compatibility");
}

SpoutViewport::~SpoutViewport() {
    if (_spout != nullptr) {
        _spout->release_sender();
    }
}
