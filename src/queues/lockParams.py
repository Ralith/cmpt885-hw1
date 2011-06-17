import commands, sys


WORK = sys.argv[1]

def run(prog, work, cores, bstart, bmax):
	cmd = "".join(["./",prog," -t ", str(cores), " -i ", str(work), " -a ", str(bstart), " -b ", str(bmax)])
	#start = time.time()
	(_,t) = commands.getstatusoutput(cmd)
	t = t[:-1]
	t = t.split(":")
	#elapsed = (time.time() - start)
	return int(t[1])


numCores = [1,6,12,18,24]
PROGS = ["oneLockQueue","twoLockQueue"]
backStart = [1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192, 16384, 32768, 65536, 131072, 262144, 524288]
backMax = [32,64,128,256,1024,2028,4096,8192,16384,32768, 65536, 131072, 262144, 524288, 1048576]
TRIALS = 5


print "\t".join(["TRIAL","PROG","WORK","CORE","BSTART","BMAX","TIME","PROG_LONG"])

for c in numCores:
	for p in PROGS:
	    for s in backStart:
	        for m in backMax:
	            if s < m:
		            for t in range(TRIALS):
			            elapsed = run(p, WORK, c, s, m)
			            print "\t".join(map(str,[t,p,WORK,c,s,m,elapsed,"-".join([p,s,m])]))

