# The libspam test dynamic library and spam-loader test executable
# Copyright 2017 Itiviti AB, Anton Smyk <Anton.Smyk@itiviti.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
# of the Software, and to permit persons to whom the Software is furnished to do
# so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

CPPFLAGS := -I.
CFLAGS := -std=gnu99 -pedantic -Wall -Wextra -pthread -g
LDFLAGS := -pthread

.PHONY: all
all: libspam-1.so libspam-2.so spam-loader

libspam-1.so: LDFLAGS += -shared -llttng-ust
libspam-1.so: libspam-foo-1.o libspam-trace-1.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

libspam-2.so: LDFLAGS += -shared -llttng-ust
libspam-2.so: libspam-foo-2.o libspam-trace-2.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

libspam-trace-1.h: libspam-trace-1.tp
	lttng-gen-tp libspam-trace-1.tp -o $@
libspam-trace-1.c: libspam-trace-1.tp
	lttng-gen-tp libspam-trace-1.tp -o $@

libspam-trace-2.h: libspam-trace-2.tp
	lttng-gen-tp libspam-trace-2.tp -o $@
libspam-trace-2.c: libspam-trace-2.tp
	lttng-gen-tp libspam-trace-2.tp -o $@

libspam-foo-1.o: CFLAGS += -fPIC
libspam-foo-1.o: libspam-trace-1.h libspam-foo-1.c

libspam-foo-2.o: CFLAGS += -fPIC
libspam-foo-2.o: libspam-trace-2.h libspam-foo-2.c

libspam-trace-1.o: CFLAGS += -fPIC
libspam-trace-1.o: libspam-trace-1.h libspam-trace-1.c

libspam-trace-2.o: CFLAGS += -fPIC
libspam-trace-2.o: libspam-trace-2.h libspam-trace-2.c

spam-loader: LDFLAGS += -ldl
spam-loader: spam-loader.o
	cc -pthread spam-loader.o -ldl  -o spam-loader

run:
	./spam-loader ./libspam-1.so ./libspam-2.so

run-reverse:
	./spam-loader ./libspam-2.so ./libspam-1.so
.PHONY: clean
clean:
	rm -f spam-loader
	rm -f libspam-1.so libspam-2.so
	rm -f *.o
	rm -f libspam-trace-1.h libspam-trace-1.c libspam-trace-2.h libspam-trace-2.c

