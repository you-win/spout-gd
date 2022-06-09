#include "spout_gd.h"

Array Spout::get_senders() {
	Array arr;

	int count = spout_lib->GetSenderCount();
	arr.resize(count);

	for (int i = 0; i < count; i++) {
		char sender_name[256];

		spout_lib->GetSender(i, sender_name);

		arr.append(String(sender_name));
	}

	return arr;
}

String Spout::get_active_sender() {
	char sender_name[256];

	return spout_lib->GetActiveSender(sender_name) ? String(sender_name) : String();
}

Error Spout::set_active_sender(const String p_sender_name) {
	return spout_lib->SetActiveSender(p_sender_name.utf8().get_data()) ? Error::OK : Error::FAILED;
}

Error Spout::send_image(Ref<Image> p_image) {
	PoolVector<uint8_t>::Read data = p_image->get_data().read();
	const unsigned char *p = (const unsigned char *)data.ptr();
	return spout_lib->SendImage(p, sender_width, sender_height) ? Error::OK : Error::FAILED;
}

Error Spout::send_fbo(const uint32_t p_fbo_id) {
	return spout_lib->SendFbo(p_fbo_id, sender_width, sender_height) ? Error::OK : Error::FAILED;
}

Error Spout::send_texture(const uint32_t p_shared_texture_id, const uint32_t p_godot_texture_id) {
	return spout_lib->SendTexture(p_shared_texture_id, p_godot_texture_id, sender_width, sender_height) ? Error::FAILED : Error::OK;
}

void Spout::set_receiver_name(const String p_sender_name) {
	spout_lib->SetReceiverName(p_sender_name.empty() ? nullptr : p_sender_name.utf8().get_data());
}

void Spout::release_receiver() {
	spout_lib->ReleaseReceiver();
}

Error Spout::receive_texture(int p_shared_texture_id, int p_godot_texture_id, bool p_invert, int p_host_fbo) {
	return spout_lib->ReceiveTexture((uint32_t)p_shared_texture_id, (uint32_t)p_godot_texture_id, p_invert, p_host_fbo) ? Error::OK : Error::FAILED;
}

bool Spout::is_updated() {
	return spout_lib->IsUpdated();
}

bool Spout::is_connected() {
	return spout_lib->IsConnected();
}

bool Spout::is_frame_new() {
	return spout_lib->IsFrameNew();
}

String Spout::get_sender_name() {
	const char *sender_name = spout_lib->GetSenderName();

	return sender_name ? String(sender_name) : String();
}

Vector2 Spout::get_sender_size() {
	return Vector2((real_t)spout_lib->GetSenderWidth(), (real_t)spout_lib->GetSenderHeight());
}

double_t Spout::get_sender_fps() {
	return spout_lib->GetSenderFps();
}

double_t Spout::get_sender_frame() {
	return (double_t)spout_lib->GetSenderFrame();
}

bool Spout::get_sender_cpu() {
	return spout_lib->GetSenderFrame();
}

bool Spout::get_sender_gldx() {
	return spout_lib->GetSenderGLDX();
}

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
	return (int)spout_lib->GetSharedTextureID();
}

void Spout::set_size(int p_width, int p_height) {
	sender_width = p_width;
	sender_height = p_height;
}

double_t Spout::get_fps() {
	return spout_lib->GetFps();
}

double_t Spout::get_frame() {
	return (double_t)spout_lib->GetFrame();
}

bool Spout::get_cpu() {
	return spout_lib->GetCPU();
}

bool Spout::get_gldx() {
	return spout_lib->GetGLDX();
}

void Spout::set_frame_count(bool p_enabled) {
	spout_lib->SetFrameCount(p_enabled);
}

bool Spout::is_frame_count_enabled() {
	return spout_lib->IsFrameCountEnabled();
}

void Spout::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_senders"), &Spout::get_senders);
	ClassDB::bind_method(D_METHOD("get_active_sender"), &Spout::get_active_sender);
	ClassDB::bind_method(D_METHOD("set_active_sender"), &Spout::set_active_sender);

	ClassDB::bind_method(D_METHOD("send_image", "image"), &Spout::send_image);
	ClassDB::bind_method(D_METHOD("send_fbo", "fbo_id"), &Spout::send_fbo);
	ClassDB::bind_method(D_METHOD("send_texture", "shared_texture_id", "godot_texture_id"), &Spout::send_texture);

	// TODO add receiver methods

	ClassDB::bind_method(D_METHOD("is_initialized"), &Spout::is_initialized);
	ClassDB::bind_method(D_METHOD("bind_shared_texture"), &Spout::bind_shared_texture);
	ClassDB::bind_method(D_METHOD("unbind_shared_texture"), &Spout::unbind_shared_texture);
	ClassDB::bind_method(D_METHOD("get_shared_texture_id"), &Spout::get_shared_texture_id);

	ClassDB::bind_method(D_METHOD("set_size", "width", "height"), &Spout::set_size);

	ClassDB::bind_method(D_METHOD("get_fps"), &Spout::get_fps);
	ClassDB::bind_method(D_METHOD("get_frame"), &Spout::get_frame);
	ClassDB::bind_method(D_METHOD("get_cpu"), &Spout::get_cpu);
	ClassDB::bind_method(D_METHOD("get_gldx"), &Spout::get_gldx);

	ClassDB::bind_method(D_METHOD("set_frame_count", "enabled"), &Spout::set_frame_count);
	ClassDB::bind_method(D_METHOD("is_frame_count_enabled"), &Spout::is_frame_count_enabled);
}

Spout::Spout() {
	strcpy_s(sender_name, 256, "SpoutLibrary Sender - GD");

	spout_lib = GetSpout();
	spout_lib->EnableSpoutLog();
	spout_lib->SetSpoutLogLevel(SPOUT_LOG_VERBOSE);

	sender_width = 100.0f;
	sender_height = 100.0f;
	spout_lib->SetSenderName(sender_name);
}

Spout::~Spout() {
	spout_lib->ReleaseSender();
	spout_lib->Release();
}
