from distutils.core import setup, Extension
import numpy.distutils.misc_util

setup(
    ext_modules=[
        Extension("_ABY", [
            "../../../abycore/circuit/circuit.cpp",
            "../../../abycore/circuit/abycircuit.cpp",
            "../../../abycore/circuit/share.cpp",
            "../../../abycore/circuit/booleancircuits.cpp",
            "../../../abycore/circuit/arithmeticcircuits.cpp",
            "../../../abycore/aby/abyparty.cpp",
            "../../../abycore/aby/abysetup.cpp",
            "../../../abycore/sharing/sharing.cpp",
            "../../../abycore/sharing/arithsharing.cpp",
            "../../../abycore/sharing/yaoclientsharing.cpp",
            "../../../abycore/sharing/yaoserversharing.cpp",
            "../../../abycore/sharing/yaosharing.cpp",
            "../../../abycore/sharing/boolsharing.cpp",
            "../../../abycore/util/crypto/crypto.cpp",
            "../../../abycore/util/cbitvector.cpp",
            "../../../abycore/util/connection.cpp",
            "../../../abycore/util/djn.cpp",
            "../../../abycore/util/powmod.cpp",
            "../../../abycore/util/dgk.cpp",
            "../../../abycore/util/parse_options.cpp",
            #"../../../abycore/util/crypto/ecc-pk-crypto.cpp",
            "../../../abycore/util/crypto/gmp-pk-crypto.cpp",
            "../../../abycore/ot/external/util/crypto/ecc-pk-crypto.cpp",
            "../../../abycore/ot/external/ot/iknp-ot-ext-snd.cpp",
            "../../../abycore/ot/external/ot/iknp-ot-ext-rec.cpp",
            "../../../abycore/ot/external/ot/naor-pinkas.cpp",
            "../../../abycore/ot/external/ot/ot-ext-rec.cpp",
            "../../../abycore/ot/external/ot/ot-ext-snd.cpp",
            "../../../abycore/DJN/djnparty.cpp",
            "../../../abycore/DGK/dgkparty.cpp",
            "argmin_python_client.cpp",
            "_ABY.cpp",
            ], libraries=['miracl','ssl','crypto','gmp','gmpxx','pthread'])],
    include_dirs=['/tmp/ABY/src/abycore/util/miracl_lib'] + numpy.distutils.misc_util.get_numpy_include_dirs(),
    library_dirs=['/tmp/ABY/src/abycore/util/miracl_lib'],
    data_files=[('/usr/local/lib','/tmp/ABY/src/abycore/util/miracl_lib/miracl.a', '/tmp/ABY/src/abycore/util/miracl_lib/*.o')],
    )
#-lgmp -lgmpxx -lpthread ${CORE}/util/miracl_lib/miracl.a -L /usr/lib -lssl -lcrypto -lrt
