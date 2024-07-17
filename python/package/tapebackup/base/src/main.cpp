#include <Python.h>

#include "app/app.h"

#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))

struct module_state {
    PyObject* error;
};

static PyObject* write(PyObject* self, PyObject* args) {
    const char *path, *dev;
    bool write_protect;
    bool verbose;
    int64_t block_size_read;
    int64_t block_size_write;
    int64_t block_size_hmac;
    int64_t block_size_archive;
    if (
        !PyArg_ParseTuple(
            args,
            "ppssllll",
            &write_protect,
            &verbose,
            &path,
            &dev,
            &block_size_read,
            &block_size_write,
            &block_size_hmac,
            &block_size_archive
        )) {
        return NULL;
    }
    try {
        TBCLI::App app;
        app.set_block_size_read(block_size_read);
        app.set_block_size_write(block_size_read);
        app.set_block_size_hmac(block_size_read);
        app.set_block_size_archive(block_size_read);
        app.write(
            (char*)dev,
            (char*)path,
            write_protect,
            verbose
        );
    } catch (TBCLI::App::Err err) {
        switch (err) {
        case TBCLI::App::Err::WRITE_PROTECT_TAMPERED:
            PyErr_SetString(PyExc_RuntimeError, "APP WRITE_PROTECT_TAMPERED");
            break;
        case TBCLI::App::Err::WRITE_PROTECTED:
            PyErr_SetString(PyExc_RuntimeError, "APP WRITE_PROTECTED");
            break;
        case TBCLI::App::Err::WRITE:
            PyErr_SetString(PyExc_RuntimeError, "APP WRITE");
            break;
        case TBCLI::App::Err::INVALID_SIGNATURE:
            PyErr_SetString(PyExc_RuntimeError, "APP INVALID_SIGNATURE");
            break;
        case TBCLI::App::Err::READ:
            PyErr_SetString(PyExc_RuntimeError, "APP READ");
            break;
        case TBCLI::App::Err::FILE_TAMPERED:
            PyErr_SetString(PyExc_RuntimeError, "APP FILE_TAMPERED");
            break;
        }
    } catch (TBCLI::Connector::Err err) {
        switch (err) {
        case TBCLI::Connector::Err::CONNECTION:
            PyErr_SetString(PyExc_RuntimeError, "CONNECTOR CONNECTION");
            break;
        case TBCLI::Connector::Err::STMT_CREATION:
            PyErr_SetString(PyExc_RuntimeError, "CONNECTOR STMT_CREATION");
            break;
        case TBCLI::Connector::Err::STMT_EXECUTION:
            PyErr_SetString(PyExc_RuntimeError, "CONNECTOR STMT_EXECUTION");
            break;
        case TBCLI::Connector::Err::STMT_BIND:
            PyErr_SetString(PyExc_RuntimeError, "CONNECTOR STMT_BIND");
            break;
        }
    } catch (TBCLI::Util::Err err) {
        switch (err) {
        case TBCLI::Util::Err::INVALID_DIR:
            PyErr_SetString(PyExc_RuntimeError, "UTIL INVALID_DIR");
            break;
        case TBCLI::Util::Err::DEVICE_OPEN:
            PyErr_SetString(PyExc_RuntimeError, "UTIL DEVICE_OPEN");
            break;
        case TBCLI::Util::Err::DEVICE_WRITE:
            PyErr_SetString(PyExc_RuntimeError, "UTIL DEVICE_WRITE");
            break;
        case TBCLI::Util::Err::DIRECTORY_LISTING:
            PyErr_SetString(PyExc_RuntimeError, "UTIL DIRECTORY_LISTING");
            break;
        case TBCLI::Util::Err::DIRECTORY_COMPRESSION:
            PyErr_SetString(PyExc_RuntimeError, "UTIL DIRECTORY_COMPRESSION");
            break;
        case TBCLI::Util::Err::ARCHIVE_WRITE:
            PyErr_SetString(PyExc_RuntimeError, "UTIL ARCHIVE_WRITE");
            break;
        case TBCLI::Util::Err::ARCHIVE_READ:
            PyErr_SetString(PyExc_RuntimeError, "UTIL ARCHIVE_READ");
            break;
        case TBCLI::Util::Err::READONLY_FLAG:
            PyErr_SetString(PyExc_RuntimeError, "UTIL READONLY_FLAG");
            break;
        }
    } catch (TBCLI::Util::Env::Err err) {
        switch (err) {
        case TBCLI::Util::Env::Err::DIRECTORY_CREATION:
            PyErr_SetString(PyExc_RuntimeError, "ENV DIRECTORY_CREATION");
            break;
        }
    } catch (TBCLI::Util::Gen::Err err) {
        switch (err) {
        case TBCLI::Util::Gen::Err::OPEN:
            PyErr_SetString(PyExc_RuntimeError, "GEN OPEN");
            break;
        case TBCLI::Util::Gen::Err::READ:
            PyErr_SetString(PyExc_RuntimeError, "GEN READ");
            break;
        }
    } catch (TBCLI::Util::Archiver::Err err) {
        switch (err) {
        case TBCLI::Util::Archiver::Err::OPEN:
            PyErr_SetString(PyExc_RuntimeError, "ARCHIVER OPEN");
            break;
        case TBCLI::Util::Archiver::Err::APPEND:
            PyErr_SetString(PyExc_RuntimeError, "ARCHIVER APPEND");
            break;
        }
    } catch (TBCLI::Util::HMAC::Err err) {
        switch (err) {
        case TBCLI::Util::HMAC::Err::CREATION:
            PyErr_SetString(PyExc_RuntimeError, "HMAC CREATION");
            break;
        case TBCLI::Util::HMAC::Err::INITIALIZATION:
            PyErr_SetString(PyExc_RuntimeError, "HMAC INITIALIZATION");
            break;
        case TBCLI::Util::HMAC::Err::UPDATE:
            PyErr_SetString(PyExc_RuntimeError, "HMAC UPDATE");
            break;
        case TBCLI::Util::HMAC::Err::FINALIZATION:
            PyErr_SetString(PyExc_RuntimeError, "HMAC FINALIZATION");
            break;
        }
    } catch (TBCLI::Util::Writer::Err err) {
        switch (err) {
        case TBCLI::Util::Writer::Err::OPEN:
            PyErr_SetString(PyExc_RuntimeError, "WRITER OPEN");
            break;
        case TBCLI::Util::Writer::Err::READ:
            PyErr_SetString(PyExc_RuntimeError, "WRITER READ");
            break;
        case TBCLI::Util::Writer::Err::WRITE:
            PyErr_SetString(PyExc_RuntimeError, "WRITER WRITE");
            break;
        }
    }
    Py_RETURN_NONE;
}

