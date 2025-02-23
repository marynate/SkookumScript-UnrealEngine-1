//=======================================================================================
// SkookumScript Plugin for Unreal Engine 4
// Copyright (c) 2015 Agog Labs Inc. All rights reserved.
//
// Main entry point for the SkookumScript runtime plugin
// 
// Author: Markus Breyer
//=======================================================================================

#pragma once

#include "ModuleManager.h"

class SkClass;
class UClass;
class UBlueprint;

//---------------------------------------------------------------------------------------
// Interface class for the runtime plugin to call the editor plugin
class ISkookumScriptRuntimeEditorInterface
  {
  public:

  #if WITH_EDITORONLY_DATA
    virtual FString       ensure_temp_project(const TCHAR * generated_overlay_name_p) = 0;
    virtual FString       make_project_editable() = 0;
    virtual void          set_overlay_path(const FString & scripts_path, const FString & overlay_name) = 0; // E.g. ProjectName/Scripts/Project-Generated
    virtual int32         get_overlay_path_depth() const = 0;
    virtual void          generate_all_class_script_files() = 0;
    virtual void          recompile_blueprints_with_errors() = 0;
    virtual UBlueprint *  load_blueprint_asset(const FString & class_path, bool * sk_class_deleted_p) = 0;
    virtual void          on_class_updated(UClass * ue_class_p) = 0;
  #endif

  };


//---------------------------------------------------------------------------------------
// The public interface to this module
class ISkookumScriptRuntime : public IModuleInterface
  {
  public:

    // Types

    enum eChangeType
      {
      ChangeType_created,
      ChangeType_modified,
      ChangeType_deleted
      };

    // Methods

    virtual void  startup_skookum() = 0;
    virtual bool  is_skookum_initialized() const = 0;
    virtual bool  is_freshen_binaries_pending() const = 0;

    #if WITH_EDITOR

      virtual void  set_editor_interface(ISkookumScriptRuntimeEditorInterface * editor_interface_p) = 0;

      virtual void  on_editor_map_opened() = 0;
      virtual void  on_class_scripts_changed_by_editor(const FString & class_name, eChangeType change_type) = 0;
      virtual void  show_ide(const FString & focus_class_name, const FString & focus_member_name, bool is_data_member, bool is_class_member) = 0;
      virtual void  freshen_compiled_binaries_if_have_errors() = 0;

      virtual bool  is_static_class_known_to_skookum(UClass * class_p) const = 0;
      virtual bool  is_static_struct_known_to_skookum(UStruct * struct_p) const = 0;
      virtual bool  is_static_enum_known_to_skookum(UEnum * enum_p) const = 0;
      virtual bool  has_skookum_default_constructor(UClass * class_p) const = 0;
      virtual bool  has_skookum_destructor(UClass * class_p) const = 0;
      virtual bool  is_skookum_component_class(UClass * class_p) const = 0;
      virtual bool  is_skookum_blueprint_function(UFunction * function_p) const = 0;
      virtual bool  is_skookum_blueprint_event(UFunction * function_p) const = 0;

    #endif

  };

