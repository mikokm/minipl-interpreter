var nTimes : int := 0;
print "How many times?";
read nTimes;
var x : int;
for x in 0..nTimes-1 do
print x;
print " : Hello, World!\n";
end for;
print "\nx:\n";
print x;
print "\nnTimes:\n";
print nTimes;
assert (x < nTimes);
