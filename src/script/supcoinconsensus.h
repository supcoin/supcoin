// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SUPCOIN_SUPCOINCONSENSUS_H
#define SUPCOIN_SUPCOINCONSENSUS_H

#if defined(BUILD_SUPCOIN_INTERNAL) && defined(HAVE_CONFIG_H)
#include "config/supcoin-config.h"
  #if defined(_WIN32)
    #if defined(DLL_EXPORT)
      #if defined(HAVE_FUNC_ATTRIBUTE_DLLEXPORT)
        #define EXPORT_SYMBOL __declspec(dllexport)
      #else
        #define EXPORT_SYMBOL
      #endif
    #endif
  #elif defined(HAVE_FUNC_ATTRIBUTE_VISIBILITY)
    #define EXPORT_SYMBOL __attribute__ ((visibility ("default")))
  #endif
#elif defined(MSC_VER) && !defined(STATIC_LIBSUPCOINCONSENSUS)
  #define EXPORT_SYMBOL __declspec(dllimport)
#endif

#ifndef EXPORT_SYMBOL
  #define EXPORT_SYMBOL
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define SUPCOINCONSENSUS_API_VER 0

typedef enum supcoinconsensus_error_t
{
    supcoinconsensus_ERR_OK = 0,
    supcoinconsensus_ERR_TX_INDEX,
    supcoinconsensus_ERR_TX_SIZE_MISMATCH,
    supcoinconsensus_ERR_TX_DESERIALIZE,
} supcoinconsensus_error;

/** Script verification flags */
enum
{
    supcoinconsensus_SCRIPT_FLAGS_VERIFY_NONE      = 0,
    supcoinconsensus_SCRIPT_FLAGS_VERIFY_P2SH      = (1U << 0), // evaluate P2SH (BIP16) subscripts
    supcoinconsensus_SCRIPT_FLAGS_VERIFY_DERSIG    = (1U << 2), // enforce strict DER (BIP66) compliance
};

/// Returns 1 if the input nIn of the serialized transaction pointed to by
/// txTo correctly spends the scriptPubKey pointed to by scriptPubKey under
/// the additional constraints specified by flags.
/// If not NULL, err will contain an error/success code for the operation
EXPORT_SYMBOL int supcoinconsensus_verify_script(const unsigned char *scriptPubKey, unsigned int scriptPubKeyLen,
                                    const unsigned char *txTo        , unsigned int txToLen,
                                    unsigned int nIn, unsigned int flags, supcoinconsensus_error* err);

EXPORT_SYMBOL unsigned int supcoinconsensus_version();

#ifdef __cplusplus
} // extern "C"
#endif

#undef EXPORT_SYMBOL

#endif // SUPCOIN_SUPCOINCONSENSUS_H
