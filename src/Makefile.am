DIST_SUBDIRS = secp256k1
AM_LDFLAGS = $(PTHREAD_CFLAGS) $(LIBTOOL_LDFLAGS)


if EMBEDDED_LEVELDB
LEVELDB_CPPFLAGS += -I$(srcdir)/leveldb/include
LEVELDB_CPPFLAGS += -I$(srcdir)/leveldb/helpers/memenv
LIBLEVELDB += $(builddir)/leveldb/libleveldb.a
LIBMEMENV  += $(builddir)/leveldb/libmemenv.a

# NOTE: This dependency is not strictly necessary, but without it make may try to build both in parallel, which breaks the LevelDB build system in a race
$(LIBLEVELDB): $(LIBMEMENV)

$(LIBLEVELDB) $(LIBMEMENV):
	@echo "Building LevelDB ..." && $(MAKE) -C $(@D) $(@F) CXX="$(CXX)" \
	  CC="$(CC)" PLATFORM=$(TARGET_OS) AR="$(AR)" $(LEVELDB_TARGET_FLAGS) \
          OPT="$(CXXFLAGS) $(CPPFLAGS)"
endif

SUPCOIN_CONFIG_INCLUDES=-I$(builddir)/config
SUPCOIN_INCLUDES=-I$(builddir) -I$(builddir)/obj $(BOOST_CPPFLAGS) $(LEVELDB_CPPFLAGS) $(CRYPTO_CFLAGS) $(SSL_CFLAGS)

SUPCOIN_INCLUDES += -I$(srcdir)/secp256k1/include

LIBSUPCOIN_SERVER=libsupcoin_server.a
LIBSUPCOIN_WALLET=libsupcoin_wallet.a
LIBSUPCOIN_COMMON=libsupcoin_common.a
LIBSUPCOIN_CLI=libsupcoin_cli.a
LIBSUPCOIN_UTIL=libsupcoin_util.a
LIBSUPCOIN_CRYPTO=crypto/libsupcoin_crypto.a
LIBSUPCOIN_UNIVALUE=univalue/libsupcoin_univalue.a
LIBSUPCOINQT=qt/libsupcoinqt.a
LIBSECP256K1=secp256k1/libsecp256k1.la

