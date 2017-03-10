/*
   Copyright (C) 2017  Statoil ASA, Norway.

   The file 'ecl_type.c' is part of ERT - Ensemble based Reservoir Tool.

   ERT is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   ERT is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.

   See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html>
   for more details.
*/

#include <stdlib.h>
#include <string.h>

#include <ert/util/util.h>
#include <ert/ecl/ecl_util.h>
#include <ert/ecl/ecl_type.h>

/*****************************************************************/
/* The string names for the different ECLIPSE low-level
   types.
*/
#define ECL_TYPE_NAME_CHAR     "CHAR"
#define ECL_TYPE_NAME_C010     "C010"
#define ECL_TYPE_NAME_FLOAT    "REAL"
#define ECL_TYPE_NAME_INT      "INTE"
#define ECL_TYPE_NAME_DOUBLE   "DOUB"
#define ECL_TYPE_NAME_BOOL     "LOGI"
#define ECL_TYPE_NAME_MESSAGE  "MESS"

ecl_data_type * ecl_type_alloc_copy(const ecl_data_type * src_type) {
    ecl_data_type * ecl_type;
    ecl_type = util_malloc(ecl_type_get_sizeof_ctype(src_type));
    memcpy(ecl_type, src_type, ecl_type_get_sizeof_ctype(src_type));
    return ecl_type;
}

void ecl_type_free(ecl_data_type * ecl_type) {
    if(ecl_type == NULL)
        return;

    free(ecl_type);
}

ecl_data_type ecl_type_get_data_type(const ecl_type_enum type, const size_t element_size) {
    ecl_data_type * ecl_type = NULL;
    switch(type) {
    case(ECL_CHAR_TYPE):
      ecl_type = &ECL_CHAR;
      break;
    case(ECL_INT_TYPE):
      ecl_type = &ECL_INT;
      break;
    case(ECL_FLOAT_TYPE):
      ecl_type = &ECL_FLOAT;
      break;
    case(ECL_DOUBLE_TYPE):
      ecl_type = &ECL_DOUBLE;
      break;
    case(ECL_BOOL_TYPE):
      ecl_type = &ECL_BOOL;
      break;
    case(ECL_MESS_TYPE):
      ecl_type = &ECL_MESS;
      break;
    default:
      util_abort("%s: invalid ecl_type:(%d, %d)\n", __func__, type, element_size);
    }

    if(ecl_type->element_size != element_size)
        util_abort(
                "%s: element_size mismatch for type %d, was: %d, expected: %d\n",
                __func__, type, element_size, ecl_type->element_size);

    return *ecl_type;
}

ecl_data_type ecl_type_get_data_type_from_type(const ecl_type_enum type) {
    ecl_data_type * ecl_type = NULL;
    switch(type) {
    case(ECL_CHAR_TYPE):
      ecl_type = &ECL_CHAR;
      break;
    case(ECL_INT_TYPE):
      ecl_type = &ECL_INT;
      break;
    case(ECL_FLOAT_TYPE):
      ecl_type = &ECL_FLOAT;
      break;
    case(ECL_DOUBLE_TYPE):
      ecl_type = &ECL_DOUBLE;
      break;
    case(ECL_BOOL_TYPE):
      ecl_type = &ECL_BOOL;
      break;
    case(ECL_MESS_TYPE):
      ecl_type = &ECL_MESS;
      break;
    default:
      util_abort("%s: invalid ecl_type: %d\n", __func__, type);
    }

    return *ecl_type;
}

