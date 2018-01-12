#!/usr/bin/python

print "# handlers"
for i in range(0, 256):
    name = "vector%d" % i
    if not (i == 8 or (10 <= i and i <= 14) or i == 17):
        print "TRAPHANDLER_NOEC(%s, %d)" % (name, i)
    else:
        print "TRAPHANDLER(%s, %d)" % (name, i)

print ""
print "# vector table"
print ".data"
print ".globl vectors"
print "vectors:"
for i in range(0, 256):
    print "  .long vector%d" % i
