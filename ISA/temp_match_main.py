import ctypes
from img_pre_proc import pre_proc
from img_post_proc import post_proc

in_arg = pre_proc()

fun = ctypes.CDLL("libfun.so")

'''for a in in_arg:
    print(a)
'''
#print(type(fun.top.argtypes))

fun.top.argtypes = (ctypes.POINTER(ctypes.c_char), ctypes.POINTER(ctypes.c_char)) 

#fun.top.argtypes = (ctypes.c_char, ctypes.c_char) 

in_array = (ctypes.c_char * len(in_arg))(*in_arg)

out_arg = bytearray(40000*4)

out_array = (ctypes.c_char * len(out_arg))(*out_arg)

fun.top(in_array, out_array)

post_proc(out_array)


