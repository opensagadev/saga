#pragma once

struct nuvec_s;
struct rtldata_s;
struct GameObject_s;

void SetPanelLights(float intensity);
void ResetLights(nuvec_s *position, rtldata_s *data, void *set);
void InitGameObjectLights(void);
void LightGameObject(GameObject_s *object, void *set);
