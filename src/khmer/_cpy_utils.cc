#include "khmer/_cpy_utils.hh"
#include "oxli/oxli.hh"

using namespace oxli;
using namespace oxli::read_parsers;

namespace khmer {

// Convert a hash to a python long object.
bool convert_HashIntoType_to_PyObject(const HashIntoType &hashval,
        PyObject **value)
{
    *value = PyLong_FromUnsignedLongLong(hashval);
    return true;
}


// Take a Python object and (try to) convert it to a HashIntoType.
// Note: will set error condition and return false if cannot do.

bool convert_PyObject_to_HashIntoType(PyObject * value,
        HashIntoType& hashval,
        WordLength ksize)
{
    if (PyInt_Check(value) || PyLong_Check(value)) {
        return convert_PyLong_to_HashIntoType(value, hashval);
    } else {
        PyErr_SetString(PyExc_ValueError,
                        "must use a hash");
        return false;
    }
}


bool convert_Pytablesizes_to_vector(PyListObject * sizes_list_o,
                                           std::vector<uint64_t>& sizes)
{
    Py_ssize_t sizes_list_o_length = PyList_GET_SIZE(sizes_list_o);
    if (sizes_list_o_length < 1) {
        PyErr_SetString(PyExc_ValueError,
                        "tablesizes needs to be one or more numbers");
        return false;
    }
    for (Py_ssize_t i = 0; i < sizes_list_o_length; i++) {
        PyObject * size_o = PyList_GET_ITEM(sizes_list_o, i);
        if (PyLong_Check(size_o)) {
            sizes.push_back(PyLong_AsUnsignedLongLong(size_o));
        } else if (PyInt_Check(size_o)) {
            sizes.push_back(PyInt_AsLong(size_o));
        } else if (PyFloat_Check(size_o)) {
            sizes.push_back(PyFloat_AS_DOUBLE(size_o));
        } else {
            PyErr_SetString(PyExc_TypeError,
                            "2nd argument must be a list of ints, longs, or floats");
            return false;
        }
    }
    return true;
}

FastxParserPtr& _PyObject_to_khmer_ReadParser(PyObject * py_object);

}
