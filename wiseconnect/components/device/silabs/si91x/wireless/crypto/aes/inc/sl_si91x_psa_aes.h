/*******************************************************************************
 * @file  sl_si91x_psa_aes.h
 * @brief
 *******************************************************************************
 * # License
 * <b>Copyright 2025 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#ifndef SL_SI91X_PSA_AES_H
#define SL_SI91X_PSA_AES_H

#include "sli_si91x_crypto_driver_functions.h"
#include "string.h"
#include "sl_status.h"
#include "sl_si91x_crypto.h"
#include "sl_si91x_aes.h"

typedef struct {
  psa_algorithm_t alg;                         //!< Algorithm to be used for the operation
  sli_si91x_psa_aes_multipart_config_t config; //!< Configuration for the operation
} sli_si91x_crypto_cipher_operation_t;

/** Encrypt a message using a AES cipher.
 * \note The signature of this function is that of a PSA driver
 *       cipher_encrypt entry point. This function behaves as a
 *       cipher_encrypt entry point as defined in the PSA driver
 *       interface specification for transparent drivers.
 *
 * \param[in] attributes        The attributes of the key to use for the
 *                              operation.
 * \param[in] key_buffer        The buffer containing the key context.
 * \param[in] key_buffer_size   Size of the \p key_buffer buffer in bytes.
 * \param[in] alg               The cipher algorithm to compute
 *                              (\c PSA_ALG_XXX value such that
 *                              PSA_ALG_IS_CIPHER(\p alg) is true).
 * \param[in] iv                Buffer containing the IV for encryption. The
 *                              IV has been generated by the core.
 * \param[in] iv_length         Size of the \p iv in bytes.
 * \param[in] input             Buffer containing the message to encrypt.
 * \param[in] input_length      Size of the \p input buffer in bytes.
 * \param[in,out] output        Buffer where the output is to be written.
 * \param[in]  output_size      Size of the \p output buffer in bytes.
 * \param[out] output_length    On success, the number of bytes that make up
 *                              the returned output. Initialized to zero
 *                              by the core.
 *
 * \retval PSA_SUCCESS
 * \retval PSA_ERROR_NOT_SUPPORTED
 * \retval PSA_ERROR_INSUFFICIENT_MEMORY
 * \retval PSA_ERROR_CORRUPTION_DETECTED
 * \retval PSA_ERROR_BUFFER_TOO_SMALL
 *         The size of the \p output buffer is too small.
 * \retval PSA_ERROR_INVALID_ARGUMENT
 *         The size \p iv_length is not acceptable for the chosen algorithm,
 *         or the chosen algorithm does not use an IV.
 *         The total input size passed to this operation is not valid for
 *         this particular algorithm. For example, the algorithm is a based
 *         on block cipher and requires a whole number of blocks, but the
 *         total input size is not a multiple of the block size.
 * \retval PSA_ERROR_INVALID_PADDING
 *         This is a decryption operation for an algorithm that includes
 *         padding, and the ciphertext does not contain valid padding.
 */
psa_status_t sli_si91x_crypto_cipher_encrypt(const psa_key_attributes_t *attributes,
                                             const uint8_t *key_buffer,
                                             size_t key_buffer_size,
                                             psa_algorithm_t alg,
                                             const uint8_t *iv,
                                             size_t iv_length,
                                             const uint8_t *input,
                                             size_t input_length,
                                             uint8_t *output,
                                             size_t output_size,
                                             size_t *output_length);

/** Decrypt a message using a AES cipher.
 *
 * \note The signature of this function is that of a PSA driver
 *       cipher_encrypt entry point. This function behaves as a
 *       cipher_encrypt entry point as defined in the PSA driver
 *       interface specification for transparent drivers.
 *
 * \param[in]  attributes       The attributes of the key to use for the
 *                              operation.
 * \param[in]  key_buffer       The buffer containing the key context.
 * \param[in]  key_buffer_size  Size of the \p key_buffer buffer in bytes.
 * \param[in]  alg              The cipher algorithm to compute
 *                              (\c PSA_ALG_XXX value such that
 *                              PSA_ALG_IS_CIPHER(\p alg) is true).
 * \param[in]  input            Buffer containing the message to decrypt.
 * \param[in]  input_length     Size of the \p input buffer in bytes.
 * \param[out] output           Buffer where the output is to be written.
 * \param[in]  output_size      Size of the \p output buffer in bytes.
 * \param[out] output_length    On success, the number of bytes that make up
 *                              the returned output. Initialized to zero
 *                              by the core.
 *
 * \retval PSA_SUCCESS
 * \retval PSA_ERROR_NOT_SUPPORTED
 * \retval PSA_ERROR_INSUFFICIENT_MEMORY
 * \retval PSA_ERROR_CORRUPTION_DETECTED
 * \retval PSA_ERROR_BUFFER_TOO_SMALL
 *         The size of the \p output buffer is too small.
 * \retval PSA_ERROR_INVALID_ARGUMENT
 *         The size \p iv_length is not acceptable for the chosen algorithm,
 *         or the chosen algorithm does not use an IV.
 *         The total input size passed to this operation is not valid for
 *         this particular algorithm. For example, the algorithm is a based
 *         on block cipher and requires a whole number of blocks, however the
 *         total input size is not a multiple of the block size.
 * \retval PSA_ERROR_INVALID_PADDING
 *         This is a decryption operation for an algorithm which includes
 *         padding, and the ciphertext does not contain valid padding.
 */
