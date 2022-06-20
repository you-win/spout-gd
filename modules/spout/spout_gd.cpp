#include "spout_gd.h"

#pragma region Sender

void Spout::sender_set_sender_name(const String &sender_name) {
	spout_lib->SetSenderName(sender_name.utf8().get_data());
}

void Spout::sender_release_sender(int msec) {
	spout_lib->ReleaseSender(static_cast<DWORD>(msec));
}

Error Spout::sender_send_fbo(const uint32_t p_fbo_id) {
	return spout_lib->SendFbo(p_fbo_id, static_cast<unsigned int>(send_size.x), static_cast<unsigned int>(send_size.y)) ? Error::OK : Error::FAILED;
}

Error Spout::sender_send_texture(const uint32_t p_godot_texture_id, const uint32_t p_shared_texture_id, const bool p_invert, const int p_host_fbo) {
	return spout_lib->SendTexture(
				   static_cast<GLuint>(p_godot_texture_id),
				   static_cast<GLuint>(p_shared_texture_id),
				   static_cast<unsigned int>(send_size.x),
				   static_cast<unsigned int>(send_size.y),
				   p_invert,
				   static_cast<GLuint>(p_host_fbo))
			? Error::OK
			: Error::FAILED;
}

Error Spout::sender_send_image(Ref<Image> p_image, const bool p_invert, const int p_gl_format) {
	PoolVector<uint8_t>::Read data = p_image->get_data().read();
	const unsigned char *p = (const unsigned char *)data.ptr();
	return spout_lib->SendImage(p, static_cast<unsigned int>(send_size.x), static_cast<unsigned int>(send_size.y), p_gl_format, p_invert) ? Error::OK : Error::FAILED;
}

String Spout::sender_get_name() {
	const char *sender_name = spout_lib->GetName();

	return sender_name ? String(sender_name) : String();
}

double_t Spout::sender_get_fps() {
	return spout_lib->GetFps();
}

double_t Spout::sender_get_frame() {
	return static_cast<double_t>(spout_lib->GetFrame());
}

bool Spout::sender_get_cpu() {
	return spout_lib->GetCPU();
}

bool Spout::sender_get_gldx() {
	return spout_lib->GetGLDX();
}

#pragma endregion

#pragma region Receiver

void Spout::receiver_set_receiver_name(const String &p_sender_name) {
	spout_lib->SetReceiverName(p_sender_name.empty() ? nullptr : p_sender_name.utf8().get_data());
}

void Spout::receiver_release_receiver() {
	spout_lib->ReleaseReceiver();
}

Error Spout::receiver_receive_texture(int p_godot_texture_id, int p_shared_texture_id, bool p_invert, int p_host_fbo) {
	return spout_lib->ReceiveTexture(static_cast<GLuint>(p_shared_texture_id), static_cast<GLuint>(p_godot_texture_id), p_invert, static_cast<GLuint>(p_host_fbo)) ? Error::OK : Error::FAILED;
}

Error Spout::receiver_receive_image(Ref<Image> p_image, const bool p_invert, const int p_host_fbo, const int p_gl_format) {
	PoolByteArray::Read data = p_image->get_data().read();
	unsigned char *p = (unsigned char *)data.ptr();

	return spout_lib->ReceiveImage(p, p_gl_format, p_invert, p_host_fbo) ? Error::OK : Error::FAILED;
}

bool Spout::receiver_is_updated() {
	return spout_lib->IsUpdated();
}

bool Spout::receiver_is_connected() {
	return spout_lib->IsConnected();
}

bool Spout::receiver_is_frame_new() {
	return spout_lib->IsFrameNew();
}

String Spout::receiver_get_sender_name() {
	const char *sender_name = spout_lib->GetSenderName();

	return sender_name ? String(sender_name) : String();
}

Vector2 Spout::receiver_get_sender_size() {
	return Vector2(static_cast<real_t>(spout_lib->GetSenderWidth()), static_cast<real_t>(spout_lib->GetSenderHeight()));
}

double_t Spout::receiver_get_sender_fps() {
	return spout_lib->GetSenderFps();
}

double_t Spout::receiver_get_sender_frame() {
	return static_cast<double_t>(spout_lib->GetSenderFrame());
}

