#!/usr/bin/env python
# coding: utf-8
import sys
import numpy as np

with open("tests/input_2000.txt", "r") as f:
	A = f.read()

with open("outputs/output_L.txt", "r") as f:
	L = f.read()

with open("outputs/output_U.txt", "r") as f:
	U = f.read()


def convert2numpy(mat):
	mat = mat.strip().split("\n")
	mat = [i.strip().split() for i in mat]
	for x in mat:
		for y in x:
			a, b = y.strip().split('.')
			assert len(b) <= 12, "incorrect precision"

	return np.array(mat).astype(np.double)

A, L, U = (convert2numpy(x) for x in (A,L,U))

print(A.shape)
print(L.shape)
print(U.shape)

# print(A)

# forcing the matrices to be triangular
# L = np.tril(L)
# U = np.triu(U)

# print(L)
# print(U)

assert U.shape == L.shape == A.shape, "invalid shape"
print("N is {}".format(A.shape[0]))

A_dash = np.matmul(L, U)

# print("A_dash is:")
# print(A_dash)

print("max deviation from true values was {}".format(abs(A - A_dash).max()))

# print()
# print()

if np.allclose(A, A_dash, atol = 1e-3) and abs(np.linalg.det(U) - 1) < 1e-3:
	print("Test Passed")
else:
	print("Test Failed")