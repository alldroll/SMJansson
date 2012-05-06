#if defined _jansson_included_
	#endinput
#endif
#define _jansson_included_


/**
 * --- Type
 *
 * The JSON specification (RFC 4627) defines the following data types:
 * object, array, string, number, boolean, and null.
 * JSON types are used dynamically; arrays and objects can hold any
 * other data type, including themselves. For this reason, Jansson�s
 * type system is also dynamic in nature. There�s one Handle type to
 * represent all JSON values, and the referenced structure knows the
 * type of the JSON value it holds.
 *
 */
enum json_type {
	JSON_OBJECT,
	JSON_ARRAY,
	JSON_STRING,
	JSON_INTEGER,
	JSON_REAL,
	JSON_TRUE,
	JSON_FALSE,
	JSON_NULL
}

/**
 * Return the type of the JSON value.
 *
 * @param hObj              Handle to the JSON value
 *
 * @return                  json_type of the value.
 */
native json_type:json_typeof(Handle:hObj);

/**
 * The type of a JSON value is queried and tested using these macros
 *
 * @param %1                Handle to the JSON value
 *
 * @return                  True if the value has the correct type.
 */
#define json_is_object(%1)			( json_typeof(%1) == JSON_OBJECT )
#define json_is_array(%1)			( json_typeof(%1) == JSON_ARRAY )
#define json_is_string(%1)			( json_typeof(%1) == JSON_STRING )
#define json_is_integer(%1)			( json_typeof(%1) == JSON_INTEGER )
#define json_is_real(%1)			( json_typeof(%1) == JSON_REAL )
#define json_is_true(%1)			( json_typeof(%1) == JSON_TRUE )
#define json_is_false(%1)			( json_typeof(%1) == JSON_FALSE )
#define json_is_null(%1)			( json_typeof(%1) == JSON_NULL )
#define json_is_number(%1)			( json_typeof(%1) == JSON_INTEGER || json_typeof(%1) == JSON_REAL )
#define json_is_boolean(%1)			( json_typeof(%1) == JSON_TRUE || json_typeof(%1) == JSON_FALSE )

/**
 * Saves json_type as a String in out
 *
 * @param in                json_type value to convert to string
 * @param out               Buffer to store the json_type value
 * @param maxlength         Maximum length of string buffer.
 *
 * @return                  False if the type does not exist.
 */
stock bool:Stringify_json_type(json_type:in, String:out[], maxlength) {
	switch(in) {
		case JSON_OBJECT:		strcopy(out, maxlength, "Object");
		case JSON_ARRAY:		strcopy(out, maxlength, "Array");
		case JSON_STRING:		strcopy(out, maxlength, "String");
		case JSON_INTEGER:		strcopy(out, maxlength, "Integer");
		case JSON_REAL:			strcopy(out, maxlength, "Real");
		case JSON_TRUE:			strcopy(out, maxlength, "True");
		case JSON_FALSE:		strcopy(out, maxlength, "False");
		case JSON_NULL:			strcopy(out, maxlength, "Null");
		default: return false;
	}

	return true;
}



/**
 * --- Equality
 *
 * - Two integer or real values are equal if their contained numeric
 *   values are equal. An integer value is never equal to a real value,
 *   though.
 * - Two strings are equal if their contained UTF-8 strings are equal,
 *   byte by byte. Unicode comparison algorithms are not implemented.
 * - Two arrays are equal if they have the same number of elements and
 *   each element in the first array is equal to the corresponding
 *   element in the second array.
 * - Two objects are equal if they have exactly the same keys and the
 *   value for each key in the first object is equal to the value of
 *   the corresponding key in the second object.
 * - Two true, false or null values have no "contents", so they are
 *   equal if their types are equal.
 *
 */

/**
 * Test whether two JSON values are equal.
 *
 * @param hObj              Handle to the first JSON object
 * @param hOther            Handle to the second JSON object
 *
 * @return                  Returns false if they are inequal or one
 *                          or both of the pointers are NULL.
 */
native bool:json_equal(Handle:hObj, Handle:hOther);




/**
 * --- Copying
 *
 * Jansson supports two kinds of copying: shallow and deep. There is
 * a difference between these methods only for arrays and objects.
 *
 * Shallow copying only copies the first level value (array or object)
 * and uses the same child values in the copied value.
 *
 * Deep copying makes a fresh copy of the child values, too. Moreover,
 * all the child values are deep copied in a recursive fashion.
 *
 */

