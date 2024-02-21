#ifndef SPOUT_GD_SPOUT_TEXTURE_H
#define SPOUT_GD_SPOUT_TEXTURE_H

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/image_texture.hpp>

#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/rid.hpp>
#include <godot_cpp/variant/variant.hpp>

#include "spout_gd.h"

using namespace godot;

class SpoutTexture : public ImageTexture {
    GDCLASS(SpoutTexture, ImageTexture);
	
    private:
        Spout *_spout;
        Ref<Image> _image;
        PackedByteArray _buffer;
        String _sender_name;

        void rebuild_image(int32_t width, int32_t height);
        void poll_server();
    protected:
        static void _bind_methods();
    public:
        SpoutTexture();
        ~SpoutTexture();

        void set_sender_name(String sender_name);
        String get_sender_name() const;
};

#endif  // SPOUT_GD_SPOUT_TEXTURE_H
