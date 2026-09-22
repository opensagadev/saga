#include "decomp.h"
#include "gamelib_util_types.h"
#include "gamelib/util/Utilities.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nuqfnt.h"

#include <stdio.h>

void NetSmallStats::Draw(float x, float y, float width, float height, NetSmallStats::eInfo) const {
    width *= 0.5f;
    ++NuPrimCSPos;
    NuPrimSetCoordinateSystem(NUPRIM_SCALEMODE_NORMALISED);
    NuPrim2DBegin(3, 5, NULL);

    NuRndrPrimSetColour(0x80000080);
    NuRndrPrimPosition(x, y, 0.0f);
    NuRndrPrimSetColour(0x80000080);
    NuRndrPrimPosition(x, y - height, 0.0f);
    NuRndrPrimSetColour(0x80000080);
    x += width;
    NuRndrPrimPosition(x, y - height, 0.0f);

    NuPrim2DEnd();
    NuPrimSetCoordinateSystem(NuPrimCoordSystemStack[--NuPrimCSPos]);

    NuQFntPushCoordinateSystem(NUQFNT_CSMODE_NORMALISED);
    NuQFntPushPrintMode(2);
    NUQFNT *font = system_qfont;
    float font_height = NuQFntHeight(font);
    NuQFntSetColour(font, 0x80808080);
    NuQFntSetScale(font, 0.75f, 0.75f);

    y += font_height;
    NuQFntMove(font, x, y, 0.0f);
    NuQFntPrintU(font, const_cast<char *>(name));

    char text[128];
    y += font_height;
    NuQFntMove(font, x, y, 0.0f);
    sprintf(text, "total b %d : %d", total.values[0], total.values[1]);
    NuQFntPrintU(font, text);

    y += font_height;
    NuQFntMove(font, x, y, 0.0f);
    sprintf(text, "total p%d : %d", total.values[2], total.values[3]);
    NuQFntPrintU(font, text);

    NuQFntPopPrintMode();
    NuQFntPopCoordinateSystem();
}

void NetSample::Reset() {
    values[0] = 0;
    values[1] = 0;
    values[2] = 0;
    values[3] = 0;
}

void NetSample::operator+=(NetSample const &other) {
    values[0] += other.values[0];
    values[1] += other.values[1];
    values[2] += other.values[2];
    values[3] += other.values[3];
}

void NetSample::operator-=(NetSample const &other) {
    values[0] -= other.values[0];
    values[1] -= other.values[1];
    values[2] -= other.values[2];
    values[3] -= other.values[3];
}

void NetSample::Max(NetSample const &other) {
    values[0] = values[0] < other.values[0] ? other.values[0] : values[0];
    values[1] = values[1] < other.values[1] ? other.values[1] : values[1];
    values[2] = values[2] < other.values[2] ? other.values[2] : values[2];
    values[3] = values[3] < other.values[3] ? other.values[3] : values[3];
}