/**
 * Get a shallow copy of the passed object
 *
 * @param hObj              Handle to JSON object to be copied
 *
 * @return                  Returns a shallow copy of the object,
 *                          or INVALID_HANDLE on error.
 */
native Handle:json_copy(Handle:hObj);

/**
 * Get a deep copy of the passed object
 *
 * @param hObj              Handle to JSON object to be copied
 *
 * @return                  Returns a deep copy of the object,
 *                          or INVALID_HANDLE on error.
 */
native Handle:json_deep_copy(Handle:hObj);




/**
 * --- Objects
 *
 * A JSON object is a dictionary of key-value pairs, where the
 * key is a Unicode string and the value is any JSON value.
 *
 */

/**
 * Returns a handle to a new JSON object, or INVALID_HANDLE on error.
 * Initially, the object is empty.
 *
 * @return                  Handle to a new JSON object.
 */
native Handle:json_object();

/**
 * Returns the number of elements in hObj
 *
 * @param hObj              Handle to JSON object
 *
 * @return                  Number of elements in hObj,
 *                          or 0 if hObj is not a JSON object.
 */
native json_object_size(Handle:hObj);

/**
 * Get a value corresponding to sKey from hObj
 *
 * @param hObj              Handle to JSON object to get a value from
 * @param sKey              Key to retrieve
 *
 * @return                  Handle to a the JSON object or
 *                          INVALID_HANDLE on error.
 */
native Handle:json_object_get(Handle:hObj, const String:sKey[]);

/**
 * Set the value of sKey to hValue in hObj.
 * If there already is a value for key, it is replaced by the new value.
 *
 * @param hObj              Handle to JSON object to set a value on
 * @param sKey              Key to store in the object
 *                          Must be a valid null terminated UTF-8 encoded
 *                          Unicode string.
 * @param hValue            Value to store in the object
 *
 * @return					True on success.
 */
native bool:json_object_set(Handle:hObj, const String:sKey[], Handle:hValue);

/**
 * Set the value of sKey to hValue in hObj.
 * If there already is a value for key, it is replaced by the new value.
 * This function automatically closes the Handle to the value object.
 *
 * @param hObj              Handle to JSON object to set a value on
 * @param sKey              Key to store in the object
 *                          Must be a valid null terminated UTF-8 encoded
 *                          Unicode string.
 * @param hValue            Value to store in the object
 *
 * @return					True on success.
 */
native bool:json_object_set_new(Handle:hObj, const String:sKey[], Handle:hValue);

/**
 * Delete sKey from hObj if it exists.
 *
 * @param hObj              Handle to JSON object to delete a key from
 * @param sKey              Key to delete
 *
 * @return                  True on success.
 */
native bool:json_object_del(Handle:hObj, const String:sKey[]);

/**
 * Remove all elements from hObj.
 *
 * @param hObj              Handle to JSON object to remove all
 *                          elements from.
 *
 * @return                  True on success.
 */
native bool:json_object_clear(Handle:hObj);

/**
 * Update hObj with the key-value pairs from hOther, overwriting
 * existing keys.
 *
 * @param hObj              Handle to JSON object to update
 * @param hOther            Handle to JSON object to get update
 *                          keys/values from.
 *
 * @return                  True on success.
 */
native bool:json_object_update(Handle:hObj, Handle:hOther);

/**
 * Like json_object_update(), but only the values of existing keys
 * are updated. No new keys are created.
 *
 * @param hObj              Handle to JSON object to update
 * @param hOther            Handle to JSON object to get update
 *                          keys/values from.
 *
 * @return                  True on success.
 */
native bool:json_object_update_existing(Handle:hObj, Handle:hOther);

/**
 * Like json_object_update(), but only new keys are created.
 * The value of any existing key is not changed.
 *
 * @param hObj              Handle to JSON object to update
 * @param hOther            Handle to JSON object to get update
 *                          keys/values from.
 *
 * @return                  True on success.
 */
native bool:json_object_update_missing(Handle:hObj, Handle:hOther);




/**
 * Object iteration
 *
 * Example code:
 *  - We assume hObj is a Handle to a valid JSON object.
 *
 *
 * new Handle:hIterator = json_object_iter(hObj);
 * while(hIterator != INVALID_HANDLE)
 * {
 *     	new String:sKey[128];
 *      json_object_iter_key(hIterator, sKey, sizeof(sKey));
 *
 *      new Handle:hValue = json_object_iter_value(hIterator);
 *
 *      // Do something with sKey and hValue
 *
 *      CloseHandle(hValue);
 *
 *      hIterator = json_object_iter_next(hObj, hIterator);
 * }
 *
 */

