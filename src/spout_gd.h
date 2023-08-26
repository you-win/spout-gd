#ifndef SPOUT_GD_H
#define SPOUT_GD_H

#include <Spout2/SPOUTSDK/SpoutLibrary/SpoutLibrary.h>

#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

// Redefined to match SpoutLibrary.h
// typedef unsigned int GLuint;
// typedef unsigned int GLenum;

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
        RGBA = 0x1908,
        BGRA = 0x80E1,
        BGRA_EXT = 0x80E1
    };

    void set_sender_name(const String &p_sender_name = String());
    void set_sender_format() {
        UtilityFunctions::printerr("Not supported!");
    }
    /// NOTE parameters are not supported! Requires `DWORD` casting.
    void release_sender();
    bool send_fbo(GLuint p_fbo_id, unsigned int p_width, unsigned int p_height, bool p_invert = true);
    bool send_texture(GLuint p_texture_id, GLuint p_texture_target, unsigned int p_width, unsigned int p_height, bool p_invert = true, GLuint p_host_fbo = 0);
    bool send_image(const Ref<Image> p_image, unsigned int p_width, unsigned int p_height, GLFormat p_gl_format, bool p_invert = false);
    String get_name();
    unsigned int get_width();
    unsigned int get_height();
    double get_fps();
    long get_frame();
    // NOTE casts the pointer to a uint!
    unsigned int get_handle();
    bool get_cpu();
    bool get_gldx();

    Spout();
    ~Spout();
};

VARIANT_ENUM_CAST(Spout::SpoutLogLevel);
VARIANT_ENUM_CAST(Spout::GLFormat);

#endif  // SPOUT_GD_H