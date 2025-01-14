//=======================================================================================
// SkookumScript C++ library.
// Copyright (c) 2015 Agog Labs Inc. All rights reserved.
//
// SkookumScript Euler angles class
//
// Author: Markus Breyer
//=======================================================================================

//=======================================================================================
// Includes
//=======================================================================================

#include "../../SkookumScriptRuntimePrivatePCH.h"
#include "SkRotationAngles.hpp"
#include "SkRotation.hpp"

//=======================================================================================
// Method Definitions
//=======================================================================================

namespace SkRotationAngles_Impl
  {

  //---------------------------------------------------------------------------------------
  // # Skookum:   RotationAngles@!yaw_pitch_roll(Real yaw, Real pitch, Real roll) RotationAngles
  // # Author(s): Markus Breyer
  static void mthd_ctor_yaw_pitch_roll(SkInvokedMethod * scope_p, SkInstance ** result_pp)
    {
    // Results are ignored for constructors
    scope_p->get_this()->construct<SkRotationAngles>(
      scope_p->get_arg<SkReal>(SkArg_2),
      scope_p->get_arg<SkReal>(SkArg_1),
      scope_p->get_arg<SkReal>(SkArg_3));
    }

  //---------------------------------------------------------------------------------------
  // # Skookum:   RotationAngles@String() String
  // # Author(s): Markus Breyer
  static void mthd_String(SkInvokedMethod * scope_p, SkInstance ** result_pp)
    {
    // Do nothing if result not desired
    if (result_pp)
      {
      const FRotator & rot = scope_p->this_as<SkRotationAngles>();
      AString str(128u, "(yaw=%g, pitch=%g, roll=%g)", double(rot.Yaw), double(rot.Pitch), double(rot.Roll));

      *result_pp = SkString::new_instance(str);
      }
    }

  //---------------------------------------------------------------------------------------
  // # Skookum:   RotationAngles@Rotation() Rotation
  // # Author(s): Markus Breyer
  static void mthd_Rotation(SkInvokedMethod * scope_p, SkInstance ** result_pp)
    {
    // Do nothing if result not desired
    if (result_pp)
      {
      const FRotator & rot = scope_p->this_as<SkRotationAngles>();
      *result_pp = SkRotation::new_instance(rot.Quaternion());
      }
    }

  //---------------------------------------------------------------------------------------
  // # Skookum:   RotationAngles@set(Real yaw, Real pitch, Real roll) RotationAngles
  // # Author(s): Markus Breyer
  static void mthd_set(SkInvokedMethod * scope_p, SkInstance ** result_pp)
    {
    SkInstance * this_p = scope_p->get_this();
    FRotator & rot = this_p->as<SkRotationAngles>();

    rot.Yaw   = scope_p->get_arg<SkReal>(SkArg_1);
    rot.Pitch = scope_p->get_arg<SkReal>(SkArg_2);
    rot.Roll  = scope_p->get_arg<SkReal>(SkArg_3);

    // Return this if result desired
    if (result_pp)
      {
      this_p->reference();
      *result_pp = this_p;
      }
    }

  //---------------------------------------------------------------------------------------
  // # Skookum:   RotationAngles@zero() RotationAngles
  // # Author(s): Markus Breyer
  static void mthd_zero(SkInvokedMethod * scope_p, SkInstance ** result_pp)
    {
    SkInstance * this_p = scope_p->get_this();

    FRotator & rot = this_p->as<SkRotationAngles>();
    rot.Yaw   = 0.0f;
    rot.Pitch = 0.0f;
    rot.Roll  = 0.0f;

    // Return this if result desired
    if (result_pp)
      {
      this_p->reference();
      *result_pp = this_p;
      }
    }

  //---------------------------------------------------------------------------------------
  // # Skookum:   RotationAngles@zero?() Boolean
  // # Author(s): Markus Breyer
  static void mthd_zeroQ(SkInvokedMethod * scope_p, SkInstance ** result_pp)
    {
    // Do nothing if result not desired
    if (result_pp)
      {
      *result_pp = SkBoolean::new_instance(scope_p->this_as<SkRotationAngles>().IsZero());
      }
    }

  //---------------------------------------------------------------------------------------

  // Instance method array
  static const SkClass::MethodInitializerFunc methods_i[] =
    {
      { "!yaw_pitch_roll",    mthd_ctor_yaw_pitch_roll },

      { "String",             mthd_String },
      { "Rotation",           mthd_Rotation },

      { "set",                mthd_set },
      { "zero?",              mthd_zeroQ },
      { "zero",               mthd_zero },
    };

  } // namespace

//---------------------------------------------------------------------------------------

void SkRotationAngles::register_bindings()
  {
  tBindingBase::register_bindings("RotationAngles");

  ms_class_p->register_method_func_bulk(SkRotationAngles_Impl::methods_i, A_COUNT_OF(SkRotationAngles_Impl::methods_i), SkBindFlag_instance_no_rebind);

  ms_class_p->register_raw_accessor_func(&SkUEClassBindingHelper::access_raw_data_struct<SkRotationAngles>);
  SkUEClassBindingHelper::resolve_raw_data_struct(ms_class_p, TEXT("Rotator"));
  }
