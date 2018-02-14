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
#include <stdio.h>
#include <dlfcn.h>

#define DLOPEN_FLAGS RTLD_NOW
#define FN_NAME "spam_foo"
#define NUM_ITERATIONS 10

static void test_module(const char *path) {

	fprintf(stderr, "Loading %s... ", path);
	void *module = dlopen(path, DLOPEN_FLAGS);
	if (module == NULL) {
		fprintf(stderr, "dlopen() failed: %s\n", dlerror());
		return;
	}
	fprintf(stderr, "OK\n");

	fprintf(stderr, "Searching function %s... ", FN_NAME);
	void *sym = dlsym(module, FN_NAME);
	if (sym == NULL) {
		fprintf(stderr, "dlsym() failed: %s\n", dlerror());
		return;
	} 
	fprintf(stderr, "OK\n");

	fprintf(stderr, "Calling function %s... ", FN_NAME);
	void (*fn)(unsigned) = sym;
	for (unsigned i = 0; i < NUM_ITERATIONS; i++) {
		fn(i);
	}
	fprintf(stderr, "OK\n");
}

int main(int argc, char **argv) {
	int i;
	for (i = 1; i < argc; i++)
		test_module(argv[i]);
	fprintf(stderr, "Finished.\n");
}

