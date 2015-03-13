/**
 * LoadsStoresList.h
 *
 * Created on: Oct 3, 2014
 *     Author: Ignacio Laguna, Simone Atzeni
 *    Contact: ilaguna@llnl.gov, simone@cs.utah.edu
 *
 * LineMemoryUsage - this is a module pass that measures memory usage, in terms
 * of loads and stores, for each lines of source code. It intercepts every load
 * and stores instruction and adds a function call that records the execution
 * of the instruction. The pass saves line+file+dir information as and ID in a
 * table, which is printed at the end of the pass.
 *
 */

#include "llvm/Pass.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/ToolOutputFile.h"

#include <list>
#include <vector>
#include <map>
#include <string>
#include <sstream>

#include "CommonLLVM.h"
#include "Util.h"

using namespace llvm;

#define DEBUG_TYPE "archer-memoryusage"

namespace {

class LineMemoryUsage : public ModulePass
{
private: 
  std::string ls_content;
  std::string fc_content;
  std::string dir;

bool writeFile(std::string modulename, std::string ext, std::string &content) {
    std::pair<StringRef, StringRef> filename = StringRef(modulename).rsplit('.');
    std::string FileName;
    FileName = dir + "/" + filename.first.str() + ext;
    content = "# Module:" + filename.first.str() + "\n" + content;

    std::string ErrInfo;
    tool_output_file F(FileName.c_str(), ErrInfo, llvm::sys::fs::F_Append);

    if (ErrInfo.empty()) {
      F.os() << content;
      F.os().close();
      if (!F.os().has_error()) {
	F.keep();
      }
    } else {    
      errs() << "error opening file for writing!\n";
      F.os().clear_error();
      return false;
   }

   return true;
}

public:
  static char ID;

  LineMemoryUsage() : ModulePass(ID) {
    ls_content = "";
    fc_content = "";
    dir = ".blacklists";

    if(llvm::sys::fs::create_directory(Twine(dir))) {
      llvm::errs() << "Unable to create \"" << dir << "\" directory.\n";
      exit(-1);
    }
  }

  virtual void getAnalysisUsage(AnalysisUsage &AU) const
  {
  }

  /// Attempts to instrument the given instructions. It instruments "only"
  /// instructions in which we can find debugging meta-data to obtain line
  /// number information. Returns the number of instrumented instructions.
  void infoInstruction(Instruction *Inst, std::string FunctionName, Module *M, std::string *content)
  {
    std::string val;

    if (MDNode *N = Inst->getMetadata("dbg")) {
      DILocation Loc(N);
      unsigned Line = Loc.getLineNumber();
      StringRef File = Loc.getFilename();
      StringRef Dir = Loc.getDirectory();
      // std::pair<StringRef, StringRef> filename = StringRef(M->getModuleIdentifier()).rsplit('.');
      // if(File.compare(filename.first) == 0) {
      if(true) {
	val = NumberToString<unsigned>(Line) + "," + FunctionName + "," + File.str() + "," + Dir.str() + "\n";
	errs() << val << "\n";
	if(content->find(val) == std::string::npos) {
	  *content += val;	
	}
      }
    }
  }

  virtual bool runOnModule(Module &M)
  {
    for (auto F = M.begin(), e = M.end(); F!=e; ++F) {
      // Discard function declarations
      if (F->isDeclaration())
	continue;
	
      for (auto BB = F->begin(), ee = F->end(); BB != ee; ++BB)  {
	for (auto I = BB->begin(), eee = BB->end(); I != eee; ++I) {
	  Instruction *inst = &(*I);
          if(isa<CallInst>(inst)) {
	    if(cast<CallInst>(inst)->getCalledFunction() != NULL)
	      infoInstruction(inst, cast<CallInst>(inst)->getCalledFunction()->getName(), &M, &fc_content);
          }
	  if (isa<LoadInst>(inst) || isa<StoreInst>(inst)) {
            infoInstruction(inst, F->getName(), &M, &ls_content);
	  }
	}
      }
    }

    std::string ModuleName = M.getModuleIdentifier();

    writeFile(ModuleName, LS_LINES, ls_content);
    writeFile(ModuleName, FC_LINES, fc_content);

    return true;
  }
  
};

}

char LineMemoryUsage::ID = 0;
static RegisterPass<LineMemoryUsage> X("archer-memoryusage", "Archer Memory usage (loads/stores) per source line.", false, false);