static PyObject* read(PyObject* self, PyObject* args) {
    const char *path, *dev;
    bool verbose;
    int64_t block_size_read;
    int64_t block_size_write;
    int64_t block_size_hmac;
    int64_t block_size_archive;
    if (
        !PyArg_ParseTuple(
            args,
            "pssllll",
            &verbose,
            &path,
            &dev,
            &block_size_read,
            &block_size_write,
            &block_size_hmac,
            &block_size_archive
        )) {
        return NULL;
    }
    try {
        TBCLI::App app;
        app.set_block_size_read(block_size_read);
        app.set_block_size_write(block_size_read);
        app.set_block_size_hmac(block_size_read);
        app.set_block_size_archive(block_size_read);
        app.read(
            (char*)dev,
            (char*)path,
            verbose
        );
    } catch (TBCLI::App::Err err) {
        switch (err) {
        case TBCLI::App::Err::WRITE_PROTECT_TAMPERED:
            PyErr_SetString(PyExc_RuntimeError, "APP WRITE_PROTECT_TAMPERED");
            break;
        case TBCLI::App::Err::WRITE_PROTECTED:
            PyErr_SetString(PyExc_RuntimeError, "APP WRITE_PROTECTED");
            break;
        case TBCLI::App::Err::WRITE:
            PyErr_SetString(PyExc_RuntimeError, "APP WRITE");
            break;
        case TBCLI::App::Err::INVALID_SIGNATURE:
            PyErr_SetString(PyExc_RuntimeError, "APP INVALID_SIGNATURE");
            break;
        case TBCLI::App::Err::READ:
            PyErr_SetString(PyExc_RuntimeError, "APP READ");
            break;
        case TBCLI::App::Err::FILE_TAMPERED:
            PyErr_SetString(PyExc_RuntimeError, "APP FILE_TAMPERED");
            break;
        }
    } catch (TBCLI::Connector::Err err) {
        switch (err) {
        case TBCLI::Connector::Err::CONNECTION:
            PyErr_SetString(PyExc_RuntimeError, "CONNECTOR CONNECTION");
            break;
        case TBCLI::Connector::Err::STMT_CREATION:
            PyErr_SetString(PyExc_RuntimeError, "CONNECTOR STMT_CREATION");
            break;
        case TBCLI::Connector::Err::STMT_EXECUTION:
            PyErr_SetString(PyExc_RuntimeError, "CONNECTOR STMT_EXECUTION");
            break;
        case TBCLI::Connector::Err::STMT_BIND:
            PyErr_SetString(PyExc_RuntimeError, "CONNECTOR STMT_BIND");
            break;
        }
    } catch (TBCLI::Util::Err err) {
        switch (err) {
        case TBCLI::Util::Err::INVALID_DIR:
            PyErr_SetString(PyExc_RuntimeError, "UTIL INVALID_DIR");
            break;
        case TBCLI::Util::Err::DEVICE_OPEN:
            PyErr_SetString(PyExc_RuntimeError, "UTIL DEVICE_OPEN");
            break;
        case TBCLI::Util::Err::DEVICE_WRITE:
            PyErr_SetString(PyExc_RuntimeError, "UTIL DEVICE_WRITE");
            break;
        case TBCLI::Util::Err::DIRECTORY_LISTING:
            PyErr_SetString(PyExc_RuntimeError, "UTIL DIRECTORY_LISTING");
            break;
        case TBCLI::Util::Err::DIRECTORY_COMPRESSION:
            PyErr_SetString(PyExc_RuntimeError, "UTIL DIRECTORY_COMPRESSION");
            break;
        case TBCLI::Util::Err::ARCHIVE_WRITE:
            PyErr_SetString(PyExc_RuntimeError, "UTIL ARCHIVE_WRITE");
            break;
        case TBCLI::Util::Err::ARCHIVE_READ:
            PyErr_SetString(PyExc_RuntimeError, "UTIL ARCHIVE_READ");
            break;
        case TBCLI::Util::Err::READONLY_FLAG:
            PyErr_SetString(PyExc_RuntimeError, "UTIL READONLY_FLAG");
            break;
        }
    } catch (TBCLI::Util::Env::Err err) {
        switch (err) {
        case TBCLI::Util::Env::Err::DIRECTORY_CREATION:
            PyErr_SetString(PyExc_RuntimeError, "ENV DIRECTORY_CREATION");
            break;
        }
    } catch (TBCLI::Util::Gen::Err err) {
        switch (err) {
        case TBCLI::Util::Gen::Err::OPEN:
            PyErr_SetString(PyExc_RuntimeError, "GEN OPEN");
            break;
        case TBCLI::Util::Gen::Err::READ:
            PyErr_SetString(PyExc_RuntimeError, "GEN READ");
            break;
        }
    } catch (TBCLI::Util::Archiver::Err err) {
        switch (err) {
        case TBCLI::Util::Archiver::Err::OPEN:
            PyErr_SetString(PyExc_RuntimeError, "ARCHIVER OPEN");
            break;
        case TBCLI::Util::Archiver::Err::APPEND:
            PyErr_SetString(PyExc_RuntimeError, "ARCHIVER APPEND");
            break;
        }
    } catch (TBCLI::Util::HMAC::Err err) {
        switch (err) {
        case TBCLI::Util::HMAC::Err::CREATION:
            PyErr_SetString(PyExc_RuntimeError, "HMAC CREATION");
            break;
        case TBCLI::Util::HMAC::Err::INITIALIZATION:
            PyErr_SetString(PyExc_RuntimeError, "HMAC INITIALIZATION");
            break;
        case TBCLI::Util::HMAC::Err::UPDATE:
            PyErr_SetString(PyExc_RuntimeError, "HMAC UPDATE");
            break;
        case TBCLI::Util::HMAC::Err::FINALIZATION:
            PyErr_SetString(PyExc_RuntimeError, "HMAC FINALIZATION");
            break;
        }
    } catch (TBCLI::Util::Writer::Err err) {
        switch (err) {
        case TBCLI::Util::Writer::Err::OPEN:
            PyErr_SetString(PyExc_RuntimeError, "WRITER OPEN");
            break;
        case TBCLI::Util::Writer::Err::READ:
            PyErr_SetString(PyExc_RuntimeError, "WRITER READ");
            break;
        case TBCLI::Util::Writer::Err::WRITE:
            PyErr_SetString(PyExc_RuntimeError, "WRITER WRITE");
            break;
        }
    }
    Py_RETURN_NONE;
}

