/*
 * Copyright 1997-2007 Sun Microsystems, Inc.  All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa Clara,
 * CA 95054 USA or visit www.sun.com if you need additional information or
 * have any questions.
 *  
 */
// This file is a derivative work resulting from (and including) modifications
// made by Azul Systems, Inc.  The date of such changes is 2010.
// Copyright 2010 Azul Systems, Inc.  All Rights Reserved.
//
// Please contact Azul Systems, Inc., 1600 Plymouth Street, Mountain View, 
// CA 94043 USA, or visit www.azulsystems.com if you need additional information 
// or have any questions.
#ifndef ARRAYKLASS_HPP
#define ARRAYKLASS_HPP


#include "universe.hpp"

// arrayKlass is the abstract baseclass for all array classes

class arrayKlass: public Klass {
 private:
  int      _dimension;         // This is n'th-dimensional array.
  klassRef _higher_dimension;  // Refers the (n+1)'th-dimensional array (if present).
  klassRef _lower_dimension;   // Refers the (n-1)'th-dimensional array (if present).
  int      _vtable_len;        // size of vtable for this klass  
  juint    _alloc_size;        // allocation profiling support
  objectRef _component_mirror; // component type, as a java/lang/Class

 public:
  // Testing operation
  bool oop_is_array() const { return true; }

  // Instance variables
  int dimension() const                 { return _dimension;      }
  void set_dimension(int dimension)     { _dimension = dimension; }

  klassOop higher_dimension() const     { return lvb_klassRef(&_higher_dimension).as_klassOop(); }
  void set_higher_dimension(klassOop k) { ref_store_without_check(&_higher_dimension, klassRef(k)); }
  klassRef* adr_higher_dimension()      { return &this->_higher_dimension;}

  klassOop lower_dimension() const      { return lvb_klassRef(&_lower_dimension).as_klassOop(); }
  void set_lower_dimension(klassOop k)  { ref_store_without_check(&_lower_dimension, klassRef(k)); }
  klassRef* adr_lower_dimension()       { return &this->_lower_dimension;}

  // Allocation profiling support
  juint alloc_size() const              { return _alloc_size; }
  void set_alloc_size(juint n)          { _alloc_size = n; }

  // offset of first element, including any padding for the sake of alignment
  int  array_header_in_bytes() const    { return layout_helper_header_size(layout_helper()); }
  int  log2_element_size() const        { return layout_helper_log2_element_size(layout_helper()); }
  // type of elements (T_OBJECT for both oop arrays and array-arrays)
  BasicType element_type() const        { return layout_helper_element_type(layout_helper()); }

  oop  component_mirror() const         { return lvb_ref(&_component_mirror).as_oop(); }
  void set_component_mirror(oop m)      { ref_store_without_check(&_component_mirror, objectRef(m)); }
  heapRef* adr_component_mirror()       { return (heapRef*)&this->_component_mirror;}

  // Compiler/Interpreter offset
  static ByteSize component_mirror_offset() { return byte_offset_of(arrayKlass, _component_mirror); }

  virtual klassOop java_super() const;//{ return SystemDictionary::object_klass(); }

  // Allocation
  // Sizes points to the first dimension of the array, subsequent dimensions
  // are always in higher memory.  The callers of these set that up.
virtual oop multi_allocate(int rank,jint*sizes,intptr_t sba_hint,TRAPS);
objArrayOop allocate_arrayArray(int n,int length,intptr_t sba_hint,TRAPS);

  // Lookup operations
  methodOop uncached_lookup_method(symbolOop name, symbolOop signature) const;

  // Casting from klassOop
  static arrayKlass* cast(klassOop k) {
    Klass* kp = k->klass_part();
    assert(kp->null_vtbl() || kp->oop_is_array(), "cast to arrayKlass");
    return (arrayKlass*) kp;
  }

  objArrayOop compute_secondary_supers(int num_extra_slots, TRAPS);
  bool compute_is_subtype_of(klassOop k);

  // Sizing
  static int header_size()                 { return oopDesc::header_size() + sizeof(arrayKlass)/HeapWordSize; }
  int object_size(int header_size) const;

  bool object_is_parsable() const          { return _vtable_len > 0; }

  // Java vtable
  klassVtable* vtable() const;             // return new klassVtable
  int  vtable_length() const               { return _vtable_len; }
  static int base_vtable_length()          { return Universe::base_vtable_size(); }
  void set_vtable_length(int len)          { assert(len == base_vtable_length(), "bad length"); _vtable_len = len; }
 protected:
  inline intptr_t* start_of_vtable() const;

 public:
  // Iterators
  void array_klasses_do(void f(klassOop k));
  void with_array_klasses_do(void f(klassOop k));

  // Shared creation method
  static arrayKlassHandle base_create_array_klass(
                                          const Klass_vtbl& vtbl,
                                          int header_size, KlassHandle klass,
                                          juint klassId,
                                          TRAPS);
  // Return a handle.
  static void     complete_create_array_klass(arrayKlassHandle k, KlassHandle super_klass, TRAPS);

 public:
   // jvm support
   jint compute_modifier_flags(TRAPS) const;

 public:
   // JVMTI support
   jint jvmti_class_status() const;

#ifndef PRODUCT
 public:
  // Printing
  void oop_print_on(oop obj, outputStream* st);
#endif
 public:
  // Verification
  void oop_verify_on(oop obj, outputStream* st);
};

#endif // ARRAYKLASS_HPP
