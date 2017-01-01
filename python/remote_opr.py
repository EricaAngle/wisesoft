import pexpect 
import pexpect.pxssh
import sys
import getpass

'''
    run('scp foo user@172.17.0.4:.', event=['(?i)password]': mypassword)
'''
def doWithPexpect():
    pchild = pexpect.spawn("ssh 172.17.0.4 -l root")
    pchild.logfile = sys.stdout
    pchild.expect("[pP]assword:")
    pchild.sendline("zhengqiang")
    pchild.expect("#")
    pchild.sendline("cd /home/skylark/cpp")
    pchild.expect("#")
    pchild.sendline("ls -alh thrift> help.txt")
    pchild.expect("#")
    pchild.sendline("exit")


'''
  use pexpect.pssh to process remote command.
'''
def doWithXssh(): 
    try:
        s = pexpect.pxssh.pxssh()
        hostname = "172.17.0.4"
        username = "root"
        passwd = getpass.getpass("please input password: ")
        s.login(hostname,username,passwd)
        s.sendline('uptime')
        s.prompt()
        print s.before
        s.sendline('ls -l ')
        s.prompt();
        print s.before
        s.sendline('df')
        s.prompt()
        s.logout() 
    except pexpect.pxssh.ExceptionPxssh, e:
        print("--------------")
        str(e)   
        print("---------------")
    except Exception ,e:
        str(e)
        print('NND')


def main():
    doWithXssh();

if __name__ == '__main__':
    main()

