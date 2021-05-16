#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "predictor.h"

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

PREDICTOR::PREDICTOR(void){
  tage = new my_predictor();
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

bool   PREDICTOR::GetPrediction(UINT32 PC){
  bool gpred = tage->predict (PC & 0x3ffff, OPTYPE_JMP_DIRECT_COND)||tage->predict (PC & 0x3ffff, OPTYPE_JMP_INDIRECT_COND);
  return gpred;
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

void  PREDICTOR::UpdatePredictor(UINT32 PC, bool resolveDir, bool predDir, UINT32 branchTarget){
  tage->update(PC & 0x3ffff, OPTYPE_JMP_DIRECT_COND, resolveDir, branchTarget & 0x7f);
  tage->update(PC & 0x3ffff, OPTYPE_JMP_INDIRECT_COND, resolveDir, branchTarget & 0x7f);
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

void    PREDICTOR::TrackOtherInst(UINT32 PC, OpType opType, UINT32 branchTarget){
  // Decode instruction
  bool call   = (opType == OPTYPE_CALL_DIRECT_UNCOND )||(opType == OPTYPE_CALL_INDIRECT_UNCOND )||(opType == OPTYPE_CALL_DIRECT_COND ) ||(opType == OPTYPE_CALL_INDIRECT_COND );
  bool ret    = (opType == OPTYPE_RET_UNCOND )||(opType==OPTYPE_RET_COND) ;
  bool uncond = (opType == OPTYPE_JMP_DIRECT_UNCOND)|| (opType==OPTYPE_JMP_INDIRECT_UNCOND);
  bool cond   = (opType==OPTYPE_JMP_DIRECT_COND)||(opType==OPTYPE_JMP_INDIRECT_COND);
  bool indir  = (opType == OPTYPE_JMP_INDIRECT_UNCOND)||(opType==OPTYPE_JMP_INDIRECT_COND) ;
  bool resolveDir = true;

  assert(!cond);

  if (call || ret || indir || uncond) {
    tage->update(PC & 0x3ffff, opType, resolveDir, branchTarget & 0x7f);
  }

  return;
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
