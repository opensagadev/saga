#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/nuvec.h"

#ifdef __cplusplus
extern "C" {
#endif

    i32 EdFileOpen(char *filepath, NUFILEMODE mode);
    i32 EdFileClose();
    i32 EdFileBackup(char *source, char *destination);
    i32 EdFileReadMemCard(char *filename, i32 size, void *data);
    i32 EdFileWriteMemCard(char *filename, i32 size, void *data);
    void EdFileSetMedia(i32 media);
    void EdFileSetPakFile(void *pak);
    void EdFileSetReadWrongEndianess(i32 value);

    void EdFileRead(void *buf, i32 len);
    char EdFileReadChar();
    unsigned char EdFileReadUnsignedChar();
    f32 EdFileReadFloat();
    i32 EdFileReadInt();
    u32 EdFileReadUnsignedInt();
    i16 EdFileReadShort();
    u16 EdFileReadUnsignedShort();
    void EdFileReadNuVec(NUVEC *out);

    void EdFileWrite(void *data, i32 len);
    void EdFileWriteFloat(f32 value);
    void EdFileWriteInt(i32 value);
    void EdFileWriteUnsignedInt(u32 value);
    void EdFileWriteShort(i16 value);
    void EdFileWriteUnsignedShort(u16 value);
    void EdFileWriteChar(char value);
    void EdFileWriteUnsignedChar(u8 value);
    void EdFileWriteNuVec(NUVEC *value);

    void EdFileSwapEndianess16(void *data);
    void EdFileSwapEndianess32(void *data);

#ifdef __cplusplus
}
#endif
