#pragma once

struct nuvec_s;
struct rtldata_s;
struct GameObject_s;

void SetPanelLights(float intensity);
void SetLights_RTLDATA(rtldata_s *data, float scale);
void SetLevelLights(void *set, float scale);
void ResetLights(nuvec_s *position, rtldata_s *data, void *set);
void InitGameObjectLights(void);
void LightGameObject(GameObject_s *object, void *set);

// Declared here for the other units that use them.
struct GameObject_s;
struct nucolour3_s;
struct nuvec_s;
struct rtldata_s;
struct rtlset;
extern i32 (*Lighting_BlueFlickerFn)(GameObject_s *);
extern i32 Lighting_HighlightFlash;
extern nuvec_s paneldir[3];
extern rtldata_s lev_rtldata;
void EnableShadowMapRenderingFn();
void FindAndSetLights(nuvec_s *, f32, void *);
void ResetShadowMapRenderingFn();
void SetLights(nucolour3_s *colour0, nuvec_s *direction0, nucolour3_s *colour1, nuvec_s *direction1, nucolour3_s *colour2, nuvec_s *direction2, nuvec_s *ambient);
void SetShadowLight();
void SetZeroLights(void);
extern "C" {
    char *rtlGetEnvPath(void);
    char *rtlGetEnvSceneName(void);
    rtlset *rtlGetEnvSet(void);
}
