#include "spout_gd.h"

void Spout::set_sender_name(const String &p_sender_name) {
    if (p_sender_name.length() < 1) {
        lib->SetSenderName();
    } else {
        lib->SetSenderName(p_sender_name.utf8().get_data());
    }
}

void Spout::release_sender() {
    lib->ReleaseSender();
}

bool Spout::send_fbo(GLuint p_fbo_id, unsigned int p_width, unsigned int p_height, bool p_invert) {
    return lib->SendFbo(p_fbo_id, p_width, p_height, p_invert);
}

bool Spout::send_texture(GLuint p_texture_id, GLuint p_texture_target, unsigned int p_width, unsigned int p_height, bool p_invert, GLuint p_host_fbo) {
    return lib->SendTexture(p_texture_id, p_texture_target, p_width, p_height, p_invert, p_host_fbo);
}

bool Spout::send_image(const Ref<Image> p_image, unsigned int p_width, unsigned int p_height, GLFormat p_gl_format, bool p_invert) {
    PackedByteArray data = p_image->get_data();
    const unsigned char *p = (const unsigned char *)data.ptr();

    return lib->SendImage(p, p_width, p_height, p_gl_format, p_invert);
}

String Spout::get_name() {
    return String(lib->GetName());
}

unsigned int Spout::get_width() {
    return lib->GetWidth();
}

unsigned int Spout::get_height() {
    return lib->GetHeight();
}

double Spout::get_fps() {
    return lib->GetFps();
}

int Spout::get_frame() {
    return (int)lib->GetFrame();
}

bool Spout::get_cpu() {
    return lib->GetCPU();
}

bool Spout::get_gldx() {
    return lib->GetGLDX();
}

void Spout::set_receiver_name(const String p_sender_name) {
    if (p_sender_name.length() < 1) {
        lib->SetReceiverName();
    } else {
        lib->SetReceiverName(p_sender_name.utf8().get_data());
    }
}

void Spout::release_receiver() {
    lib->ReleaseReceiver();
}

bool Spout::receive_texture(GLuint p_texture_id, GLuint p_texture_target, bool p_invert, GLuint p_host_fbo) {
    return lib->ReceiveTexture(p_texture_id, p_texture_target, p_invert, p_host_fbo);
}

bool Spout::receive_buffer(PackedByteArray p_data, GLFormat p_gl_format, bool p_invert, GLuint p_host_fbo) {
    
    // ptrw creates a copy on write, so we use ptr for a direct reusable reference
    unsigned char *p = (unsigned char *)p_data.ptr();

    bool result = lib->ReceiveImage(p, p_gl_format, p_invert, p_host_fbo);

    if (!result) {
        godot::UtilityFunctions::print("[spout-gd] error receiving image");
    }

    return result;
}

bool Spout::receive_image(Ref<Image> p_image, GLFormat p_gl_format, bool p_invert, GLuint p_host_fbo) {
    // this creates a copy, we must reassign afterwards
    // if you are simply sampling a Spout receiver, this function is okay
    // when you need more performance, use receive_buffer with a reusable byte array
    PackedByteArray data = p_image->get_data();

    bool result = receive_buffer(data, p_gl_format, p_invert, p_host_fbo);

    if (result) {
        // assign the new data buffer, overwriting to the old specification
        // this must happen as close to the vector as possible,
        // otherwise it might get refcounted away
        p_image->set_data(
            p_image->get_width(),
            p_image->get_height(),
            p_image->has_mipmaps(),
            p_image->get_format(),
            data
        );
    }

    return result;
}

bool Spout::is_updated() {
    return lib->IsUpdated();
}

bool Spout::is_connected() {
    return lib->IsConnected();
}

bool Spout::is_frame_new() {
    return lib->IsFrameNew();
}

String Spout::get_sender_name() {
    return String(lib->GetSenderName());
}

unsigned int Spout::get_sender_width() {
    return lib->GetSenderWidth();
}

unsigned int Spout::get_sender_height() {
    return lib->GetSenderHeight();
}

double Spout::get_sender_fps() {
    return lib->GetSenderFps();
}

int Spout::get_sender_frame() {
    return (int)lib->GetSenderFrame();
}

bool Spout::get_sender_cpu() {
    return lib->GetSenderCPU();
}

bool Spout::get_sender_gldx() {
    return lib->GetSenderGLDX();
}