psa_status_t sli_si91x_crypto_cipher_decrypt(const psa_key_attributes_t *attributes,
                                             const uint8_t *key_buffer,
                                             size_t key_buffer_size,
                                             psa_algorithm_t alg,
                                             const uint8_t *input,
                                             size_t input_length,
                                             uint8_t *output,
                                             size_t output_size,
                                             size_t *output_length);

/** Set the key for a multipart symmetric encryption operation.
 *
 * \param[in,out] operation     The operation object to set up. It must have
 *                              been initialized as per the documentation for
 *                              #psa_cipher_operation_t and not yet in use.
 * \param attributes            Attributes of the key
 *                              It must allow the usage #PSA_KEY_USAGE_ENCRYPT.
 * \param key_buffer            Buffer containing key data
 * \param key_buffer_size       Size of key buffer in bytes
 * \param alg                   The cipher algorithm to compute
 *                              (\c PSA_ALG_XXX value such that
 *                              #PSA_ALG_IS_CIPHER(\p alg) is true).
 *
 * \retval #PSA_SUCCESS
 *         Success.
 * \retval #PSA_ERROR_INVALID_HANDLE \emptydescription
 * \retval #PSA_ERROR_NOT_PERMITTED \emptydescription
 * \retval #PSA_ERROR_INVALID_ARGUMENT
 *         \p key is not compatible with \p alg.
 * \retval #PSA_ERROR_NOT_SUPPORTED
 *         \p alg is not supported or is not a cipher algorithm.
 * \retval #PSA_ERROR_INSUFFICIENT_MEMORY \emptydescription
 * \retval #PSA_ERROR_COMMUNICATION_FAILURE \emptydescription
 * \retval #PSA_ERROR_HARDWARE_FAILURE \emptydescription
 * \retval #PSA_ERROR_CORRUPTION_DETECTED \emptydescription
 * \retval #PSA_ERROR_STORAGE_FAILURE \emptydescription
 * \retval #PSA_ERROR_BAD_STATE.
 */
psa_status_t sli_si91x_crypto_cipher_encrypt_setup(sli_si91x_crypto_cipher_operation_t *operation,
                                                   const psa_key_attributes_t *attributes,
                                                   const uint8_t *key_buffer,
                                                   size_t key_buffer_size,
                                                   psa_algorithm_t alg);

/** Set the key for a multipart symmetric decryption operation.
 *
 * \param[in,out] operation     The operation object to set up. It must have
 *                              been initialized as per the documentation for
 *                              #psa_cipher_operation_t and not yet in use.
 * \param attributes            Attributes of the key
 *                              It must allow the usage #PSA_KEY_USAGE_DECRYPT.
 * \param key_buffer            Buffer containing key data
 * \param key_buffer_size       Size of key buffer in bytes
 * \param alg                   The cipher algorithm to compute
 *                              (\c PSA_ALG_XXX value such that
 *                              #PSA_ALG_IS_CIPHER(\p alg) is true).
 *
 * \retval #PSA_SUCCESS
 *         Success.
 * \retval #PSA_ERROR_INVALID_HANDLE \emptydescription
 * \retval #PSA_ERROR_NOT_PERMITTED \emptydescription
 * \retval #PSA_ERROR_INVALID_ARGUMENT
 *         \p key is not compatible with \p alg.
 * \retval #PSA_ERROR_NOT_SUPPORTED
 *         \p alg is not supported or is not a cipher algorithm.
 * \retval #PSA_ERROR_INSUFFICIENT_MEMORY \emptydescription
 * \retval #PSA_ERROR_COMMUNICATION_FAILURE \emptydescription
 * \retval #PSA_ERROR_HARDWARE_FAILURE \emptydescription
 * \retval #PSA_ERROR_CORRUPTION_DETECTED \emptydescription
 * \retval #PSA_ERROR_STORAGE_FAILURE \emptydescription
 * \retval #PSA_ERROR_BAD_STATE
 */
psa_status_t sli_si91x_crypto_cipher_decrypt_setup(sli_si91x_crypto_cipher_operation_t *operation,
                                                   const psa_key_attributes_t *attributes,
                                                   const uint8_t *key_buffer,
                                                   size_t key_buffer_size,
                                                   psa_algorithm_t alg);

