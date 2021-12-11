//==============================================================================
// 05-log - introducing logging
//==============================================================================

New Functions
  - provide functions bl_log() and bl_trc() for log tracing
  - ad micro second time stamps
  - provide colors
  - provide color change if unprovisioned or provisioned

Basis
  - demo derived from 04-toggle
  - ocore3.c derived from ocore2.c (event notification added)
  - based on ocore2

Config Parameters
  - stack configuration was initially CONFIG_MAIN_STACK_SIZE=512
  - this leaded to stack crashes (memory faults)
  - so it was changed to CONFIG_MAIN_STACK_SIZE=1024