unsigned int Spout::get_sender_count() {
    return lib->GetSenderCount();
}

String Spout::get_sender(unsigned int idx) {
    char senderName[256];
    
    lib->GetSender(idx, senderName);

    return String(senderName);
}

void Spout::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_sender_name", "sender_name"), &Spout::set_sender_name);
    ClassDB::bind_method(D_METHOD("set_sender_format"), &Spout::set_sender_format);
    ClassDB::bind_method(D_METHOD("release_sender"), &Spout::release_sender);
    ClassDB::bind_method(D_METHOD("send_fbo", "fbo_id", "width", "height", "invert"), &Spout::send_fbo, DEFVAL(true));
    ClassDB::bind_method(D_METHOD("send_texture", "texture_id", "texture_target", "width", "height", "invert", "host_fbo"), &Spout::send_texture, DEFVAL(true), DEFVAL(0));
    ClassDB::bind_method(D_METHOD("send_image", "image", "width", "height", "gl_format", "invert"), &Spout::send_image, DEFVAL(Spout::FORMAT_RGBA), DEFVAL(true));
    ClassDB::bind_method(D_METHOD("get_name"), &Spout::get_name);
    ClassDB::bind_method(D_METHOD("get_width"), &Spout::get_width);
    ClassDB::bind_method(D_METHOD("get_height"), &Spout::get_height);
    ClassDB::bind_method(D_METHOD("get_fps"), &Spout::get_fps);
    ClassDB::bind_method(D_METHOD("get_frame"), &Spout::get_frame);
    ClassDB::bind_method(D_METHOD("get_handle"), &Spout::get_handle);
    ClassDB::bind_method(D_METHOD("get_cpu"), &Spout::get_cpu);
    ClassDB::bind_method(D_METHOD("get_gldx"), &Spout::get_gldx);

    ClassDB::bind_method(D_METHOD("set_receiver_name", "sender_name"), &Spout::set_receiver_name);
    ClassDB::bind_method(D_METHOD("release_receiver"), &Spout::release_receiver);
    ClassDB::bind_method(D_METHOD("receive_texture", "texture_id", "texture_target", "invert", "host_fbo"), &Spout::receive_texture, DEFVAL(0), DEFVAL(0), DEFVAL(false), DEFVAL(0));
    ClassDB::bind_method(D_METHOD("receive_image", "image", "gl_format", "invert", "host_fbo"), &Spout::receive_image, DEFVAL(false), DEFVAL(0));
    ClassDB::bind_method(D_METHOD("is_updated"), &Spout::is_updated);
    ClassDB::bind_method(D_METHOD("is_sender_connected"), &Spout::is_connected);
    ClassDB::bind_method(D_METHOD("is_frame_new"), &Spout::is_frame_new);
    ClassDB::bind_method(D_METHOD("get_sender_name"), &Spout::get_sender_name);
    ClassDB::bind_method(D_METHOD("get_sender_width"), &Spout::get_sender_width);
    ClassDB::bind_method(D_METHOD("get_sender_height"), &Spout::get_sender_height);
    ClassDB::bind_method(D_METHOD("get_sender_format"), &Spout::get_sender_format);
    ClassDB::bind_method(D_METHOD("get_sender_fps"), &Spout::get_sender_fps);
    ClassDB::bind_method(D_METHOD("get_sender_frame"), &Spout::get_sender_frame);
    ClassDB::bind_method(D_METHOD("get_sender_handle"), &Spout::get_sender_handle);
    ClassDB::bind_method(D_METHOD("get_sender_cpu"), &Spout::get_sender_cpu);
    ClassDB::bind_method(D_METHOD("get_sender_gldx"), &Spout::get_sender_gldx);
    ClassDB::bind_method(D_METHOD("select_sender"), &Spout::select_sender);

    ClassDB::bind_method(D_METHOD("get_sender_count"), &Spout::get_sender_count);
    ClassDB::bind_method(D_METHOD("get_sender", "index"), &Spout::get_sender, DEFVAL(0));

    BIND_ENUM_CONSTANT(FORMAT_RGBA);
    BIND_ENUM_CONSTANT(FORMAT_BGRA);
    BIND_ENUM_CONSTANT(FORMAT_BGRA_EXT);
}

Spout::Spout() {
    lib = GetSpout();
}

Spout::~Spout() {
    lib->Release();
}
