//=======================================================================================
// SkookumScript C++ library.
// Copyright (c) 2001 Agog Labs Inc.,
// All rights reserved.
//
// Class Type Scope Context
// Author(s):   Conan Reis
// Notes:          
//=======================================================================================


#ifndef __SKTYPEDCONTEXT_HPP
#define __SKTYPEDCONTEXT_HPP


//=======================================================================================
// Includes
//=======================================================================================

#include <AgogCore/AList.hpp>
#include <SkookumScript/SkNamed.hpp>
#include <SkookumScript/SkTyped.hpp>
#include <SkookumScript/SkParameters.hpp>

//=======================================================================================
// Global Structures
//=======================================================================================

// Pre-declarations
class SkClassUnaryBase;
class SkExpressionBase;
class SkInstance;
class SkIdentifierLocal;
class SkParameters;

#ifdef A_PLAT_PC
  template<class _ElementType> class AVCompactArrayBase;
  template<class _ElementType, class _KeyType = _ElementType> class AVCompactSortedLogical;
#else
  #include <AgogCore/AVCompactSorted.hpp>
#endif

typedef APArrayLogical<SkNamedIndexed, ASymbol> tSkNamedIndexedArray;

//---------------------------------------------------------------------------------------
// Notes      Class Type Scope Context - used to track variable scope & type when
//            parsing/compiling.
// Author(s)  Conan Reis
struct SkTypeContext
  {
  // Nested Structures

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Variables (and member variables that have their type changed) and their class types
    // for a specific scope level.
    struct ScopeVars : public AListNode<ScopeVars>
      {
      // Public Data Members

        // Available/type-modified variables at a particular scope.
        tSkTypedNamesIndexed m_vars;

        // Current number of data indices in use
        uint32_t m_data_idx_count;

        // Maximum number of data indices concurrently in use during lifetime of this scope
        uint32_t m_data_idx_count_max;

        // Variables that are no longer available - used to ensure unique names across scopes.
        tSkTypedNames m_var_history;

      // Methods

        ScopeVars(uint32_t data_idx_count) { m_data_idx_count_max = m_data_idx_count = data_idx_count; }
        ~ScopeVars() { empty(); }

        void empty() { m_vars.free_all(); m_var_history.free_all(); }

      };
  
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Tracking info for a set of captured variables.
    struct CapturedVars : public AListNode<CapturedVars>
      {
      // Public Data Members

        // Variables captured by this closure - note that data members do not need to be
        // captured - just 'this'.
        tSkTypedNamesIndexed m_vars;

        // Parent scope at which the capture was created
        tSkTypedNamesIndexed * m_scope_p;

        // Keep track of any indexed names used during this capture session
        tSkNamedIndexedArray m_indices_to_patch;

      // Methods

        CapturedVars(tSkTypedNamesIndexed * scope_p) : m_scope_p(scope_p) {}

      };
  

  // Public Data Members

    // Class type scope - used to retrieve member data and methods
    SkClassUnaryBase * m_obj_scope_p;

    // If in a routine the parameters that are being used otherwise nullptr.
    // Used to determine the desired type for a return parameter.
    SkParameters * m_params_p;

    // Scope name - member/etc.
    ASymbol m_scope_name;

    // Initial variable scope on the stack - namely the arguments & temporaries created
    // within the topmost code block or single path nested code blocks.
    ScopeVars m_top_scope;

    // Stack of typed variables - local variables and any member variables that have had
    // their types changed.  Class types of variables can change throughout the life of a
    // code block.  A new variable scope is placed on the stack each time there are
    // alternate code paths to follow.
    AList<ScopeVars> m_scope_stack;

    // The current scope 
    ScopeVars * m_current_scope_p;
    tSkTypedNamesIndexed * m_current_vars_p;

    // Captured temporary variables stack
    AList<CapturedVars> m_capture_stack;

    // Innermost capture context - if non-nullptr then there are one or more capture contexts.
    CapturedVars * m_capture_current_p;

  // Methods

    SK_NEW_OPERATORS(SkTypeContext);

    SkTypeContext();
    ~SkTypeContext();

    SkClassDescBase * finalize_generic(const SkClassDescBase & type) const;

    // Local Data Methods - variables from arguments and code block temporaries

      SkClassDescBase * get_rparam_type(const ASymbol & var_name) const;
      SkClassDescBase * get_local_variable_type(const ASymbol & var_name) const;

      uint32_t              append_local(const ASymbol & var_name, SkClassDescBase * type_p, bool is_return_arg);
      void                  archive_locals(const AVCompactArrayBase<ASymbol> & var_names);
      SkTypedNameIndexed *  find_local_variable(const ASymbol & var_name, SkTypedNameIndexed ** duplicate_var_pp = nullptr) const;
      void                  free_locals(const AVCompactArrayBase<ASymbol> & var_names);
      void                  free_locals(const tSkParamList & param_names);
      void                  free_all_locals();
      bool                  is_locals() const;
      void                  merge(tSkTypedNamesIndexed * merge_vars_p) const;
      void                  merge_locals(tSkTypedNamesIndexed * merge_vars_p, bool first_path_b) const;
      void                  capture_locals_start();
      void                  capture_locals_stop(tSkIndexedNames * captured_p);
      void                  on_identifier_created(SkIdentifierLocal * identifier_p);
      void                  nest_locals();
      void                  unnest_locals(eAHistory history = AHistory_remember);
      void                  accept_nest();

    // Combined Member & Local Methods

      void              change_variable_type(const ASymbol & var_name, SkClassDescBase * type_p);
      void              change_variable_types(const tSkTypedNamesIndexed & vars);
      SkClassDescBase * get_variable_type(const ASymbol & var_name, bool skip_current_scope_b = false, uint32_t * data_idx_p = nullptr, bool * is_return_arg_p = nullptr) const;
      bool              is_previous_variable(const ASymbol & var_name) const;
      bool              is_variable(const ASymbol & var_name) const;

  protected:

  // Internal Methods

    void capture_local(SkTypedNameIndexed * var_p, CapturedVars * top_nesting_p) const;

  };  // SkTypeContext


//=======================================================================================
// Inline Functions
//=======================================================================================

#ifndef A_INL_IN_CPP
  #include <SkookumScript/SkTypedContext.inl>
#endif


#endif  // __SKTYPEDCONTEXT_HPP

