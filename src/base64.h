/*! \file base64.h
 *  \brief functions for decoding and encoding strings with base64
 *
 */
# pragma once

#include <stdint.h>
#include <stdlib.h>

/*!
 * builds the decoding table ones.
 * only executed if no table available
 * @attention free the table.
 */
void build_decoding_table1();
/*!
 * encodes the String *data with input_length and returns the pointer *encoded_data with *output_length
 *
 *
 *
 * @param const unsigned char *data data which is encoded
 * @param size_t input_length length of the encoded data
 * @param size_t *output_length pinter which returns the length of the encoded String
 * @return char *encoded_data encoded data which is returned
 */
char *base64_encode(const unsigned char *, size_t, size_t *);

/*!
 * decodes the String *data with input_length and returnes the pointer *decoded_data with *output_length
 *
 *
 * @attention the decoded string is not null ('\o') terminated
 * @param const unsigned char *data data which is decoded
 * @param size_t input_length length of the decoded data
 * @param size_t *output_length pinter which returns the length of the decoded String
 * @return char *encoded_data decoded Data which is returned
 */
unsigned char *base64_decode(const char *data, size_t, size_t *);

/*!
 * frees the decoding table
 * @attention should only be used once at the end of the programm shutdown
 */
void base64_cleanup();