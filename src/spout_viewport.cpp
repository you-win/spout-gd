#include "spout_viewport.h"
#include <godot_cpp/classes/rendering_device.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/rd_texture_format.hpp>

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

    if (_spout->get_sender_name() != _sender_name) {
        _spout->release_sender();
        _spout->set_sender_name(_sender_name);
    }

    auto rs = godot::RenderingServer::get_singleton();
    auto rd = rs->get_rendering_device();
    
    auto rid = get_viewport_rid();
    auto rs_tex_rid = rs->viewport_get_texture(rid);
    auto rd_tex_rid = rs->texture_get_rd_texture(rs_tex_rid);
    auto bytes = rd->texture_get_data(rd_tex_rid, 0);
    {
        std::unique_lock<std::mutex> lock(_buffer_mutex);
        if (_buffer.size() != bytes.size()) {
            _buffer.resize(bytes.size());
        }

        // update metadata
        _size = get_size();

        // copy _buffer
        memcpy(_buffer.ptrw(), bytes.ptr(), _buffer.size());
        _new_frame_ready.store(true);
    }
    _spout_signal.notify_one();
}

void SpoutViewport::_notification(int p_what) {
    if (p_what == NOTIFICATION_READY && !Engine::get_singleton()->is_editor_hint()) {
        _spout = Ref(memnew(Spout));

        auto _update = callable_mp(this, &SpoutViewport::poll_server);

        RenderingServer::get_singleton()->connect(
            "frame_post_draw",
            _update
        );

        start_worker_thread();
    }
    else if (p_what == NOTIFICATION_PREDELETE) {
        _stop_worker = true;
        if (_spout != nullptr) {
            _spout->release_sender();
        }    
    }
}

void SpoutViewport::start_worker_thread() {
    _spout_thread = std::thread([this] {
        while (true) {
            std::unique_lock<std::mutex> lock(_buffer_mutex);
            _spout_signal.wait(lock, [this] { return _new_frame_ready.load() || _stop_worker; });

            if (_stop_worker)
                return; // Exit safely

            _new_frame_ready.store(false);

            _spout->send_bytes(
                _buffer, _size.x, _size.y,
                Spout::GLFormat::FORMAT_RGBA,
                false
            );
        }
    });
}

SpoutViewport::SpoutViewport() {
    // create a placeholder image for spout
    _sender_name = String("");
}

SpoutViewport::~SpoutViewport() {
    if (_spout != nullptr) {
        _spout->release_sender();
    }
    _stop_worker = true;
    if (_spout_thread.joinable()) {
        _spout_thread.join();
    }
}