static PyObject* check(PyObject* self, PyObject* args) {
    const char *dev;
    bool verbose;
    int64_t block_size_read;
    int64_t block_size_write;
    int64_t block_size_hmac;
    int64_t block_size_archive;
    if (
        !PyArg_ParseTuple(
            args,
            "psllll",
            &verbose,
            &dev,
            &block_size_read,
            &block_size_write,
            &block_size_hmac,
            &block_size_archive
        )) {
        return NULL;
    }
    try {
        TBCLI::App app;
        app.set_block_size_read(block_size_read);
        app.set_block_size_write(block_size_read);
        app.set_block_size_hmac(block_size_read);
        app.set_block_size_archive(block_size_read);
        app.read(
            (char*)dev,
            verbose
        );
    } catch (TBCLI::App::Err err) {
        switch (err) {
        case TBCLI::App::Err::WRITE_PROTECT_TAMPERED:
            PyErr_SetString(PyExc_RuntimeError, "APP WRITE_PROTECT_TAMPERED");
            break;
        case TBCLI::App::Err::WRITE_PROTECTED:
            PyErr_SetString(PyExc_RuntimeError, "APP WRITE_PROTECTED");
            break;
        case TBCLI::App::Err::WRITE:
            PyErr_SetString(PyExc_RuntimeError, "APP WRITE");
            break;
        case TBCLI::App::Err::INVALID_SIGNATURE:
            PyErr_SetString(PyExc_RuntimeError, "APP INVALID_SIGNATURE");
            break;
        case TBCLI::App::Err::READ:
            PyErr_SetString(PyExc_RuntimeError, "APP READ");
            break;
        case TBCLI::App::Err::FILE_TAMPERED:
            PyErr_SetString(PyExc_RuntimeError, "APP FILE_TAMPERED");
            break;
        }
    } catch (TBCLI::Connector::Err err) {
        switch (err) {
        case TBCLI::Connector::Err::CONNECTION:
            PyErr_SetString(PyExc_RuntimeError, "CONNECTOR CONNECTION");
            break;
        case TBCLI::Connector::Err::STMT_CREATION:
            PyErr_SetString(PyExc_RuntimeError, "CONNECTOR STMT_CREATION");
            break;
        case TBCLI::Connector::Err::STMT_EXECUTION:
            PyErr_SetString(PyExc_RuntimeError, "CONNECTOR STMT_EXECUTION");
            break;
        case TBCLI::Connector::Err::STMT_BIND:
            PyErr_SetString(PyExc_RuntimeError, "CONNECTOR STMT_BIND");
            break;
        }
    } catch (TBCLI::Util::Err err) {
        switch (err) {
        case TBCLI::Util::Err::INVALID_DIR:
            PyErr_SetString(PyExc_RuntimeError, "UTIL INVALID_DIR");
            break;
        case TBCLI::Util::Err::DEVICE_OPEN:
            PyErr_SetString(PyExc_RuntimeError, "UTIL DEVICE_OPEN");
            break;
        case TBCLI::Util::Err::DEVICE_WRITE:
            PyErr_SetString(PyExc_RuntimeError, "UTIL DEVICE_WRITE");
            break;
        case TBCLI::Util::Err::DIRECTORY_LISTING:
            PyErr_SetString(PyExc_RuntimeError, "UTIL DIRECTORY_LISTING");
            break;
        case TBCLI::Util::Err::DIRECTORY_COMPRESSION:
            PyErr_SetString(PyExc_RuntimeError, "UTIL DIRECTORY_COMPRESSION");
            break;
        case TBCLI::Util::Err::ARCHIVE_WRITE:
            PyErr_SetString(PyExc_RuntimeError, "UTIL ARCHIVE_WRITE");
            break;
        case TBCLI::Util::Err::ARCHIVE_READ:
            PyErr_SetString(PyExc_RuntimeError, "UTIL ARCHIVE_READ");
            break;
        case TBCLI::Util::Err::READONLY_FLAG:
            PyErr_SetString(PyExc_RuntimeError, "UTIL READONLY_FLAG");
            break;
        }
    } catch (TBCLI::Util::Env::Err err) {
        switch (err) {
        case TBCLI::Util::Env::Err::DIRECTORY_CREATION:
            PyErr_SetString(PyExc_RuntimeError, "ENV DIRECTORY_CREATION");
            break;
        }
    } catch (TBCLI::Util::Gen::Err err) {
        switch (err) {
        case TBCLI::Util::Gen::Err::OPEN:
            PyErr_SetString(PyExc_RuntimeError, "GEN OPEN");
            break;
        case TBCLI::Util::Gen::Err::READ:
            PyErr_SetString(PyExc_RuntimeError, "GEN READ");
            break;
        }
    } catch (TBCLI::Util::Archiver::Err err) {
        switch (err) {
        case TBCLI::Util::Archiver::Err::OPEN:
            PyErr_SetString(PyExc_RuntimeError, "ARCHIVER OPEN");
            break;
        case TBCLI::Util::Archiver::Err::APPEND:
            PyErr_SetString(PyExc_RuntimeError, "ARCHIVER APPEND");
            break;
        }
    } catch (TBCLI::Util::HMAC::Err err) {
        switch (err) {
        case TBCLI::Util::HMAC::Err::CREATION:
            PyErr_SetString(PyExc_RuntimeError, "HMAC CREATION");
            break;
        case TBCLI::Util::HMAC::Err::INITIALIZATION:
            PyErr_SetString(PyExc_RuntimeError, "HMAC INITIALIZATION");
            break;
        case TBCLI::Util::HMAC::Err::UPDATE:
            PyErr_SetString(PyExc_RuntimeError, "HMAC UPDATE");
            break;
        case TBCLI::Util::HMAC::Err::FINALIZATION:
            PyErr_SetString(PyExc_RuntimeError, "HMAC FINALIZATION");
            break;
        }
    } catch (TBCLI::Util::Writer::Err err) {
        switch (err) {
        case TBCLI::Util::Writer::Err::OPEN:
            PyErr_SetString(PyExc_RuntimeError, "WRITER OPEN");
            break;
        case TBCLI::Util::Writer::Err::READ:
            PyErr_SetString(PyExc_RuntimeError, "WRITER READ");
            break;
        case TBCLI::Util::Writer::Err::WRITE:
            PyErr_SetString(PyExc_RuntimeError, "WRITER WRITE");
            break;
        }
    }
    Py_RETURN_NONE;
}

static PyMethodDef _base_methods[] = {
    {"write", (PyCFunction)write, METH_VARARGS, NULL},
    {"read", (PyCFunction)read, METH_VARARGS, NULL},
    {"check", (PyCFunction)check, METH_VARARGS, NULL},
    {0}
};

static int _base_traverse(PyObject *m, visitproc visit, void *arg) {
    Py_VISIT(GETSTATE(m)->error);
    return 0;
}

static int _base_clear(PyObject *m) {
    Py_CLEAR(GETSTATE(m)->error);
    return 0;
}

static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "tapebackup_base",
    NULL,
    sizeof(struct module_state),
    _base_methods,
    NULL,
    _base_traverse,
    _base_clear,
    NULL
};

PyMODINIT_FUNC PyInit__base(void) {
    PyObject *module = PyModule_Create(&moduledef);
    if (!module) return NULL;
    return module;
}