/**
 * Returns a handle to an iterator which can be used to iterate over
 * all key-value pairs in hObj.
 * If you are not iterating to the end of hObj make sure to close the
 * handle to the iterator manually.
 *
 * @param hObj              Handle to JSON object to get an iterator
 *                          for.
 *
 * @return                  Handle to JSON object iterator,
 *                          or INVALID_HANDLE on error.
 */
native Handle:json_object_iter(Handle:hObj);

/**
 * Like json_object_iter(), but returns an iterator to the key-value
 * pair in object whose key is equal to key.
 * Iterating forward to the end of object only yields all key-value
 * pairs of the object if key happens to be the first key in the
 * underlying hash table.
 *
 * @param hObj              Handle to JSON object to get an iterator
 *                          for.
 * @param sKey              Start key for the iterator
 *
 * @return                  Handle to JSON object iterator,
 *                          or INVALID_HANDLE on error.
 */
native Handle:json_object_iter_at(Handle:hObj, const String:key[]);

/**
 * Returns an iterator pointing to the next key-value pair in object.
 * This automatically closes the Handle to the iterator hIter.
 *
 * @param hObj              Handle to JSON object.
 * @param hIter             Handle to JSON object iterator.
 *
 * @return                  Handle to JSON object iterator,
 *                          or INVALID_HANDLE on error, or if the
 *                          whole object has been iterated through.
 */
native Handle:json_object_iter_next(Handle:hObj, Handle:hIter);

/**
 * Extracts the associated key of hIter as a null terminated UTF-8
 * encoded string in the passed buffer.
 *
 * @param hIter             Handle to the JSON String object
 * @param sKeyBuffer        Buffer to store the value of the String.
 * @param maxlength         Maximum length of string buffer.
 * @error                   Invalid JSON Object Iterator.
 * @return                  Length of the returned string or -1 on error.
 */
native json_object_iter_key(Handle:hIter, String:sKeyBuffer[], maxlength);

/**
 * Returns a handle to the value hIter is pointing at.
 *
 * @param hIter             Handle to JSON object iterator.
 *
 * @return                  Handle to value or INVALID_HANDLE on error.
 */
native Handle:json_object_iter_value(Handle:hIter);

/**
 * Set the value of the key-value pair in hObj, that is pointed to
 * by hIter, to hValue.
 *
 * @param hObj              Handle to JSON object.
 * @param hIter             Handle to JSON object iterator.
 * @param hValue            Handle to JSON value.
 *
 * @return                  True on success.
 */
native bool:json_object_iter_set(Handle:hObj, Handle:hIter, Handle:hValue);

/**
 * Set the value of the key-value pair in hObj, that is pointed to
 * by hIter, to hValue.
 * This function automatically closes the Handle to the value object.
 *
 * @param hObj              Handle to JSON object.
 * @param hIter             Handle to JSON object iterator.
 * @param hValue            Handle to JSON value.
 *
 * @return                  True on success.
 */
native bool:json_object_iter_set_new(Handle:hObj, Handle:hIter, Handle:hValue);




/**
 * Arrays
 *
 * A JSON array is an ordered collection of other JSON values.
 *
 */

/**
 * Returns a handle to a new JSON array, or INVALID_HANDLE on error.
 *
 * @return                  Handle to the new JSON array
 */
native Handle:json_array();

/**
 * Returns the number of elements in hArray
 *
 * @param hObj              Handle to JSON array
 *
 * @return                  Number of elements in hArray,
 *                          or 0 if hObj is not a JSON array.
 */
native json_array_size(Handle:hArray);

/**
 * Returns the element in hArray at position iIndex.
 *
 * @param hArray            Handle to JSON array to get a value from
 * @param iIndex            Position to retrieve
 *
 * @return                  Handle to a the JSON object or
 *                          INVALID_HANDLE on error.
 */
native Handle:json_array_get(Handle:hArray, iIndex);

/**
 * Replaces the element in array at position iIndex with hValue.
 * The valid range for iIndex is from 0 to the return value of
 * json_array_size() minus 1.
 *
 * @param hArray            Handle to JSON array
 * @param iIndex            Position to replace
 * @param hValue            Value to store in the array
 *
 * @return					True on success.
 */