bool Spout::receiver_get_sender_cpu() {
	return spout_lib->GetSenderFrame();
}

bool Spout::receiver_get_sender_gldx() {
	return spout_lib->GetSenderGLDX();
}

#pragma endregion

#pragma region Frame counting

void Spout::set_frame_count(bool p_enabled) {
	spout_lib->SetFrameCount(p_enabled);
}

void Spout::disable_frame_count() {
	spout_lib->DisableFrameCount();
}

bool Spout::is_frame_count_enabled() {
	return spout_lib->IsFrameCountEnabled();
}

void Spout::hold_fps(const int p_fps) {
	spout_lib->HoldFps(p_fps);
}

void Spout::set_frame_sync(const String &p_sender_name) {
	spout_lib->SetFrameSync(p_sender_name.utf8().get_data());
}

#pragma endregion

#pragma region Data sharing

Error Spout::write_memory_buffer(const String &p_sender_name, const PoolByteArray p_data, const int p_length) {
	const uint8_t *data = p_data.read().ptr();

	return spout_lib->WriteMemoryBuffer(
				   p_sender_name.utf8().get_data(),
				   (char *)data, // TODO hmmm
				   p_length)
			? Error::OK
			: Error::FAILED;
}

PoolByteArray Spout::read_memory_buffer(const String &p_sender_name, const int p_max_length) {
	// TODO dynamically size this
	char data[256];

	bool success = spout_lib->ReadMemoryBuffer(p_sender_name.utf8().get_data(), data, p_max_length);

	PoolByteArray read_data = PoolByteArray();

	if (!success) {
		return read_data;
	}

	PoolByteArray::Write write = read_data.write();
	for (int i = 0; i < 256; i++) {
		write[i] = data[i];
	}

	return read_data;
}

Error Spout::create_memory_buffer(const String &p_name, const int p_length) {
	return spout_lib->CreateMemoryBuffer(p_name.utf8().get_data(), p_length) ? Error::OK : Error::FAILED;
}

Error Spout::delete_memory_buffer() {
	return spout_lib->DeleteMemoryBuffer() ? Error::OK : Error::FAILED;
}

int Spout::get_memory_buffer_size(const String &p_name) {
	return spout_lib->GetMemoryBufferSize(p_name.utf8().get_data());
}

#pragma endregion

#pragma region Log utilites

void Spout::enable_spout_log() {
	spout_lib->EnableSpoutLog();
}

void Spout::enable_spout_log_file(const String p_filename, const bool p_append) {
	spout_lib->EnableSpoutLogFile(p_filename.utf8().get_data(), p_append);
}

String Spout::get_spout_log() {
	std::string log = spout_lib->GetSpoutLog();

	return String(log.c_str());
}

void Spout::show_spout_logs() {
	spout_lib->ShowSpoutLogs();
}

void Spout::disable_spout_log() {
	spout_lib->DisableSpoutLog();
}

void Spout::set_spout_log_level(SpoutLogLevel p_level) {
	spout_lib->SetSpoutLogLevel(static_cast<LibLogLevel>(p_level));
}

void Spout::spout_log(const String p_format) {
	spout_lib->SpoutLog(p_format.utf8().get_data());
}

void Spout::spout_log_verbose(const String p_format) {
	spout_lib->SpoutLogVerbose(p_format.utf8().get_data());
}

void Spout::spout_log_notice(const String p_format) {
	spout_lib->SpoutLogNotice(p_format.utf8().get_data());
}

void Spout::spout_log_warning(const String p_format) {
	spout_lib->SpoutLogWarning(p_format.utf8().get_data());
}

void Spout::spout_log_error(const String p_format) {
	spout_lib->SpoutLogError(p_format.utf8().get_data());
}

void Spout::spout_log_fatal(const String p_format) {
	spout_lib->SpoutLogFatal(p_format.utf8().get_data());
}

#pragma endregion

#pragma region OpenGL shared texture

bool Spout::is_initialized() {
	return spout_lib->IsInitialized();
}

bool Spout::bind_shared_texture() {
	return spout_lib->BindSharedTexture();
}

