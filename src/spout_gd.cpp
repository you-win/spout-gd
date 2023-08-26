#include "spout_gd.h"

void Spout::set_sender_name(const String &p_sender_name) {
    lib->SetSenderName(p_sender_name.utf8().get_data());
}

Spout::Spout() {
    lib = GetSpout();
}

Spout::~Spout() {
    lib->Release();
}