const char * ecl_type_get_type_name(const ecl_data_type * ecl_type) {
  switch (ecl_type->type) {
  case(ECL_CHAR_TYPE):
    return ECL_TYPE_NAME_CHAR ;
  case(ECL_C010_TYPE):
    return ECL_TYPE_NAME_C010;
  case(ECL_FLOAT_TYPE):
    return ECL_TYPE_NAME_FLOAT;
  case(ECL_DOUBLE_TYPE):
    return ECL_TYPE_NAME_DOUBLE;
  case(ECL_INT_TYPE):
    return ECL_TYPE_NAME_INT;
  case(ECL_BOOL_TYPE):
    return ECL_TYPE_NAME_BOOL;
  case(ECL_MESS_TYPE):
    return ECL_TYPE_NAME_MESSAGE;
  default:
    util_abort("Internal error in %s - internal eclipse_type: %d not recognized - aborting \n",__func__ , ecl_type->type);
  }
  return NULL; /* Dummy */
}

ecl_data_type ecl_type_get_type_from_name( const char * type_name ) {
  if (strncmp( type_name , ECL_TYPE_NAME_FLOAT , ECL_TYPE_LENGTH) == 0)
    return ECL_FLOAT;
  else if (strncmp( type_name , ECL_TYPE_NAME_INT , ECL_TYPE_LENGTH) == 0)
    return ECL_INT;
  else if (strncmp( type_name , ECL_TYPE_NAME_DOUBLE , ECL_TYPE_LENGTH) == 0)
    return ECL_DOUBLE;
  else if (strncmp( type_name , ECL_TYPE_NAME_CHAR , ECL_TYPE_LENGTH) == 0)
    return ECL_CHAR;
  else if (strncmp( type_name , ECL_TYPE_NAME_C010 , ECL_TYPE_LENGTH) == 0)
    return ECL_C010;
  else if (strncmp( type_name , ECL_TYPE_NAME_MESSAGE , ECL_TYPE_LENGTH) == 0)
    return ECL_MESS;
  else if (strncmp( type_name , ECL_TYPE_NAME_BOOL , ECL_TYPE_LENGTH) == 0)
    return ECL_BOOL;
  else {
    util_abort("%s: unrecognized type name:%s \n",__func__ , type_name);
    return ECL_INT; /* Dummy */
  }
}


int ecl_type_get_sizeof_ctype_fortio(const ecl_data_type * ecl_type) {
  if(ecl_type_is_char(ecl_type) || ecl_type_is_C010(ecl_type))
      return (ecl_type->element_size - 1) * sizeof(char);
  else
      return ecl_type_get_sizeof_ctype(ecl_type);
}

int ecl_type_get_sizeof_ctype(const ecl_data_type * ecl_type) {
   return ecl_type->element_size * sizeof(char);
}

bool ecl_type_is_numeric(const ecl_data_type * ecl_type) {
    return (ecl_type->type == ECL_INT_TYPE ||
            ecl_type->type == ECL_FLOAT_TYPE ||
            ecl_type->type == ECL_DOUBLE_TYPE);
}

bool ecl_type_is_equal(const ecl_data_type * ecl_type1, const ecl_data_type * ecl_type2) {
    return (ecl_type1->type == ecl_type2->type && ecl_type1->element_size == ecl_type2->element_size);
}

bool ecl_type_is_char(const ecl_data_type * ecl_type) {
    return (ecl_type->type == ECL_CHAR_TYPE);
}

bool ecl_type_is_int(const ecl_data_type * ecl_type) {
    return (ecl_type->type == ECL_INT_TYPE);
}

bool ecl_type_is_float(const ecl_data_type * ecl_type) {
    return (ecl_type->type == ECL_FLOAT_TYPE);
}

bool ecl_type_is_double(const ecl_data_type * ecl_type) {
    return (ecl_type->type == ECL_DOUBLE_TYPE);
}

bool ecl_type_is_mess(const ecl_data_type * ecl_type) {
    return (ecl_type->type == ECL_MESS_TYPE);
}
 
bool ecl_type_is_bool(const ecl_data_type * ecl_type) {
    return (ecl_type->type == ECL_BOOL_TYPE);
}

bool ecl_type_is_C010(const ecl_data_type * ecl_type) {
    return (ecl_type->type == ECL_C010_TYPE);
}
