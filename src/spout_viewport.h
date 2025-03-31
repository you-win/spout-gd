#ifndef SPOUT_GD_SPOUT_VIEWPORT_H
#define SPOUT_GD_SPOUT_VIEWPORT_H

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/viewport_texture.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>

#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/rid.hpp>

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include "spout_gd.h"

using namespace godot;

class SpoutViewport : public SubViewport {
    GDCLASS(SpoutViewport, SubViewport);

    private:
        Ref<Spout> _spout;
        String _sender_name;

        godot::Vector2i _size;
        PackedByteArray _buffer;

        std::thread _spout_thread;
        std::condition_variable _spout_signal;
        std::mutex _buffer_mutex;
        std::atomic<bool> _new_frame_ready{false};
        bool _stop_worker = false;

        void poll_server();
        void start_worker_thread();
    protected:
        static void _bind_methods();
        void _notification(int p_what);
    public:
        SpoutViewport();
        ~SpoutViewport();

        void set_sender_name(String sender_name);
        String get_sender_name() const;
};

#endif  // SPOUT_GD_SPOUT_VIEWPORT_H