native bool:json_array_set(Handle:hArray, iIndex, Handle:hValue);

/**
 * Replaces the element in array at position iIndex with hValue.
 * The valid range for iIndex is from 0 to the return value of
 * json_array_size() minus 1.
 * This function automatically closes the Handle to the value object.
 *
 * @param hArray            Handle to JSON array
 * @param iIndex            Position to replace
 * @param hValue            Value to store in the array
 *
 * @return					True on success.
 */
native bool:json_array_set_new(Handle:hArray, iIndex, Handle:hValue);

/**
 * Appends value to the end of array, growing the size of array by 1.
 *
 * @param hArray            Handle to JSON array
 * @param hValue            Value to append to the array
 *
 * @return					True on success.
 */
native bool:json_array_append(Handle:hArray, Handle:hValue);

/**
 * Appends value to the end of array, growing the size of array by 1.
 * This function automatically closes the Handle to the value object.
 *
 * @param hArray            Handle to JSON array
 * @param hValue            Value to append to the array
 *
 * @return					True on success.
 */
native bool:json_array_append_new(Handle:hArray, Handle:hValue);

/**
 * Inserts value to hArray at position iIndex, shifting the elements at
 * iIndex and after it one position towards the end of the array.
 *
 * @param hArray            Handle to JSON array
 * @param iIndex            Position to insert at
 * @param hValue            Value to store in the array
 *
 * @return					True on success.
 */
native bool:json_array_insert(Handle:hArray, iIndex, Handle:hValue);

/**
 * Inserts value to hArray at position iIndex, shifting the elements at
 * iIndex and after it one position towards the end of the array.
 * This function automatically closes the Handle to the value object.
 *
 * @param hArray            Handle to JSON array
 * @param iIndex            Position to insert at
 * @param hValue            Value to store in the array
 *
 * @return					True on success.
 */
native bool:json_array_insert_new(Handle:hArray, iIndex, Handle:hValue);

/**
 * Removes the element in hArray at position iIndex, shifting the
 * elements after iIndex one position towards the start of the array.
 *
 * @param hArray            Handle to JSON array
 * @param iIndex            Position to insert at
 *
 * @return					True on success.
 */
native bool:json_array_remove(Handle:hArray, iIndex);

/**
 * Removes all elements from hArray.
 *
 * @param hArray            Handle to JSON array
 *
 * @return					True on success.
 */
native bool:json_array_clear(Handle:hArray);

/**
 * Appends all elements in hOther to the end of hArray.
 *
 * @param hArray            Handle to JSON array to be extended
 * @param hOther            Handle to JSON array, source to copy from
 *
 * @return					True on success.
 */
native bool:json_array_extend(Handle:hArray, Handle:hOther);




/**
 * Strings
 *
 * Jansson uses UTF-8 as the character encoding. All JSON strings must
 * be valid UTF-8 (or ASCII, as it�s a subset of UTF-8). Normal null
 * terminated C strings are used, so JSON strings may not contain
 * embedded null characters.
 *
 */

/**
 * Returns a handle to a new JSON string, or INVALID_HANDLE on error.
 *
 * @param sValue            Value for the new String object
 *							Must be a valid UTF-8 encoded Unicode string.
 * @return                  Handle to the new String object
 */
native Handle:json_string(String:sValue[]);

/**
 * Saves the associated value of hString as a null terminated UTF-8
 * encoded string in the passed buffer.
 *
 * @param hString           Handle to the JSON String object
 * @param sValueBuffer      Buffer to store the value of the String.
 * @param maxlength         Maximum length of string buffer.
 * @error                   Invalid JSON String Object.
 * @return                  Length of the returned string or -1 on error.
 */
native json_string_value(Handle:hString, String:sValueBuffer[], maxlength);

/**
 * Sets the associated value of JSON String object to value.
 *
 * @param hString           Handle to the JSON String object
 * @param sValue            Value to set the object to.
 *                          Must be a valid UTF-8 encoded Unicode string.
 * @error                   Invalid JSON String Object.
 * @return                  True on success.
 */
native bool:json_string_set(Handle:hString, String:sValue[]);




/**
 * Numbers
 *
 * The JSON specification only contains one numeric type, 'number'.
 * The C (and Pawn) programming language has distinct types for integer
 * and floating-point numbers, so for practical reasons Jansson also has
 * distinct types for the two. They are called 'integer' and 'real',
 * respectively. (Whereas 'real' is a 'Float' for Pawn).
 * Therefore a number is represented by either a value of the type
 * JSON_INTEGER or of the type JSON_REAL.
 *
 */

