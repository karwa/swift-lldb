//===-- ClangPersistentVariables.h ------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef liblldb_ClangPersistentVariables_h_
#define liblldb_ClangPersistentVariables_h_

#include "ClangExpressionVariable.h"
#include "ClangModulesDeclVendor.h"

#include "lldb/Expression/ExpressionVariable.h"

#include "llvm/ADT/DenseMap.h"

namespace lldb_private
{
    
//----------------------------------------------------------------------
/// @class ClangPersistentVariables ClangPersistentVariables.h "lldb/Expression/ClangPersistentVariables.h"
/// @brief Manages persistent values that need to be preserved between expression invocations.
///
/// A list of variables that can be accessed and updated by any expression.  See
/// ClangPersistentVariable for more discussion.  Also provides an increasing,
/// 0-based counter for naming result variables.
//----------------------------------------------------------------------
class ClangPersistentVariables : public PersistentExpressionState
{
public:
    
    //----------------------------------------------------------------------
    /// Constructor
    //----------------------------------------------------------------------
    ClangPersistentVariables ();
    
    ~ClangPersistentVariables () { }
    
    //------------------------------------------------------------------
    // llvm casting support
    //------------------------------------------------------------------
    static bool classof(const PersistentExpressionState *pv)
    {
        return pv->getKind() == PersistentExpressionState::eKindClang;
    }

    lldb::ExpressionVariableSP
    CreatePersistentVariable (const lldb::ValueObjectSP &valobj_sp) override;

    lldb::ExpressionVariableSP
    CreatePersistentVariable (ExecutionContextScope *exe_scope,
                              const ConstString &name, 
                              const CompilerType& compiler_type, 
                              lldb::ByteOrder byte_order, 
                              uint32_t addr_byte_size) override;

    //----------------------------------------------------------------------
    /// Return the next entry in the sequence of strings "$0", "$1", ... for
    /// use naming persistent expression convenience variables.
    ///
    /// @return
    ///     A string that contains the next persistent variable name.
    //----------------------------------------------------------------------
    ConstString
    GetNextPersistentVariableName () override;
    
    void
    RemovePersistentVariable (lldb::ExpressionVariableSP variable) override;
    
    lldb::addr_t
    LookupSymbol (const ConstString &name) override { return LLDB_INVALID_ADDRESS; }

    void
    RegisterPersistentType (const ConstString &name,
                            clang::TypeDecl *tag_decl);
    
    clang::TypeDecl *
    GetPersistentType (const ConstString &name);
    
    void
    AddHandLoadedClangModule(ClangModulesDeclVendor::ModuleID module)
    {
        m_hand_loaded_clang_modules.push_back(module);
    }
    
    const ClangModulesDeclVendor::ModuleVector &GetHandLoadedClangModules()
    {
        return m_hand_loaded_clang_modules;
    }
    
private:
    uint32_t                                                m_next_persistent_variable_id;  ///< The counter used by GetNextResultName().
    
    typedef llvm::DenseMap<const char *, clang::TypeDecl *> PersistentTypeMap;
    PersistentTypeMap                                       m_persistent_types;             ///< The persistent types declared by the user.
    
    ClangModulesDeclVendor::ModuleVector                    m_hand_loaded_clang_modules;    ///< These are Clang modules we hand-loaded; these are the highest-
                                                                                            ///< priority source for macros.
};

}

#endif