bool Spout::unbind_shared_texture() {
	return spout_lib->UnBindSharedTexture();
}

uint32_t Spout::get_shared_texture_id() {
	return static_cast<int>(spout_lib->GetSharedTextureID());
}

#pragma endregion

#pragma region Sender names

int Spout::get_sender_count() {
	return spout_lib->GetSenderCount();
}

String Spout::get_sender(const int p_index, const int p_max_size) {
	// TODO set this dynamically
	char sender_name[256];

	spout_lib->GetSender(p_index, sender_name, p_max_size);

	return String(sender_name);
}

bool Spout::find_sender_name(const String &p_sender_name) {
	return spout_lib->FindSenderName(p_sender_name.utf8().get_data());
}

String Spout::get_active_sender() {
	char sender_name[256];

	return spout_lib->GetActiveSender(sender_name) ? String(sender_name) : String();
}

Error Spout::set_active_sender(const String &p_sender_name) {
	return spout_lib->SetActiveSender(p_sender_name.utf8().get_data()) ? Error::OK : Error::FAILED;
}

#pragma endregion

#pragma region Information

String Spout::get_host_path(const String p_sender_name, const int p_max_chars) {
	// TODO create the size dynamically
	char hostpath[256];

	spout_lib->GetHostPath(p_sender_name.utf8().get_data(), hostpath, p_max_chars);

	return String(hostpath);
}

int Spout::get_vertical_sync() {
	return spout_lib->GetVerticalSync();
}

Error Spout::set_vertical_sync(const bool p_sync) {
	return spout_lib->SetVerticalSync(p_sync) ? Error::OK : Error::FAILED;
}

int Spout::get_spout_version() {
	return spout_lib->GetSpoutVersion();
}

#pragma endregion

#pragma region Graphics compatibility

bool Spout::get_auto_share() {
	return spout_lib->GetAutoShare();
}

void Spout::set_auto_share(const bool p_is_auto) {
	spout_lib->SetAutoShare(p_is_auto);
}

bool Spout::is_gldx_ready() {
	return spout_lib->IsGLDXready();
}

#pragma endregion

#pragma region Graphics adapter

int Spout::get_num_adapters() {
	return spout_lib->GetNumAdapters();
}

String Spout::get_adapter_name(const int p_index, const int p_max_chars) {
	// TODO create size dynamically
	char adapter_name[256];

	spout_lib->GetAdapterName(p_index, adapter_name, p_max_chars);

	return String(adapter_name);
}

String Spout::adapter_name() {
	return String(spout_lib->AdapterName());
}

int Spout::get_adapter() {
	return spout_lib->GetAdapter();
}

Error Spout::set_adapter(const int p_index) {
	return spout_lib->SetAdapter(p_index) ? Error::OK : Error::FAILED;
}

#pragma endregion

Array Spout::get_senders() {
	Array arr;

	int count = get_sender_count();
	arr.resize(count);

	for (int i = 0; i < count; i++) {
		arr.append(get_sender(i));
	}

	return arr;
}

Vector2 Spout::get_send_size() {
	return send_size;
}

void Spout::set_send_size(Vector2 p_size) {
	send_size = p_size;
}

