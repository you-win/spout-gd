#include "spout_gd.h"

void SpoutGD::send(Ref<Image> image) {
    PoolVector<uint8_t>::Read data = image->get_data().read();
    const unsigned char *p = (const unsigned char *)data.ptr();
    sender->SendImage(p, sender_width, sender_height);
}

void SpoutGD::set_size(int width, int height) {
    sender_width = width;
    sender_height = height;
}

void SpoutGD::set_rot(int x, int y) {
    rot_x = x;
    rot_y = y;
}

double_t SpoutGD::get_fps() {
    return sender->GetFps();
}

double_t SpoutGD::get_frame() {
    return (double_t)sender->GetFrame();
}

void SpoutGD::set_frame_count(bool enabled) {
    sender->SetFrameCount(enabled);
}

bool SpoutGD::is_frame_count_enabled() {
    return sender->IsFrameCountEnabled();
}

void SpoutGD::_bind_methods() {
    ClassDB::bind_method(D_METHOD("send", "image"), &SpoutGD::send);
    ClassDB::bind_method(D_METHOD("set_size", "width", "height"), &SpoutGD::set_size);
    ClassDB::bind_method(D_METHOD("set_rot", "x", "y"), &SpoutGD::set_rot);
    ClassDB::bind_method(D_METHOD("get_fps"), &SpoutGD::get_fps);
    ClassDB::bind_method(D_METHOD("get_frame"), &SpoutGD::get_frame);
    ClassDB::bind_method(D_METHOD("set_frame_count", "enabled"), &SpoutGD::set_frame_count);
    ClassDB::bind_method(D_METHOD("is_frame_count_enabled"), &SpoutGD::is_frame_count_enabled);
}

SpoutGD::SpoutGD() {
    strcpy_s(sender_name, 256, "SpoutLibrary Sender - GD");
    
    sender = GetSpout();
    sender->EnableSpoutLog();
    sender->SetSpoutLogLevel(SPOUT_LOG_VERBOSE);

    rot_x = 0.0f;
    rot_y = 0.0f;
    sender_width = 100.0f;
    sender_height = 100.0f;
    sender->SetSenderName(sender_name);
}

SpoutGD::~SpoutGD() {
    sender->ReleaseSender();
    sender->Release();
}
