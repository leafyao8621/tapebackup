from setuptools import setup, Extension

base =\
    Extension(
        'tapebackup._base',
        sources=[
            "tapebackup/base/src/main.cpp",
            "tapebackup/base/src/app/app.cpp",
            "tapebackup/base/src/app/app_check.cpp",
            "tapebackup/base/src/app/app_read.cpp",
            "tapebackup/base/src/app/app_write.cpp",
            "tapebackup/base/src/app/app_report_daily.cpp",
            "tapebackup/base/src/db/connector.cpp",
            "tapebackup/base/src/db/connector_stmt.cpp",
            "tapebackup/base/src/util/util_archive.cpp",
            "tapebackup/base/src/util/util_dev.cpp",
            "tapebackup/base/src/util/util_dir.cpp",
            "tapebackup/base/src/util/util_env.cpp",
            "tapebackup/base/src/util/util_gen.cpp",
            "tapebackup/base/src/util/util_get_hex.cpp",
            "tapebackup/base/src/util/util_hmac.cpp",
            "tapebackup/base/src/util/util_reader.cpp",
            "tapebackup/base/src/util/util_writer.cpp",
            "tapebackup/base/src/report/report_daily.cpp",
        ],
        libraries=["archive", "sqlite3", "crypto"]
    )

setup(
    name="tapebackup",
    version="0.1",
    packages=[
        "tapebackup",
    ],
    ext_modules=[base]
)
