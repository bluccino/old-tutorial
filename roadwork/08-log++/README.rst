//==============================================================================
// 06-log+ logging, this time with a C++ mains program
//==============================================================================

New Functions
  - provide functions bl_log() and bl_trc() for log tracing
  - ad micro second time stamps
  - provide colors
  - provide color change if unprovisioned or provisioned

Basis
  - demo derived from 05-log
  - ocore3.c derived from ocore2.c (event notification added)
  - ocore3.c has been adopted in 05-log project

Config Parameters
  - stack configuration was initially CONFIG_MAIN_STACK_SIZE=512
  - this leaded to stack crashes (memory faults)
  - so it was changed to CONFIG_MAIN_STACK_SIZE=1024