/**
 * Returns a handle to a new JSON integer, or INVALID_HANDLE on error.
 *
 * @param iValue            Value for the new Integer object
 * @return                  Handle to the new Integer object
 */
native Handle:json_integer(iValue);

/**
 * Returns the associated value of a JSON Integer Object.
 *
 * @param hInteger          Handle to the JSON Integer object
 * @error                   Invalid JSON Integer Object.
 * @return                  Value of the hInteger,
 *                          or 0 if hInteger is not a JSON integer.
 */
native json_integer_value(Handle:hInteger);

/**
 * Sets the associated value of JSON Integer to value.
 *
 * @param hInteger          Handle to the JSON Integer object
 * @param iValue            Value to set the object to.
 * @error                   Invalid JSON Integer Object.
 * @return                  True on success.
 */
native bool:json_integer_set(Handle:hInteger, iValue);

/**
 * Returns a handle to a new JSON real, or INVALID_HANDLE on error.
 *
 * @param fValue            Value for the new Real object
 * @return                  Handle to the new String object
 */
native Handle:json_real(Float:fValue);

/**
 * Returns the associated value of a JSON Real.
 *
 * @param hReal             Handle to the JSON Real object
 * @error                   Invalid JSON Real Object.
 * @return                  Float value of hReal,
 *                          or 0.0 if hReal is not a JSON Real.
 */
native Float:json_real_value(Handle:hReal);

/**
 * Sets the associated value of JSON Real to fValue.
 *
 * @param hReal             Handle to the JSON Integer object
 * @param fValue            Value to set the object to.
 * @error                   Invalid JSON Real handle.
 * @return                  True on success.
 */
native bool:json_real_set(Handle:hReal, Float:value);

/**
 * Returns the associated value of a JSON integer or a
 * JSON Real, cast to Float regardless of the actual type.
 *
 * @param hNumber           Handle to the JSON Number
 * @error                   Not a JSON Real or JSON Integer
 * @return                  Float value of hNumber,
 *                          or 0.0 on error.
 */
native Float:json_number_value(Handle:hNumber);




/**
 * Decoding
 *
 * This sections describes the functions that can be used to decode JSON text
 * to the Jansson representation of JSON data. The JSON specification requires
 * that a JSON text is either a serialized array or object, and this
 * requirement is also enforced with the following functions. In other words,
 * the top level value in the JSON text being decoded must be either array or
 * object.
 *
 */

/**
 * Decodes the JSON string sJSON and returns the array or object it contains.
 *
 * @param sJSON             String containing valid JSON

 * @return                  Handle to JSON object or array.
 *                          or INVALID_HANDLE on error.
 */
native Handle:json_load(const String:sJSON[]);

/**
 * Decodes the JSON text in file sFilePath and returns the array or object
 * it contains
 *
 * @param sFilePath         Path to a file containing pure JSON

 * @return                  Handle to JSON object or array.
 *                          or INVALID_HANDLE on error.
 */
native Handle:json_load_file(const String:sFilePath[PLATFORM_MAX_PATH]);




/**
 * Encoding
 *
 * This sections describes the functions that can be used to encode values
 * to JSON. By default, only objects and arrays can be encoded directly,
 * since they are the only valid root values of a JSON text.
 *
 */

/**
 * Saves the JSON representation of hObject in sJSON.
 *
 * @param hObject           String containing valid JSON
 * @param sJSON             Buffer to store the created JSON string.
 * @param maxlength         Maximum length of string buffer.
 * @param iIndentWidth      Indenting with iIndentWidth spaces.
 *                          The valid range for this is between 0 and 31 (inclusive),
 *                          other values result in an undefined output. If this is set
 *                          to 0, no newlines are inserted between array and object items.
 * @param bEnsureAscii      If this is set, the output is guaranteed
 *                          to consist only of ASCII characters. This is achieved
 *                          by escaping all Unicode characters outside the ASCII range.
 * @param bSortKeys         If this flag is used, all the objects in output are sorted
 *                          by key. This is useful e.g. if two JSON texts are diffed
 *                          or visually compared.
 * @param bPreserveOrder    If this flag is used, object keys in the output are sorted
 *                          into the same order in which they were first inserted to
 *                          the object. For example, decoding a JSON text and then
 *                          encoding with this flag preserves the order of object keys.
 * @return                  Length of the returned string or -1 on error.
 */
