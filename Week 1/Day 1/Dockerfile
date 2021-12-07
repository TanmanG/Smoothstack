FROM centos:latest

RUN yum update -y
RUN install epel-release -y
RUN yum install gdb -y
RUN yum install vim -y
RUN install iftop -y
RUN install htop -y
RUN install iotop -y
RUN install cscope -y
RUN install tree -y
RUN install clang -y
RUN install git -y
RUN install nano -y

RUN dnf group install "Development Tools" -y
