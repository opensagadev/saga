#include "nu2api/nucore/NuInputDevice.h"
#include "nu2api/nucore/NuTouchInputElement.h"

struct NuButtonLayout {
    NuTouchInputElement *elements[50];

    u32 unknown_c8;

    void ActivateLayout();
    void DeactivateLayout();
    void Render();
    void Update(NuInputTouchData const *);
    void UpdateButtons(i32 index);
    ~NuButtonLayout();
};

class NuVirtualTouchDevice : public NuInputDeviceTranslator {
  public:
    NuVirtualTouchDevice(u32 unknown);

    virtual void Execute(u32 port, NUPADTYPE in_type, NUPADATTACHMENTTYPE in_attch_type, u32 in_caps, u32 in_buttons,
                         const float *in_analog, const float *in_motion, const NuInputTouchData *in_touch_data,
                         const NuInputMouseData *in_mouse_data, NUPADTYPE &out_pad_type,
                         NUPADATTACHMENTTYPE &out_attch_type, u32 &out_caps, u32 &out_buttons, float *out_analog,
                         float *out_motion, NuInputTouchData *out_touch_data,
                         NuInputMouseData *out_mouse_data) override;

    void CreateDefaultLayout(u32 unknown);

    void AddAlwaysActiveElement(NuTouchInputElement *element) {
        if (element != NULL && unknown_08.unknown_c8 < 50) {
            unknown_08.elements[unknown_08.unknown_c8++] = element;
        }
    }

    f32 GetAspectRatio();
    u32 GetCurrentLayoutIndex() const {
        return unknown_04;
    }
    void Render();
    void SetCurrentLayoutIndex(u32 index);

  private:
    // Type uncertain.
    u32 unknown_04;

    NuButtonLayout unknown_08;
    NuButtonLayout unknown_d4[10];
};
