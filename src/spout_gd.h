#ifndef SPOUT_GD_H
#define SPOUT_GD_H

#include <Spout2/SPOUTSDK/SpoutLibrary/SpoutLibrary.h>

#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

class Spout : public RefCounted {
    GDCLASS(Spout, RefCounted);

   private:
    SPOUTLIBRARY *lib;

   protected:
    static void _bind_methods();

   public:
    enum SpoutLogLevel {
        SPOUT_LOG_SILENT = SpoutLibLogLevel::SPOUT_LOG_SILENT,
        SPOUT_LOG_VERBOSE = SpoutLibLogLevel::SPOUT_LOG_VERBOSE,
        SPOUT_LOG_NOTICE = SpoutLibLogLevel::SPOUT_LOG_NOTICE,
        SPOUT_LOG_WARNING = SpoutLibLogLevel::SPOUT_LOG_WARNING,
        SPOUT_LOG_ERROR = SpoutLibLogLevel::SPOUT_LOG_ERROR,
        SPOUT_LOG_FATAL = SpoutLibLogLevel::SPOUT_LOG_FATAL,
        SPOUT_LOG_NONE = SpoutLibLogLevel::SPOUT_LOG_NONE
    };

    enum GLFormat {
        FORMAT_RGB = 0x1907,
        FORMAT_RGBA = 0x1908,
        FORMAT_BGRA = 0x80E1,
        FORMAT_BGRA_EXT = 0x80E1
    };

    void set_sender_name(const String &p_sender_name = String());
    void set_sender_format() {
        UtilityFunctions::printerr("Not supported!");
    }
    /// NOTE parameters are not supported! Requires `DWORD` casting.
    void release_sender();
    bool send_fbo(GLuint p_fbo_id, unsigned int p_width, unsigned int p_height, bool p_invert = true);
    bool send_texture(GLuint p_texture_id, GLuint p_texture_target, unsigned int p_width, unsigned int p_height, bool p_invert = true, GLuint p_host_fbo = 0);
    bool send_image(const Ref<Image> p_image, unsigned int p_width, unsigned int p_height, GLFormat p_gl_format = GLFormat::FORMAT_RGBA, bool p_invert = false);
    String get_name();
    unsigned int get_width();
    unsigned int get_height();
    double get_fps();
    int get_frame();
    void get_handle() {
        UtilityFunctions::printerr("Not supported!");
    }
    bool get_cpu();
    bool get_gldx();

    void set_receiver_name(const String p_sender_name = String());
    void release_receiver();
    bool receive_texture(GLuint p_texture_id = 0, GLuint p_texture_target = 0, bool p_invert = false, GLuint p_host_fbo = 0);
    bool receive_buffer(PackedByteArray p_data, GLFormat p_gl_format, bool p_invert = false, GLuint p_host_fbo = 0);
    bool receive_image(Ref<Image> p_image, GLFormat p_gl_format, bool p_invert = false, GLuint p_host_fbo = 0);
    bool is_updated();
    bool is_connected();
    bool is_frame_new();
    String get_sender_name();
    unsigned int get_sender_width();
    unsigned int get_sender_height();
    void get_sender_format() {
        UtilityFunctions::printerr("Not supported!");
    }
    double get_sender_fps();
    int get_sender_frame();
    void get_sender_handle() {
        UtilityFunctions::printerr("Not supported!");
    }
    bool get_sender_cpu();
    bool get_sender_gldx();
    void select_sender() {
        UtilityFunctions::printerr("Not supported!");
    }

    // utility functions
    unsigned int get_sender_count();
    String get_sender(unsigned int idx);

    Spout();
    ~Spout();
};

VARIANT_ENUM_CAST(Spout::SpoutLogLevel);
VARIANT_ENUM_CAST(Spout::GLFormat);

#endif  // SPOUT_GD_H