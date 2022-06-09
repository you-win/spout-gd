#ifndef SPOUT_GD_H
#define SPOUT_GD_H

#include "core/array.h"
#include "core/error_list.h"
#include "core/math/vector2.h"
#include "core/reference.h"
#include "scene/resources/texture.h"

#include "thirdparty/SPOUTSDK/SpoutLibrary/Binaries/x64/SpoutLibrary.h"

class Spout : public Reference {
	GDCLASS(Spout, Reference);

private:
	SPOUTLIBRARY *spout_lib;
	char sender_name[256];
	uint32_t sender_width;
	uint32_t sender_height;

protected:
	static void _bind_methods();

public:
	// Sender names
	Array get_senders();
	String get_active_sender();
	Error set_active_sender(const String p_sender_name);

	// Sender
	Error send_image(Ref<Image> p_image);
	Error send_fbo(const uint32_t p_fbo_id);
	Error send_texture(const uint32_t p_shared_texture_id, const uint32_t p_godot_texture_id);

	// Receiver
	void set_receiver_name(const String p_sender_name);
	void release_receiver();
	Error receive_texture(int p_shared_texture_id, int p_godot_texture_id, bool p_invert, int p_host_fbo);
	bool is_updated();
	bool is_connected();
	bool is_frame_new();
	String get_sender_name();
	Vector2 get_sender_size();
	double_t get_sender_fps();
	double_t get_sender_frame();
	bool get_sender_cpu();
	bool get_sender_gldx();

	// OpenGL shared texture
	bool is_initialized();
	bool bind_shared_texture();
	bool unbind_shared_texture();
	uint32_t get_shared_texture_id();

	void set_size(int p_width, int p_height);

	double_t get_fps();
	double_t get_frame();
	bool get_cpu();
	bool get_gldx();

	void set_frame_count(bool p_enabled);
	bool is_frame_count_enabled();

	Spout();
	~Spout();
};

#endif
