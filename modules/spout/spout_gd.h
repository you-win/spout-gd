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

	Vector2 send_size;

private:
	SPOUTLIBRARY *spout_lib;

protected:
	static void _bind_methods();

public:
	enum SpoutLogLevel {
		SPOUT_LOG_SILENT = LibLogLevel::SPOUT_LOG_SILENT,
		SPOUT_LOG_VERBOSE = LibLogLevel::SPOUT_LOG_VERBOSE,
		SPOUT_LOG_NOTICE = LibLogLevel::SPOUT_LOG_NOTICE,
		SPOUT_LOG_WARNING = LibLogLevel::SPOUT_LOG_WARNING,
		SPOUT_LOG_ERROR = LibLogLevel::SPOUT_LOG_ERROR,
		SPOUT_LOG_FATAL = LibLogLevel::SPOUT_LOG_FATAL
	};

#pragma region Sender

	void sender_set_sender_name(const String &sender_name = String());
	void sender_release_sender(int p_msec = 0);
	Error sender_send_fbo(const uint32_t p_fbo_id);
	Error sender_send_texture(const uint32_t p_godot_texture_id, const uint32_t p_shared_texture_id, const bool p_invert = true, const int p_host_fbo = 0);
	Error sender_send_image(Ref<Image> p_image, const bool p_invert = false, const int p_gl_format = GL_RGBA);
	String sender_get_name();
	double_t sender_get_fps();
	double_t sender_get_frame();
	bool sender_get_cpu();
	bool sender_get_gldx();

#pragma endregion

#pragma region Receiver

	void receiver_set_receiver_name(const String &p_sender_name = String());
	void receiver_release_receiver();
	Error receiver_receive_texture(int p_godot_texture_id = 0, int p_shared_texture_id = 0, bool p_invert = false, int p_host_fbo = 0);
	Error receiver_receive_image(Ref<Image> p_image, const bool p_invert = false, const int p_host_fbo = 0, const int p_gl_format = GL_RGBA);
	bool receiver_is_updated();
	bool receiver_is_connected();
	bool receiver_is_frame_new();
	String receiver_get_sender_name();
	Vector2 receiver_get_sender_size();
	double_t receiver_get_sender_fps();
	double_t receiver_get_sender_frame();
	bool receiver_get_sender_cpu();
	bool receiver_get_sender_gldx();

#pragma endregion

#pragma region Frame counting

	void set_frame_count(bool p_enabled);
	void disable_frame_count();
	bool is_frame_count_enabled();
	void hold_fps(const int p_fps);
	void set_frame_sync(const String &p_sender_name);
	// NOTE WaitFrameSync is not implemented since i think that might require actual waiting?

#pragma endregion

#pragma region Data sharing

	Error write_memory_buffer(const String &p_sender_name, const PoolByteArray p_data, const int p_length);
	PoolByteArray read_memory_buffer(const String &p_sender_name, const int p_max_length);
	Error create_memory_buffer(const String &p_name, const int p_length);
	Error delete_memory_buffer();
	int get_memory_buffer_size(const String &p_name);

#pragma endregion

#pragma region Log utilites

	void enable_spout_log();
	void enable_spout_log_file(const String p_filename, const bool p_append = false);
	String get_spout_log();
	void show_spout_logs();
	void disable_spout_log();
	void set_spout_log_level(SpoutLogLevel p_level);
	// NOTE The library accepts varargs for these but that's not easy to expose to GDScript
	void spout_log(const String p_format);
	void spout_log_verbose(const String p_format);
	void spout_log_notice(const String p_format);
	void spout_log_warning(const String p_format);
	void spout_log_error(const String p_format);
	void spout_log_fatal(const String p_format);

#pragma endregion

#pragma region OpenGL shared texture

	bool is_initialized();
	bool bind_shared_texture();
	bool unbind_shared_texture();
	uint32_t get_shared_texture_id();

#pragma endregion

#pragma region Sender names

	int get_sender_count();
	String get_sender(const int p_index, const int p_max_size = 256);
	bool find_sender_name(const String &p_sender_name);
	String get_active_sender();
	Error set_active_sender(const String &p_sender_name);

#pragma endregion

#pragma region Information

	String get_host_path(const String p_sender_name, const int p_max_chars);
	int get_vertical_sync();
	Error set_vertical_sync(const bool p_sync = true);
	int get_spout_version();

#pragma endregion

#pragma region Graphics compatibility

	bool get_auto_share();
	void set_auto_share(const bool p_is_auto = true);
	bool is_gldx_ready();

#pragma endregion

#pragma region Graphics adapter

	int get_num_adapters();
	String get_adapter_name(const int p_index, const int p_max_chars = 256);
	String adapter_name();
	int get_adapter();
	Error set_adapter(const int p_index = 0);

#pragma endregion

	Array get_senders();

	Vector2 get_send_size();
	void set_send_size(Vector2 p_size);

	Spout();
	~Spout();
};

VARIANT_ENUM_CAST(Spout::SpoutLogLevel);

#endif
