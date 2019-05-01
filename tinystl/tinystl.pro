TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
Qt += sql
SOURCES += main.cpp

HEADERS += \
    stl_construct.h \
    stl_iterator.h \
    __type_traits.h \
    malloc_alloc_template.h \
    stl_uninitialized.h \
    alloc.h \
    stl_algobase.h \
    stl_vector.h \
    stl_list.h \
    stl_numeric.h \
    stl_deque.h \
    stl_function.h \
    stl_algo.h \
    stl_string.h \
    stl_base_string.h \
    stl_slist.h \
    stl_pair.h \
    stl_rb_tree.h \
    stl_heap.h \
    stl_map.h \
    stl_set.h \
    shared_ptr.h