void NetStats::Draw(float x, float y, float width, float height, volatile NetSmallStats::eInfo info) const {
    NetSample visible_maximum;
    for (i32 i = 0; i < 30; ++i) {
        visible_maximum.Max(samples[i]);
    }

    const float graph_width = width * 0.5f;
    const float x_step = graph_width / 30.0f;
    float byte_maximum;
    if (visible_maximum.values[0] <= visible_maximum.values[1]) {
        byte_maximum = static_cast<float>(visible_maximum.values[1]);
    } else {
        byte_maximum = static_cast<float>(visible_maximum.values[0]);
    }
    byte_maximum = 4352.0f < byte_maximum ? byte_maximum : 4352.0f;
    const float byte_scale = height / byte_maximum;
    float packet_maximum;
    if (visible_maximum.values[2] <= visible_maximum.values[3]) {
        packet_maximum = static_cast<float>(visible_maximum.values[3]);
    } else {
        packet_maximum = static_cast<float>(visible_maximum.values[2]);
    }
    const float packet_scale = height / packet_maximum;
    const float graph_end = x + graph_width;

    ++NuPrimCSPos;
    NuPrimSetCoordinateSystem(NUPRIM_SCALEMODE_NORMALISED);
    NuPrim2DBegin(3, 5, NULL);
    NuRndrPrimSetColour(0x80000080);
    NuRndrPrimPosition(x, y, 0.0f);
    NuRndrPrimSetColour(0x80000080);
    const float graph_bottom = y - height;
    NuRndrPrimPosition(x, graph_bottom, 0.0f);
    NuRndrPrimSetColour(0x80000080);
    NuRndrPrimPosition(graph_end, graph_bottom, 0.0f);
    NuPrim2DEnd();

    NuPrim2DBegin(3, 5, NULL);
    NuRndrPrimSetColour(0x80000080);
    const float byte_limit_y = graph_bottom + byte_scale * 4352.0f;
    NuRndrPrimPosition(x, byte_limit_y, 0.0f);
    NuRndrPrimSetColour(0x80000080);
    NuRndrPrimPosition(graph_end, byte_limit_y, 0.0f);
    NuPrim2DEnd();

    NuPrim2DBegin(3, 5, NULL);
    i32 sample = sample_index + 1;
    for (i32 i = 0; i < 30; ++i) {
        if (sample > 29) {
            sample = 0;
        }
        const float sample_x = x + static_cast<float>(i) * x_step;
        float sample_y;
        if (info == static_cast<NetSmallStats::eInfo>(0)) {
            sample_y = graph_bottom + static_cast<float>(samples[sample].values[0]) * byte_scale;
        } else if (info == static_cast<NetSmallStats::eInfo>(1)) {
            sample_y = graph_bottom + static_cast<float>(samples[sample].values[2]) * packet_scale;
        }
        NuRndrPrimSetColour(0x80008000);
        NuRndrPrimPosition(sample_x, sample_y, 0.0f);
        ++sample;
    }
    NuPrim2DEnd();

    NuPrim2DBegin(3, 5, NULL);
    sample = sample_index + 1;
    for (i32 i = 0; i < 30; ++i) {
        if (sample > 29) {
            sample = 0;
        }
        const float sample_x = x + static_cast<float>(i) * x_step;
        float sample_y;
        if (info == static_cast<NetSmallStats::eInfo>(0)) {
            sample_y = graph_bottom + static_cast<float>(samples[sample].values[1]) * byte_scale;
        } else if (info == static_cast<NetSmallStats::eInfo>(1)) {
            sample_y = graph_bottom + static_cast<float>(samples[sample].values[3]) * packet_scale;
        }
        NuRndrPrimSetColour(0x80008080);
        NuRndrPrimPosition(sample_x, sample_y, 0.0f);
        ++sample;
    }
    NuPrim2DEnd();

    NuPrimSetCoordinateSystem(NuPrimCoordSystemStack[--NuPrimCSPos]);

    NuQFntPushCoordinateSystem(NUQFNT_CSMODE_NORMALISED);
    NuQFntPushPrintMode(2);
    NUQFNT *font = system_qfont;
    const float font_height = NuQFntHeight(font);
    NuQFntSetColour(font, 0x80808080);
    NuQFntSetScale(font, 0.75f, 0.75f);

    y += font_height;
    NuQFntMove(font, graph_end, y, 0.0f);
    NuQFntPrintU(font, const_cast<char *>(name));

    char text[128];
    y += font_height;
    NuQFntMove(font, graph_end, y, 0.0f);
    sprintf(text, "bps %d : %d", samples[sample_index].values[0], samples[sample_index].values[1]);
    NuQFntPrintU(font, text);

    y += font_height;
    NuQFntMove(font, graph_end, y, 0.0f);
    sprintf(text, "pps %d : %d", samples[sample_index].values[2], samples[sample_index].values[3]);
    NuQFntPrintU(font, text);

    y += font_height;
    NuQFntMove(font, graph_end, y, 0.0f);
    sprintf(text, "max bps %d : %d", maximum.values[0], maximum.values[1]);
    NuQFntPrintU(font, text);

    y += font_height;
    NuQFntMove(font, graph_end, y, 0.0f);
    sprintf(text, "max pps %d : %d", maximum.values[2], maximum.values[3]);
    NuQFntPrintU(font, text);

    NuQFntPopPrintMode();
    NuQFntPopCoordinateSystem();
}

void NetStats::Update() {
    if (UtilGetFrameStartTime() - sample_time > 1000) {
        i32 next_sample = sample_index + 1;
        if (next_sample >= 30) {
            next_sample = 0;
        }
        samples[next_sample] = total;
        samples[next_sample] -= previous;
        maximum.Max(samples[next_sample]);
        sample_index = next_sample;
        previous = total;
        sample_time = UtilGetFrameStartTime();
    }
}
