import commands, sys

BACKSTART = 123
BACKMAX = 456
WORK = sys.argv[1]

def run(prog, work, cores):
	cmd = "".join(["./",prog," -t ", str(len(cores)), " -i ", str(work), " -a ", str(BACKSTART), " -b ", str(BACKMAX)])
	#start = time.time()
	(_,t) = commands.getstatusoutput(cmd)
	t = t[:-1]
	t = t.split(":")
	#elapsed = (time.time() - start)
	return t[1]

cores = [0,4,8,12,16,20,1,5,9,13,17,21,2,6,10,14,18,22,3,7,11,15,19,23]
numCores = [1,3,6,9,12,15,18,21,24]
PROGS = ["twoLockQueue", "oneLockQueue"]
TRIALS = 5


print "\t".join(["TRIAL","PROG","WORK","CORE","TIME"])

for c in numCores:
	for p in PROGS:
		for t in range(TRIALS):
			elapsed = run(p, WORK, cores[0:c])
			print "\t".join(map(str,[t,p,WORK,c,elapsed]))

