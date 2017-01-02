import paramiko

hname = '172.17.0.4'
usrname="root"
passwd="zhengqiang"
paramiko.util.log_to_file('login.log')
ssh = paramiko.SSHClient()
ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
ssh.load_system_host_keys('/root/.ssh/known_hosts')
ssh.connect(hostname=hname, username=usrname, password=passwd)
stdin, stdout, stderr = ssh.exec_command('free -m')
print stdout.read()
ssh.close()