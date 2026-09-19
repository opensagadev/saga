#pragma once

struct FLOWBOX_s;
struct GIZFLOW_s;
struct GIZFLOWPROGRESS_s;
struct GIZMO_s;

void PerformActionFlowBox(GIZFLOW_s *system, FLOWBOX_s *box);
void ProcessGizFlow(GIZFLOW_s *system, float delta_time);
void DynamicAddGizmoToFlow(GIZFLOW_s *system, GIZMO_s *gizmo);
void ResetGizFlowPointers(GIZFLOW_s *system);
void ResetGizFlow(GIZFLOW_s *system, GIZFLOWPROGRESS_s *progress);
void GizFlowStoreProgress(GIZFLOW_s *system, GIZFLOWPROGRESS_s *progress);
