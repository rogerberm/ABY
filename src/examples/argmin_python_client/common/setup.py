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
            "../../../abycore/sharing/arithsharing.cpp",
            "../../../abycore/sharing/yaoclientsharing.cpp",
            "../../../abycore/sharing/yaoserversharing.cpp",
            "../../../abycore/sharing/yaosharing.cpp",
            "../../../abycore/sharing/boolsharing.cpp",
            "../../../abycore/util/crypto/crypto.cpp",
            "../../../abycore/util/crypto/gmp-pk-crypto.cpp",
            "../../../abycore/util/crypto/ecc-pk-crypto.cpp",
            "../../../abycore/ot/external/util/crypto/crypto.cpp",
            "argmin_python_client.cpp",
            "_ABY.cpp",
            ])],
    include_dirs=['/usr/include/openssl/'] + numpy.distutils.misc_util.get_numpy_include_dirs(),
    )

../../../abycore/util/miracl_lib
