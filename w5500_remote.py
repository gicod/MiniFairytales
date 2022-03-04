import requests
import paramiko
import time
import os

try:
    import configparser
except ImportError:
    import ConfigParser as configparser

Import("env", "projenv")

config = configparser.ConfigParser()
config.read("platformio.ini")

ip_addr = config.get("env:my_env", "ip_addr")
host = config.get("env:my_env", "host")
username = config.get("env:my_env", "login")
password = config.get("env:my_env", "password")

# SFTP
port = 22
transport = paramiko.Transport((host, port))
transport.connect(username=username, password=password)
sftp = paramiko.SFTPClient.from_transport(transport)

# SSH
ssh = paramiko.SSHClient()
ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
ssh.connect(hostname=host, username=username, password=password, port=port)


def get_timeout(file_size: int) -> float:
    return float(file_size) / 1000.0 + 3.0


def upload(source, target, env):

    firmware_dir = env['PIOENV']
    firmware_name = env['PROGNAME'] + '.bin'
    firmware_path = '.pio/build/' + firmware_dir + '/' + firmware_name
    firmware_size = os.path.getsize(firmware_path)
    print("uploading firmware to router ip:" + host)
    exec_timeout = get_timeout(firmware_size)
    print(f"Timeout: {exec_timeout} s")
    filepath = '/root/' + firmware_name
    localpath = firmware_path
    print(localpath)
    print(sftp.put(localpath, filepath))
    sftp.close()

    print("uploaded file is in " + filepath)

    print("Resetting arduino...")
    # arduino reset
    channel = ssh.get_transport().open_session()
    channel.get_pty()
    channel.settimeout(5.0)
    channel.exec_command('curl ' + ip_addr + ':44444')
    print(channel.recv(1024))

    channel.close()

    time.sleep(1)

    # TFTP Download
    print("Firmware upgrade...")
    channel = ssh.get_transport().open_session()
    channel.get_pty()
    channel.settimeout(exec_timeout)
    channel.exec_command(
        'atftp --option "mode octet" -p -l ' + firmware_name + ' ' + ip_addr)
    print(channel.recv(1024))
    print(channel.recv(1024))
    print(channel.recv(1024))
    print(channel.recv(1024))

    channel.close()
    # Closing all conections
    ssh.close()

    # print "TFTP connection to " + ipaddr

    #firmware_dir = env['PROJECTBUILD_DIR'] + '/' + env['PIOENV']
    #firmware_dir = env['PIOENV']
    #firmware_name = env['PROGNAME'] + '.bin'

    # env.Execute()

    #client = tftpy.TftpClient(ipaddr, 69)
    #client.upload(firmware_name, '.pioenvs\my_env')

    #env.Execute("ping google.ru")
    print("uploaded")

    # env.Execute("pwd")
    #env.Execute("telnet " + ip_addr)


env.Replace(
    UPLOADCMD=upload
)

env.AddPostAction(
    "$BUILD_DIR\${PROGNAME}.elf",
    env.VerboseAction(" ".join([
        "$OBJCOPY", "-O", "binary",
        "$BUILD_DIR\${PROGNAME}.elf", "$BUILD_DIR\${PROGNAME}.bin"
    ]), "Creating $BUILD_DIR\${PROGNAME}.bin")
)
