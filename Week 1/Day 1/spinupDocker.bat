@ECHO OFF

docker run -it --privileged --mount src=I:\Docker,target=/Docker,type=bind dockerdev bash

PAUSE