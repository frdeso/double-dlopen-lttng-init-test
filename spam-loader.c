/*
 * The spam-loader test executable
 * Copyright 2017 Itiviti AB, Anton Smyk <Anton.Smyk@itiviti.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <dlfcn.h>

#define DLOPEN_FLAGS RTLD_NOW
void *handle;
void (*fptr)();

static void * test_load(const char *path, int i) {
	fprintf(stderr, "Loading %s...", path);
	void *p = dlopen(path, DLOPEN_FLAGS);
	*(void **)(&fptr) = dlsym(p, "spam_foo");
	if (p != NULL) {
		fprintf(stderr, " OK\n");
	} else {
		fprintf(stderr, "dlopen(%s) failed: %s\n", path, dlerror());
	}

	return p;
}

int main(int argc, char **argv) {
	int i;
	for (i = 1; i < argc; i++) {
		test_load(argv[i], i);
		fptr();
	}

	fprintf(stderr, "Success!\n");
}
