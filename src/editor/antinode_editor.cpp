#include "decomp.h"
#include "globals.h"
#include "editor/aieditor_state.h"
#include "editor/aieditor_settings.h"
#include "editor/antinode_editor_private.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edfile.h"
#include "legoapi/ai/game/gameantinode.h"
#include "legoapi/render/core/render.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nuspecial.h"
#include <string.h>

extern "C" {
    extern i32 AIEDITOR_ANTINODES;
    extern i32 aidata_version;
}

extern "C" {

    void antinodeEditorDrawAntinodes(void) {
        i32 solid = aieditorsettings.solid_antinode_display;
        NULISTHDR *list = reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x42e94);
        for (EDANTINODE_s *node = reinterpret_cast<EDANTINODE_s *>(NuLinkedListGetHead(list)); node != nullptr;
             node = reinterpret_cast<EDANTINODE_s *>(NuLinkedListGetNext(
                 reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x42e94), &node->link))) {
            u32 colour = 0x50505050;
            i32 segments = 8;
            if (aieditorsettings.current_mode == AIEDITOR_ANTINODES) {
                if (node == aieditor->mode_selection_42e9c) {
                    segments = 32;
                    if (node == *reinterpret_cast<EDANTINODE_s **>(reinterpret_cast<u8 *>(aieditor) + 0x42ea0))
                        colour = NuSpecialExistsFn(&node->special) ? 0xffff0064 : 0xffff0000;
                    else
                        colour = NuSpecialExistsFn(&node->special) ? 0x80800064 : 0x80800000;
                } else if (node == *reinterpret_cast<EDANTINODE_s **>(reinterpret_cast<u8 *>(aieditor) + 0x42ea0)) {
                    colour = 0xffffffff;
                    segments = 32;
                } else {
                    colour = 0x80640000;
                }
            }
            if (node->type == 0) {
                if (solid)
                    LocaledbitsDrawSolidCircleXY(&node->position, node->radius, node->position.y + node->lower_height,
                                                 node->position.y + node->upper_height, colour, 0, segments);
                else
                    LocaledbitsDrawCircleXY(&node->position, node->radius, colour, 0, segments);
            } else if (node->type == 1) {
                f32 lower = solid ? node->position.y + node->lower_height : node->position.y;
                f32 upper = solid ? node->position.y + node->upper_height : node->position.y;
                LocaledbitsDrawSolidEllipseXY(&node->position, node->base_radius, node->base_height, node->flags, lower,
                                              upper, colour, 0, segments);
            } else if (node->type == 2) {
                NUMTX matrix;
                NuMtxSetTranslation(&matrix, &node->position);
                NuMtxPreRotateY(&matrix, node->flags);
                NUVEC minimum = {-node->base_radius, node->lower_height, -node->base_height};
                NUVEC maximum = {node->base_radius, node->upper_height, node->base_height};
                if (!solid) {
                    minimum.y = 0.0f;
                    maximum.y = 0.0f;
                }
                NuRndrBoundingBox(&minimum, &maximum, &matrix, colour);
            }
        }
    }

    void antinodeEditorSaveData(void) {
        if (aidata_version <= 12)
            return;
        NULISTHDR *list = reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x42e94);
        EDANTINODE_s *head = reinterpret_cast<EDANTINODE_s *>(NuLinkedListGetHead(list));
        if (head == nullptr) {
            EdFileWriteInt(0);
            return;
        }
        i32 count = 0;
        for (EDANTINODE_s *node = head; node != nullptr;
             node = reinterpret_cast<EDANTINODE_s *>(NuLinkedListGetNext(
                 reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x42e94), &node->link)))
            ++count;
        EdFileWriteInt(count);
        for (EDANTINODE_s *node = reinterpret_cast<EDANTINODE_s *>(
                 NuLinkedListGetHead(reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x42e94)));
             node != nullptr;
             node = reinterpret_cast<EDANTINODE_s *>(NuLinkedListGetNext(
                 reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x42e94), &node->link))) {
            if (node->type == 2)
                node->radius = NuFsqrt(node->base_radius * node->base_radius + node->base_height * node->base_height);
            else if (node->type == 1)
                node->radius = node->base_radius > node->base_height ? node->base_radius : node->base_height;
            EdFileWriteFloat(node->position.x);
            EdFileWriteFloat(node->position.y);
            EdFileWriteFloat(node->position.z);
            EdFileWriteFloat(node->radius);
            EdFileWriteFloat(node->position.y + node->lower_height);
            EdFileWriteFloat(node->position.y + node->upper_height);
            if (aidata_version > 14) {
                EdFileWriteInt(node->flags);
                EdFileWriteFloat(node->base_radius);
                EdFileWriteFloat(node->base_height);
                EdFileWriteChar(node->type);
            } else {
                EdFileWriteChar(0);
            }
            EdFileWriteChar(0);
            EdFileWriteChar(0);
            EdFileWriteChar(node->game_flags);
            char *name = NuSpecialExistsFn(&node->special) ? NuSpecialGetName(&node->special) : nullptr;
            if (name == nullptr) {
                EdFileWriteChar(0);
            } else {
                i32 length = strlen(name) + 1;
                EdFileWriteChar(length);
                EdFileWrite(name, length);
                EdFileWriteFloat(node->special_position.x);
                EdFileWriteFloat(node->special_position.y);
                EdFileWriteFloat(node->special_position.z);
                if (aidata_version > 14)
                    EdFileWriteInt(node->rotation_offset);
            }
        }
    }

    void antinodeEditor_UpdateAntiNodesOnPlatforms(void) {
        NULISTHDR *list = reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x42e94);
        NUVEC rotated;
        NUVEC forward = {0.0f, 0.0f, 1.0f};
        for (EDANTINODE_s *node = reinterpret_cast<EDANTINODE_s *>(NuLinkedListGetHead(list)); node != nullptr;
             node = reinterpret_cast<EDANTINODE_s *>(NuLinkedListGetNext(
                 reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x42e94), &node->link))) {
            if (!NuSpecialExistsFn(&node->special))
                continue;
            NUMTX *matrix = NuSpecialGetDrawMtx(&node->special);
            NuVecMtxTransform(&node->position, &node->special_position, matrix);
            NuVecMtxRotate(&rotated, &forward, matrix);
            node->flags = NuAtan2D(rotated.x, rotated.z);
            node->flags = NuAngAdd(node->flags, node->rotation_offset);
        }
    }

} // extern "C"
