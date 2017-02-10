import time
import ftplib
class UploadJob: 
    self.str_time_now = None
    self.tarfilename = "3165.tar"
    self.timeformat = "%Y%m%d%H%M%S"
    self.ftpusername= ""
    self.ftppassword=""
    self.ftpclient = None
    self.ftpserver = None
    def __init__(self):
        timenow = time.localtime()
        self.str_time_now = time.strftime(self.timeformat,self.timenow)
        self.ftpclient = ftplib.FTP(self.ftpserver)
        self.ftpclient.login(self.ftpusername, self.ftppassword)

    def doDirPrepare(self):
        self.ftpclient.nlst()
        t = self.ftpclient.dir() # return directory list.
        direxisted = False
        for i in t:
            if ( i== self.timeformat):
                direxisted = True
                break;
        if( not direxisted):
            self.ftpclient.mkd(self.timeformat):
        self.ftpclient.cwd(self.timeformat)
    def doUpload(self):
        bufsize = 1024    
        fp = open(self.tarfilename,'rb')  
        self.ftpclient.storbinary('STOR '+ tarfilename ,fp,bufsize) #上传文件  
        self.ftpclient.set_debuglevel(0)  
        fp.close() #关闭文件  
        self.ftpclient.quit()  

def main():
    job = UploadJob()
    job.doDirPrepare();
    job.doUpload();