$(LIBSECP256K1): $(wildcard secp256k1/src/*) $(wildcard secp256k1/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

# Make is not made aware of per-object dependencies to avoid limiting building parallelization
# But to build the less dependent modules first, we manually select their order here:
EXTRA_LIBRARIES = \
  crypto/libsupcoin_crypto.a \
  libsupcoin_util.a \
  libsupcoin_common.a \
  univalue/libsupcoin_univalue.a \
  libsupcoin_server.a \
  libsupcoin_cli.a
if ENABLE_WALLET
SUPCOIN_INCLUDES += $(BDB_CPPFLAGS)
EXTRA_LIBRARIES += libsupcoin_wallet.a
endif

if BUILD_SUPCOIN_LIBS
lib_LTLIBRARIES = libsupcoinconsensus.la
LIBSUPCOIN_CONSENSUS=libsupcoinconsensus.la
else
LIBSUPCOIN_CONSENSUS=
endif

bin_PROGRAMS =
TESTS =

if BUILD_SUPCOIND
  bin_PROGRAMS += supcoind
endif

if BUILD_SUPCOIN_UTILS
  bin_PROGRAMS += supcoin-cli supcoin-tx
endif

.PHONY: FORCE
# supcoin core #
SUPCOIN_CORE_H = \
  addrman.h \
  alert.h \
  allocators.h \
  amount.h \
  base58.h \
  bloom.h \
  chain.h \
  chainparams.h \
  chainparamsbase.h \
  chainparamsseeds.h \
  checkpoints.h \
  checkqueue.h \
  clientversion.h \
  coincontrol.h \
  coins.h \
  compat.h \
  compressor.h \
  primitives/block.h \
  primitives/transaction.h \
  core_io.h \
  crypter.h \
  db.h \
  eccryptoverify.h \
  ecwrapper.h \
  hash.h \
  init.h \
  key.h \
  keystore.h \
  leveldbwrapper.h \
  limitedmap.h \
  main.h \
  merkleblock.h \
  miner.h \
  mruset.h \
  netbase.h \
  net.h \
  noui.h \
  pow.h \
  protocol.h \
  pubkey.h \
  random.h \
  rpcclient.h \
  rpcprotocol.h \
  rpcserver.h \
  script/interpreter.h \
  script/script.h \
  script/sigcache.h \
  script/sign.h \
  script/standard.h \
  script/script_error.h \
  serialize.h \
  streams.h \
  sync.h \
  threadsafety.h \
  timedata.h \
  tinyformat.h \
  txdb.h \
  txmempool.h \
  ui_interface.h \
  uint256.h \
  undo.h \
  util.h \
  utilstrencodings.h \
  utilmoneystr.h \
  utiltime.h \
  version.h \
  wallet.h \
  wallet_ismine.h \
  compat/byteswap.h \
  walletdb.h \
  compat/endian.h \
  compat/sanity.h

JSON_H = \
  json/json_spirit.h \
  json/json_spirit_error_position.h \
  json/json_spirit_reader.h \
  json/json_spirit_reader_template.h \
  json/json_spirit_stream_reader.h \
  json/json_spirit_utils.h \
  json/json_spirit_value.h \
  json/json_spirit_writer.h \
  json/json_spirit_writer_template.h

obj/build.h: FORCE
	@$(MKDIR_P) $(builddir)/obj
	@$(top_srcdir)/share/genbuild.sh $(abs_top_builddir)/src/obj/build.h \
	  $(abs_top_srcdir)
libsupcoin_util_a-clientversion.$(OBJEXT): obj/build.h

# server: shared between supcoind and supcoin-qt
libsupcoin_server_a_CPPFLAGS = $(SUPCOIN_INCLUDES) $(MINIUPNPC_CPPFLAGS)
libsupcoin_server_a_SOURCES = \
  addrman.cpp \
  alert.cpp \
  bloom.cpp \
  chain.cpp \
  checkpoints.cpp \
  init.cpp \
  leveldbwrapper.cpp \
  main.cpp \
  merkleblock.cpp \
  miner.cpp \
  net.cpp \
  noui.cpp \
  pow.cpp \
  rest.cpp \
  rpcblockchain.cpp \
  rpcmining.cpp \
  rpcmisc.cpp \
  rpcnet.cpp \
  rpcrawtransaction.cpp \
  rpcserver.cpp \
  script/sigcache.cpp \
  timedata.cpp \
  txdb.cpp \
  txmempool.cpp \
  $(JSON_H) \
  $(SUPCOIN_CORE_H)

# wallet: shared between supcoind and supcoin-qt, but only linked
# when wallet enabled
libsupcoin_wallet_a_CPPFLAGS = $(SUPCOIN_INCLUDES)
libsupcoin_wallet_a_SOURCES = \
  db.cpp \
  crypter.cpp \
  rpcdump.cpp \
  rpcwallet.cpp \
  wallet.cpp \
  wallet_ismine.cpp \
  walletdb.cpp \
  $(SUPCOIN_CORE_H)

# crypto primitives library
crypto_libsupcoin_crypto_a_CPPFLAGS = $(SUPCOIN_CONFIG_INCLUDES)
crypto_libsupcoin_crypto_a_SOURCES = \
  crypto/sha1.cpp \
  crypto/sha256.cpp \
  crypto/sha512.cpp \
  crypto/hmac_sha256.cpp \
  crypto/rfc6979_hmac_sha256.cpp \
  crypto/hmac_sha512.cpp \
  crypto/ripemd160.cpp \
  crypto/common.h \
  crypto/sha256.h \
  crypto/sha512.h \
  crypto/hmac_sha256.h \
  crypto/rfc6979_hmac_sha256.h \
  crypto/hmac_sha512.h \
  crypto/sha1.h \
  crypto/ripemd160.h

# univalue JSON library
univalue_libsupcoin_univalue_a_SOURCES = \
  univalue/univalue.cpp \
  univalue/univalue_read.cpp \
  univalue/univalue_write.cpp \
  univalue/univalue_escapes.h \
  univalue/univalue.h

# common: shared between supcoind, and supcoin-qt and non-server tools
libsupcoin_common_a_CPPFLAGS = $(SUPCOIN_INCLUDES)
libsupcoin_common_a_SOURCES = \
  allocators.cpp \
  amount.cpp \
  base58.cpp \
  chainparams.cpp \
  coins.cpp \
  compressor.cpp \
  primitives/block.cpp \
  primitives/transaction.cpp \
  core_read.cpp \
  core_write.cpp \
  eccryptoverify.cpp \
  ecwrapper.cpp \
  hash.cpp \
  key.cpp \
  keystore.cpp \
  netbase.cpp \
  protocol.cpp \
  pubkey.cpp \
  script/interpreter.cpp \
  script/script.cpp \
  script/sign.cpp \
  script/standard.cpp \
  script/script_error.cpp \
  $(SUPCOIN_CORE_H)

# util: shared between all executables.
# This library *must* be included to make sure that the glibc
# backward-compatibility objects and their sanity checks are linked.
libsupcoin_util_a_CPPFLAGS = $(SUPCOIN_INCLUDES)
libsupcoin_util_a_SOURCES = \
  compat/strnlen.cpp \
  compat/glibc_sanity.cpp \
  compat/glibcxx_sanity.cpp \
  chainparamsbase.cpp \
  clientversion.cpp \
  random.cpp \
  rpcprotocol.cpp \
  sync.cpp \
  uint256.cpp \
  util.cpp \
  utilstrencodings.cpp \
  utilmoneystr.cpp \
  utiltime.cpp \
  $(SUPCOIN_CORE_H)

if GLIBC_BACK_COMPAT
libsupcoin_util_a_SOURCES += compat/glibc_compat.cpp
libsupcoin_util_a_SOURCES += compat/glibcxx_compat.cpp
endif

# cli: shared between supcoin-cli and supcoin-qt
libsupcoin_cli_a_CPPFLAGS = $(SUPCOIN_INCLUDES)
libsupcoin_cli_a_SOURCES = \
  rpcclient.cpp \
  $(SUPCOIN_CORE_H)

nodist_libsupcoin_util_a_SOURCES = $(srcdir)/obj/build.h
#

# supcoind binary #
supcoind_LDADD = \
  $(LIBSUPCOIN_SERVER) \
  $(LIBSUPCOIN_COMMON) \
  $(LIBSUPCOIN_UNIVALUE) \
  $(LIBSUPCOIN_UTIL) \
  $(LIBSUPCOIN_CRYPTO) \
  $(LIBLEVELDB) \
  $(LIBMEMENV) \
  $(LIBSECP256K1)

if ENABLE_WALLET
supcoind_LDADD += libsupcoin_wallet.a
endif
supcoind_SOURCES = supcoind.cpp
#

if TARGET_WINDOWS
supcoind_SOURCES += supcoind-res.rc
endif

supcoind_LDADD += $(BOOST_LIBS) $(BDB_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(MINIUPNPC_LIBS)
supcoind_CPPFLAGS = $(SUPCOIN_INCLUDES)
supcoind_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

# supcoin-cli binary #
supcoin_cli_LDADD = \
  $(LIBSUPCOIN_CLI) \
  $(LIBSUPCOIN_UTIL) \
  $(BOOST_LIBS) \
  $(SSL_LIBS) \
  $(CRYPTO_LIBS)

supcoin_cli_SOURCES = \
  supcoin-cli.cpp

supcoin_cli_CPPFLAGS = $(SUPCOIN_INCLUDES)
#

# supcoin-tx binary #
supcoin_tx_LDADD = \
  $(LIBSUPCOIN_UNIVALUE) \
  $(LIBSUPCOIN_COMMON) \
  $(LIBSUPCOIN_UTIL) \
  $(LIBSUPCOIN_CRYPTO) \
  $(LIBSECP256K1) \
  $(BOOST_LIBS) \
  $(CRYPTO_LIBS)

supcoin_tx_SOURCES = supcoin-tx.cpp
supcoin_tx_CPPFLAGS = $(SUPCOIN_INCLUDES)
#
supcoin_tx_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
supcoin_cli_SOURCES += supcoin-cli-res.rc
endif
supcoin_cli_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if BUILD_SUPCOIN_LIBS
include_HEADERS = script/supcoinconsensus.h
libsupcoinconsensus_la_SOURCES = \
  primitives/transaction.cpp \
  crypto/hmac_sha512.cpp \
  crypto/sha1.cpp \
  crypto/sha256.cpp \
  crypto/sha512.cpp \
  crypto/ripemd160.cpp \
  eccryptoverify.cpp \
  ecwrapper.cpp \
  hash.cpp \
  pubkey.cpp \
  script/script.cpp \
  script/interpreter.cpp \
  script/supcoinconsensus.cpp \
  uint256.cpp \
  utilstrencodings.cpp

if GLIBC_BACK_COMPAT
  libsupcoinconsensus_la_SOURCES += compat/glibc_compat.cpp
  libsupcoinconsensus_la_SOURCES += compat/glibcxx_compat.cpp
endif

libsupcoinconsensus_la_LDFLAGS = -no-undefined $(RELDFLAGS)
libsupcoinconsensus_la_LIBADD = $(CRYPTO_LIBS)
libsupcoinconsensus_la_CPPFLAGS = $(CRYPTO_CFLAGS) -I$(builddir)/obj -DBUILD_SUPCOIN_INTERNAL
if USE_LIBSECP256K1
libsupcoinconsensus_la_LIBADD += secp256k1/libsecp256k1.la
endif
endif

CLEANFILES = leveldb/libleveldb.a leveldb/libmemenv.a *.gcda *.gcno

DISTCLEANFILES = obj/build.h

EXTRA_DIST = leveldb

clean-local:
	-$(MAKE) -C leveldb clean
	-$(MAKE) -C secp256k1 clean
	rm -f leveldb/*/*.gcno leveldb/helpers/memenv/*.gcno
	-rm -f config.h

.rc.o:
	@test -f $(WINDRES)
	$(AM_V_GEN) $(WINDRES) -DWINDRES_PREPROC -i $< -o $@

.mm.o:
	$(AM_V_CXX) $(OBJCXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	  $(CPPFLAGS) $(AM_CXXFLAGS) $(QT_INCLUDES) $(CXXFLAGS)  -c -o $@ $<

%.pb.cc %.pb.h: %.proto
	@test -f $(PROTOC)
	$(AM_V_GEN) $(PROTOC) --cpp_out=$(@D) --proto_path=$(abspath $(<D) $<)

if ENABLE_TESTS
include Makefile.test.include
endif

if ENABLE_QT
include Makefile.qt.include
endif

if ENABLE_QT_TESTS
include Makefile.qttest.include
endif
