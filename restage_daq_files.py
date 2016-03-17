#!/opt/star/sl64_gcc447/bin/python
""" This script restages a large amount of daq files to disk taking into account the available disk space and prepares files for batch
submission """
__author__ = "Mustafa Mustafa"
__email__ = "mmustafa@lbl.gov"
import os
import subprocess
import time
import random
import fcntl

inputListFile = "batch_ZF.lis"
targetDir = "/star/institutions/lbl_scratch/hft/daq/" # DAQ files target directory
bufferSpace = 500 # GB
daqFileSize = 5 # GB on average
daqFilesRequestSleepInterval = 60*60 # one hour

filesToRequest = []
requestedFiles = []
toBeSubmitted = []

submissionDir = "."
listOfSubmitedJobs = submissionDir+"/listOfSubmitedJobs.list"
listOfRequestedFiles = submissionDir+"/listOfRequestedFiles.list"

jobSubmitters = ['xin','guannan']

def freeSpace():
    """ returns free space in GB """
    s = os.statvfs(targetDir)
    return (s.f_bavail * s.f_frsize) / (1024*1024*1024) # return in GB

def requestMoreFiles():
    """ request more files from HPSS if there is enough space on disk """
    global filesToRequest
    global requestedFiles

    file = open(listOfRequestedFiles,"a")
    # divide freeSpace by 2 to account for the duplication on gpfs
    while len(filesToRequest) and (freeSpace()/2 - bufferSpace - len(requestedFiles)*daqFileSize > daqFileSize) :
        daqFile = filesToRequest.pop()
        subprocess.call(('hpss_user.pl','-z',daqFile.split(" ")[0],daqFile.split(" ")[1]))
        requestedFiles.append(daqFile.split(" ")[1])
        file.write(requestedFiles[-1]+'\n')
    file.close()

def checkIfRequestedFilesAreOnDisk():
    """ check if requested files are already on disk """
    global requestedFiles
    global toBeSubmitted

    for x in requestedFiles[:]:
        if os.path.exists(x): 
            os.system("chmod g+rw "+x)
            toBeSubmitted.append(x)
            requestedFiles.remove(x)

def pipeIntoFile(f,a):

    file = open(listOfSubmitedJobs,'a')
    fSub = open(f,'a')
    fcntl.flock(file,fcntl.LOCK_EX) # lock file to write to it
    fcntl.flock(fSub,fcntl.LOCK_EX) # lock file to write to it

    for x in a:
        fSub.write(x+'\n')
        file.write(x+'\n')

    fcntl.flock(file,fcntl.LOCK_UN) # release lock on file
    fcntl.flock(fSub,fcntl.LOCK_UN) # release lock on file
    file.close()
    fSub.close()

    os.system("chmod g+rw "+listOfSubmitedJobs)
    os.system("chmod g+rw "+f)

def submitJobs():
    """ prepare lists of files to be submitted """
    global toBeSubmitted
    
    nFilesToSubmit = len(toBeSubmitted)
    if not nFilesToSubmit: return
    nSubm = len(jobSubmitters)
    
    filesPerSubmitter = nFilesToSubmit/nSubm

    for idx, sub in enumerate(jobSubmitters[:]):
        outFileName = submissionDir+"/"+"files_for_"+sub+".list"
        fF = idx*filesPerSubmitter
        lF = idx*filesPerSubmitter+filesPerSubmitter
        pipeIntoFile(outFileName,toBeSubmitted[fF:lF])

    # assign any remaining jobs to be submitted by *any* submitter
    if nSubm*filesPerSubmitter!=nFilesToSubmit:
        r = random.randrange(nSubm)
        outFileName = submissionDir+"/"+"files_for_"+jobSubmitters[r]+".list"
        pipeIntoFile(outFileName,toBeSubmitted[nSubm*filesPerSubmitter:])

    del toBeSubmitted[:]


def main():

    file = open(inputListFile,'r')
    global filesToRequest
    global requestedFiles
    filesToRequest = file.readlines()
    file.close()
    filesToRequest[:] = map(str.rstrip,filesToRequest)

    while len(filesToRequest) or len(requestedFiles):
        checkIfRequestedFilesAreOnDisk()
        submitJobs()
        requestMoreFiles()
        print "number of files remaining to request: ",len(filesToRequest)
        if len(filesToRequest) or len(requestedFiles):
            os.system("date")
            print "... waiting for another ",daqFilesRequestSleepInterval/60," minutes before trying to submit again"
            time.sleep(daqFilesRequestSleepInterval)

if __name__ == '__main__':
    main()