void Spout::_bind_methods() {
	ClassDB::bind_method(D_METHOD("sender_set_sender_name", "sender_name"), &Spout::sender_set_sender_name, DEFVAL(""));
	ClassDB::bind_method(D_METHOD("sender_release_sender", "msec"), &Spout::sender_release_sender, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("sender_send_fbo", "fbo_id"), &Spout::sender_send_fbo);
	ClassDB::bind_method(D_METHOD("sender_send_texture", "godot_texture_id", "shared_texture_id", "invert", "host_fbo"), &Spout::sender_send_texture, DEFVAL(0), DEFVAL(0), DEFVAL(false), DEFVAL(0));
	ClassDB::bind_method(D_METHOD("sender_send_image", "image", "invert", "gl_format"), &Spout::sender_send_image, DEFVAL(false), DEFVAL(GL_RGBA));
	ClassDB::bind_method(D_METHOD("sender_get_name"), &Spout::sender_get_name);
	ClassDB::bind_method(D_METHOD("sender_get_fps"), &Spout::sender_get_fps);
	ClassDB::bind_method(D_METHOD("sender_get_frame"), &Spout::sender_get_frame);
	ClassDB::bind_method(D_METHOD("sender_get_cpu"), &Spout::sender_get_cpu);
	ClassDB::bind_method(D_METHOD("sender_get_gldx"), &Spout::sender_get_gldx);

	ClassDB::bind_method(D_METHOD("receiver_set_receiver_name", "sender_name"), &Spout::receiver_set_receiver_name, DEFVAL(""));
	ClassDB::bind_method(D_METHOD("receiver_release_receiver"), &Spout::receiver_release_receiver);
	ClassDB::bind_method(D_METHOD("receiver_receive_texture", "godot_texture_id", "shared_texture_id", "invert", "host_fbo"), &Spout::receiver_receive_texture, DEFVAL(false), DEFVAL(0));
	ClassDB::bind_method(D_METHOD("receiver_receive_image", "image", "invert", "host_fbo", "gl_format"), &Spout::receiver_receive_image, DEFVAL(false), DEFVAL(0), DEFVAL(GL_RGBA));
	ClassDB::bind_method(D_METHOD("receiver_is_updated"), &Spout::receiver_is_updated);
	ClassDB::bind_method(D_METHOD("receiver_is_connected"), &Spout::receiver_is_connected);
	ClassDB::bind_method(D_METHOD("receiver_is_frame_new"), &Spout::receiver_is_frame_new);
	ClassDB::bind_method(D_METHOD("receiver_get_sender_name"), &Spout::receiver_get_sender_name);
	ClassDB::bind_method(D_METHOD("receiver_get_sender_size"), &Spout::receiver_get_sender_size);
	ClassDB::bind_method(D_METHOD("receiver_get_sender_fps"), &Spout::receiver_get_sender_fps);
	ClassDB::bind_method(D_METHOD("receiver_get_sender_frame"), &Spout::receiver_get_sender_frame);
	ClassDB::bind_method(D_METHOD("receiver_get_sender_cpu"), &Spout::receiver_get_sender_cpu);
	ClassDB::bind_method(D_METHOD("receiver_get_sender_gldx"), &Spout::receiver_get_sender_gldx);

	ClassDB::bind_method(D_METHOD("set_frame_count", "enabled"), &Spout::set_frame_count);
	ClassDB::bind_method(D_METHOD("disable_frame_count"), &Spout::disable_frame_count);
	ClassDB::bind_method(D_METHOD("is_frame_count_enabled"), &Spout::is_frame_count_enabled);
	ClassDB::bind_method(D_METHOD("hold_fps", "fps"), &Spout::hold_fps);
	ClassDB::bind_method(D_METHOD("set_frame_sync", "sender_name"), &Spout::set_frame_sync);

	ClassDB::bind_method(D_METHOD("write_memory_buffer", "sender_name", "data", "length"), &Spout::write_memory_buffer);
	ClassDB::bind_method(D_METHOD("read_memory_buffer", "sender_name", "max_length"), &Spout::read_memory_buffer);
	ClassDB::bind_method(D_METHOD("create_memory_buffer", "name", "length"), &Spout::create_memory_buffer);
	ClassDB::bind_method(D_METHOD("delete_memory_buffer"), &Spout::delete_memory_buffer);
	ClassDB::bind_method(D_METHOD("get_memory_buffer_size", "name"), &Spout::get_memory_buffer_size);

	ClassDB::bind_method(D_METHOD("enable_spout_log"), &Spout::enable_spout_log);
	ClassDB::bind_method(D_METHOD("enable_spout_log_file", "filename", "append"), &Spout::enable_spout_log_file, DEFVAL(false));
	ClassDB::bind_method(D_METHOD("get_spout_logs"), &Spout::get_spout_log);
	ClassDB::bind_method(D_METHOD("show_spout_logs"), &Spout::show_spout_logs);
	ClassDB::bind_method(D_METHOD("disable_spout_log"), &Spout::disable_spout_log);
	ClassDB::bind_method(D_METHOD("set_spout_log_level"), &Spout::set_spout_log_level);
	ClassDB::bind_method(D_METHOD("spout_log", "format"), &Spout::spout_log);
	ClassDB::bind_method(D_METHOD("spout_log_verbose", "format"), &Spout::spout_log_verbose);
	ClassDB::bind_method(D_METHOD("spout_log_notice", "format"), &Spout::spout_log_notice);
	ClassDB::bind_method(D_METHOD("spout_log_warning", "format"), &Spout::spout_log_warning);
	ClassDB::bind_method(D_METHOD("spout_log_error", "format"), &Spout::spout_log_error);
	ClassDB::bind_method(D_METHOD("spout_log_fatal", "format"), &Spout::spout_log_fatal);

	ClassDB::bind_method(D_METHOD("is_initialized"), &Spout::is_initialized);
	ClassDB::bind_method(D_METHOD("bind_shared_texture"), &Spout::bind_shared_texture);
	ClassDB::bind_method(D_METHOD("unbind_shared_texture"), &Spout::unbind_shared_texture);
	ClassDB::bind_method(D_METHOD("get_shared_texture_id"), &Spout::get_shared_texture_id);

	ClassDB::bind_method(D_METHOD("get_sender_count"), &Spout::get_sender_count);
	ClassDB::bind_method(D_METHOD("get_sender", "index", "max_size"), &Spout::get_sender, DEFVAL(256));
	ClassDB::bind_method(D_METHOD("find_sender_name", "sender_name"), &Spout::find_sender_name);
	ClassDB::bind_method(D_METHOD("get_active_sender"), &Spout::get_active_sender);
	ClassDB::bind_method(D_METHOD("set_active_sender", "sender_name"), &Spout::set_active_sender);

	ClassDB::bind_method(D_METHOD("get_host_path", "sender_name", "max_chars"), &Spout::get_host_path);
	ClassDB::bind_method(D_METHOD("get_vertical_sync"), &Spout::get_vertical_sync);
	ClassDB::bind_method(D_METHOD("set_vertical_sync", "sync"), &Spout::set_vertical_sync, DEFVAL(true));
	ClassDB::bind_method(D_METHOD("get_spout_version"), &Spout::get_spout_version);

	ClassDB::bind_method(D_METHOD("get_auto_share"), &Spout::get_auto_share);
	ClassDB::bind_method(D_METHOD("set_auto_share", "is_auto"), &Spout::set_auto_share, DEFVAL(true));
	ClassDB::bind_method(D_METHOD("is_gldx_ready"), &Spout::is_gldx_ready);

	ClassDB::bind_method(D_METHOD("get_num_adapters"), &Spout::get_num_adapters);
	ClassDB::bind_method(D_METHOD("get_adapter_name", "index", "max_chars"), &Spout::get_adapter_name, DEFVAL(256));
	ClassDB::bind_method(D_METHOD("adapter_name"), &Spout::adapter_name);
	ClassDB::bind_method(D_METHOD("get_adapter"), &Spout::get_adapter);
	ClassDB::bind_method(D_METHOD("set_adapter", "index"), &Spout::set_adapter, DEFVAL(0));

	ClassDB::bind_method(D_METHOD("get_senders"), &Spout::get_senders);
	ClassDB::bind_method(D_METHOD("get_send_size"), &Spout::get_send_size);
	ClassDB::bind_method(D_METHOD("set_send_size", "size"), &Spout::set_send_size);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "send_size"), "set_send_size", "get_send_size");

	BIND_CONSTANT(SPOUT_LOG_SILENT);
	BIND_CONSTANT(SPOUT_LOG_VERBOSE);
	BIND_CONSTANT(SPOUT_LOG_NOTICE);
	BIND_CONSTANT(SPOUT_LOG_WARNING);
	BIND_CONSTANT(SPOUT_LOG_ERROR);
	BIND_CONSTANT(SPOUT_LOG_FATAL);
}

Spout::Spout() {
	spout_lib = GetSpout();
	enable_spout_log();
	set_spout_log_level(SpoutLogLevel::SPOUT_LOG_VERBOSE);

	send_size = Vector2(100.0f, 100.0f);
	sender_set_sender_name(String("Godot_Spout"));
}

Spout::~Spout() {
	sender_release_sender();
	spout_lib->Release();
}
