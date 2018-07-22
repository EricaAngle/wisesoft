from alpine:latest

LABEL maintainer: zhengqiang99@126.com

ARG SALT_VER=latest

EXPOSE 4505 4506 8041 8042 8043 8044
RUN apk --no-cache add git gcc g++ python3-dev openssh linux-headers curl linux-pam supervisor libsodium --update && \
   curl -SsLO https://bootstrap.pypa.io/get-pip.py && python get-pip.py   && \
   pip install supervisor  && \
   mkdir -p /etc/supervisor.d /run /home/saltstack

WORKDIR /home/saltstack

RUN  git clone https://github.com/saltstack/salt.git  /home/saltstack && \

RUN apk add --no-cache libffi-dev libffi openssl-dev && python3  setup.py install && \
     pip3 --no-cache-dir install cherrypy pyopenssl && \
     pip3 --no-cache-dir install -r /home/saltstack/requirements/base.txt && \
     pip3 --no-cache-dir install -r /home/saltstack/requirements/opt.txt && \
     pip3 --no-cache-dir install -r /home/saltstack/requirements/raet.txt && \
     pip3 --no-cache-dir install -r /home/saltstack/requirements/zeromq.txt && \
     mkdir -p supervisor.d && \
     echo "/usr/bin/supervisord -n -c /etc/supervisord.conf" > /docker-entrypoint.sh  && chmod uo+x /docker-entrypoint.sh
ADD app.ini  /etc/supervisor.d/app.ini

RUN  pip3 --no-cache-dir install pyOpenSSL && adduser -S salt && echo "salt:salt2018" | chpasswd  && \
    apk del git gcc g++ python-dev linux-headers libffi-dev opnssl-dev curl

ENTRYPOINT /docker-entrypoint.sh

HEALTHCHECK --interval=5m --timeout=3s cmd python3 --version || exit 1
