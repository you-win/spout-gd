#ifndef SPOUT_GD_H
#define SPOUT_GD_H

#include "core/reference.h"
#include "scene/resources/texture.h"

#include "thirdparty/SPOUTSDK/SpoutLibrary/Binaries/x64/SpoutLibrary.h"

class SpoutGD : public Reference {
    GDCLASS(SpoutGD, Reference);

private:
    SPOUTLIBRARY *sender;
    char sender_name[256];
    uint32_t sender_width;
    uint32_t sender_height;

    float rot_x, rot_y;

protected:
    static void _bind_methods();

public:
    void send(Ref<Image> image);
    
    void set_size(int width, int height);
    void set_rot(int x, int y);

    double_t get_fps();
    double_t get_frame();

    void set_frame_count(bool enabled);
    bool is_frame_count_enabled();

    SpoutGD();
    ~SpoutGD();
};

#endif