native json_dump(Handle:hObject, String:sJSON[], maxlength, iIndentWidth = 4, bool:bEnsureAscii = false, bool:bSortKeys = false, bool:bPreserveOrder = false);

/**
 * Write the JSON representation of hObject to the file sFilePath.
 * If sFilePath already exists, it is overwritten.
 *
 * @param hObject           String containing valid JSON
 * @param sFilePath         Buffer to store the created JSON string.
 * @param iIndentWidth      Indenting with iIndentWidth spaces.
 *                          The valid range for this is between 0 and 31 (inclusive),
 *                          other values result in an undefined output. If this is set
 *                          to 0, no newlines are inserted between array and object items.
 * @param bEnsureAscii      If this is set, the output is guaranteed
 *                          to consist only of ASCII characters. This is achieved
 *                          by escaping all Unicode characters outside the ASCII range.
 * @param bSortKeys         If this flag is used, all the objects in output are sorted
 *                          by key. This is useful e.g. if two JSON texts are diffed
 *                          or visually compared.
 * @param bPreserveOrder    If this flag is used, object keys in the output are sorted
 *                          into the same order in which they were first inserted to
 *                          the object. For example, decoding a JSON text and then
 *                          encoding with this flag preserves the order of object keys.
 * @return                  Length of the returned string or -1 on error.
 */
native bool:json_dump_file(Handle:hObject, const String:sFilePath[], iIndentWidth = 4, bool:bEnsureAscii = false, bool:bSortKeys = false, bool:bPreserveOrder = false);





/**
 * Not yet implemented
 *
 * native json_object_foreach(Handle:hObj, ForEachCallback:cb);
 * native Handle:json_pack(const String:sFormat[], ...);
 * native Handle:json_unpack(const String:sFormat[], ...);
 *
 */






/**
 * Do not edit below this line!
 */
public Extension:__ext_smjansson =
{
	name = "SMJansson",
	file = "smjansson.ext",
#if defined AUTOLOAD_EXTENSIONS
	autoload = 1,
#else
	autoload = 0,
#endif
#if defined REQUIRE_EXTENSIONS
	required = 1,
#else
	required = 0,
#endif
};

#if !defined REQUIRE_EXTENSIONS
public __ext_smjansson_SetNTVOptional()
{
	MarkNativeAsOptional("json_typeof");
	MarkNativeAsOptional("json_equal");

	MarkNativeAsOptional("json_copy");
	MarkNativeAsOptional("json_deep_copy");

	MarkNativeAsOptional("json_object");
	MarkNativeAsOptional("json_object_size");
	MarkNativeAsOptional("json_object_get");
	MarkNativeAsOptional("json_object_set");
	MarkNativeAsOptional("json_object_del");
	MarkNativeAsOptional("json_object_clear");
	MarkNativeAsOptional("json_object_update");
	MarkNativeAsOptional("json_object_update_existing");
	MarkNativeAsOptional("json_object_update_missing");

	MarkNativeAsOptional("json_object_iter");
	MarkNativeAsOptional("json_object_iter_at");
	MarkNativeAsOptional("json_object_iter_next");
	MarkNativeAsOptional("json_object_iter_key");
	MarkNativeAsOptional("json_object_iter_value");
	MarkNativeAsOptional("json_object_iter_set");

	MarkNativeAsOptional("json_array");
	MarkNativeAsOptional("json_array_size");
	MarkNativeAsOptional("json_array_get");
	MarkNativeAsOptional("json_array_set");
	MarkNativeAsOptional("json_array_append");
	MarkNativeAsOptional("json_array_insert");
	MarkNativeAsOptional("json_array_remove");
	MarkNativeAsOptional("json_array_clear");
	MarkNativeAsOptional("json_array_extend");

	MarkNativeAsOptional("json_string");
	MarkNativeAsOptional("json_string_value");
	MarkNativeAsOptional("json_string_set");

	MarkNativeAsOptional("json_integer");
	MarkNativeAsOptional("json_integer_value");
	MarkNativeAsOptional("json_integer_set");

	MarkNativeAsOptional("json_real");
	MarkNativeAsOptional("json_real_value");
	MarkNativeAsOptional("json_real_set");
	MarkNativeAsOptional("json_number_value");

	MarkNativeAsOptional("json_load");
	MarkNativeAsOptional("json_load_file");

	MarkNativeAsOptional("json_dump");
	MarkNativeAsOptional("json_dump_file");
}
#endif