/** Set the IV for a symmetric encryption or decryption operation.
 *
 * This function sets the IV (initialization vector), nonce
 * or initial counter value for the encryption or decryption operation.
 *
 * \param[in,out] operation     Active cipher operation.
 * \param[in] iv                Buffer containing the IV to use.
 * \param iv_length             Size of the IV in bytes.
 *
 * \retval #PSA_SUCCESS
 *         Success.
 * \retval #PSA_ERROR_INVALID_ARGUMENT
 *         The size of \p iv is not acceptable for the chosen algorithm,
 *         or the chosen algorithm does not use an IV.
 * \retval #PSA_ERROR_INSUFFICIENT_MEMORY \emptydescription
 * \retval #PSA_ERROR_COMMUNICATION_FAILURE \emptydescription
 * \retval #PSA_ERROR_HARDWARE_FAILURE \emptydescription
 * \retval #PSA_ERROR_CORRUPTION_DETECTED \emptydescription
 * \retval #PSA_ERROR_STORAGE_FAILURE \emptydescription
 * \retval #PSA_ERROR_BAD_STATE
 *         The operation state is not valid (it must be an active cipher
 *         encrypt operation, with no IV set)
 */
psa_status_t sli_si91x_crypto_cipher_set_iv(sli_si91x_crypto_cipher_operation_t *operation,
                                            const uint8_t *iv,
                                            size_t iv_length);

/** Encrypt or decrypt a message fragment in an active cipher operation.
 *
 * \param[in,out] operation     Active cipher operation.
 * \param[in] input             Buffer containing the message fragment to
 *                              encrypt or decrypt.
 * \param input_length          Size of the \p input buffer in bytes.
 * \param[out] output           Buffer where the output is to be written.
 * \param output_size           Size of the \p output buffer in bytes.
 * \param[out] output_length    On success, the number of bytes
 *                              that make up the returned output.
 *
 * \retval #PSA_SUCCESS
 *         Success.
 * \retval #PSA_ERROR_BUFFER_TOO_SMALL
 *         The size of the \p output buffer is too small.
 * \retval #PSA_ERROR_INSUFFICIENT_MEMORY \emptydescription
 * \retval #PSA_ERROR_COMMUNICATION_FAILURE \emptydescription
 * \retval #PSA_ERROR_HARDWARE_FAILURE \emptydescription
 * \retval #PSA_ERROR_CORRUPTION_DETECTED \emptydescription
 * \retval #PSA_ERROR_STORAGE_FAILURE \emptydescription
 * \retval #PSA_ERROR_BAD_STATE
 *         The operation state is not valid (it must be active, with an IV set
 *         if required for the algorithm)
 */
psa_status_t sli_si91x_crypto_cipher_update(sli_si91x_crypto_cipher_operation_t *operation,
                                            const uint8_t *input,
                                            size_t input_length,
                                            uint8_t *output,
                                            size_t output_size,
                                            size_t *output_length);

/** Finish encrypting or decrypting a message in a cipher operation.
 *
 * \param[in,out] operation     Active cipher operation.
 * \param[out] output           Buffer where the output is to be written.
 * \param output_size           Size of the \p output buffer in bytes.
 * \param[out] output_length    On success, the number of bytes
 *                              that make up the returned output.
 *
 * \retval #PSA_SUCCESS
 *         Success.
 * \retval #PSA_ERROR_INVALID_ARGUMENT
 *         The total input size passed to this operation is not valid for
 *         this particular algorithm. For example, the algorithm is a based
 *         on block cipher and requires a whole number of blocks, but the
 *         total input size is not a multiple of the block size.
 * \retval #PSA_ERROR_INVALID_PADDING
 *         This is a decryption operation for an algorithm that includes
 *         padding, and the ciphertext does not contain valid padding.
 * \retval #PSA_ERROR_BUFFER_TOO_SMALL
 *         The size of the \p output buffer is too small.
 * \retval #PSA_ERROR_INSUFFICIENT_MEMORY \emptydescription
 * \retval #PSA_ERROR_COMMUNICATION_FAILURE \emptydescription
 * \retval #PSA_ERROR_HARDWARE_FAILURE \emptydescription
 * \retval #PSA_ERROR_CORRUPTION_DETECTED \emptydescription
 * \retval #PSA_ERROR_STORAGE_FAILURE \emptydescription
 * \retval #PSA_ERROR_BAD_STATE
 *         The operation state is not valid (it must be active, with an IV set
 *         if required for the algorithm)
 */
psa_status_t sli_si91x_crypto_cipher_finish(sli_si91x_crypto_cipher_operation_t *operation,
                                            uint8_t *output,
                                            size_t output_size,
                                            size_t *output_length);

/** Abort a cipher operation.
 *
 * Aborting an operation frees all associated resources except for the
 * \p operation structure itself.
 *
 * \param[in,out] operation     Initialized cipher operation.
 *
 * \retval #PSA_SUCCESS \emptydescription
 * \retval #PSA_ERROR_COMMUNICATION_FAILURE \emptydescription
 * \retval #PSA_ERROR_HARDWARE_FAILURE \emptydescription
 * \retval #PSA_ERROR_CORRUPTION_DETECTED \emptydescription
 * \retval #PSA_ERROR_BAD_STATE
 */
psa_status_t sli_si91x_crypto_cipher_abort(sli_si91x_crypto_cipher_operation_t *operation);

#endif /* SL_SI91X_PSA_AES_H */